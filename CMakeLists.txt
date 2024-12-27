cmake_minimum_required(VERSION 3.5)

project(dhutil
	VERSION 0.2.0
	LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DHUTIL_VERSION_SO 0)

set(CURSES_NEED_WIDE ON)

find_package(Gettext)
find_package(PkgConfig)
find_package(Curses)

pkg_search_module(GIO REQUIRED gio-2.0)
pkg_search_module(rl REQUIRED readline)

message(${PKG_CONFIG_EXECUTABLE})

set(DHUTIL_LIB dhutil)

add_definitions(-DGETTEXT_PACKAGE="dhutil")

add_definitions (
    -DPACKAGE="${PACKAGE}"
    -DLOCALEDIR="${INSTALL_PREFIX}/share/locale"
)

file(GLOB HEADERS
    dh_string_util.h
    dh_file_util.h
    dh_list_util.h
    dh_validator.h
    dh_mt_table.h
    dh_nc_rl.h
    dh_validator_cpp.hpp
    dh_readline_cpp.hpp
)
set(SOURCES
    dh_string_util.c
    dh_file_util.c
    dh_list_util.c
    dh_validator.c
    dh_mt_table.c
    dh_nc_rl.c
    dh_validator_cpp.cpp
    dh_readline_cpp.cpp
)

message(${CURSES_LIBRARIES})

add_library(dhutil SHARED "${HEADERS}" "${SOURCES}")

target_include_directories(dhutil
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        "${GIO_INCLUDE_DIRS}")

target_link_libraries(dhutil PUBLIC ${GIO_LIBRARIES} ${rl_LIBRARIES} ${CURSES_LIBRARIES})

install(FILES "dh_string_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_file_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_list_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_validator.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dhutil.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/library_config/libdhutil.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/libdhutil.pc" @ONLY)

message("${CMAKE_INSTALL_FULL_INCLUDEDIR}")
message("${CMAKE_LIBRARY_PATH}")

add_executable(example example.c)

target_link_libraries(example dhutil)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libdhutil.pc" DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")
install(TARGETS "${DHUTIL_LIB}"
	EXPORT "${DHUTIL_LIB}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}"
)

set_target_properties("${DHUTIL_LIB}"
	PROPERTIES
	SOVERSION "${DHUTIL_VERSION_SO}"
	VERSION "${PROJECT_VERSION}"
)
