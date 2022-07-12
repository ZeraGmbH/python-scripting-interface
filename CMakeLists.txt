cmake_minimum_required(VERSION 3.1)

#set project name and language
project(PythonScriptingInterface LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set project Version
set(PythonScriptingInterface_VERSION_MAJOR "1")
set(PythonScriptingInterface_VERSION_MINOR "2")
set(PythonScriptingInterface_VERSION_PATCH "0")

set(PROJECT_VERSION "${PythonScriptingInterface_VERSION_MAJOR}.${PythonScriptingInterface_VERSION_MINOR}.${PythonScriptingInterface_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(PythonLibs 3.8 REQUIRED)



#sum up project Files 
file(GLOB SOURCES
${PROJECT_SOURCE_DIR}/src/*.cpp
    )


file(GLOB HEADER
${PROJECT_SOURCE_DIR}/src/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

include_directories(
    ${PYTHON_INCLUDE_DIRS}
    )

#create executable
add_library(PythonScriptingInterface SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    ${PLUGIN_RESOURCES}
    )


#link dependencies
target_link_libraries(PythonScriptingInterface
    PRIVATE
    Qt5::Core
    ${PYTHON_LIBRARIES}
)	





#set target Version
set_target_properties(PythonScriptingInterface PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(PythonScriptingInterface PROPERTIES SOVERSION ${PythonScriptingInterface_VERSION_MAJOR})


#generate export header
include(GenerateExportHeader)
generate_export_header(PythonScriptingInterface)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pythonscriptinginterface_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/PythonScriptingInterface
    )

# announce headers - target perspective
target_include_directories(PythonScriptingInterface
    PUBLIC
    $<INSTALL_INTERFACE:include/PythonScriptingInterface>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

# install library
install(TARGETS PythonScriptingInterface
    EXPORT PythonScriptingInterfaceExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    PythonScriptingInterfaceConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

# configure *Config.cmake
configure_file(PythonScriptingInterfaceConfig.cmake.in PythonScriptingInterfaceConfig.cmake @ONLY)



# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/PythonScriptingInterfaceConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/PythonScriptingInterfaceConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PythonScriptingInterface
    )

# install targets cmake-files
install(EXPORT PythonScriptingInterfaceExport
    FILE PythonScriptingInterfaceTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PythonScriptingInterface
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/PythonScriptingInterface
    )
    


# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)



#





