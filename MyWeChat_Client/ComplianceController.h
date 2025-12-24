#ifndef COMPLIANCECONTROLLER_H
#define COMPLIANCECONTROLLER_H

#include <QObject>

class ComplianceController : public QObject {
    Q_OBJECT
public:
    // 这里只声明，不要加 {}
    explicit ComplianceController(QObject *parent = nullptr);

    QString checkAndFilter(const QString &text);
};

#endif // COMPLIANCECONTROLLER_H
