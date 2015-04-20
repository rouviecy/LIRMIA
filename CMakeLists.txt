cmake_minimum_required(VERSION 2.8)
project(betterave)

find_package(PkgConfig)

set(ENABLE_GRAPHVIZ ON)
set(ENABLE_PYTHON OFF)
set(ENABLE_JAVA OFF)
set(ENABLE_SDL OFF)
set(JAVA_LIBMAWT_PATH /etc/java-config-2/current-system-vm/jre/lib/amd64/xawt)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/vision/CMakeLists.txt)

add_definitions(-DENABLE_SERIAL)
add_definitions(-DENABLE_I2C)		# needs serial
#add_definitions(-DENABLE_DEPTH)	# needs serial and I2C
#add_definitions(-DENABLE_IMU)		# needs serial and I2C
add_definitions(-DENABLE_MOTORS)	# needs serial and I2C
#add_definitions(-DENABLE_TCP)
add_definitions(-DENABLE_CAM1)
#add_definitions(-DENABLE_CAM2)

find_package(OpenCV REQUIRED)
set(OTHER_LIBS
	${OpenCV_LIBS}
)

add_library(BETTERAVE_USER
	src/interfaces/FSM.h		src/interfaces/FSM.cpp
	src/interfaces/FSMDraw.h	src/interfaces/FSMDraw.cpp
	src/interfaces/Serial.h		src/interfaces/Serial.cpp
	src/interfaces/TCP_client.h	src/interfaces/TCP_client.cpp
	src/interfaces/TCP_server.h	src/interfaces/TCP_server.cpp
	src/parts/Acoustic_modem.h	src/parts/Acoustic_modem.cpp
	src/parts/Clock.h		src/parts/Clock.cpp
	src/parts/Autonomy.h		src/parts/Autonomy.cpp
	src/parts/Cameras.h		src/parts/Cameras.cpp
	src/parts/Depth.h		src/parts/Depth.cpp
	src/parts/Echosonder.h		src/parts/Echosonder.cpp
	src/parts/I2C.h			src/parts/I2C.cpp
	src/parts/Imu.h			src/parts/Imu.cpp
	src/parts/Logger.h		src/parts/Logger.cpp
	src/parts/Mapping.h		src/parts/Mapping.cpp
	src/parts/Motors.h		src/parts/Motors.cpp
	src/parts/Remote.h		src/parts/Remote.cpp
	src/parts/State.h		src/parts/State.cpp
	src/parts/State_machine.h	src/parts/State_machine.cpp
)

add_executable(lirmia src/mains/Lirmia.cpp)
target_link_libraries(lirmia ${LIBS} ${OTHER_LIBS} BETTERAVE_USER BETTERAVE_CORE VISION_LIBS)

add_executable(remote src/remote_controller.cpp)
target_link_libraries(remote ${LIBS} ${OTHER_LIBS} BETTERAVE_USER BETTERAVE_CORE VISION_LIBS)
