#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QWidget>

#include <QGraphicsView>
#include <QMouseEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget* parent = 0);
    QPolygonF m_MousePressPos;   //多边形的顶点

signals:
    void sigDrawPolygon();   //发送绘制多边形信号
protected:
    virtual  void  mousePressEvent(QMouseEvent* event);
};

#endif // GRAPHICSVIEW_H
