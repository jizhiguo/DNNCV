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

QTGUI::QTGUI(QWidget *parent)
    : QDialog(parent)
{
    
    ui.setupUi(this);

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

    //connect button signals & slot functions
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(coconames_open()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(coconames_save()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(coconames_reset()));
   
    ui.videoWidget = new QVideoWidget;
    play();
}

void QTGUI::coconames_open()
{
    QString curPath = QCoreApplication::applicationDirPath(); 
    QString filename = QFileDialog::getOpenFileName(this, u8"��һ���ļ�", curPath, u8"�����ļ�(*.txt);;�����ļ�(*.*)");
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

        int rowCnt = fileContent.count(); //�ı���������1���Ǳ���
        model->clear();
        model->setRowCount(rowCnt - 1);
        QString header = fileContent.at(0);//��1���Ǳ�ͷ
        QStringList headerList = header.split(QRegExp("\\t+"), QString::SkipEmptyParts);
        model->setHorizontalHeaderLabels(headerList); //���ñ�ͷ����
        //���ñ������
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

void QTGUI::play()
{
    QVideoWidget* videoWidget = ui.videoWidget;
    QMediaPlayer* player = new QMediaPlayer;
    QMediaPlaylist* playlist = new QMediaPlaylist;

    videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
    videoWidget->setFullScreen(true);


    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile("d:\\a.mp4"));
    player->setPlaylist(playlist);

    QFile file("d:\\a.mp4");
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "Could not open file";

    player->setVideoOutput(videoWidget);
    player->setPlaylist(playlist);

    player->play();

  //  QWidget* widget = new QWidget;
  //  QVBoxLayout* layout = new QVBoxLayout;

  //  QMediaPlayer* player = new QMediaPlayer;
  //  QVideoWidget* videoWidget = new QVideoWidget;
  //  QMediaPlaylist* playlist = new QMediaPlaylist;

  //  videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
  //  videoWidget->setFullScreen(true);

  //  layout->addWidget(videoWidget);
  //  widget->setLayout(layout);

  //  playlist->clear();
  //  playlist->addMedia(QUrl::fromLocalFile("d:\\a.mp4"));
  //  player->setPlaylist(playlist);

  ///*  QFile file("d:\\a.mp4");
  //  if (!file.open(QIODevice::ReadOnly))
  //      qDebug() << "Could not open file";*/

  //  player->setVideoOutput(videoWidget);
  //  player->setPlaylist(playlist);

  //  widget->showFullScreen();
  //  player->play();

}

void QTGUI::coconames_save()
{
    if (coconamesfilename.isEmpty())
    {
        QString curPath = QCoreApplication::applicationDirPath();
        QString coconamesfilename = QFileDialog::getOpenFileName(this, u8"����һ���ļ�", curPath, u8"�����ļ�(*.txt);;�����ļ�(*.*)");
        if (coconamesfilename.isEmpty())
        {
            return;
        }
    }

    QFile aFile(coconamesfilename);
    if (!(aFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)))
        return; //�Զ�д������ԭ�����ݷ�ʽ���ļ�

    QTextStream aStream(&aFile); //���ı�����ȡ�ļ�
    QStandardItem* aItem;
    QString str;
    //��ȡ��ͷ����
    for (int i = 0; i < model->columnCount(); i++)
    {
        aItem = model->horizontalHeaderItem(i); //��ȡ��ͷ��������
        str = str + aItem->text() + "\t\t";  //��TAB������
    }
    aStream << str << "\n";  //���з� \n

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

void QTGUI::coconames_reset()
{
    load_coconames(coconamesfilename);

}
