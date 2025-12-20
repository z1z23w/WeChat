cmake_minimum_required(VERSION 3.16)

project(we VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network Multimedia QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appwe main.cpp)

qt_add_qml_module(appwe
    URI we
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES P2PClient.h P2PClient.cpp
)

# 链接必要的库
target_link_libraries(appwe
    PRIVATE Qt6::Quick Qt6::Network Qt6::Multimedia Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appwe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
