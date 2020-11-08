#include "QTGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTGUI w;
    w.show();
    return a.exec();
}
