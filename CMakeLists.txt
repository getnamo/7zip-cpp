cmake_minimum_required(VERSION 2.8)
project(7zpp)

file(GLOB 7ZPP_SOURCES  ${PROJECT_SOURCE_DIR}/7zpp/*.cpp )
add_library(7zpp ${7ZPP_SOURCES})
target_include_directories(7zpp PRIVATE  ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/7zpp ${PROJECT_SOURCE_DIR}/7z/C  ${PROJECT_SOURCE_DIR}/7z/CPP)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost COMPONENTS filesystem ) 

add_executable(TestApp  ${PROJECT_SOURCE_DIR}/7zpp-TestApp/7zpp-TestApp.cpp)
target_include_directories(TestApp PRIVATE 
${PROJECT_SOURCE_DIR}/7zpp-TestApp/ 
${PROJECT_SOURCE_DIR} 
${PROJECT_SOURCE_DIR}/7zpp 
${PROJECT_SOURCE_DIR}/7z/C  
${PROJECT_SOURCE_DIR}/7z/CPP
${PROJECT_SOURCE_DIR}/googletest/googletest/include
${Boost_INCLUDE_DIRS} 
)
add_subdirectory(googletest)
add_dependencies(TestApp 7zpp)
target_link_libraries(TestApp 7zpp ${Boost_LIBRARIES})