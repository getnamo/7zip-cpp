cmake_minimum_required(VERSION 2.8)
project(7zpp)

file(GLOB 7ZPP_SOURCES  ${PROJECT_SOURCE_DIR}/7zpp/*.cpp )
add_library(7zpp ${7ZPP_SOURCES})
target_include_directories(7zpp PRIVATE  ${PROJECT_SOURCE_DIR}   ${PROJECT_SOURCE_DIR}/7zpp ${PROJECT_SOURCE_DIR}/7z/C  ${PROJECT_SOURCE_DIR}/7z/CPP)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost COMPONENTS filesystem ) 
# Setup testing
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Debug ... FORCE )
ENDIF()

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
    COMMAND TestApp
)
