import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "添加好友"
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    standardButtons: Dialog.Cancel

    signal searchTriggered(string keyword)

    ColumnLayout {
        TextField {
            id: sInput
            placeholderText: "输入账号"
            Layout.fillWidth: true
            onAccepted: btn.clicked() // 回车键触发
        }
        Button {
            id: btn
            text: "搜索"
            Layout.fillWidth: true
            // 点击时发射信号
            onClicked: {
                if (sInput.text !== "") {
                    root.searchTriggered(sInput.text)
                }
            }
        }
    }
}
