import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "找到用户"
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    standardButtons: Dialog.Yes | Dialog.No

    // 外部传入的目标用户名
    property string targetUser: ""

    // 定义确认信号
    signal confirmAdded(string user)

    Label {
        text: "是否添加 " + root.targetUser + " 为好友?"
    }

    onAccepted: {
        root.confirmAdded(root.targetUser)
        // 注意：Dialog.Yes 默认会触发 accepted，不需要手动 close，除非你覆盖了 standardButtons 的行为
    }
}
