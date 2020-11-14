#pragma once

#include <QtWidgets/QDialog>
#include "ui_QTGUI.h"
#include <QStandardItemModel>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui_c.h>
#include<opencv2\xfeatures2d\nonfree.hpp>
#include "ImageWidget.h"

class QTGUI : public QDialog
{
	Q_OBJECT

public:
	QTGUI(QWidget* parent = Q_NULLPTR);

private:
	Ui::QTGUIClass ui;

	QStandardItemModel* model = NULL;//tableView的数据模型
	QString coconamesfilename;//分类配置文件路径
	QString imgfilename;//图片文件路径

	QString yoloCfgfilename;//yolo配置文件路径
	QString yoloWeightsfilename;//yolo权重文件路径
	QString yoloCocofilename;//yolo分类文件路径

	QGraphicsScene* scene = new QGraphicsScene;
	QGraphicsPolygonItem* polygon = NULL;
	ImageWidget* imageWidget = NULL;
	bool inPoly(cv::Rect box);
	bool inChecklist(int classID);

	void load_coconames(QString& filename);
	void load_image(QString& filename);
	void load_image(QImage image);
	void setupTable1();
	void setupTable2();

private slots:
	void onOpenCoco();
	void onSaveCoco();
	void onResetCoco();
	void onLoadImage();
	void doDetection();
	void onAddPoint();
	void onDelPoint();
	void onSigDrawPolygon();
	void on_pushButton_8_clicked();
	void on_pushButton_9_clicked();
	void on_pushButton_10_clicked();
	void on_pushButton_11_clicked();
	void on_pushButton_12_clicked();
	void on_pushButton_13_clicked();
};
