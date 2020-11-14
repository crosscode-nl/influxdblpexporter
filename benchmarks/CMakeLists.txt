cmake_minimum_required(VERSION 3.8.2)
set(PARENT_PROJECT_NAME ${PROJECT_NAME})
project(influxdblpexporter_benchmarks)

include(cmake/googlebenchmark.cmake)

file(GLOB BENCHMARKS_SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${BENCHMARKS_SRC})
target_link_libraries(${PROJECT_NAME} ${PARENT_PROJECT_NAME} ${3RDPARTY_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

