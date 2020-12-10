#include "QTGUI.h"
#include <QtWidgets/QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    QTGUI w;
//    w.show();
//    return a.exec();
//}

#include "HalconMatcher.h"

int main(int argc, char* argv[])
{
    std::string imgTemplatePath = "D:/temp/temp/p11.jpg";
    std::string imgSrcPath = "D:/temp/temp/wall2.jpg";
    HImage imgSrc(imgSrcPath.c_str());

    HalconMatcher halconMatcher;
    halconMatcher.addModel(imgTemplatePath);
    halconMatcher.addModel(imgTemplatePath);
    halconMatcher.addModel(imgTemplatePath);
    int v = halconMatcher.doMatching(imgSrc);
    return 0;
}
