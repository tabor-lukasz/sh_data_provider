cmake_minimum_required(VERSION 3.5)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(tools D:/SysGCC/raspberry_8_3)
file(GLOB Qt_bin C:/Qt/5.11.3/mingw53_32/bin)

set(CMAKE_SYSROOT ${tools}/arm-linux-gnueabihf/sysroot)
#set(CMAKE_STAGING_PREFIX /home/devel/stage)


set(CMAKE_MAKE_PROGRAM ${tools}/bin/make.exe)
#set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc.exe)
#set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc.exe)


find_program(QT_MOC_EXECUTABLE qt_moc moc PATHS ${Qt_bin})
add_executable(Qt5::moc IMPORTED)
set_property(TARGET Qt5::moc PROPERTY IMPORTED_LOCATION ${QT_MOC_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sh_data_provider LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

set(CMAKE_INSTALL_PREFIX "/home/pi/smart_home/dev")

#set(CMAKE_PREFIX_PATH ${tools}/arm-linux-gnueabihf/sysroot/usr/lib/arm-linux-gnueabihf/cmake)

set(Qt5_DIR ${tools}/arm-linux-gnueabihf/sysroot/usr/lib/arm-linux-gnueabihf/cmake/Qt5/)
find_package(Qt5 COMPONENTS Core Sql Network REQUIRED)

set(sources /
    main.cpp /
    sensorreader.cpp /
    dbfeeder.cpp /
    httpserver.cpp)

add_executable(sh_data_provider ${sources})
target_link_libraries(sh_data_provider Qt5::Core Qt5::Sql Qt5::Network)

message(${CMAKE_INSTALL_PREFIX})
message(${PROJECT_NAME})
message(${Qt5Core_DIR})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
