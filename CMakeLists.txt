cmake_minimum_required(VERSION 3.5)

project(dhutil
	VERSION 0.1.8
	LANGUAGES C)

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DHUTIL_VERSION_SO 0)

find_package(Gettext)
find_package(PkgConfig)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GIO REQUIRED gio-2.0)

set(DHUTIL_LIB dhutil)

add_definitions(-DGETTEXT_PACKAGE="dhutil")

add_subdirectory(src)
add_subdirectory(po)

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
