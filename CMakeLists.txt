cmake_minimum_required(VERSION 2.8)
project(7zpp)

file(GLOB 7ZPP_SOURCES  ${PROJECT_SOURCE_DIR}/7zpp/*.cpp )
add_library(7zpp ${7ZPP_SOURCES})
target_include_directories(7zpp PRIVATE  ${PROJECT_SOURCE_DIR}   ${PROJECT_SOURCE_DIR}/7zpp ${PROJECT_SOURCE_DIR}/7z/C  ${PROJECT_SOURCE_DIR}/7z/CPP)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost COMPONENTS filesystem ) 
# Setup testing

execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
message("Building googletest")
execute_process(
    COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ../ -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/build/ginst -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}  
    WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build)
execute_process(
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build --config Release 
        WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build)
execute_process(
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build --config Debug   
        WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build)
execute_process(
    COMMAND ${CMAKE_COMMAND} --build  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build --target Install
        WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/build)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/build/ginst/lib/cmake/GTest;${CMAKE_MODULE_PATH}")
set(GTEST_INCLUDE_DIR  ${PROJECT_SOURCE_DIR}/build/ginst/include)
set(GTEST_LIBRARY ${PROJECT_SOURCE_DIR}/build/ginst/lib/gtest.lib)
set(GTEST_MAIN_LIBRARY ${PROJECT_SOURCE_DIR}/build/ginst/lib/gtest_main.lib)

set(GTEST_LIBRARY_DEBUG ${PROJECT_SOURCE_DIR}/build/ginst/lib/gtestd.lib)
set(GTEST_MAIN_LIBRARY_DEBUG ${PROJECT_SOURCE_DIR}/build/ginst/lib/gtest_maind.lib)
enable_testing()
#find_package(GTest REQUIRED)

add_executable(TestApp  ${PROJECT_SOURCE_DIR}/7zpp-TestApp/7zpp-TestApp.cpp)

target_include_directories(TestApp PRIVATE 
${PROJECT_SOURCE_DIR}/7zpp-TestApp/ 
${PROJECT_SOURCE_DIR} 
${PROJECT_SOURCE_DIR}/7zpp 
${PROJECT_SOURCE_DIR}/7z/C  
${PROJECT_SOURCE_DIR}/7z/CPP
${Boost_INCLUDE_DIRS} 
${GTEST_INCLUDE_DIR}
)


add_dependencies(TestApp 7zpp)
target_link_libraries(TestApp 7zpp  ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG} ${Boost_LIBRARIES})

add_test(
    NAME runUnitTests
    COMMAND TestApp
)
