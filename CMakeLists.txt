cmake_minimum_required(VERSION 2.8)
project(betterave)

find_package(PkgConfig)

set(IS_LIRMIA ON)
set(IS_UNKNOWN OFF)
set(IS_R2D2 OFF)
set(IS_SIMU ON)

if(${IS_LIRMIA})
	set(ENABLE_PYTHON OFF)
	set(ENABLE_JAVA OFF)
	if(${IS_SIMU})
		set(ENABLE_GRAPHVIZ ON)
		set(ENABLE_SDL ON)
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_RECORDCAM)	# needs CAM1/CAM2
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1/CAM2
#		add_definitions(-DENABLE_MODEM)		# needs serial RS232
		add_definitions(-DENABLE_SIMULATOR)
	else(${IS_SIMU})
		set(ENABLE_GRAPHVIZ OFF)
		set(ENABLE_SDL OFF)
		add_definitions(-DENABLE_SERIAL_ARDUINO)
		add_definitions(-DENABLE_SERIAL_ISS)
		add_definitions(-DENABLE_SERIAL_RS232)
		add_definitions(-DENABLE_DEPTH)		# needs serial Arduino
		add_definitions(-DENABLE_IMU)		# needs serial Arduino
		add_definitions(-DENABLE_MOTORS)	# needs serial ISS
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_RECORDCAM)	# needs CAM1/CAM2
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1/CAM2
		add_definitions(-DENABLE_MODEM)		# needs serial RS232
	endif(${IS_SIMU})
elseif(${IS_UNKNOWN})
	set(ENABLE_PYTHON OFF)
	set(ENABLE_JAVA OFF)
	if(${IS_SIMU})
		set(ENABLE_GRAPHVIZ ON)
		set(ENABLE_SDL ON)
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
#		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1/CAM2
		add_definitions(-DENABLE_SIMULATOR)
#		add_definitions(-DENABLE_MODEM)		# needs serial RS232
	else(${IS_SIMU})
		set(ENABLE_GRAPHVIZ OFF)
		set(ENABLE_SDL OFF)
		add_definitions(-DENABLE_SERIAL_ARDUINO)
		add_definitions(-DENABLE_SERIAL_ISS)
		add_definitions(-DENABLE_SERIAL_RS232)
		add_definitions(-DENABLE_I2C)		# needs serial ISS
		add_definitions(-DENABLE_MOTORS)	# needs serial arduino, serial ISS and I2C
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1/CAM2
		add_definitions(-DENABLE_MODEM)		# needs serial RS232
	endif(${IS_SIMU})
elseif(${IS_R2D2})
	set(ENABLE_PYTHON OFF)
	set(ENABLE_JAVA OFF)
	if(${IS_SIMU})
		set(ENABLE_GRAPHVIZ ON)
		set(ENABLE_SDL ON)
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1
	else(${IS_SIMU})
		set(ENABLE_GRAPHVIZ OFF)
		set(ENABLE_SDL OFF)
#		add_definitions(-DENABLE_SERIAL)
#		add_definitions(-DENABLE_MOTORS)	# needs serial
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1
	endif(${IS_SIMU})
else(${IS_LIRMIA})
	set(ENABLE_GRAPHVIZ ON)
	set(ENABLE_SDL ON)
	add_definitions(-DENABLE_TCP)
	add_definitions(-DENABLE_CAM1)
	add_definitions(-DENABLE_CAM2)
	add_definitions(-DENABLE_RECORDCAM)	# needs CAM1/CAM2
	add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1/CAM2
#	add_definitions(-DENABLE_MODEM)		# needs serial RS232
endif(${IS_LIRMIA})

include(${CMAKE_CURRENT_SOURCE_DIR}/src/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/vision/CMakeLists.txt)

find_package(OpenCV REQUIRED)
set(OTHER_LIBS
	${OpenCV_LIBS}
)

add_library(BETTERAVE_USER_COMMON
	src/interfaces/Camera_server.h		src/interfaces/Camera_server.cpp
	src/interfaces/FSM.h			src/interfaces/FSM.cpp
	src/interfaces/FSMDraw.h		src/interfaces/FSMDraw.cpp
	src/interfaces/Joystick.h		src/interfaces/Joystick.cpp
	src/interfaces/Serial.h			src/interfaces/Serial.cpp
	src/interfaces/TCP_client.h		src/interfaces/TCP_client.cpp
	src/interfaces/TCP_server.h		src/interfaces/TCP_server.cpp
	src/parts/Clock.h			src/parts/Clock.cpp
	src/parts/Depth.h			src/parts/Depth.cpp
	src/parts/Echosonder.h			src/parts/Echosonder.cpp
	src/parts/Gps.h				src/parts/Gps.cpp
	src/parts/Imu_razor.h			src/parts/Imu_razor.cpp
	src/parts/Subscriber.h			src/parts/Subscriber.cpp
)

add_library(BETTERAVE_USER_LIRMIA
	src/parts/lirmia/Acoustic_modem.h	src/parts/lirmia/Acoustic_modem.cpp
	src/parts/lirmia/Autonomy.h		src/parts/lirmia/Autonomy.cpp
	src/parts/lirmia/Cameras.h		src/parts/lirmia/Cameras.cpp
	src/parts/lirmia/Logger.h		src/parts/lirmia/Logger.cpp
	src/parts/lirmia/Mapping.h		src/parts/lirmia/Mapping.cpp
	src/parts/lirmia/Motors.h		src/parts/lirmia/Motors.cpp
	src/parts/lirmia/Remote_control.h	src/parts/lirmia/Remote_control.cpp
	src/parts/lirmia/Remote_monitor.h	src/parts/lirmia/Remote_monitor.cpp
	src/parts/lirmia/Simulator.h		src/parts/lirmia/Simulator.cpp
	src/parts/lirmia/State.h		src/parts/lirmia/State.cpp
	src/parts/lirmia/State_machine.h	src/parts/lirmia/State_machine.cpp
)

add_library(BETTERAVE_USER_UNKNOWN
	src/parts/unknown/Acoustic_modem.h	src/parts/unknown/Acoustic_modem.cpp
	src/parts/unknown/Autonomy.h		src/parts/unknown/Autonomy.cpp
	src/parts/unknown/Cameras.h		src/parts/unknown/Cameras.cpp
	src/parts/unknown/Logger.h		src/parts/unknown/Logger.cpp
	src/parts/unknown/Motors.h		src/parts/unknown/Motors.cpp
	src/parts/unknown/Remote_control.h	src/parts/unknown/Remote_control.cpp
	src/parts/unknown/Remote_monitor.h	src/parts/unknown/Remote_monitor.cpp
	src/parts/unknown/Simulator.h		src/parts/unknown/Simulator.cpp
	src/parts/unknown/State.h		src/parts/unknown/State.cpp
	src/parts/unknown/State_machine.h	src/parts/unknown/State_machine.cpp
)

add_library(BETTERAVE_USER_R2D2
	src/parts/r2d2/Autonomy.h		src/parts/r2d2/Autonomy.cpp
	src/parts/r2d2/Cameras.h		src/parts/r2d2/Cameras.cpp
	src/parts/r2d2/Logger.h			src/parts/r2d2/Logger.cpp
	src/parts/r2d2/Motors.h			src/parts/r2d2/Motors.cpp
	src/parts/r2d2/Remote_control.h		src/parts/r2d2/Remote_control.cpp
	src/parts/r2d2/Remote_monitor.h		src/parts/r2d2/Remote_monitor.cpp
	src/parts/r2d2/State_machine.h		src/parts/r2d2/State_machine.cpp
)

add_executable(lirmia src/mains/Lirmia.cpp)
add_executable(unknown src/mains/Unknown.cpp)
add_executable(r2d2 src/mains/R2D2.cpp)

target_link_libraries(lirmia ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_LIRMIA BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
target_link_libraries(unknown ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_UNKNOWN BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
target_link_libraries(r2d2 ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_R2D2 BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)

add_executable(remote_lirmia src/remote_control_monitor_lirmia.cpp)
add_executable(remote_unknown src/remote_control_monitor_unknown.cpp)
add_executable(remote_r2d2 src/remote_control_monitor_r2d2.cpp)

target_link_libraries(remote_lirmia ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_LIRMIA BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
target_link_libraries(remote_unknown ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_UNKNOWN BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
target_link_libraries(remote_r2d2 ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_R2D2 BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)

add_executable(camera_lirmia src/remote_cam_lirmia.cpp)
add_executable(camera_unknown src/remote_cam_unknown.cpp)
add_executable(camera_r2d2 src/remote_cam_r2d2.cpp)

target_link_libraries(camera_lirmia ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
target_link_libraries(camera_unknown ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
target_link_libraries(camera_r2d2 ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)

add_executable(recam src/local_cam_rec.cpp)
target_link_libraries(recam ${LIBS} ${OTHER_LIBS} BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS)
