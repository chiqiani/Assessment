set( CMAKE_CXX_FLAGS "-std=c++11")

find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable ( main main.cpp)
target_link_libraries( main ${OpenCV_LIBS})
