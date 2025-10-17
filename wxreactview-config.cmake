get_filename_component(WXREACTVIEW_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${WXREACTVIEW_CMAKE_DIR})

if(NOT TARGET wxreactview::wxreactview)
    include("${WXREACTVIEW_CMAKE_DIR}/wxreactview-targets.cmake")
endif()

set(wxreactview_LIBRARIES wxreactview::wxreactview)