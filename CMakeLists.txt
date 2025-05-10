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
set(wxUSE_WEBVIEW_CHROMIUM ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(wxWidgets)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CEF_ROOT}/cmake)
find_package(CEF REQUIRED)

add_library(wxreactview STATIC ${CMAKE_CURRENT_SOURCE_DIR}/wxreactview.cpp)
target_include_directories(wxreactview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CEF_ROOT})
target_link_directories(wxreactview PUBLIC ${CEF_ROOT}/Release)
target_link_libraries(wxreactview PUBLIC wx::core wx::base wx::webview ${CEF_STANDARD_LIBS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)
