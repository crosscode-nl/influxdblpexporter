include(FetchContent)

SET(BUILD_SIMPLE_INSTRUMENTS_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        simple_instruments
        GIT_REPOSITORY https://github.com/crosscode-nl/simple_instruments
)
FetchContent_MakeAvailable(simple_instruments)

target_link_libraries(${PROJECT_NAME} simple_instruments)