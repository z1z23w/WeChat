import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import we 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 400
    height: 700
    title: "WeChat"
    Material.theme: Material.Light
    Material.accent: "#07c160"

    // --- Control Instance ---
    CommunicationController {
        id: controller
        onNotificationTriggered: (t, m) => notifPanel.show(t, m)
        onMessageReceived: (from, content) => {
            if(controller.currentSession.currentTarget === from) {
                chatModel.append({"sender": "other", "text": content})
            }
        }
    }

    // --- Boundary Components ---
    NotificationPanel { id: notifPanel }

    // --- Login View ---
    ColumnLayout {
        anchors.centerIn: parent
        visible: controller.currentUser.status !== "Online"
        spacing: 15
        width: 300

        TextField { id: ipInput; text: "127.0.0.1"; placeholderText: "Server IP"; Layout.fillWidth: true }
        TextField { id: userInput; placeholderText: "Username"; Layout.fillWidth: true }
        TextField { id: passInput; placeholderText: "Password"; echoMode: TextInput.Password; Layout.fillWidth: true }

        RowLayout {
            Button {
                text: "Login"
                Layout.fillWidth: true
                onClicked: {
                    controller.connectToServer(ipInput.text)
                    controller.login(userInput.text, passInput.text)
                }
            }
            Button {
                text: "Register"
                Layout.fillWidth: true
                onClicked: {
                    controller.connectToServer(ipInput.text)
                    controller.registerUser(userInput.text, passInput.text)
                }
            }
        }
    }

    // --- Main Chat View ---
    Page {
        anchors.fill: parent
        visible: controller.currentUser.status === "Online"

        header: ToolBar {
            RowLayout {
                anchors.fill: parent
                ToolButton { text: "Contacts"; onClicked: swipe.currentIndex = 0 }
                Label {
                    text: swipe.currentIndex === 0 ? "Contacts" : controller.currentSession.currentTarget
                    Layout.alignment: Qt.AlignCenter
                    color: "white"
                    font.bold: true
                }
                ToolButton {
                    text: "+"
                    visible: swipe.currentIndex === 0
                    onClicked: searchDialog.open()
                }
            }
        }

        SwipeView {
            id: swipe
            anchors.fill: parent
            interactive: false

            // 1. Contact List
            ListView {
                model: controller.friendList
                delegate: ItemDelegate {
                    width: parent.width
                    text: modelData.username
                    onClicked: {
                        controller.selectFriend(modelData.username)
                        chatModel.clear()
                        swipe.currentIndex = 1
                    }
                }
            }

            // 2. Chat Session
            ColumnLayout {
                ListView {
                    id: chatView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: ListModel { id: chatModel }
                    clip: true
                    delegate: Row {
                        spacing: 10; padding: 10
                        layoutDirection: model.sender==="me"?Qt.RightToLeft:Qt.LeftToRight
                        width: parent.width
                        Rectangle { width: 40; height: 40; color: "gray"; radius: 20 }
                        Rectangle {
                            width: txt.implicitWidth+20; height: txt.implicitHeight+20
                            color: model.sender==="me"?"#95ec69":"white"; radius: 5
                            Text { id: txt; text: model.text; anchors.centerIn: parent }
                        }
                    }
                }

                RowLayout {
                    TextField { id: msgInput; Layout.fillWidth: true }
                    Button {
                        text: "Voice"
                        onPressed: controller.startMediaSession()
                        onReleased: controller.endMediaSession()
                    }
                    Button {
                        text: "Send"
                        onClicked: {
                            controller.sendMessage(msgInput.text)
                            chatModel.append({"sender":"me", "text":msgInput.text})
                            msgInput.text = ""
                        }
                    }
                }
            }
        }
    }

    // Search Dialog (简略版)
    Dialog {
        id: searchDialog
        title: "Search User"
        x: (parent.width-width)/2; y: (parent.height-height)/2
        modal: true
        standardButtons: Dialog.Cancel
        ColumnLayout {
            TextField { id: sInput }
            Button {
                text: "Search"
                onClicked: controller.searchUser(sInput.text)
            }
            Button {
                text: "Add"
                onClicked: { controller.addFriend(sInput.text); searchDialog.close() }
            }
        }
    }
}
