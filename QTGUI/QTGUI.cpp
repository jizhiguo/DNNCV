#include "QTGUI.h"
#include "CustomTableItem.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtCore\qfile.h>
#include <QtMultimediaWidgets/qvideowidget.h>
#include <qtextstream.h>
#include <QtMultimedia/qmediaplayer.h>
#include <QtMultimedia/qmediaplaylist.h>
#include <QVBoxLayout>
#include "GraphicsView.h"
#include "../Detector/Detector.h"
#include "CocoHelper.h"
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

QTGUI::QTGUI(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	setupTable1();
	setupTable2();
	//connect button signals & slot functions
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onOpenCoco()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onSaveCoco()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onResetCoco()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(onLoadImage()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(doDetection()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(onAddPoint()));
	connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(onDelPoint()));
	connect(ui.graphicsView, SIGNAL(sigDrawPolygon()), this, SLOT(onSigDrawPolygon()));
}

void QTGUI::setupTable1()
{
	//tableview items delegates
	CheckBoxDelegate* cbd = new CheckBoxDelegate;
	ReadOnlyDelegate* rod = new ReadOnlyDelegate;
	ComboDelegate* combo = new ComboDelegate;
	QString itemsStr = "50%   90%  10%   20%  30%   40%  50%  60%  70%  80%  100%";
	QStringList itemList = itemsStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	combo->setItems(itemList);

	//column 0,checkbox
	//column 1,2,3read only
	//column 4,combobox
	cbd->setColumn(0);
	ui.tableView->setItemDelegateForColumn(1, rod);
	ui.tableView->setItemDelegateForColumn(2, rod);
	ui.tableView->setItemDelegateForColumn(3, rod);
	ui.tableView->setItemDelegateForColumn(4, combo);

	//model setup
	model = new QStandardItemModel;
	model->setRowCount(10);
	model->setColumnCount(5);
	ui.tableView->setModel(model);
	ui.tableView->setItemDelegate(cbd);
	ui.tableView->verticalHeader()->setHidden(true);

	//load coconamems
	load_coconames(QCoreApplication::applicationDirPath().append("/coconames.txt"));
}

void QTGUI::setupTable2()
{
	int rows = 4;
	int cols = 3;
	//model setup
	QStandardItemModel* model = new QStandardItemModel;
	model->setRowCount(rows);
	model->setColumnCount(cols);
	SpinBoxDelegate* sbd = new SpinBoxDelegate;
	ReadOnlyDelegate* rod = new ReadOnlyDelegate;
	ui.tableView_2->setItemDelegate(sbd);
	ui.tableView_2->setItemDelegateForColumn(0, rod);

	//设置表头文字
	QString header = u8"点\tx\ty";
	QStringList headerList = header.split(QRegExp("\\t+"), QString::SkipEmptyParts);
	model->setHorizontalHeaderLabels(headerList);
	//设置表格数据
	QStandardItem* aItem;
	for (int i = 0; i < model->rowCount(); i++)
	{
		for (int j = 0; j < model->columnCount(); j++)
		{
			if (0 == j)
			{
				aItem = new QStandardItem(QString("%1").arg(i));
			}
			else
			{
				aItem = new QStandardItem(QString("%1").arg(i * 10 + j * 10));
			}
			model->setItem(i, j, aItem);
		}
	}

	ui.tableView_2->setModel(model);
	ui.tableView_2->verticalHeader()->setHidden(true);
	ui.tableView_2->resizeColumnsToContents();
	ui.tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void QTGUI::onOpenCoco()
{
	QString curPath = QCoreApplication::applicationDirPath();
	QString filename = QFileDialog::getOpenFileName(this, u8"打开一个文件", curPath, u8"数据文件(*.txt);;所有文件(*.*)");
	if (filename.isEmpty())
	{
		return;
	}
	load_coconames(filename);
}

void QTGUI::load_coconames(QString& filename) {

	QStringList fileContent;
	QFile file(filename);
	if (!file.exists()) return;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		coconamesfilename = filename;
		QTextStream textStream(&file);
		while (!textStream.atEnd())
		{
			QString str = textStream.readLine();
			fileContent.append(str.toUtf8());
		}
		file.close();

		int rowCnt = fileContent.count(); //文本行数，第1行是标题
		model->clear();
		model->setRowCount(rowCnt - 1);
		QString header = fileContent.at(0);//第1行是表头
		QStringList headerList = header.split(QRegExp("\\t+"), QString::SkipEmptyParts);
		model->setHorizontalHeaderLabels(headerList); //设置表头文字
		//设置表格数据
		QString aText;
		QStringList tmpList;
		QStandardItem* aItem;
		for (int i = 1; i < rowCnt; i++)
		{
			QString aLineText = fileContent.at(i);
			QStringList tmpList = aLineText.split(QRegExp("\\t+"), QString::SkipEmptyParts);
			for (int j = 0; j < 5; j++)
			{
				aItem = new QStandardItem(tmpList.at(j));
				model->setItem(i - 1, j, aItem);
			}
		}
	}
	ui.tableView->resizeColumnsToContents();
}

void QTGUI::load_image(QString& filename)
{
	QFile aFile(filename);
	if (!(aFile.exists()))
	{
		return;
	}
	QImage image;
	image.load(filename);
	load_image(image);
}

void QTGUI::load_image(QImage image)
{
	if (!scene)
		scene = new QGraphicsScene;
	scene->clear();
	ui.graphicsView->m_MousePressPos.clear();
	QPixmap qPixmap = QPixmap::fromImage(image);
	imageWidget = new ImageWidget(&qPixmap);//实例化类ImageWidget的对象m_Image，该类继承自QGraphicsItem
	int nwith = ui.graphicsView->width();//获取界面控件Graphics View的宽度
	int nheight = ui.graphicsView->height();//获取界面控件Graphics View的高度
	imageWidget->setQGraphicsViewWH(nwith, nheight);//将界面控件Graphics View的width和height传进类m_Image中
	scene->addItem(imageWidget);//将QGraphicsItem类对象放进QGraphicsScene中
	ui.graphicsView->setSceneRect(QRectF(-(nwith / 2), -(nheight / 2), nwith, nheight));//使视窗的大小固定在原始大小，不会随图片的放大而放大（默认状态下图片放大的时候视窗两边会自动出现滚动条，并且视窗内的视野会变大），防止图片放大后重新缩小的时候视窗太大而不方便观察图片
	ui.graphicsView->setScene(scene);
	ui.graphicsView->setFocus();//将界面的焦点设置到当前Graphics View控件
}

void QTGUI::onSaveCoco()
{
	if (coconamesfilename.isEmpty())
	{
		QString curPath = QCoreApplication::applicationDirPath();
		QString coconamesfilename = QFileDialog::getOpenFileName(this, u8"保存一个文件", curPath, u8"数据文件(*.txt);;所有文件(*.*)");
		if (coconamesfilename.isEmpty())
		{
			return;
		}
	}

	QFile aFile(coconamesfilename);
	if (!(aFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)))
		return; //以读写、覆盖原有内容方式打开文件

	QTextStream aStream(&aFile); //用文本流读取文件
	QStandardItem* aItem;
	QString str;
	//获取表头文字
	for (int i = 0; i < model->columnCount(); i++)
	{
		aItem = model->horizontalHeaderItem(i); //获取表头的项数据
		str = str + aItem->text() + "\t\t";  //以TAB符隔开
	}
	aStream << str << "\n";  //换行符 \n

	for (int i = 0; i < model->rowCount(); i++)
	{
		str = "";
		for (int j = 0; j < model->columnCount(); j++)
		{
			aItem = model->item(i, j);
			str = str + aItem->text() + QString::asprintf("\t\t");
		}
		aStream << str << "\n";
	}
}

void QTGUI::onResetCoco()
{
	load_coconames(coconamesfilename);

}

void QTGUI::onLoadImage()
{
	QString curPath = QCoreApplication::applicationDirPath();
	QString filename = QFileDialog::getOpenFileName(this, u8"打开一个图片文件", curPath, u8"jpg文件(*.jpg);;所有文件(*.*)");
	if (filename.isEmpty())
	{
		return;
	}
	imgfilename = filename;
	load_image(filename);
}

void QTGUI::onAddPoint()
{
	QStandardItemModel* model = (QStandardItemModel*)(ui.tableView_2->model());
	QList< QStandardItem*> items;
	model->appendRow(items);

	QStandardItem* aItem;
	aItem = new QStandardItem(QString("%1").arg(model->rowCount() - 1));
	model->setItem(model->rowCount() - 1, 0, aItem);
}

void QTGUI::onDelPoint()
{
	QStandardItemModel* model = (QStandardItemModel*)(ui.tableView_2->model());
	model->removeRow(model->rowCount() - 1);
}

void QTGUI::onSigDrawPolygon()
{
	//clear polygon in scene
	QList<QGraphicsItem*> itemList = scene->items();
	for (auto i = 0; i < itemList.size(); i++) {
		if ((itemList[i]->type() == QGraphicsPolygonItem::Type)) {//多边形类型的图元
			scene->removeItem(itemList[i]);
			delete itemList[i];
		}
	}

	//add a brand new polygon to scene
	polygon = scene->addPolygon(ui.graphicsView->m_MousePressPos, QPen(QColor(255, 0, 0, 100)), QBrush(QColor(0, 0, 255, 50)));

	//update table of points' coordinate.
	QStandardItemModel* model = (QStandardItemModel*)(ui.tableView_2->model());
	while (model->rowCount() > 0)model->removeRow(0);
	QVectorIterator<QPointF> iterator(polygon->mapFromScene(polygon->polygon()));
	QPointF f; int i = 0;
	QStandardItem* aItem;
	QList< QStandardItem*> items;
	while (iterator.hasNext())
	{
		f = ui.graphicsView->mapFromScene(iterator.next());//场景坐标转视图坐标
		model->appendRow(items);
		aItem = new QStandardItem(QString("%1").arg(i));
		model->setItem(i, 0, aItem);
		aItem = new QStandardItem(QString("%1").arg(f.x()));
		model->setItem(i, 1, aItem);
		aItem = new QStandardItem(QString("%1").arg(f.y()));
		model->setItem(i, 2, aItem);
		i++;
	}
}

void QTGUI::doDetection()
{
	yoloCfgfilename = "D:\\Server_Project\\model\\yolov4.cfg";
	yoloWeightsfilename = "D:\\Server_Project\\model\\yolov4.weights";
	yoloCocofilename = "D:\\Server_Project\\model\\coco.names";
	//yoloCfgfilename = "D:\\yolov4\\darknet\\cfg\\yolov4.cfg";
	//yoloWeightsfilename = "D:\\yolov4\\darknet\\build\\darknet\\x64\\yolov4.weights";
	//yoloCocofilename = "D:\\yolov4\\darknet\\cfg\\coco.names";
	cv::Mat img;
	img = cv::imread(imgfilename.toStdString());
	if (img.data == NULL)
	{
		ui.textBrowser->append(u8"请选择图片.");
		return;
	}

	std::vector<cv::Rect> out_Boxes;
	std::vector<int> out_ClassIds;
	std::vector<float> out_Confidences;
	std::vector<cv::String> out_ClassNames;
	double confidence = 0.2;
	Detector detector(yoloCfgfilename, yoloWeightsfilename, yoloCocofilename);
	QString result = detector.doDetection(img, confidence, out_Boxes, out_ClassIds, out_Confidences, out_ClassNames);
	ui.textBrowser->append(result);
	if (!(out_Boxes.size() > 0))
	{
		ui.textBrowser->append(QString(u8"未侦测到任何对象.(confidence=%1)").arg(confidence));
		return;
	}
	//cv::namedWindow("camera", CV_WINDOW_NORMAL);
	//cv::imshow("camera", img);
	Coconames c;
	cv::Mat im = c.Base2Mat(c.Mat2Base64(img, "jpg"));
	cv::imshow("camera", im);
	size_t len = out_Boxes.size();
	cv::Rect cr;
	for (size_t i = 0; i < len; i++) {
		//id,name,conf,in poly,in checklist
		cr = out_Boxes[i];
		ui.textBrowser->append(QString(u8"id:%1(x=%2,y=%3)").arg(out_ClassIds[i]).arg(cr.x).arg(cr.y));
		ui.textBrowser->append(QString(u8"名称:%1").arg(out_ClassNames[i].c_str()));
		ui.textBrowser->append(QString(u8"可能性:%1f").arg(out_Confidences[i]));
		ui.textBrowser->append(QString(u8"是否在多边形1内:%1").arg(inPoly(out_Boxes[i]) ? u8"是" : u8"否"));
		ui.textBrowser->append(QString(u8"是否识别:%1").arg(inChecklist(out_ClassIds[i]) ? u8"是" : u8"否"));
	}
}

bool  QTGUI::inPoly(cv::Rect box)
{
	if (polygon != NULL)
	{
		QPolygonF qp = polygon->scene()->views()[0]->mapFromScene(polygon->polygon().toPolygon());
		QPolygonF qp1 = imageWidget->mapToItem(polygon, polygon->polygon());
		return qp.containsPoint(QPoint(box.x, box.y), Qt::WindingFill);
	}

	return false;
}

bool  QTGUI::inChecklist(int classID)
{
	int rows = ui.tableView->model()->rowCount();
	for (size_t i = 0; i < rows; i++)
	{
		if (model->item(i, 1)->text().toInt() == classID) return model->item(i, 0)->text().toInt();
	}
	return false;
}

bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
int match_method;
int max_Trackbar = 5;
void MatchingMethod(int, void*);

void QTGUI::on_pushButton_8_clicked()
{

	QMessageBox::information(this, "adf", QString("QThread::idealThreadCount()=%1").arg(QThread::idealThreadCount()));
	img = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person.jpg");
	if (!img.data)
	{
		std::cout << "原始图读取失败" << endl;
		return;
	}
	templ = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person2.jpg");
	if (!templ.data)
	{
		std::cout << "模板图读取失败" << endl;
		return;
	}
	imshow("ICON", templ);

	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);

	const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
	MatchingMethod(0, 0);

	return;
}


void MatchingMethod(int, void*)
{
	Mat img_display;
	img.copyTo(img_display);
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);
	bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
	if (use_mask && method_accepts_mask)
	{
		matchTemplate(img, templ, result, match_method, mask);
	}
	else
	{
		matchTemplate(img, templ, result, match_method);
	}
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	imshow(image_window, img_display);
	imshow(result_window, result);
	return;
}

//void on_matching(int, void*)
//{
//	Mat img_display;
//	img.copyTo(img_display);
//	int result_cols = img.cols - templ.cols + 1;
//	int result_rows = img.rows - templ.rows + 1;
//	result.create(result_rows, result_cols, CV_32FC1);
//	bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
//	if (use_mask && method_accepts_mask)
//	{
//		matchTemplate(img, templ, result, match_method, mask);
//	}
//	else
//	{
//		matchTemplate(img, templ, result, match_method);
//	}
//	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
//	double minVal; double maxVal; Point minLoc; Point maxLoc;
//	Point matchLoc;
//	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//
//
//	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
//	{
//		matchLoc = minLoc;
//	}
//	else
//	{
//		matchLoc = maxLoc;
//	}
//	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//	putText(result, cv::String(QString("max=%1,min=%2").arg(maxVal).arg(minVal).toStdString()), Point(20, 20), 1, 1, Scalar::all(128), 2, 8, 0);
//	imshow(image_window, img_display);
//	imshow(result_window, result);
//	return;
//}



Mat src_gray;
int thresh = 100;
RNG rng(12345);
void thresh_callback(int, void*);

void QTGUI::on_pushButton_9_clicked() {
	Mat src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person.jpg");
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	const char* source_window = "Source";
	namedWindow(source_window);
	imshow(source_window, src);
	const int max_thresh = 255;
	createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);
}

void thresh_callback(int, void*)
{
	Mat canny_output;
	Canny(src_gray, canny_output, thresh, thresh * 2);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector<vector<Point> >hull(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		//convexHull(contours[i], hull[i]);
	}

	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
		//drawContours(drawing, hull, (int)i, color);
	}
	imshow("Contours", drawing);
}
void QTGUI::on_pushButton_10_clicked() {
	Mat src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person2.jpg");
	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	std::vector<KeyPoint> keypoints;
	detector->detect(src, keypoints);
	//-- Draw keypoints
	Mat img_keypoints;
	drawKeypoints(src, keypoints, img_keypoints);
	//-- Show detected (drawn) keypoints
	imshow("SURF Keypoints", img_keypoints);

	src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person3.jpg");
	keypoints.clear();
	detector->detect(src, keypoints);
	drawKeypoints(src, keypoints, img_keypoints);
	imshow("SURF Keypoints3", img_keypoints);

	src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person4.jpg");
	keypoints.clear();
	detector->detect(src, keypoints);
	drawKeypoints(src, keypoints, img_keypoints);
	imshow("SURF Keypoints4", img_keypoints);

	src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person5.jpg");
	keypoints.clear();
	detector->detect(src, keypoints);
	drawKeypoints(src, keypoints, img_keypoints);
	imshow("SURF Keypoints5", img_keypoints);

	src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person6.jpg");
	keypoints.clear();
	detector->detect(src, keypoints);
	drawKeypoints(src, keypoints, img_keypoints);
	imshow("SURF Keypoints6", img_keypoints);

	src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person7.jpg");
	keypoints.clear();
	detector->detect(src, keypoints);
	drawKeypoints(src, keypoints, img_keypoints);
	imshow("SURF Keypoints7", img_keypoints);

}
void QTGUI::on_pushButton_11_clicked() {
	// Declare the output variables
	Mat dst, cdst, cdstP;
	// Loads an image
	Mat src = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person.jpg");
	// Check if image is loaded fine
	if (src.empty()) {
		return;
	}
	// Edge detection
	Canny(src, dst, 50, 200, 3);
	// Copy edges to the images that will display the results in BGR
	cvtColor(dst, cdst, COLOR_GRAY2BGR);
	cdstP = cdst.clone();
	// Standard Hough Line Transform
	vector<Vec2f> lines; // will hold the results of the detection
	HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
	}
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection
	HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < linesP.size(); i++)
	{
		Vec4i l = linesP[i];
		line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
	}
	// Show results
	imshow("Source", src);
	imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
	imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
}

void QTGUI::on_pushButton_12_clicked()
{
	int64 t1 = cv::getTickCount();
	Mat img1 = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person.jpg");
	Mat img2 = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person2.jpg");
	if (img1.empty() || img2.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return;
	}

	//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
	detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

	////-- Step 2: Matching descriptor vectors with a brute force matcher
	//// Since SURF is a floating-point descriptor NORM_L2 is used
	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
	//std::vector< DMatch > matches;
	//matcher->match(descriptors1, descriptors2, matches);
	////-- Draw matches
	//Mat img_matches;
	//drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);
	////-- Show detected matches
	//imshow("Matches", img_matches);

	  //-- Step 2: Matching descriptor vectors with a FLANN based matcher
	// Since SURF is a floating-point descriptor NORM_L2 is used
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
	//-- Filter matches using the Lowe's ratio test
	const float ratio_thresh = 0.4f; 
	
	std::vector<DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		//if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		if (knn_matches[i][0].distance < ui.doubleSpinBox->value() * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
	//-- Draw matches
	Mat img_matches;
	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//-- Show detected matches
	putText(img_matches, QString("%1").arg((getTickCount() - t1) / getTickFrequency() / 1000).toStdString(), Point(10, 10), 1, 1, Scalar::all(128));
	imshow("Good Matches", img_matches);

}

void QTGUI::on_pushButton_13_clicked()
{

	Mat img_scene = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person.jpg");
	Mat img_object = cv::imread(QCoreApplication::applicationDirPath().toStdString() + "\\person2.jpg");

	if (img_object.empty() || img_scene.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return;
	}
	//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	Mat descriptors_object, descriptors_scene;
	detector->detectAndCompute(img_object, noArray(), keypoints_object, descriptors_object);
	detector->detectAndCompute(img_scene, noArray(), keypoints_scene, descriptors_scene);
	//-- Step 2: Matching descriptor vectors with a FLANN based matcher
	// Since SURF is a floating-point descriptor NORM_L2 is used
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(descriptors_object, descriptors_scene, knn_matches, 2);
	//-- Filter matches using the Lowe's ratio test
	const float ratio_thresh = 0.75f;
	std::vector<DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
	//-- Draw matches
	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, RANSAC);
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point2f(0, 0);
	obj_corners[1] = Point2f((float)img_object.cols, 0);
	obj_corners[2] = Point2f((float)img_object.cols, (float)img_object.rows);
	obj_corners[3] = Point2f(0, (float)img_object.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f((float)img_object.cols, 0),
		scene_corners[1] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f((float)img_object.cols, 0),
		scene_corners[2] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f((float)img_object.cols, 0),
		scene_corners[3] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f((float)img_object.cols, 0),
		scene_corners[0] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
}

