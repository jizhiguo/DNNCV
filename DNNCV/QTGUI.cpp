#include "QTGUI.h"
#include <QStandardItemModel>
#include "CustomTableItem.h"

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

    QStandardItemModel* model = new QStandardItemModel;
    model->setRowCount(80);
    model->setColumnCount(5);

    QString header = QString::fromLocal8Bit("是否识别   ID  英文名称    中文名称    置信度");//表头
    QStringList headerList = header.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    model->setHorizontalHeaderLabels(headerList); //设置表头文字

    for (int row = 0; row < 80; row++)
    {
        for (int col = 0; col < 5; col++) {
            model->setData(model->index(row, col), QString( "%1:%2").arg(row).arg(col), Qt::EditRole);
            model->setData(model->index(row, 0), false, Qt::EditRole);
            model->setData(model->index(row, 4), 50, Qt::EditRole);
        }
    }

    ui.tableView->setModel(model);
    ui.tableView->setItemDelegate(cbd);
    ui.tableView->resizeColumnsToContents();
    ui.tableView->verticalHeader()->setHidden(true);
}
