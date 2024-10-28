cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# project name and enable C++ support
project(movement_trainer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  
 
include( files.cmake )
 
find_package(Boost COMPONENTS system filesystem thread unit_test_framework REQUIRED)


add_library( QuantLib SHARED IMPORTED )
set_property( TARGET QuantLib PROPERTY IMPORTED_LOCATION /home/peta/code_pub/QuantLib/install/lib/libQuantLib.so )
target_include_directories( QuantLib INTERFACE /home/peta/code_pub/QuantLib/install/include )

add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})


set( EXECUTABLE_NAME test1 )

add_executable(${EXECUTABLE_NAME} ${sources} ${include_files} )

target_include_directories(${EXECUTABLE_NAME} PRIVATE include . )
target_link_directories(${EXECUTABLE_NAME} PRIVATE ${Boost_LIBRARY_DIRS} )

target_link_libraries(${EXECUTABLE_NAME} QuantLib )
