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
    height: 700
    title: "P2P Chat - " + client.localIp

    // --- 全局样式 ---
    Material.theme: Material.Light
    Material.accent: Material.Teal
    Material.primary: Material.BlueGrey

    property color myBubbleColor: "#dcf8c6"
    property color peerBubbleColor: "#ffffff"
    property color bgColor: "#ece5dd"

    P2PClient {
        id: client
        onMessageReceived: (msg) => {
            chatModel.append({"sender": "对方", "content": msg, "time": Qt.formatTime(new Date(), "hh:mm")})
            if (swipeView.currentIndex !== 1) {
                chatTabBtn.badgeValue = "!"
            }
        }
        onStatusChanged: {
             if (status.includes("成功") || status.includes("已连接")) {
                 chatTabBtn.badgeValue = ""
             }
        }
    }

    // --- 顶部标题栏 ---
    header: ToolBar {
        topPadding: 10
        bottomPadding: 10
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            Label {
                // 这里使用 currentItem.title 获取当前页面的标题
                text: swipeView.currentItem ? swipeView.currentItem.title : ""
                font.pixelSize: 20
                font.bold: true
                color: "white"
                Layout.fillWidth: true
            }
            Rectangle {
                width: 12; height: 12; radius: 6
                color: client.status.includes("成功") || client.status.includes("已连接") ? "#2ecc71" : "#e74c3c"
            }
            Label {
                text: client.localIp
                color: "#ecf0f1"
                font.pixelSize: 12
            }
            Item { width: 20 }
        }
    }

    // --- 底部标签栏 ---
    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton { text: "连接" }
        TabButton {
            id: chatTabBtn
            text: "聊天"
            property string badgeValue: ""
            Rectangle {
                visible: parent.badgeValue !== ""
                width: 10; height: 10; radius: 5; color: "red"
                anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 5
            }
        }
        TabButton { text: "视频" }
    }

    // --- 主内容区域 ---
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false

        // ================== 页面 1: 连接设置 ==================
        Page {
            // 【修复点 1】去掉 property string，直接赋值
            title: "连接设置"

            background: Rectangle { color: "#f5f5f5" }

            ColumnLayout {
                anchors.centerIn: parent
                width: parent.width * 0.85
                spacing: 25

                Pane {
                    Layout.fillWidth: true
                    Material.elevation: 2
                    ColumnLayout {
                        spacing: 15
                        Label { text: "我是房主 (等待连接)"; font.bold: true; color: Material.primary }
                        Button {
                            text: client.isListening ? "监听中 (端口 8888)..." : "开启监听"
                            Layout.fillWidth: true
                            highlighted: !client.isListening
                            enabled: !client.isListening
                            onClicked: client.startListening(8888)
                        }
                        Label {
                            visible: client.isListening
                            text: "当前状态: " + client.status
                            font.pixelSize: 12; color: "gray"
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }

                Pane {
                    Layout.fillWidth: true
                    Material.elevation: 2
                    ColumnLayout {
                        spacing: 15
                        Label { text: "我是访客 (连接别人)"; font.bold: true; color: Material.primary }
                        TextField {
                            id: targetIp
                            placeholderText: "输入房主 IP"
                            text: client.localIp
                            Layout.fillWidth: true
                            selectByMouse: true
                        }
                        Button {
                            text: "连接"
                            Layout.fillWidth: true
                            enabled: targetIp.text.length > 0
                            onClicked: client.connectToPeer(targetIp.text, 8888)
                        }
                    }
                }
            }
        }

        // ================== 页面 2: 文字聊天 ==================
        Page {
            // 【修复点 2】去掉 property string，直接赋值
            title: "文字聊天"

            background: Rectangle { color: rootWindow.bgColor }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                ListView {
                    id: chatView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    topMargin: 10
                    bottomMargin: 10
                    model: ListModel { id: chatModel }
                    spacing: 15

                    delegate: RowLayout {
                        width: chatView.width
                        layoutDirection: sender === "我" ? Qt.RightToLeft : Qt.LeftToRight
                        spacing: 10

                        Rectangle {
                            width: 40; height: 40; radius: 20
                            color: sender === "我" ? Material.accent : "#bdc3c7"
                            Label { anchors.centerIn: parent; text: sender === "我" ? "我" : "Ta"; color: "white" }
                            Layout.margins: 10
                        }

                        Rectangle {
                            Layout.maximumWidth: chatView.width * 0.7
                            height: msgCol.implicitHeight + 20
                            width: msgCol.implicitWidth + 24
                            color: sender === "我" ? myBubbleColor : peerBubbleColor
                            radius: 15

                            Column {
                                id: msgCol
                                anchors.centerIn: parent
                                spacing: 5
                                Text {
                                    text: content
                                    font.pixelSize: 16
                                    color: "black"
                                    wrapMode: Text.Wrap
                                    width: Math.min(implicitWidth, chatView.width * 0.65)
                                }
                                Text {
                                    text: time
                                    font.pixelSize: 10
                                    color: "gray"
                                    anchors.right: parent.right
                                }
                            }
                        }
                        Item { Layout.fillWidth: true }
                    }
                    onCountChanged: Qt.callLater(chatView.positionViewAtEnd)
                }

                Pane {
                    Layout.fillWidth: true
                    Material.elevation: 8
                    background: Rectangle { color: "white" }
                    RowLayout {
                        TextArea {
                            id: inputMsg
                            Layout.fillWidth: true
                            placeholderText: "输入消息..."
                            wrapMode: Text.Wrap
                            background: null
                            topPadding: 12; bottomPadding: 12
                            Keys.onEnterPressed: (event) => {
                                if (!(event.modifiers & Qt.ShiftModifier)) {
                                    sendBtn.clicked();
                                    event.accepted = true;
                                }
                            }
                        }
                        Button {
                            id: sendBtn
                            text: "发送"
                            highlighted: true
                            enabled: inputMsg.text.trim().length > 0
                            onClicked: {
                                client.sendMessage(inputMsg.text)
                                chatModel.append({"sender": "我", "content": inputMsg.text, "time": Qt.formatTime(new Date(), "hh:mm")})
                                inputMsg.clear()
                            }
                        }
                    }
                }
            }
        }

        // ================== 页面 3: 视频通话 ==================
        Page {
            // 【修复点 3】去掉 property string，直接赋值
            title: "视频预览 (本地)"

            padding: 0

            Rectangle {
                anchors.fill: parent
                color: "black"

                CaptureSession {
                    id: captureSession
                    camera: Camera {
                        id: camera
                        active: SwipeView.isCurrentItem
                        onErrorOccurred: (error, errorString) => {
                            errorLabel.text = "摄像头错误: " + errorString
                        }
                    }
                    videoOutput: videoOut
                }

                VideoOutput {
                    id: videoOut
                    anchors.fill: parent
                    fillMode: VideoOutput.PreserveAspectCrop
                    transform: Scale { origin.x: videoOut.width / 2; xScale: -1 }
                }

                Label {
                    id: errorLabel
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    visible: camera.error !== Camera.NoError
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 80
                    color: "#80000000"

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 30
                        RoundButton { text: "静音"; radius: 30; Material.background: "#555" }
                        RoundButton {
                            text: "挂断"
                            radius: 35
                            width: 70; height: 70
                            Material.background: "#e74c3c"
                            onClicked: tabBar.currentIndex = 1
                        }
                        RoundButton { text: "切换"; radius: 30; Material.background: "#555" }
                    }
                }

                Label {
                    text: "当前仅显示本地摄像头画面，未传输给对方。"
                    color: "white"
                    font.pixelSize: 10
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 10
                    padding: 5
                    background: Rectangle { color: "#80000000"; radius: 5 }
                }
            }
        }
    }
}
