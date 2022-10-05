cmake_minimum_required(VERSION 3.5)

project(protonply VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    #message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
    find_package(cpr REQUIRED)
endif()

#include(conan.cmake)
#conan_cmake_configure(REQUIRES nlohmann_json/3.11.2 cpr/1.9.0
#        GENERATORS cmake)
#conan_cmake_autodetect(settings)
#conan_cmake_install(PATH_OR_REFERENCE .
#        BUILD missing
#        REMOTE conancenter
#        SETTINGS ${settings})
#if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#    conan_basic_setup()
#else()
#    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
#endif()

find_package(QT NAMES Qt6 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES protonply_en_CA.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        ${TS_FILES}
        src/protons.cpp src/protons.h src/ge_proton.cpp src/ge_proton.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(protonply
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
endif()

target_link_libraries(protonply PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    target_link_libraries(protonply PRIVATE ${CONAN_LIBS})
else()
    target_link_libraries(protonply PRIVATE cpr::cpr)
endif()

set_target_properties(protonply PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(protonply)
endif()

#install(TARGETS protonply)
