cmake_minimum_required(VERSION 3.5)

project(DiceRoller-Pro VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
        DiceRoller-Pro_en.ts
        DiceRoller-Pro_es.ts
        DiceRoller-Pro_fr.ts
        DiceRoller-Pro_ko.ts
)

set(SOURCE_FILES
        main.cpp
        mainwindow.cpp
        mainwindow.hpp
        mainwindow.ui
        dicewidget.cpp
        dicewidget.hpp
        dicewidget.ui
        diceroll.hpp
        diceroll.cpp
)

set(PROJECT_SOURCES
        ${SOURCE_FILES}
        icons.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DiceRoller-Pro
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DiceRoller-Pro APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_add_translations(DiceRoller-Pro TS_FILES ${TS_FILES} SOURCES ${SOURCE_FILES})
else()
    if(ANDROID)
        add_library(DiceRoller-Pro SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DiceRoller-Pro
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(DiceRoller-Pro PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(DiceRoller-Pro PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS DiceRoller-Pro
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DiceRoller-Pro)
endif()
