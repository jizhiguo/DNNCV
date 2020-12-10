#ifndef VIDEOVISIBILITY_H
#define VIDEOVISIBILITY_H

#include <QMutex>
#include <QQueue>

class VideoVisibility
{
public:
    //析构函数
    ~VideoVisibility();
    //获取单例
    static VideoVisibility* getInstance();
    //添加消息
    void SetVisibilityValue(const QString& videoID, double visibilityValue);
    //获取消息
    double getVisibilityValue(const QString& videoID);

private:
    //私有构造
    VideoVisibility();
private:
    //静态类对象
    static VideoVisibility* m_pInstance;
    QMutex m_mutexMsg;
    QList<double> m_VisibilityValue;
};

#endif // VIDEOVISIBILITY_H
