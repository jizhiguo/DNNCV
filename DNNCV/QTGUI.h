#pragma once

#include <QtWidgets/QDialog>
#include "ui_QTGUI.h"
#include <QStandardItemModel>

class QTGUI : public QDialog
{
    Q_OBJECT

public:
    QTGUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::QTGUIClass ui;

    QStandardItemModel* model;//tableView������ģ��
    QString coconamesfilename;//���������ļ�·��
    QString imgfilename;//ͼƬ�ļ�·��
    QString yoloCfgfilename;//yolo�����ļ�·��
    QString yoloWeightsfilename;//yoloȨ���ļ�·��
    QString cocofilename;//yolo�����ļ�·��

    void load_coconames(QString& filename);
    void load_image(QString& filename);
    void setupTable1();
    void setupTable2();

private slots:
    void onOpenCoco();
    void onSaveCoco();
    void onResetCoco();
    void onLoadImage();
    void onDetection();
    void onAddPoint();
    void onDelPoint();
};

