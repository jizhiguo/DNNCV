#pragma once

#include "detector_global.h"
#include <QString>
#include <QImage>

class DETECTOR_EXPORT Detector
{
public:
    Detector();
    Detector(QString sCfgFile, QString sWeightFile, QString sCoconamesFile);
    
public:
    QString onDetection(QImage& img);

private:
    QString cfg;
    QString weights;
    QString coconames;

private:

};