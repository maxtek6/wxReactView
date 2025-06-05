cmake_minimum_required(VERSION 3.15)

project(wxreactview)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  wxWidgets
  GIT_REPOSITORY https://github.com/maxtek6/wxWidgets.git
  GIT_TAG feature/chromium-start-request
)
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)
if(WIN32)
  set(wxUSE_WEBVIEW_EDGE ON CACHE BOOL "" FORCE)
  set(wxUSE_WEBVIEW_EDGE_STATIC ON CACHE BOOL "" FORCE)
else()
  set(wxUSE_WEBVIEW_CHROMIUM ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(wxWidgets)

FetchContent_Declare(
    hyperpage
    GIT_REPOSITORY https://github.com/maxtek6/hyperpage.git
    GIT_TAG master
)
FetchContent_MakeAvailable(hyperpage)

add_library(wxreactview STATIC ${CMAKE_CURRENT_SOURCE_DIR}/wxreactview.cpp)
if(WIN32)
    target_include_directories(wxreactview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "${WEBVIEW2_PACKAGE_DIR}/build/native/include")
    target_link_libraries(wxreactview PUBLIC wx::core wx::base wx::webview hyperpage)
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CEF_ROOT}/cmake)
    find_package(CEF REQUIRED)
    target_include_directories(wxreactview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CEF_ROOT})
    target_link_directories(wxreactview PUBLIC ${CEF_ROOT}/Release)
    target_link_libraries(wxreactview PUBLIC wx::core wx::base wx::webview hyperpage ${CEF_STANDARD_LIBS})
endif()

function(wxreactview_add_executable TARGET_NAME)
    cmake_parse_arguments(WXREACTAPP "" "FRONTEND" "SOURCES" ${ARGN})

    if(NOT WXREACTAPP_SOURCES)
        message(FATAL_ERROR "wxreactview_add_executable: SOURCES must be specified")
    endif()

    if(NOT WXREACTAPP_FRONTEND)
        message(FATAL_ERROR "wxreactview_add_executable: FRONTEND must be specified")
    endif()

    if(WIN32)
        add_executable(${TARGET_NAME} WIN32 ${WXREACTAPP_SOURCES})
    else()
        add_executable(${TARGET_NAME} ${WXREACTAPP_SOURCES})
    endif()
    
    message (STATUS "Adding executable ${TARGET_NAME} with sources: ${WXREACTAPP_SOURCES}")
    message (STATUS "Frontend directory: ${WXREACTAPP_FRONTEND}")

    if(NOT EXISTS "${WXREACTAPP_FRONTEND}/package.json")
        message(FATAL_ERROR "FRONTEND path '${WXREACTAPP_FRONTEND}' is invalid or missing package.json")
    endif()

    add_custom_command(
        TARGET ${TARGET_NAME} PRE_BUILD
        COMMAND npm run build
        WORKING_DIRECTORY ${WXREACTAPP_FRONTEND}
        COMMENT "Building React frontend"
    )

    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND $<TARGET_FILE:hyperpack> -o $<TARGET_FILE_DIR:${TARGET_NAME}>/hyperpage.db ${WXREACTAPP_FRONTEND}/dist
        COMMENT "Building hyperpack archive from React app dist folder"
    )

    if(WIN32)
    else()
        add_custom_command( 
            TARGET ${TARGET_NAME}
            POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CEF_ROOT}/Release $<TARGET_FILE_DIR:example> 
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CEF_ROOT}/Resources $<TARGET_FILE_DIR:example> 
            COMMENT "Copying CEF resources..." 
        ) 
    endif()
endfunction()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)
