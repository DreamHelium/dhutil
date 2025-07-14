cmake_minimum_required(VERSION 3.15)

project(dhutil
        VERSION 0.2.0
        LANGUAGES C CXX)

include(GNUInstallDirs)
if (NOT DHUTIL_PRE_INCLUDED)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(DHUTIL_VERSION_SO 0)
    set(DHUTIL_PRE_INCLUDED ON PARENT_SCOPE)

    #set(CURSES_NEED_WIDE ON)

    find_package(Gettext)
    find_package(PkgConfig)

    pkg_search_module(GIO REQUIRED gio-2.0)
    pkg_search_module(CURL REQUIRED libcurl)

     set(DHUTIL_LIB dhutil_base)

    if (DHUTIL_BUILD_CLI)
        # Replace Readline with libedit
        pkg_search_module(el REQUIRED libedit)
        if (${el_FOUND})
            add_definitions(-DDH_EDITLINE_USED)
        endif ()
        set(CMAKE_CXX_STANDARD 17)
        set(CLI_HEADERS dh_validator_cpp.hpp dh_readline_cpp.hpp dh_bit.hpp)
        set(CLI_SOURCES dh_validator_cpp.cpp dh_readline_cpp.cpp dh_bit.cpp)
        add_library(dhutil_cli SHARED "${CLI_HEADERS}" "${CLI_SOURCES}")
        target_link_libraries(dhutil_cli PUBLIC ${el_LIBRARIES} dhutil_base)

        add_executable(example_cpp example.cpp)
        target_link_libraries(example_cpp PUBLIC dhutil_cli)
    endif ()

    set(BASE_HEADERS dh_string_util.h dh_file_util.h dh_list_util.h dh_mt_table.h)
    set(BASE_SOURCES dh_string_util.c dh_file_util.c dh_list_util.c dh_mt_table.c)

    add_library(dhutil_base SHARED "${BASE_SOURCES}" "${BASE_HEADERS}")

    target_include_directories(dhutil_base PUBLIC ${GIO_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(dhutil_base PUBLIC ${GIO_LIBRARIES} ${CURL_LIBRARIES})

     install(FILES "dh_string_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
     install(FILES "dh_file_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
     install(FILES "dh_list_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
     install(FILES "dh_mt_table.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}")

     configure_file("${CMAKE_CURRENT_SOURCE_DIR}/library_config/dhutil.pc.in"
     	"${CMAKE_CURRENT_BINARY_DIR}/dhutil.pc" @ONLY)

    # message("${CMAKE_INSTALL_FULL_INCLUDEDIR}")
    # message("${CMAKE_LIBRARY_PATH}")

    add_executable(example example.c)
    target_link_libraries(example PUBLIC dhutil_base)

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

endif ()
