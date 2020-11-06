#include "Detector.h"

Detector::Detector()
{
}

Detector::Detector(QString& sCfgFile, QString& sWeightFile, QString& sCoconamesFile)
{
}

QString Detector::onDetection(QImage* img)
{
	return u8"test";
}
