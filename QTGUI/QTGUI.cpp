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
#include "imagewidget.h"
#include "GraphicsView.h"
#include "../Detector/Detector.h"


QTGUI::QTGUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setupTable1();
    setupTable2();
    //connect button signals & slot functions
    connect(ui.pushButton  , SIGNAL(clicked()), this, SLOT(onOpenCoco()));
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
    int rows=4;
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
    ImageWidget* imageWidget;
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
        for (int j = 0; j < model->rowCount(); j++)
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
    aItem = new QStandardItem(QString("%1").arg(model->rowCount()- 1));
    model->setItem(model->rowCount() - 1, 0, aItem);
}

void QTGUI::onDelPoint()
{
    QStandardItemModel* model = (QStandardItemModel*)(ui.tableView_2->model());
    model->removeRow(model->rowCount()-1);
}

void QTGUI::onSigDrawPolygon()
{
    //clear polygon in scene
    QList<QGraphicsItem*> itemList = scene->items();
    for (auto i = 0; i < itemList.size(); i++) {
        if ((itemList[i]->type()== QGraphicsPolygonItem::Type)) {//多边形类型的图元
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
    
    size_t len = out_Boxes.size();
    cv::Rect cr;
    for (size_t i = 0; i < len; i++) {
        //id,name,conf,in poly,in checklist
        cr = out_Boxes[i];
        ui.textBrowser->append(QString(u8"id:%1").arg(out_ClassIds[i]));
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
        return  polygon->polygon().containsPoint(QPointF(box.x, box.y), Qt::WindingFill);
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
