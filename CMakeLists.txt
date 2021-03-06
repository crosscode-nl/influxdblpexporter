cmake_minimum_required(VERSION 3.8.2)

project(influxdblpexporter
        VERSION 0.2.0
        LANGUAGES C CXX)

set(PACKAGE_NAME InfluxDBLPExporter)

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

include(cmake/buildsetup.cmake)

option(BUILD_${PROJECT_NAME_UPPER}_TESTS "Set this to ON to build unit tests" ON)
option(BUILD_${PROJECT_NAME_UPPER}_DOCS "Set this to ON to build docs")
option(BUILD_${PROJECT_NAME_UPPER}_EXAMPLES "Set this to ON to build examples")
option(BUILD_${PROJECT_NAME_UPPER}_BENCHMARKS "Set this to ON to build benchmarks")

configure_file(src/${PROJECT_NAME}/version.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/src/${PROJECT_NAME}/version.cpp)
configure_file(include/${PROJECT_NAME}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/version.h)

list(APPEND ${PROJECT_NAME}_HEADERS
        include/${PROJECT_NAME}.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/version.h
        include/${PROJECT_NAME}/influxdb_line_protocol_exporter.h
        include/${PROJECT_NAME}/instrument_factory_builder.h
        include/${PROJECT_NAME}/metadata_builder.h
        )

list(APPEND ${PROJECT_NAME}_SRC
        ${CMAKE_CURRENT_BINARY_DIR}/src/${PROJECT_NAME}/version.cpp
        src/${PROJECT_NAME}/instrument_factory_builder.cpp
        src/${PROJECT_NAME}/metadata_builder.cpp
        )

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} )

include(cmake/influxdblptool.cmake)
include(cmake/simple_instruments.cmake)
include(cmake/line_based_writers.cmake)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

include(cmake/install.cmake)

if (BUILD_${PROJECT_NAME_UPPER}_DOCS)
    include(cmake/doxygen.cmake)
endif()

if (BUILD_${PROJECT_NAME_UPPER}_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_${PROJECT_NAME_UPPER}_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if (BUILD_${PROJECT_NAME_UPPER}_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
