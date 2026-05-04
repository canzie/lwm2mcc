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
list(FILTER LWM2MCC_SOURCES EXCLUDE REGEX "src/platform/.*")

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

set(LWM2MCC_TRANSPORT "none" CACHE STRING "Transport backend (none, libcoap)")
set_property(CACHE LWM2MCC_TRANSPORT PROPERTY STRINGS none libcoap)

if(LWM2MCC_TRANSPORT STREQUAL "libcoap")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBCOAP REQUIRED libcoap-3)

    add_library(lwm2mcc_transport_libcoap STATIC
        src/platform/transport_libcoap.c
    )
    target_include_directories(lwm2mcc_transport_libcoap
        PUBLIC  include src/platform
        PRIVATE ${LIBCOAP_INCLUDE_DIRS}
    )
    target_link_libraries(lwm2mcc_transport_libcoap
        PUBLIC  lwm2mcc
        PRIVATE ${LIBCOAP_LIBRARIES}
    )
    target_compile_options(lwm2mcc_transport_libcoap PRIVATE
        -Wall -Wextra -Wpedantic -Werror
        ${LIBCOAP_CFLAGS_OTHER}
    )
    target_link_libraries(testapp PRIVATE lwm2mcc_transport_libcoap)
endif()
