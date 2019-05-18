cmake_minimum_required(VERSION 2.8)
project(7zpp)
include(CTest)

file(GLOB 7ZPP_SOURCES  ${PROJECT_SOURCE_DIR}/7zpp/*.cpp )
add_library(7zpp ${7ZPP_SOURCES})
target_include_directories(7zpp PRIVATE  ${PROJECT_SOURCE_DIR}   ${PROJECT_SOURCE_DIR}/7zpp ${PROJECT_SOURCE_DIR}/7z/C  ${PROJECT_SOURCE_DIR}/7z/CPP)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost COMPONENTS filesystem ) 
# Setup testing
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Debug)
ENDIF()
if(Boost_FOUND)
    message("Building and running tests")
    enable_testing()


    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/ ${CMAKE_SOURCE_DIR}/googletest/include ${CMAKE_SOURCE_DIR}/googletest/googletest ${CMAKE_SOURCE_DIR}/googletest/googletest/include)
    #find_package(GTest REQUIRED)

    add_executable(TestApp  ${PROJECT_SOURCE_DIR}/7zpp-TestApp/7zpp-TestApp.cpp)

    target_include_directories(TestApp PRIVATE 
    ${PROJECT_SOURCE_DIR}/7zpp-TestApp/ 
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/7zpp 
    ${PROJECT_SOURCE_DIR}/7z/C  
    ${PROJECT_SOURCE_DIR}/7z/CPP
    ${Boost_INCLUDE_DIRS} 
    )


    add_dependencies(TestApp 7zpp)
    set(GTEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc)
    add_library(gtest STATIC ${GTEST_SRC}) 

    target_link_libraries(TestApp 7zpp gtest ${Boost_LIBRARIES})


    add_test(
        NAME runUnitTests
        COMMAND TestApp --gtest_output=xml:tests.xml
    )


    file(COPY ${PROJECT_SOURCE_DIR}/7zpp-TestApp/TestFiles DESTINATION  ${CMAKE_SOURCE_DIR}/build)

    set(UNIT_TEST TestApp)
else()
    message("Not running tests:Make sure you have boost installed.")
endif()

#The following is needed to run tests properly
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
