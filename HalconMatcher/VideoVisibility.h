#ifndef VIDEOVISIBILITY_H
#define VIDEOVISIBILITY_H

#include <QMutex>
#include <QQueue>

class VideoVisibility
{
public:
    //��������
    ~VideoVisibility();
    //��ȡ����
    static VideoVisibility* getInstance();
    //�����Ϣ
    void SetVisibilityValue(const QString& videoID, double visibilityValue);
    //��ȡ��Ϣ
    double getVisibilityValue(const QString& videoID);

private:
    //˽�й���
    VideoVisibility();
private:
    //��̬�����
    static VideoVisibility* m_pInstance;
    QMutex m_mutexMsg;
    QList<double> m_VisibilityValue;
};

#endif // VIDEOVISIBILITY_H
