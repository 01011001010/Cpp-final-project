cmake_minimum_required(VERSION 3.6.2)
project(CoffeeHouse)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(SOURCE_FILES
        Table.cpp
        Counter.cpp
        Kitchen.cpp
        Coffeehouse.cpp
        fileComprehensionFunctions.cpp

        tests.cpp


        GoogleTest/gtest-death-test.cc
        GoogleTest/gtest-filepath.cc
        GoogleTest/gtest-port.cc
        GoogleTest/gtest-printers.cc
        GoogleTest/gtest-test-part.cc
        GoogleTest/gtest-typed-test.cc
        GoogleTest/gtest.cc
        GoogleTest/gtest_main.cc)

include_directories(GoogleTest)
add_executable(project ${SOURCE_FILES})
target_link_libraries(project ${CMAKE_THREAD_LIBS_INIT})
add_custom_command(TARGET project PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Items1.txt
        ${CMAKE_SOURCE_DIR}/Items2.txt
        ${CMAKE_SOURCE_DIR}/Tables1.txt
        $<TARGET_FILE_DIR:project>
        VERBATIM)