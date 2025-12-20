import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Controls.Material
import we

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 450
    height: 750
    title: "WeChat Demo"

    Material.theme: Material.Light
    Material.accent: Material.Teal
    Material.primary: Material.BlueGrey

    property string currentChatTarget: "" // 当前正在和谁聊天

    P2PClient {
        id: client
        onMessageReceived: (msg) => {
            // 收到消息，显示在聊天框里
            chatModel.append({"sender": "other", "text": msg})
        }
    }

    // ================= 1. 登录界面 =================
    Rectangle {
        id: loginPage
        anchors.fill: parent
        color: "white"
        z: 999
        visible: !client.isLoggedIn

        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.8
            spacing: 20

            Label {
                text: "WeChat"
                font.pixelSize: 40
                font.bold: true
                color: Material.accent
                Layout.alignment: Qt.AlignHCenter
            }
            TextField {
                id: usernameField
                placeholderText: "用户名 (例: User1)"
                Layout.fillWidth: true
                text: "User1"
            }
            TextField {
                id: passwordField
                placeholderText: "密码"
                echoMode: TextInput.Password
                Layout.fillWidth: true
                text: "123"
            }
            TextField {
                id: serverIpField
                placeholderText: "服务器IP"
                text: "127.0.0.1"
                Layout.fillWidth: true
            }
            Button {
                text: "登录 / 自动注册"
                Layout.fillWidth: true
                highlighted: true
                onClicked: client.loginToServer(usernameField.text, passwordField.text, serverIpField.text)
            }
            Label {
                text: client.status
                color: "red"
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    // ================= 2. 主界面 =================
    Page {
        id: mainInterface
        anchors.fill: parent
        visible: client.isLoggedIn

        header: ToolBar {
            Label {
                text: swipeView.currentIndex === 0
                      ? (currentChatTarget === "" ? "请在通讯录选择好友" : "与 " + currentChatTarget + " 聊天中")
                      : "通讯录 (在线用户)"
                color: "white"
                font.bold: true
                font.pixelSize: 18
                anchors.centerIn: parent
            }
        }

        SwipeView {
            id: swipeView
            anchors.fill: parent
            interactive: false // 禁止滑动切换，必须点底部按钮

            // --- 页面 0: 聊天窗口 ---
            Page {
                ColumnLayout {
                    anchors.fill: parent

                    // 聊天记录列表
                    // Main.qml 中的 ListView 部分

                    ListView {
                        id: chatView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: ListModel { id: chatModel }

                        // 增加间隔，不让消息贴着边
                        leftMargin: 10
                        rightMargin: 10
                        topMargin: 10
                        bottomMargin: 10
                        spacing: 15 // 消息之间的垂直间距

                        // 每次添加新消息，自动滚动到底部
                        onCountChanged: {
                            Qt.callLater(function() {
                                positionViewAtEnd()
                            })
                        }

                        delegate: Item {
                            id: messageRow
                            width: chatView.width - 20 // 减去左右margin

                            // 动态计算高度：取 头像高度 和 气泡高度 中较大的那个
                            height: Math.max(avatar.height, bubble.height)

                            property bool isMe: model.sender === "me"

                            // 【核心魔法】
                            // 如果是"我"，布局方向设为 RightToLeft，元素就会自动靠右，且顺序变成 [头像] [气泡]
                            // 如果是"别人"，布局方向设为 LeftToRight，元素靠左，顺序变成 [头像] [气泡]
                            Row {
                                anchors.fill: parent
                                layoutDirection: isMe ? Qt.RightToLeft : Qt.LeftToRight
                                spacing: 10

                                // 1. 头像
                                Rectangle {
                                    id: avatar
                                    width: 40
                                    height: 40
                                    radius: 5 // 微信是圆角矩形
                                    color: isMe ? "#ededed" : "#2e7d32" // 简单的颜色区分

                                    Text {
                                        anchors.centerIn: parent
                                        text: isMe ? "我" : model.sender.charAt(0).toUpperCase() // 取名字首字母
                                        color: isMe ? "black" : "white"
                                        font.pixelSize: 14
                                    }
                                }

                                // 2. 聊天气泡
                                Rectangle {
                                    id: bubble
                                    // 气泡宽度自适应，但最大不超过屏幕的 70%
                                    width: Math.min(msgText.implicitWidth + 24, chatView.width * 0.7)
                                    height: msgText.implicitHeight + 24 // 文字高度 + 上下padding

                                    color: isMe ? "#95ec69" : "#ffffff" // 微信经典绿 vs 白色
                                    radius: 6

                                    // 只有别人发的消息需要显示名字（可选，这里稍微做个三角形装饰效果更像）
                                    // 为了简单，我们先只做纯气泡

                                    Text {
                                                        id: msgText
                                                        text: model.text
                                                        anchors.centerIn: parent
                                                        width: parent.width - 24 // 减去左右padding

                                                        font.pixelSize: 15
                                                        color: "black"

                                                        // 【修正】只保留这一行，删掉原来多余的那行 Text.Wrap
                                                        wrapMode: Text.WrapAnywhere
                                                    }
                                }

                                // 3. 占位符 (把气泡挤向一侧)
                                Item {
                                    width: 1
                                    height: 1
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    // 底部输入栏
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        TextField {
                            id: inputField
                            Layout.fillWidth: true
                            placeholderText: currentChatTarget === "" ? "请先选择好友" : "输入消息..."
                            enabled: currentChatTarget !== ""
                        }
                        Button {
                            text: "发送"
                            enabled: currentChatTarget !== "" && inputField.text !== ""
                            highlighted: true
                            onClicked: {
                                // 1. 发给服务器转发
                                client.sendMessageTo(currentChatTarget, inputField.text)
                                // 2. 自己界面显示
                                chatModel.append({"sender": "me", "text": inputField.text})
                                inputField.text = ""
                            }
                        }
                    }
                }
            }

            // --- 页面 1: 通讯录列表 ---
            Page {
                ListView {
                    anchors.fill: parent
                    model: client.onlineUsers // 绑定 C++ 的好友列表
                    delegate: ItemDelegate {
                        width: parent.width
                        text: modelData // 显示用户名

                        // 不要和自己聊天
                        visible: modelData !== usernameField.text
                        height: visible ? implicitHeight : 0

                        highlighted: swipeView.currentIndex === 1
                        onClicked: {
                            // 点击好友，设置聊天目标，并跳转到聊天页
                            rootWindow.currentChatTarget = modelData
                            chatModel.clear() // 简单的 Demo，切换好友清空记录
                            chatModel.append({"sender": "other", "text": "已连接到 " + modelData})
                            swipeView.currentIndex = 0
                            tabBar.currentIndex = 0
                        }
                    }
                }
            }
        }

        footer: TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            TabButton {
                text: "聊天"
                onClicked: swipeView.currentIndex = 0
            }
            TabButton {
                text: "通讯录"
                onClicked: swipeView.currentIndex = 1
            }
        }
    }
}
