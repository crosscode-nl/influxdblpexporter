include(FetchContent)

SET(BUILD_LINE_BASED_WRITERS_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        line_based_writers
        GIT_REPOSITORY https://github.com/crosscode-nl/line_based_writers
        GIT_TAG v1.2.0
)
FetchContent_MakeAvailable(line_based_writers)

target_link_libraries(${PROJECT_NAME} line_based_writers)