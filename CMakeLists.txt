cmake_minimum_required(VERSION 3.16)
project(lwm2m_client C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LWM2M_TARGET_VERSION "1.2" CACHE STRING "Target LWM2M protocol version (1.0, 1.1, 1.2)")

string(REPLACE "." ";" LWM2M_VERSION_PARTS ${LWM2M_TARGET_VERSION})
list(GET LWM2M_VERSION_PARTS 0 LWM2M_VER_MAJOR)
list(GET LWM2M_VERSION_PARTS 1 LWM2M_VER_MINOR)

file(GLOB_RECURSE LWM2MCC_SOURCES src/*.c)
file(GLOB_RECURSE LWM2MCC_HEADERS include/*.h)

add_library(lwm2mcc SHARED ${LWM2MCC_SOURCES})

target_include_directories(lwm2mcc PUBLIC include)

target_compile_definitions(lwm2mcc PUBLIC
    LWM2MCC_VERSION=LWM2MCC_MAKE_VERSION\(${LWM2M_VER_MAJOR},${LWM2M_VER_MINOR},0\)
)

target_compile_options(lwm2mcc PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)

add_executable(testapp examples/testapp.c)
target_link_libraries(testapp PRIVATE lwm2mcc)
