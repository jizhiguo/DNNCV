#include "VideoVisibility.h"

//饿汉式单例模式
VideoVisibility* VideoVisibility::m_pInstance = new VideoVisibility();
VideoVisibility::VideoVisibility()
{
    //创建32路能见度值,500
    for (int i = 0; i < 32; i++) {
        m_VisibilityValue.append(500);
    }
}

VideoVisibility::~VideoVisibility()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

VideoVisibility* VideoVisibility::getInstance()
{
    return m_pInstance;
}

void VideoVisibility::SetVisibilityValue(const QString& videoID, double visibilityValue)
{
    m_mutexMsg.lock();
    //m_VisibilityValue[DBData::videoIDIndex.value(videoID)]=visibilityValue;
    m_mutexMsg.unlock();
}

double VideoVisibility::getVisibilityValue(const QString& videoID)
{
    m_mutexMsg.lock();
    //return m_VisibilityValue[DBData::videoIDIndex.value(videoID)];
    return 0;
    m_mutexMsg.unlock();
}

