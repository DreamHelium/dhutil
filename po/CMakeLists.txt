set( POT_NAME "dhutil" )

set( POT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${POT_NAME}.pot)

find_program(XGETTEXT_EXECUTABLE xgettext)

# You could just do it by yourself
#find_program(MSGINIT_EXECUTABLE msginit)

set( _abs_DHUTIL_MAIN_POT_FILE_DEPENDS )
foreach(_currentDepends ${DHUTIL_MAIN_POT_FILE_DEPENDS})
	set(_abs_DHUTIL_MAIN_POT_FILE_DEPENDS ${_abs_DHUTIL_MAIN_POT_FILE_DEPENDS} ${dhutil_SOURCE_DIR}/${_currentDepends})
endforeach()

if(XGETTEXT_EXECUTABLE)
	set( XGETTEXT_OPTIONS -k_ --default-domain=${POT_NAME} )
        add_custom_target( dhutil_mkpot ALL
		COMMAND "${XGETTEXT_EXECUTABLE}" ${XGETTEXT_OPTIONS} --output=${POT_FILE} ${DHUTIL_MAIN_POT_FILE_DEPENDS}
            WORKING_DIRECTORY ${dhutil_SOURCE_DIR}
	    DEPENDS ${dhutil_SOURCE_DIR}/${DHUTIL_MAIN_POT_FILE_DEPENDS}
            COMMENT "Extract translation messages to ${POT_FILE}"
        )
endif()

set(PO_POS ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.po")
set(OUTPUT_LANG_POS ${CMAKE_BINARY_DIR}/src/locale)

message(STATUS ${FIRST_PO_FILES})

if(NOT EXISTS "${OUTPUT_LANG_POS}")
    file(MAKE_DIRECTORY ${OUTPUT_LANG_POS})
endif()

foreach( _currentPoFile_F ${PO_FILES} )
    get_filename_component(_lang ${_currentPoFile_F} NAME_WE)
    message(STATUS ${_lang})
    set( _moFile ${OUTPUT_LANG_POS}/${_lang}/LC_MESSAGES/${POT_NAME}.mo )
    if(NOT EXISTS ${OUTPUT_LANG_POS}/${_lang}/LC_MESSAGES)
        file(MAKE_DIRECTORY ${OUTPUT_LANG_POS}/${_lang}/LC_MESSAGES)
    endif()

    add_custom_target( dhutil_update_po ALL
    COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --no-fuzzy-matching -U
        ${CMAKE_CURRENT_SOURCE_DIR}/${_currentPoFile_F} ${POT_FILE}
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_moFile} ${CMAKE_CURRENT_SOURCE_DIR}/${_currentPoFile_F}
        DEPENDS ${POT_FILE}
        COMMENT "Updating translation ${_currentPoFile_F} of ${POT_NAME}"
    )

    install( FILES ${_moFile}
	    DESTINATION share/locale/${_lang}/LC_MESSAGES
    )
endforeach()
