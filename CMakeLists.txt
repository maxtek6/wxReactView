cmake_minimum_required(VERSION 3.15)

project(wxreactview)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  wxWidgets
  GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
  GIT_TAG master  # or latest stable
)
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)
if(WIN32)
    set(wxUSE_WEBVIEW_EDGE ON CACHE BOOL "" FORCE)
    set(wxUSE_WEBVIEW_EDGE_STATIC ON CACHE BOOL "" FORCE)
    set(WEBVIEW2_INCLUDE "${WEBVIEW2_PACKAGE_DIR}/build/native/include")
endif()
FetchContent_MakeAvailable(wxWidgets)

add_library(wxreactview STATIC ${CMAKE_CURRENT_SOURCE_DIR}/wxreactview.cpp)
target_include_directories(wxreactview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${WEBVIEW2_INCLUDE})
target_link_libraries(wxreactview PUBLIC wx::core wx::base wx::webview)