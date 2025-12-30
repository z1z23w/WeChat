import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "系统通知"

    // 1. 窗口居中逻辑
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    modal: true
    standardButtons: Dialog.Ok

    // 2. 必须把这个函数加回来，否则 NetizenChatUI 找不到它！
    function show(t, m) {
        root.title = t
        msgLabel.text = m
        root.open()
    }

    // 3. 布局修复（解决 Binding loop 警告）
    contentItem: ColumnLayout {
        Label {
            id: msgLabel
            text: ""

            // 自动换行，防止内容过长撑爆窗口
            wrapMode: Text.Wrap
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignHCenter

            // 内边距，让文字不贴边
            padding: 20
        }
    }
}
