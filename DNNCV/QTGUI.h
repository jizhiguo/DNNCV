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

    QStandardItemModel* model;
private slots:
    void coconames_open();
    void coconames_save();
    void coconames_reset();
};
