import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "设置备注"
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    property string targetUser: ""

    // 信号：谁，改成了什么
    signal remarkConfirmed(string user, string remark)

    TextField {
        id: rInput
        placeholderText: "输入新备注"
        Layout.fillWidth: true
    }

    // 每次打开弹窗时，清空输入框或者填入旧备注（这里先清空）
    onOpened: rInput.text = ""

    onAccepted: {
        root.remarkConfirmed(root.targetUser, rInput.text)
    }
}
