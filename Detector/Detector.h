#pragma once

#include "detector_global.h"
#include <QString>
#include <QImage>
#include<opencv2\opencv.hpp>
#include<opencv2\dnn.hpp>
#include <opencv2\imgproc\types_c.h> 
#include <fstream>

class DETECTOR_EXPORT Detector
{
public:
    QString doDetection(cv::Mat& img, double conf,
        std::vector<cv::Rect>& out_Boxes,
        std::vector<int>& out_ClassIds,
        std::vector<float>& out_Confidences,
        std::vector<cv::String>& out_ClassNames);

public:
    Detector();
    Detector(QString sCfgFile, QString sWeightFile, QString sCoconamesFile);
    std::vector<std::string> objects_names_from_file(std::string const filename);
   
private:
    QString yoloCfg;
    QString yoloWeights;
    QString yoloCoconames;
    cv::dnn::Net net;

private:
    void setupNet(QString sCfgFile, QString sWeightFile, QString sCoconamesFile);
};

QImage Mat2QImage(cv::Mat& mat);
cv::Mat QImage2Mat(QImage& qim);