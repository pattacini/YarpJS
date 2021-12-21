cmake_minimum_required(VERSION 2.8.12)

project(YarpJS)


# Essential include files to build a node addon,
# you should add this line in every CMake.js based project.
include_directories(${CMAKE_JS_INC})

file(GLOB SOURCE_FILES "YarpJS/src/*.cpp" "YarpJS/include/*.h")

find_package(YARP)
find_package(OpenCV)


include_directories(${YARP_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("YarpJS/include")


# This line will tell CMake that we're building a shared library
# from the above source files
# named after the project's name
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})


# This line will give our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")



# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBRARIES} ${YARP_LIBRARIES})

# install(TARGETS hello_yarp
# 	RUNTIME DESTINATION 	${CMAKE_CURRENT_SOURCE_DIR}/../bin)
