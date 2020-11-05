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

    QString header = QString::fromLocal8Bit("�Ƿ�ʶ��   ID  Ӣ������    ���Ŷ�");//��ͷ
    QStringList headerList = header.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    model->setHorizontalHeaderLabels(headerList); //���ñ�ͷ����

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


    QString curPath = QCoreApplication::applicationDirPath(); //��ȡӦ�ó����·��
    QString aFileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��һ���ļ�"), curPath, QString::fromLocal8Bit("�������ļ�(*.txt);;�����ļ�(*.*)"));
    if (aFileName.isEmpty())//���δѡ���ļ����˳�
    {
        QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("��һ���ļ�"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return; 
    }

    QStringList fFileContent;//�ļ������ַ����б�
    QFile aFile(aFileName);  //���ļ���ʽ����
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //��ֻ���ı���ʽ���ļ�
    {
        QTextStream textStream(&aFile); //���ı�����ȡ�ļ�
        while (!textStream.atEnd())
        {
            QString str = textStream.readLine();//��ȡ�ļ���һ��
            fFileContent.append(str); //��ӵ� StringList
        }
        aFile.close();//�ر��ļ�

        model->clear();
        
        //��һ��StringList ��ȡ���ݣ���ʼ������Model
        int rowCnt = fFileContent.count(); //�ı���������1���Ǳ���
        model->setRowCount(rowCnt - 1); //ʵ����������
        //���ñ�ͷ
        QString header = fFileContent.at(0);//��1���Ǳ�ͷ
        //һ�������ո�TAB�ȷָ����������ַ����� �ֽ�Ϊһ��StringList
        QStringList headerList =header.split(QRegExp("\\t+"), QString::SkipEmptyParts);
        model->setHorizontalHeaderLabels(headerList); //���ñ�ͷ����
        //���ñ������
        QString aText;
        QStringList tmpList;
        int j;
        QStandardItem* aItem;
        for (int i = 1; i < rowCnt; i++)
        {
            QString aLineText = fFileContent.at(i); //��ȡ��������һ��
            //һ�������ո�TAB�ȷָ����������ַ����� �ֽ�Ϊһ��StringList
            QStringList tmpList = aLineText.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            for (j = 0; j < 4; j++) //tmpList����������FixedColumnCount, �̶���
            { //���������һ��
                aItem = new QStandardItem(tmpList.at(j));//����item
                model->setItem(i - 1, j, aItem); //Ϊģ�͵�ĳ������λ������Item
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
