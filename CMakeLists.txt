cmake_minimum_required(VERSION 3.5)

project(QGOPDD VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VCPKG_ROOT "C:/vcpkg/vcpkg")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# cmake_print_properties(VCPKG_ROOT)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets Sql Network)



# add_subdirectory(./thirdparty/fineftp-server/fineftp-server)

set(PROJECT_SOURCES
        main.cpp
        qgopdd.cpp
        qgopdd.h
        qgopdd.ui
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QGOPDD
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        igs_stations.json
        # vcpkg.json
        vcpkg-configuration.json
        ftpdownloader.h ftpdownloader.cc
        calendarwidget.h calendarwidget.cc calendarwidget.ui
        vcpkg.json
        vcpkg-configuration.json
        CMakePresets.json
    )

# find_package(fineftp CONFIG REQUIRED)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QGOPDD APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QGOPDD SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QGOPDD
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QGOPDD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QGOPDD PUBLIC
    Qt::Core
    Qt::Gui
    Qt::WebEngineWidgets
    Qt::Widgets
    Qt::Sql
    Qt::Network
)

# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/thirdparty/fineftp-server/cmake/fineftp-module")
# find_package(fineftp REQUIRED)
# target_link_libraries(QGOPDD PRIVATE fineftp::server)

# find_package(Poco REQUIRED COMPONENTS Net)
# target_link_libraries(QGOPDD PRIVATE Poco::Net)

# find_package(fineftp REQUIRED)
# target_link_libraries(QGOPDD PRIVATE fineftp::server)

# target_link_libraries(QGOPDD PRIVATE fineftp::server)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QGOPDD)
endif()
set_target_properties(QGOPDD PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QGOPDD
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QGOPDD)
endif()

# qt6_add_statecharts(ftpclient
#     simpleftp.scxml
# )

find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)
