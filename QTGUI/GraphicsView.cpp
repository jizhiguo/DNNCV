#include "graphicsview.h"


GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)

{
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    //设置缓存背景模式
    setCacheMode(CacheBackground);
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    //鼠标左键绘图
    if (event->button() == Qt::LeftButton) {
        QPointF point = mapToScene(event->pos());
        m_MousePressPos << point;
        emit sigDrawPolygon(); //发送绘制多边形
    }

    //鼠标右键清除
    if (event->button() == Qt::RightButton)
    {
        m_MousePressPos.clear();
        emit sigDrawPolygon(); //发送绘制多边形
    }

    QGraphicsView::mousePressEvent(event);
}
