################################################################
### PROJECT
cmake_minimum_required(VERSION 3.16)

project(Light)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/bin)

include_directories(
    ${CMAKE_SOURCE_DIR}/Modules/

    # Vendor
    ${CMAKE_SOURCE_DIR}/Vendor/spdlog/include/
    ${CMAKE_SOURCE_DIR}/Vendor/glfw/include/
    ${CMAKE_SOURCE_DIR}/Vendor/entt/src/
    ${CMAKE_SOURCE_DIR}/Vendor/imgui/
)

################################################################
### PLATFORM
add_compile_definitions(LIGHT_PLATFORM_NAME ${CMAKE_SYSTEM_NAME})
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(LIGHT_PLATFORM_LINUX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_definitions(LIGHT_PLATFORM_MAC)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(LIGHT_PLATFORM_WINDOWS)
endif ()

################################################################
### BUILD TYPE 
#TODO:
add_compile_definitions(LIGHT_BUILD_TYPE_DEBUG)

################################################################
### Vendor
add_subdirectory(${CMAKE_SOURCE_DIR}/Vendor/spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/Vendor/glfw)

################################################################
### LIGHT
file(GLOB_RECURSE LIGHT_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Light/*)
add_executable(Light ${LIGHT_FILES})

target_include_directories(
    Light
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules
    PRIVATE ${CMAKE_SOURCE_DIR}/Light/Source
)
target_link_libraries(
    Light
    PRIVATE Exception
    PRIVATE Mojula
    PRIVATE Logger
    PRIVATE Window
    PRIVATE Time
    PRIVATE Profiler
    PRIVATE Events
)

################################################################
### Mojula
file(GLOB_RECURSE MOJULA_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Mojula/*)
add_library(Mojula STATIC ${MOJULA_FILES})

target_include_directories(
    Mojula
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Mojula/Source
)

################################################################
### EXCEPTION
file(GLOB_RECURSE EXCEPTION_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Exception/*.cpp) # ye wtf?
add_library(Exception STATIC ${EXCEPTION_FILES})

target_include_directories(
    Exception
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Exception/Source
)

################################################################
### EVENTS
file(GLOB_RECURSE EVENTS_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Events/*)
add_library(Events  STATIC ${EVENTS_FILES})

target_include_directories(
    Events
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Events/Source
)

################################################################
### TIME
file(GLOB_RECURSE TIME_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Time/*.cpp)
add_library(Time STATIC ${TIME_FILES})

target_include_directories(
    Time
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Time/Source
)


################################################################
### FILEMANAGER
file(GLOB_RECURSE FILEMANAGER_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/FileManager/*)
add_library(FileManager STATIC ${FILEMANAGER_FILES})

target_include_directories(
    FileManager
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/FileManager/Source
)

################################################################
### PROFILER
file(GLOB_RECURSE PROFILER_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Profiler/*)
add_library(Profiler STATIC ${PROFILER_FILES})

target_include_directories(
    Profiler
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Profiler/Source
)
target_link_libraries(
    Profiler
    PRIVATE FileManager
    PRIVATE Logger
)

################################################################
### LOGGER
file(GLOB_RECURSE LOGGER_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Logger/*)
add_library(Logger STATIC ${LOGGER_FILES})

target_include_directories(
    Logger
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Logger/Source
)
target_link_libraries(
    Logger
    PRIVATE Mojula

    PRIVATE spdlog
)

################################################################
### Window
file(GLOB_RECURSE WINDOW_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Window/*)
add_library(Window STATIC ${WINDOW_FILES})

target_include_directories(
    Window
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Window/Source
)
target_link_libraries(
    Window
    PRIVATE Logger
    PRIVATE Events

    PRIVATE glfw
)
