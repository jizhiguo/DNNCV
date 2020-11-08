#pragma once

#include <QtWidgets/QDialog>
#include "ui_QTGUI.h"
#include <QStandardItemModel>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui_c.h>

class QTGUI : public QDialog
{
	Q_OBJECT

public:
	QTGUI(QWidget* parent = Q_NULLPTR);

private:
	Ui::QTGUIClass ui;

	QStandardItemModel* model = NULL;//tableView������ģ��
	QString coconamesfilename;//���������ļ�·��
	QString imgfilename;//ͼƬ�ļ�·��

	QString yoloCfgfilename;//yolo�����ļ�·��
	QString yoloWeightsfilename;//yoloȨ���ļ�·��
	QString yoloCocofilename;//yolo�����ļ�·��

	QGraphicsScene* scene = new QGraphicsScene;
	QGraphicsPolygonItem* polygon = NULL;
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
};
