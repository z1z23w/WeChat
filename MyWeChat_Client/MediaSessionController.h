#ifndef MEDIASESSIONCONTROLLER_H
#define MEDIASESSIONCONTROLLER_H

#include <QObject>
#include "AudioManager.h"

class MediaSessionController : public QObject {
    Q_OBJECT
public:
    // 构造函数声明
    explicit MediaSessionController(QObject *parent = nullptr);

    void startSession();
    void endSession();

    // 【关键】这里必须叫 playIncomingStream，才能和 cpp 对应
    void playIncomingStream(const QByteArray &data);

signals:
    // 【关键】这里必须叫 mediaStreamGenerated，才能和 cpp 对应
    void mediaStreamGenerated(QByteArray data);

private slots:
    void onAudioData(QByteArray data);

private:
    AudioManager *m_audio;
};

#endif // MEDIASESSIONCONTROLLER_H
