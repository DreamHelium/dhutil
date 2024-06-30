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

set(DHUTIL_MAIN_POT_FILE_DEPENDS
    src/dh_string_util.c
)

list(APPEND CMAKE_MODULE_PATH
    /usr/share/VCM/Vala
)

add_definitions (
    -DPACKAGE="${PACKAGE}"
    -DLOCALEDIR="${INSTALL_PREFIX}/share/locale"
)

file(GLOB HEADERS
    dh_string_util.h
    dh_file_util.h
    dh_list_util.h
    dh_data_ooc.h
    dh_generaliface.h
    dh_general_isoc.h
#?
    dh_validator.h
#   dh_questionuiiface.h dh_questionui_isoc.h
)
set(SOURCES
    dh_string_util.c
    dh_file_util.c
    dh_list_util.c
    dh_data_ooc.c
    dh_generaliface.c
    dh_general_isoc.c
    dh_validator.c
#   dh_questionuiiface.c dh_questionui_isoc.c
)

add_library(dhutil SHARED "${HEADERS}" "${SOURCES}")

target_include_directories(dhutil
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        "${GLIB_INCLUDE_DIRS}"
        "${GIO_INCLUDE_DIRS}")

target_link_libraries(dhutil PUBLIC ${GIO_LIBRARIES} readline)

install(FILES "dh_string_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_file_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_list_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_generaliface.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_general_isoc.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_data_ooc.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_validator.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dhutil.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")


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
