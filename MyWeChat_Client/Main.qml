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
    title: "WeChat Pro"
    Material.theme: Material.Light
    Material.accent: "#07c160"

    property string currentChatUser: ""
    property string currentChatRemark: ""

    // --- 实例化刚才写的三个组件 ---

    // 1. 搜索弹窗
    SearchUserDialog {
        id: searchDialog
        // 响应组件发出的信号
        onSearchTriggered: (keyword) => client.searchUser(keyword)
    }

    // 2. 确认添加弹窗
    AddFriendConfirmDialog {
        id: confirmAddDialog
        // 响应确认信号
        onConfirmAdded: (user) => {
            client.addFriend(user)
            searchDialog.close() // 添加确认后，把背后的搜索框也关掉
        }
    }

    // 3. 修改备注弹窗
    SetRemarkDialog {
        id: remarkDialog
        onRemarkConfirmed: (user, remark) => client.updateRemark(user, remark)
    }

    Dialog {
        id: infoDialog
        property alias text: label.text
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        Label { id: label }
        standardButtons: Dialog.Ok
    }

    NetworkManager {
        id: client

        onMessageReceived: (friendName, content, isMe) => {
            if (root.currentChatUser === friendName) {
                chatModel.append({ "sender": "other", "text": content })
                client.clearUnread(friendName)
            }
            // 否则 C++ 自动刷新列表显示红点
        }

        onRegisterResult: (success, msg) => {
            infoDialog.text = msg
            infoDialog.open()
        }

        // 搜索结果回来后，控制弹窗显示
        onSearchResult: (found, username, id) => {
            if (found) {
                confirmAddDialog.targetUser = username
                confirmAddDialog.open()
            } else {
                infoDialog.text = "用户不存在"
                infoDialog.open()
            }
        }

        onOperationResult: (op, success, msg) => {
            if (!success) {
                infoDialog.text = msg
                infoDialog.open()
            }
        }
    }

    // --- 登录/注册页 ---
    ColumnLayout {
        anchors.centerIn: parent
        visible: !client.isLoggedIn
        spacing: 15
        width: parent.width * 0.8

        Label { text: "WeChat Pro"; font.pixelSize: 32; font.bold: true; Layout.alignment: Qt.AlignHCenter; color: Material.accent }
        TextField { id: ipField; text: "127.0.0.1"; placeholderText: "服务器 IP"; Layout.fillWidth: true }
        TextField { id: userField; placeholderText: "账号"; Layout.fillWidth: true }
        TextField { id: passField; placeholderText: "密码"; echoMode: TextInput.Password; Layout.fillWidth: true }

        RowLayout {
            Button {
                text: "登录"; Layout.fillWidth: true; highlighted: true
                onClicked: {
                    if(userField.text==="" || passField.text==="") { infoDialog.text="请填写账号密码"; infoDialog.open(); return }
                    client.connectToServer(ipField.text); client.login(userField.text, passField.text)
                }
            }
            Button {
                text: "注册"; Layout.fillWidth: true
                onClicked: {
                    if(userField.text==="" || passField.text==="") { infoDialog.text="请填写账号密码"; infoDialog.open(); return }
                    client.connectToServer(ipField.text); client.registerUser(userField.text, passField.text)
                }
            }
        }
    }

    // --- 主界面 ---
    Page {
        anchors.fill: parent
        visible: client.isLoggedIn

        header: ToolBar {
            RowLayout {
                anchors.fill: parent
                ToolButton { text: "←"; visible: swipe.currentIndex==1; onClicked: swipe.currentIndex=0 }
                Label {
                    text: swipe.currentIndex==0 ? "通讯录" : root.currentChatRemark
                    color: "white"; font.bold: true; font.pixelSize: 18
                    Layout.alignment: Qt.AlignCenter
                }
                ToolButton { text: "+"; visible: swipe.currentIndex==0; onClicked: searchDialog.open() } // 这里直接 open()
            }
        }

        SwipeView {
            id: swipe
            anchors.fill: parent
            interactive: false

            // 0. 通讯录页
            ListView {
                id: friendView
                model: client.friendList
                delegate: ItemDelegate {
                    width: parent.width
                    text: modelData.displayName + (modelData.unread ? " [新消息]" : "")

                    Rectangle {
                        visible: modelData.unread
                        width: 10; height: 10; radius: 5; color: "red"
                        anchors.right: parent.right; anchors.rightMargin: 15; anchors.verticalCenter: parent.verticalCenter
                    }

                    onClicked: {
                        root.currentChatUser = modelData.username
                        root.currentChatRemark = modelData.displayName
                        chatModel.clear()
                        var history = client.getChatHistory(modelData.username)
                        for(var i=0; i<history.length; i++) chatModel.append(history[i])
                        client.clearUnread(modelData.username)
                        swipe.currentIndex = 1
                    }

                    MouseArea { anchors.fill: parent; acceptedButtons: Qt.RightButton; onClicked: menu.popup() }

                    Menu {
                        id: menu
                        MenuItem {
                            text: "修改备注"
                            // 这里调用组件，并传递参数
                            onTriggered: {
                                remarkDialog.targetUser = modelData.username
                                remarkDialog.open()
                            }
                        }
                        MenuItem { text: "删除好友"; onTriggered: client.deleteFriend(modelData.username) }
                    }
                }
            }

            // 1. 聊天页
            ColumnLayout {
                ListView {
                    id: chatView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: ListModel { id: chatModel }
                    spacing: 10; clip: true
                    onCountChanged: Qt.callLater(()=> positionViewAtEnd())

                    delegate: Row {
                        width: chatView.width
                        layoutDirection: model.sender === "me" ? Qt.RightToLeft : Qt.LeftToRight
                        spacing: 10; padding: 10
                        Rectangle { width: 35; height: 35; radius: 4; color: model.sender==="me"?"#ededed":"#07c160" }
                        Rectangle {
                            width: Math.min(Math.max(txt.implicitWidth+20, 40), chatView.width * 0.7)
                            height: txt.implicitHeight+20
                            color: model.sender==="me"?"#95ec69":"white"; radius: 5
                            Text { id: txt; text: model.text; anchors.centerIn: parent; width: parent.width-20; wrapMode: Text.WrapAnywhere }
                        }
                    }
                }
                RowLayout {
                    TextField { id: input; Layout.fillWidth: true; placeholderText: "发送消息..." }
                    Button {
                        text: "发送"; highlighted: true
                        onClicked: {
                            client.sendMessage(root.currentChatUser, input.text)
                            chatModel.append({"sender": "me", "text": input.text})
                            input.text = ""
                        }
                    }
                }
            }
        }
    }
}
