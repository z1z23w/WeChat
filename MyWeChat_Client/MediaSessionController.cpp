#include "MediaSessionController.h"

// 构造函数实现
MediaSessionController::MediaSessionController(QObject *parent) : QObject(parent) {
    m_audio = new AudioManager(this);
    // 连接底层音频信号
    connect(m_audio, &AudioManager::audioDataReady, this, &MediaSessionController::onAudioData);
}

void MediaSessionController::startSession() {
    if (m_audio) m_audio->startRecording();
}

void MediaSessionController::endSession() {
    if (m_audio) m_audio->stopRecording();
}

// 【关键】对应头文件的 playIncomingStream
void MediaSessionController::playIncomingStream(const QByteArray &data) {
    if (m_audio) m_audio->playAudioChunk(data);
}

// 内部槽函数
void MediaSessionController::onAudioData(QByteArray data) {
    // 【关键】对应头文件的 mediaStreamGenerated
    emit mediaStreamGenerated(data);
}
