cmake_minimum_required(VERSION 3.10)

project(zmq_out)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_HOST_UNIX)
	SET(CMAKE_CXX_FLAGS "-fPIC -DPIC")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
	add_definitions(-D_X86_)
	add_definitions(-Drabbitmq_EXPORTS)
 
elseif(CMAKE_HOST_WIN32)

 
else()
	message("current platform: unkonw ")
endif()

#set(Qt_DIR ${PROJECT_SOURCE_DIR}/include/qt)
#set(Qt5_DIR /Users/woosang/Qt5.9.7/5.9.7/clang_64/lib/cmake/Qt5)
#   specific the cmake-prefix-path of qt5
set(CMAKE_PREFIX_PATH /Users/woosang/Qt5.9.7/5.9.7/clang_64/lib/cmake)
#set(Qt5Core_DIR ${PROJECT_SOURCE_DIR}/include/qt/lib/cmake/Qt5Core)

include_directories(${PROJECT_SOURCE_DIR}/include/qt/include)
include_directories(${PROJECT_SOURCE_DIR}/include/rabbitmq-c)
include_directories(${PROJECT_SOURCE_DIR}/include/zmq)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/usr/local/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/include/qt/lib)
link_directories(${PROJECT_SOURCE_DIR}/include/zmq)

find_package(Qt5Core)
find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent)
find_package(Qt5 COMPONENTS Core Network Concurrent REQUIRED)
find_package(OpenCV REQUIRED )
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/TcpServer.cpp src/handler/tools.hpp src/handler/BaseHandler.cpp src/handler/BaseHandler.h src/handler/ZmqOutHandler.h src/handler/ZmqOutHandler.cpp)


if(CMAKE_HOST_UNIX)
	target_link_libraries(${CMAKE_PROJECT_NAME} Qt5Core Qt5Network Qt5Gui)
	target_link_libraries(${CMAKE_PROJECT_NAME} zmq)
	target_link_libraries(${CMAKE_PROJECT_NAME} evl)
	target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} )
 
elseif(CMAKE_HOST_WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} Qt5Cored Qt5Networkd)
	target_link_libraries(${CMAKE_PROJECT_NAME} libzmq-v142-mt-4_3_4)
 
else()

	message("current platform: unkonw ")
 
endif()

