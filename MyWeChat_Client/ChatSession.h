#ifndef CHATSESSION_H
#define CHATSESSION_H
#include <QObject>

class ChatSession : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentTarget READ currentTarget WRITE setCurrentTarget NOTIFY targetChanged)
public:
    explicit ChatSession(QObject *parent = nullptr) : QObject(parent) {}
    QString currentTarget() const { return m_target; }
    void setCurrentTarget(const QString &t) {
        if(m_target != t) { m_target = t; emit targetChanged(); }
    }
signals:
    void targetChanged();
private:
    QString m_target;
};
#endif
