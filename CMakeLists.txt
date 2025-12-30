cmake_minimum_required(VERSION 3.1)

project(PythonScriptingInterface LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PythonScriptingInterface_VERSION_MAJOR "1")
set(PythonScriptingInterface_VERSION_MINOR "2")
set(PythonScriptingInterface_VERSION_PATCH "0")

set(PROJECT_VERSION "${PythonScriptingInterface_VERSION_MAJOR}.${PythonScriptingInterface_VERSION_MINOR}.${PythonScriptingInterface_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)

set(QT_MAJOR_VER "5" CACHE STRING "Qt major version")
set(QTX "Qt${QT_MAJOR_VER}")

find_package(${QTX} COMPONENTS Core REQUIRED)
find_package(PythonLibs 3.8 REQUIRED)

file(GLOB SOURCES
    src/*.cpp
)

file(GLOB HEADER
    src/*.h
)

file(GLOB RESOURCES 
    *.qrc
    )

include_directories(
    ${PYTHON_INCLUDE_DIRS}
)

add_library(PythonScriptingInterface SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    ${PLUGIN_RESOURCES}
    )


target_link_libraries(PythonScriptingInterface
    PRIVATE
    ${QTX}::Core
    ${PYTHON_LIBRARIES}
)	

set_target_properties(PythonScriptingInterface PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(PythonScriptingInterface PROPERTIES SOVERSION ${PythonScriptingInterface_VERSION_MAJOR})

include(GenerateExportHeader)
generate_export_header(PythonScriptingInterface)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pythonscriptinginterface_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/PythonScriptingInterface
    )

target_include_directories(PythonScriptingInterface
    PUBLIC
    $<INSTALL_INTERFACE:include/PythonScriptingInterface>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

install(TARGETS PythonScriptingInterface
    EXPORT PythonScriptingInterfaceExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    PythonScriptingInterfaceConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(PythonScriptingInterfaceConfig.cmake.in PythonScriptingInterfaceConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/PythonScriptingInterfaceConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/PythonScriptingInterfaceConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PythonScriptingInterface
    )

install(EXPORT PythonScriptingInterfaceExport
    FILE PythonScriptingInterfaceTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PythonScriptingInterface
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/PythonScriptingInterface
    )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
