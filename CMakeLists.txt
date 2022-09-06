cmake_minimum_required(VERSION 3.16)

project(Light)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/bin)

file(GLOB_RECURSE LIGHT_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Light/*)
file(GLOB_RECURSE MOJULA_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Mojula/*)
file(GLOB_RECURSE EXCEPTION_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Exception/*.cpp)
file(GLOB_RECURSE LOGGER_FILES true ABSOLUTE ${CMAKE_SOURCE_DIR}/Modules/Logger/*)

message(${EXCEPTION_FILES}})

add_executable(Light ${LIGHT_FILES})
add_library(Mojula STATIC ${MOJULA_FILES})
add_library(Exception STATIC ${EXCEPTION_FILES})
add_library(Logger STATIC ${LOGGER_FILES})

target_include_directories(
    Light
    PRIVATE ${CMAKE_SOURCE_DIR}/Light/Source
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Logger
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Exception
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Mojula
    PRIVATE ${CMAKE_SOURCE_DIR}/Vendor/spdlog/include
)

target_include_directories(
    Mojula
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Mojula/Source
)

target_include_directories(
    Exception
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Exception/Source
)

target_include_directories(
    Logger
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Logger/Source
    PRIVATE ${CMAKE_SOURCE_DIR}/Modules/Mojula
    PRIVATE ${CMAKE_SOURCE_DIR}/Vendor/spdlog/include
)

target_link_libraries(
    Light
    PRIVATE Exception
    PRIVATE Logger
    PRIVATE Mojula
    PRIVATE spdlog
)

target_link_libraries(
    Logger
    PRIVATE Mojula
    PRIVATE spdlog
)
