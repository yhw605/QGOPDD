cmake_minimum_required(VERSION 3.5)

# set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.39.33519/bin/Hostx64/x64/cl.exe")
project(QGOPDD VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(VCPKG_ROOT "C:/vcpkg/vcpkg")
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)



# cmake_print_properties(VCPKG_ROOT)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets Sql Network)
# find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets Sql Network)



# add_subdirectory(./thirdparty/fineftp-server/fineftp-server)

set(PROJECT_SOURCES
        main.cpp
        qgopdd.cpp
        qgopdd.h
        qgopdd.ui
        igs_stations.json
)


add_subdirectory(./thirdparty/curl)
include_directories(./thirdparty/curl/include)
# find_package(CURL REQUIRED)
# target_link_libraries(QGOPDD PRIVATE curl::curl)

add_subdirectory(./thirdparty/zlib)
include_directories(./thirdparty/zlib)

add_subdirectory(./thirdparty/hatanaka/rnxcmp/source)
set(CRX2RNX_SOURCE_DIR ./thirdparty/hatanaka/rnxcmp/source)
set(CRX2RNX_SOURCES ./thirdparty/hatanaka/rnxcmp/source/crx2rnx.c)
set(CRX2RNX_BINARY_DIR ./)
# add_executable(crx2rnx ./thirdparty/hatanaka/rnxcmp/source/crx2rnx.c)

# add_subdirectory(./thirdparty/7z)
# add_subdirectory(./thirdparty/libarchive)
# include_directories(./thirdparty/libarchive/libarchive)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QGOPDD
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        igs_stations.json
        # vcpkg.json
        # vcpkg-configuration.json
        ftpdownloader.h ftpdownloader.cc
        calendarwidget.h calendarwidget.cc calendarwidget.ui
        progresscaller.h progresscaller.cc progresscaller.ui
        batchdownload.h batchdownload.cc
        # textviewer.h textviewer.cc textviewer.ui
        tformdoc.h tformdoc.cc tformdoc.ui
        textviewer.cc textviewer.h textviewer.ui
        downloadfinished.h downloadfinished.cc downloadfinished.ui
        other_resources.qrc




        # ${CRX2RNX_SOURCES}
        # vcpkg.json
        # vcpkg-configuration.json
        # CMakePresets.json
    )
# add_executable()
add_dependencies(QGOPDD CRX2RNX)
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
    CURL::libcurl
    # CRX2RNX
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



# find_package(CURL REQUIRED)
# target_link_libraries(QGOPDD PRIVATE CURL::libcurl)
