include(FetchContent)

SET(BUILD_INFLUXDBLPTOOL_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        influxdblptool
        GIT_REPOSITORY https://github.com/crosscode-nl/influxdblptool
        GIT_TAG v2.0.0
)
FetchContent_MakeAvailable(influxdblptool)

target_link_libraries(${PROJECT_NAME} influxdblptool)