cmake_minimum_required(VERSION 3.16)

project(we VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 修改这里：在列表最后加上 QuickControls2
find_package(Qt6 REQUIRED COMPONENTS Quick Network Multimedia QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appwe main.cpp)

qt_add_qml_module(appwe
    URI we
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES P2PClient.h P2PClient.cpp
)

set_target_properties(appwe PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 2. 修改这里：在列表最后加上 Qt6::QuickControls2
target_link_libraries(appwe
    PRIVATE Qt6::Quick Qt6::Network Qt6::Multimedia Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appwe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
