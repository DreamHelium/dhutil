set(DHUTIL_MAIN_POT_FILE_DEPENDS
    src/dh_string_util.c
    PARENT_SCOPE
)

add_definitions (
  -DPACKAGE="${PACKAGE}"
  -DLOCALEDIR="${INSTALL_PREFIX}/share/locale"
)

file(GLOB HEADERS dh_string_util.h  file_util.h  list_util.h dh_data_ooc.h dh_generaliface.h dh_general_isoc.h)
set(SOURCES dh_string_util.c  file_util.c  list_util.c dh_data_ooc.c dh_generaliface.c dh_general_isoc.c)

add_library(dhutil SHARED "${HEADERS}" "${SOURCES}")

target_include_directories(dhutil
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        "${GLIB_INCLUDE_DIRS}"
        "${GIO_INCLUDE_DIRS}")

target_link_libraries(dhutil PUBLIC cjson ${GLIB_LIBRARIES} ${GIO_LIBRARIES})

install(FILES "dh_string_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "file_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "list_util.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_generaliface.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
install(FILES "dh_general_isoc.h" DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/dh")
