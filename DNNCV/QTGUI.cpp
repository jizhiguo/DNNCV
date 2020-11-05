#include "QTGUI.h"
#include "CustomTableItem.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtCore\qfile.h>
#include <qtextstream.h>
QTGUI::QTGUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    CheckBoxDelegate* cbd = new CheckBoxDelegate;
    cbd->setColumn(0);

    ComboDelegate* combo = new ComboDelegate;
    QString itemsStr = "50%   90%  10%   20%  30%   40%  50%  60%  70%  80%  100%";
    QStringList itemList = itemsStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    combo->setItems(itemList);
    ui.tableView->setItemDelegateForColumn(4, combo);

    ReadOnlyDelegate* rod = new ReadOnlyDelegate;
    ui.tableView->setItemDelegateForColumn(1, rod);
    ui.tableView->setItemDelegateForColumn(2, rod);
    ui.tableView->setItemDelegateForColumn(3, rod);

    model = new QStandardItemModel;
    model->setRowCount(80);
    model->setColumnCount(4);

    QString header = QString::fromLocal8Bit("是否识别   ID  英文名称    置信度");//表头
    QStringList headerList = header.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    model->setHorizontalHeaderLabels(headerList); //设置表头文字

    for (int row = 0; row < 80; row++)
    {
        for (int col = 0; col < 4; col++) {
            model->setData(model->index(row, col), QString( "%1:%2").arg(row).arg(col), Qt::EditRole);
            model->setData(model->index(row, 0), false, Qt::EditRole);
            model->setData(model->index(row, 4), 50, Qt::EditRole);
        }
    }

    ui.tableView->setModel(model);
    ui.tableView->setItemDelegate(cbd);
    ui.tableView->resizeColumnsToContents();
    ui.tableView->verticalHeader()->setHidden(true);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(coconames_open()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(coconames_save()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(coconames_reset()));
}

void QTGUI::coconames_open()
{


    QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString aFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开一个文件"), curPath, QString::fromLocal8Bit("井数据文件(*.txt);;所有文件(*.*)"));
    if (aFileName.isEmpty())//如果未选择文件，退出
    {
        QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("打开一个文件"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return; 
    }

    QStringList fFileContent;//文件内容字符串列表
    QFile aFile(aFileName);  //以文件方式读出
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        QTextStream textStream(&aFile); //用文本流读取文件
        while (!textStream.atEnd())
        {
            QString str = textStream.readLine();//读取文件的一行
            fFileContent.append(str); //添加到 StringList
        }
        aFile.close();//关闭文件

        model->clear();
        
        //从一个StringList 获取数据，初始化数据Model
        int rowCnt = fFileContent.count(); //文本行数，第1行是标题
        model->setRowCount(rowCnt - 1); //实际数据行数
        //设置表头
        QString header = fFileContent.at(0);//第1行是表头
        //一个或多个空格、TAB等分隔符隔开的字符串， 分解为一个StringList
        QStringList headerList =header.split(QRegExp("\\t+"), QString::SkipEmptyParts);
        model->setHorizontalHeaderLabels(headerList); //设置表头文字
        //设置表格数据
        QString aText;
        QStringList tmpList;
        int j;
        QStandardItem* aItem;
        for (int i = 1; i < rowCnt; i++)
        {
            QString aLineText = fFileContent.at(i); //获取数据区的一行
            //一个或多个空格、TAB等分隔符隔开的字符串， 分解为一个StringList
            QStringList tmpList = aLineText.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            for (j = 0; j < 4; j++) //tmpList的行数等于FixedColumnCount, 固定的
            { //不包含最后一列
                aItem = new QStandardItem(tmpList.at(j));//创建item
                model->setItem(i - 1, j, aItem); //为模型的某个行列位置设置Item
            }
        }
    }
        
}

void QTGUI::coconames_save()
{



    
}

void QTGUI::coconames_reset()
{


}
