cmake_minimum_required(VERSION 3.5)

project(dhutil
	VERSION 0.1.0
	LANGUAGES C)

include(GNUInstallDirs)

find_package(PkgConfig)

pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GIO REQUIRED gio-2.0)

set(DHUTIL_LIB dhutil)

file(GLOB HEADERS dh_string_util.h  file_util.h  list_util.h)
set(SOURCES dh_string_util.c  file_util.c  list_util.c)

add_library(dhutil SHARED "${HEADERS}" "${SOURCES}")

target_include_directories(dhutil
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        "${GLIB_INCLUDE_DIRS}"
        "${GIO_INCLUDE_DIRS}")

target_link_libraries(dhutil PUBLIC cjson ${GLIB_LIBRARIES} ${GIO_LIBRARIES})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/library_config/libdhutil.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/libdhutil.pc" @ONLY)

message("${CMAKE_INSTALL_FULL_INCLUDEDIR}")
message("${CMAKE_LIBRARY_PATH}")

install(FILES "dh_string_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dhelium")
install(FILES "file_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dhelium")
install(FILES "list_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dhelium")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libdhutil.pc" DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")
install(TARGETS "${DHUTIL_LIB}"
	EXPORT "${DHUTIL_LIB}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}"
)
