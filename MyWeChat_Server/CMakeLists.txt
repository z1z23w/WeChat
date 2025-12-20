cmake_minimum_required(VERSION 3.16)
project(WeChatServer VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Sql)
qt_standard_project_setup()

qt_add_executable(WeChatServer
    main.cpp
    ChatServer.h ChatServer.cpp
    DBManager.h DBManager.cpp
)

target_link_libraries(WeChatServer PRIVATE Qt6::Core Qt6::Network Qt6::Sql)