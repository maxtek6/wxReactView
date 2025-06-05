FetchContent_Declare(
  yyjson
  GIT_REPOSITORY https://github.com/ibireme/yyjson
  GIT_TAG master
)
set(YYJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)  # disable tests
set(YYJSON_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)  # static linking
FetchContent_MakeAvailable(yyjson)

wxreactview_add_executable(example
    FRONTEND ${CMAKE_CURRENT_SOURCE_DIR}/frontend
    SOURCES main.cpp
)
target_link_libraries(example wxreactview yyjson)