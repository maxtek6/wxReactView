FetchContent_Declare(
  yyjson
  GIT_REPOSITORY https://github.com/ibireme/yyjson
  GIT_TAG master
)
set(YYJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)  # disable tests
set(YYJSON_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)  # static linking
FetchContent_MakeAvailable(yyjson)

add_executable(example WIN32 main.cpp)
target_link_libraries(example wxreactview yyjson)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/frontend/dist/index.html
    COMMAND npm run build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/frontend
    COMMENT "Building React frontend"
)
add_custom_command(
    TARGET example PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/frontend/dist
            $<TARGET_FILE_DIR:example>/dist
    COMMENT "Copying React dist/ to build output directory"
)
add_custom_target(copy_frontend ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/frontend/dist/index.html
)

add_custom_command( 
    TARGET example 
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CEF_ROOT}/Release $<TARGET_FILE_DIR:example> 
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CEF_ROOT}/Resources $<TARGET_FILE_DIR:example> 
    COMMENT "Copying CEF resources..." 
    ) 