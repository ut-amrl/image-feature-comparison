PROJECT(feature_tracker)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

FIND_PACKAGE(OpenCV EXACT 3.2.0 REQUIRED)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

FIND_PACKAGE(gflags REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

ADD_EXECUTABLE(feature_tracker src/main.cc src/ft.cc)
TARGET_LINK_LIBRARIES(feature_tracker
                      ${OpenCV_LIBS} gflags opencv_xfeatures2d)
