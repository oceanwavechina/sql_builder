cmake_minimum_required(VERSION 2.8)
project(collection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    ./
)

IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_definitions(
        -DMACOSX -DACL_MACOSX
    )
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")


set(SQL_BUILDER_SRC
    sql_builder.cpp
    builder_test.cpp
)

add_executable(test ${SQL_BUILDER_SRC})


target_link_libraries(test
    boost_system
    boost_test_exec_monitor
)
