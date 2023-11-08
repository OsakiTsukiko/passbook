cmake_minimum_required(VERSION 3.20)

# ARGUMENTS
option(PLATFORM "PLATFORM" null)
set(ENV{LD_RUN_PATH} '$ORIGIN/lib')

project(passbook)

if (${PLATFORM} STREQUAL "linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)

    # set(SOURCES main.c)
    FILE(GLOB_RECURSE SOURCES src/*.c)
    add_executable(${PROJECT_NAME} ${SOURCES})

    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS} thirdparty/include)
    target_link_directories(${PROJECT_NAME} PRIVATE thirdparty/lib)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_LIBRARIES} ${WEBKIT_LIBRARIES} webview mystd)

# TODO: WIN AND MACOS TOO
else()
    message(FATAL_ERROR "Specify a platform with -DPLATFORM=linux|win|macos")
endif()
