cmake_minimum_required(VERSION 3.16)

project(man-i-love-baseball VERSION 0.1 LANGUAGES CXX)

# ─── Enable Qt AUTOMOC/AUTOUIC/AUTORCC ────────────────────────────────────────
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ─── C++ Standard ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Find Qt ─────────────────────────────────────────────────────────────────
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# ─── Define source directories ────────────────────────────────────────────────
set(SRC_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(FORMS_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/forms")
set(RESOURCES_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(EXTERNAL_DIRi  "${CMAKE_CURRENT_SOURCE_DIR}/external")

# ————————————————————————————————————————————————————————————————
set(FORMS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/forms")

# Instruct AUTOUIC to look in forms/ for all .ui files:
set(CMAKE_AUTOUIC_SEARCH_PATHS
    "${FORMS_DIR}"
)
# ————————————————————————————————————————————————————————————————


# ─── Collect all .cpp/.h files in src/ (and its subdirectories) ─────────────
file(GLOB_RECURSE APP_SOURCES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
)

# ─── Collect all .ui files in forms/ ──────────────────────────────────────────
file(GLOB FORMS_UI
    "${FORMS_DIR}/*.ui"
)

# ─── Collect the single Qt resource file ─────────────────────────────────────
set(RESOURCES_QRC
    "${RESOURCES_DIR}/resources.qrc"
)

# ─── (Optional) A single spot to list any other .qrc or .ts files if needed ─
# file(GLOB OTHER_QRC "${RESOURCES_DIR}/*.qrc")
# list(APPEND RESOURCES_QRC ${OTHER_QRC})

# ─── Add an executable target ─────────────────────────────────────────────────
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(man-i-love-baseball
        MANUAL_FINALIZATION
        ${APP_SOURCES}
        ${FORMS_UI}
        ${RESOURCES_QRC}
    )
else()
    add_executable(man-i-love-baseball
        ${APP_SOURCES}
        ${FORMS_UI}
        ${RESOURCES_QRC}
    )
endif()

# ─── Tell CMake where to look for your headers ────────────────────────────────
#   1) src/ (so you can do #include "core/settingsmanager.h", etc.)
#   2) third_party/nlohmann_json/include/ (so you can do #include <nlohmann/json.hpp>)
target_include_directories(man-i-love-baseball PRIVATE
    ${SRC_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${EXTERNAL_DIR}/include
)

# ─── Link against Qt Widgets ──────────────────────────────────────────────────
target_link_libraries(man-i-love-baseball PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# ─── macOS / iOS bundle settings (if using Qt<6.1) ────────────────────────────
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.man-i-love-baseball)
endif()
set_target_properties(man-i-love-baseball PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ─── Install rules ────────────────────────────────────────────────────────────
include(GNUInstallDirs)
install(TARGETS man-i-love-baseball
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ─── Finalize for Qt 6 ────────────────────────────────────────────────────────
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(man-i-love-baseball)
endif()
