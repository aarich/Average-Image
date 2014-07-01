cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
 
project(averageImage)

find_package( OpenCV REQUIRED )
find_package(Boost REQUIRED)

add_executable(averageImage src/main.cpp)

#add_executable(workingFile src/workingFile.cpp)

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

target_link_libraries (averageImage ${OpenCV_LIBRARIES})

