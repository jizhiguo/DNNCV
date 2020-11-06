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
    connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(onDetection()));
    connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(onAddPoint()));
    connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(onDelPoint()));
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
    //column 1,2,read only
    //column 3,combobox
    cbd->setColumn(0);
    ui.tableView->setItemDelegateForColumn(1, rod);
    ui.tableView->setItemDelegateForColumn(2, rod);
    ui.tableView->setItemDelegateForColumn(3, combo);

    //model setup
    model = new QStandardItemModel;
    model->setRowCount(10);
    model->setColumnCount(4);
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
    coconamesfilename = filename;
}

void QTGUI::load_coconames(QString& filename) {

    QStringList fileContent;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
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
            for (int j = 0; j < 4; j++)
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
        ui.label->setText(u8"图片不存在");
        return;
    }
    ui.label->setText(u8"");
    ui.label->setPixmap(QPixmap(filename));
    ui.label->setScaledContents(true);
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

void QTGUI::onDetection()
{
    QImage img; 
    img.load(imgfilename);
    Detector d;
    QString result = d.onDetection(&img);

    ui.textBrowser->append(result);
    ui.label->clear();
    ui.label->setPixmap(QPixmap::fromImage(img));
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

