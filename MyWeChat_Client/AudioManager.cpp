#include "AudioManager.h"
#include <QDebug>

AudioManager::AudioManager(QObject *parent) : QObject(parent) {
    // 1. 配置音频格式 (模拟电话音质：8000Hz, 单声道, 16位)
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(QAudioFormat::Int16);

    // 2. 初始化扬声器
    QAudioDevice outputDev = QMediaDevices::defaultAudioOutput();
    if (outputDev.isNull()) {
        qDebug() << "❌ 没找到扬声器";
    } else {
        m_sink = new QAudioSink(outputDev, m_format, this);
        // 启动扬声器，随时准备接收数据
        m_outputDevice = m_sink->start();
    }

    // 3. 初始化麦克风 (先不 start)
    QAudioDevice inputDev = QMediaDevices::defaultAudioInput();
    if (inputDev.isNull()) {
        qDebug() << "❌ 没找到麦克风";
    } else {
        m_source = new QAudioSource(inputDev, m_format, this);
    }
}

void AudioManager::startRecording() {
    if (!m_source) return;
    qDebug() << "🎤 麦克风开启";

    m_inputDevice = m_source->start();
    connect(m_inputDevice, &QIODevice::readyRead, this, &AudioManager::onMicReadyRead);
}

void AudioManager::stopRecording() {
    if (!m_source) return;
    qDebug() << "⏹ 麦克风关闭";

    m_source->stop();
    if (m_inputDevice) {
        disconnect(m_inputDevice, &QIODevice::readyRead, this, &AudioManager::onMicReadyRead);
        m_inputDevice = nullptr;
    }
}

void AudioManager::onMicReadyRead() {
    if (!m_inputDevice) return;

    // 读取麦克风里的数据
    QByteArray data = m_inputDevice->readAll();

    if (!data.isEmpty()) {
        qDebug() << "🎤 [AudioManager] 采集到音频数据，大小:" << data.size() << "字节";

        emit audioDataReady(data);
    } else {
        qDebug() << "⚠️ [AudioManager] 麦克风有信号，但数据为空";
    }
}

void AudioManager::playAudioChunk(const QByteArray &data) {
    if (m_outputDevice && m_sink) {
        // 写入扬声器
        m_outputDevice->write(data);
    }
}
