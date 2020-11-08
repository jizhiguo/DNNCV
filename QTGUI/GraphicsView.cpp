#include "graphicsview.h"


GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)

{
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    //���û��汳��ģʽ
    setCacheMode(CacheBackground);
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    //��������ͼ
    if (event->button() == Qt::LeftButton) {
        QPointF point = mapToScene(event->pos());
        m_MousePressPos << point;
        emit sigDrawPolygon(); //���ͻ��ƶ����
    }

    //����Ҽ����
    if (event->button() == Qt::RightButton)
    {
        m_MousePressPos.clear();
        emit sigDrawPolygon(); //���ͻ��ƶ����
    }

    QGraphicsView::mousePressEvent(event);
}
