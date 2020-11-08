#include "Detector.h"

Detector::Detector(QString sCfgFile, QString sWeightFile, QString sCoconamesFile)
{
    yoloCfg = sCfgFile;
    yoloWeights = sWeightFile;
    yoloCoconames = sCoconamesFile;
    setupNet( sCfgFile,  sWeightFile,  sCoconamesFile);
}

//对象检测
QString Detector::doDetection(cv::Mat& img, double conf,
    std::vector<cv::Rect>& out_Boxes,
    std::vector<int>& out_ClassIds,
    std::vector<float>& out_Confidences,
    std::vector<cv::String>& out_ClassNames)
{
    int64 t1 = cvGetTickCount();
    cv::Mat currentFrame = img;
    cv::Mat inputBlob = cv::dnn::blobFromImage(currentFrame, 1.0 / 255, cv::Size(608, 608), cv::Scalar());
    net.setInput(inputBlob);
    std::vector<cv::String> outNames = net.getUnconnectedOutLayersNames();
    std::vector<cv::Mat> outs;
    net.forward(outs, outNames);

    //找出所有的目标及其位置
    float* data;
    cv::Mat scores;
    std::vector<cv::Rect> boxes;
    std::vector<int> classIds;
    std::vector<float> confidences;
    int centerX, centerY, width, height, left, top;
    float confidenceThreshold = conf;
    double confidence;
    cv::Point classIdPoint;

    for (size_t i = 0; i < outs.size(); ++i) {
        data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
            scores = outs[i].row(j).colRange(5, outs[i].cols);
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confidenceThreshold) {
                centerX = (int)(data[0] * currentFrame.cols);
                centerY = (int)(data[1] * currentFrame.rows);
                width = (int)(data[2] * currentFrame.cols);
                height = (int)(data[3] * currentFrame.rows);
                left = centerX - width / 2;
                top = centerY - height / 2;
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.3, 0.2, indices);//todo:parameterize the threshold.

    //将预测框和内容overlap到图像
    cv::Scalar rectColor, textColor; //box 和 text 的颜色
    cv::Rect box, textBox;
    int idx;
    cv::String className;
    cv::Size labelSize;
    std::vector<std::string> obj_names = objects_names_from_file(yoloCoconames.toStdString());
    std::cout << obj_names[0];
    for (size_t i = 0; i < indices.size(); ++i) {
        idx = indices[i];
        className = obj_names[classIds[idx]];
        labelSize = cv::getTextSize(className, cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
        box = boxes[idx];
        out_Boxes.push_back(box);
        out_ClassIds.push_back(classIds[idx]);
        out_Confidences.push_back(confidences[idx]);
        out_ClassNames.push_back(className);

        textBox = cv::Rect(cv::Point(box.x - 1, box.y),
            cv::Point(box.x + labelSize.width, box.y - labelSize.height));
        rectColor = cv::Scalar(idx * 11 % 256, idx * 22 % 256, idx * 33 % 256);
        textColor = cv::Scalar(255 - idx * 11 % 256, 255 - idx * 22 % 256, 255 - idx * 33 % 256);

        rectangle(currentFrame, box, rectColor, 2, 8, 0);
        rectangle(currentFrame, textBox, rectColor, -1, 8, 0);
        putText(currentFrame, className.c_str(), cv::Point(box.x, box.y - 2), cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, textColor, 1, 8);
    }
    //cv::imshow("【result】", currentFrame);
    return QString(u8"detection time elapsed:  %1 ms").arg((cvGetTickCount() - t1) / cvGetTickFrequency()/1000);
}

QImage Mat2QImage(cv::Mat& mat)
{
    cvtColor(mat, mat,cv::COLOR_BGR2RGB);
    QImage qim((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step,
        QImage::Format_RGB888);
    return qim;
}
cv::Mat QImage2Mat(QImage& qim)
{
    cv::Mat mat = cv::Mat(qim.height(), qim.width(),
        CV_8UC3, (void*)qim.constBits(), qim.bytesPerLine());
    return mat;
}

std::vector<std::string> Detector::objects_names_from_file(std::string const filename)
{
    std::ifstream file(filename);
    std::vector<std::string> file_lines;
    if (!file.is_open()) return file_lines;
    for (std::string line; getline(file, line);) file_lines.push_back(line);
    std::cout << "object names loaded \n";
    return file_lines;
}

void Detector::setupNet(QString sCfgFile, QString sWeightFile, QString sCoconamesFile)
{
    cv::String cfg = yoloCfg.toLocal8Bit();
    cv::String weight = yoloWeights.toLocal8Bit();
    std::string coconames = yoloCoconames.toLocal8Bit();
    net = cv::dnn::readNetFromDarknet(cfg, weight);
    net.setPreferableBackend(cv::dnn::Backend::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::Target::DNN_TARGET_CUDA);
    //net.setPreferableBackend(cv::dnn::Backend::DNN_BACKEND_OPENCV);
    //net.setPreferableTarget(cv::dnn::Target::DNN_TARGET_CPU);
}
