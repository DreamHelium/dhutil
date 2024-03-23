set(DHUTIL_MAIN_POT_FILE_DEPENDS
    src/dh_string_util.c
    PARENT_SCOPE
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
#   dh_questionuiiface.h dh_questionui_isoc.h
)
set(SOURCES
    dh_string_util.c
    dh_file_util.c
    dh_list_util.c
    dh_data_ooc.c
    dh_generaliface.c
    dh_general_isoc.c
#   dh_questionuiiface.c dh_questionui_isoc.c
)

add_library(dhutil SHARED "${HEADERS}" "${SOURCES}")

target_include_directories(dhutil
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        "${GLIB_INCLUDE_DIRS}"
        "${GIO_INCLUDE_DIRS}")

target_link_libraries(dhutil PUBLIC cjson ${GLIB_LIBRARIES} ${GIO_LIBRARIES})

install(FILES "dh_string_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_file_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_list_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_generaliface.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_general_isoc.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_data_ooc.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dhutil.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_deprecated.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
# install(FILES "dh_questionuiiface.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
# install(FILES "dh_questionui_isoc.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
