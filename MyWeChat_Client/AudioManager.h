#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QAudioSource>
#include <QAudioSink>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QIODevice>
#include <QAudioFormat>

class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = nullptr);

    // 开始录音
    void startRecording();
    // 停止录音
    void stopRecording();

    // 播放音频数据块
    void playAudioChunk(const QByteArray &data);

signals:
    void audioDataReady(QByteArray data);

private slots:
    void onMicReadyRead();

private:
    QAudioSource *m_source = nullptr; // 麦克风
    QAudioSink *m_sink = nullptr;     // 扬声器
    QIODevice *m_inputDevice = nullptr;  // 录音流
    QIODevice *m_outputDevice = nullptr; // 播放流

    QAudioFormat m_format; // 音频格式
};

#endif // AUDIOMANAGER_H
