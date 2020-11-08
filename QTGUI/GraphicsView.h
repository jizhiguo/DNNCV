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
    QPolygonF m_MousePressPos;   //����εĶ���

signals:
    void sigDrawPolygon();   //���ͻ��ƶ�����ź�
protected:
    virtual  void  mousePressEvent(QMouseEvent* event);
};

#endif // GRAPHICSVIEW_H
