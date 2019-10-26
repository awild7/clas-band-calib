# CMakeLists for SRC_Generator

# Set up
cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(ROOT REQUIRED)

project (hvScan)

set(INCLUDES ${ROOT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include )
include_directories(${INCLUDES})

add_executable(mksnp mksnp.cpp)
target_link_libraries(mksnp ${ROOT_LIBRARIES} )

add_executable(combFiles combFiles.cpp)
target_link_libraries(combFiles ${ROOT_LIBRARIES} )

add_executable(gainFit gainFit.cpp)
target_link_libraries(gainFit ${ROOT_LIBRARIES} )
