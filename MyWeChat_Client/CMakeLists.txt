cmake_minimum_required(VERSION 3.16)

project(appwe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql Network Multimedia)

qt_standard_project_setup()

qt_add_executable(appwe main.cpp)

qt_add_qml_module(appwe
    URI "we"
    VERSION 1.0
    QML_FILES
        NetizenChatUI.qml
        NotificationPanel.qml
    SOURCES
        # --- Entity (实体类) ---
        Netizen.h
        ChatSession.h

        # --- Infrastructure (基础设施) ---
        AudioManager.h AudioManager.cpp

        # --- Control (控制类) ---
        ComplianceController.h ComplianceController.cpp
        MediaSessionController.h MediaSessionController.cpp
        CommunicationController.h CommunicationController.cpp
)

target_link_libraries(appwe PRIVATE Qt6::Quick Qt6::Sql Qt6::Network Qt6::Multimedia)
