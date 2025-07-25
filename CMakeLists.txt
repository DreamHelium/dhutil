cmake_minimum_required(VERSION 3.15)

project(dhutil
        VERSION 0.2.0
        LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DHUTIL_VERSION_SO 0)

find_package(Gettext)
find_package(PkgConfig)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GIO REQUIRED gio-2.0)
pkg_search_module(CURL REQUIRED libcurl)

if (DHUTIL_BUILD_CLI)
    # Replace Readline with libedit
    pkg_search_module(el REQUIRED libedit)
    if (${el_FOUND})
        add_definitions(-DDH_EDITLINE_USED)
    endif ()
    set(CMAKE_CXX_STANDARD 17)
    set(CLI_HEADERS dh_validator_cpp.hpp dh_readline_cpp.hpp)
    set(CLI_SOURCES dh_validator_cpp.cpp dh_readline_cpp.cpp)
    add_library(dhutil_cli SHARED "${CLI_HEADERS}" "${CLI_SOURCES}")
    target_link_libraries(dhutil_cli PUBLIC ${el_LIBRARIES} dhutil_base)

    add_executable(example_cpp example.cpp)
    target_link_libraries(example_cpp PUBLIC dhutil_cli)
endif ()

set(STRING_HEADERS dh_string_util.h)
set(STRING_SOURCES dh_string_util.c)

set(FILELIB_HEADERS dh_file_util.h dh_list_util.h)
set(FILELIB_SOURCES dh_file_util.c dh_list_util.c)

add_library(dhutil_file SHARED ${FILELIB_HEADERS} ${FILELIB_SOURCES})
add_library(dhutil_string SHARED "${STRING_SOURCES}" "${STRING_HEADERS}")

target_include_directories(dhutil_string PUBLIC ${GLIB_INCLUDE_DIRS})
target_include_directories(dhutil_file PUBLIC ${GIO_INCLUDE_DIRS})

target_link_libraries(dhutil_string PUBLIC ${GLIB_LIBRARIES})
target_link_libraries(dhutil_file PUBLIC ${GIO_LIBRARIES} ${CURL_LIBRARIES})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/library_config/dhutil_string.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/dhutil_string.pc" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/library_config/dhutil_file.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/dhutil_file.pc" @ONLY)

add_executable(example example.c)
target_link_libraries(example PUBLIC dhutil_string)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dhutil_string.pc"
        "${CMAKE_CURRENT_BINARY_DIR}/dhutil_file.pc"
        DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")

install(TARGETS dhutil_string dhutil_file
        LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}")
install(FILES ${BASE_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})

set_target_properties(dhutil_string dhutil_file
        PROPERTIES
        SOVERSION "${DHUTIL_VERSION_SO}"
        VERSION "${PROJECT_VERSION}")