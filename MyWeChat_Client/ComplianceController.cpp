#include "ComplianceController.h"

// 修复：加上参数 (QObject *parent)，并初始化基类
ComplianceController::ComplianceController(QObject *parent) : QObject(parent) {
}

QString ComplianceController::checkAndFilter(const QString &text) {
    QString filtered = text;
    // 简单合规性检查：替换敏感词
    return filtered.replace("笨蛋", "**");
}
