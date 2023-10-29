cmake_minimum_required(VERSION 3.5)

project(protonply VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets LinguistTools REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets LinguistTools REQUIRED)

#CMake Warning at buildtrees/versioning_/versions/qtbase/348dbf9cdcda0559adb72c98c249e06f8a2e50e8/portfile.cmake:38 (message):
#qtbase currently requires packages from the system package manager.  They
#can be installed on Ubuntu systems via sudo apt-get install '^libxcb.*-dev'
#libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev
#libxkbcommon-x11-dev.

find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

find_package(LibArchive REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(TS_FILES protonply_en_CA.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        ${TS_FILES}
        src/protons.cpp src/protons.h src/ge_proton.cpp src/ge_proton.h
        src/untar.cpp src/untar.h)

qt_add_executable(protonply
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES})
#qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(protonply PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(protonply PRIVATE cpr::cpr LibArchive::LibArchive)



