#pragma once

#include <QtWidgets/QDialog>
#include "ui_QTGUI.h"

class QTGUI : public QDialog
{
    Q_OBJECT

public:
    QTGUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::QTGUIClass ui;
};
