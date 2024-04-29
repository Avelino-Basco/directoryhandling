cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

project(stitching VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(run run.cpp)

target_link_libraries( run ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
