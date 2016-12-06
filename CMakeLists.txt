cmake_minimum_required(VERSION 2.8)
project(betterave)

find_package(PkgConfig)

set(IS_LIRMIA ON)
set(IS_SAYAB OFF)
set(IS_R2D2 OFF)
set(IS_COOKIE OFF)
set(IS_SIMU OFF)

set(ENABLE_PYTHON OFF)
set(ENABLE_JAVA OFF)

if(${IS_LIRMIA})
	set(ENABLE_GRAPHVIZ ON)
	set(ENABLE_SDL ON)
	if(${IS_SIMU})
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_RECORDCAM)		# needs CAM1/CAM2
		add_definitions(-DENABLE_TCPCAM)		# needs TCP and CAM1/CAM2
		add_definitions(-DENABLE_SIMULATOR)
	else(${IS_SIMU})
		add_definitions(-DENABLE_SERIAL_ARDUINO)
		add_definitions(-DENABLE_SERIAL_ISS)
#		add_definitions(-DENABLE_SERIAL_RS232_MODEM)
#		add_definitions(-DENABLE_SERIAL_RS232_ECHO_X)
#		add_definitions(-DENABLE_SERIAL_RS232_ECHO_Y)
		add_definitions(-DENABLE_SERIAL_RS232_IMU)
		add_definitions(-DENABLE_DEPTH)			# needs serial Arduino
		add_definitions(-DENABLE_IMU)			# needs serial Arduino
		add_definitions(-DENABLE_MOTORS)		# needs serial ISS
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_RECORDCAM)		# needs CAM1/CAM2
		add_definitions(-DENABLE_TCPCAM)		# needs TCP and CAM1/CAM2
	endif(${IS_SIMU})
elseif(${IS_SAYAB})
	set(ENABLE_GRAPHVIZ ON)
	set(ENABLE_SDL ON)
	if(${IS_SIMU})
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_TCPCAM)		# needs TCP and CAM1/CAM2
		add_definitions(-DENABLE_SIMULATOR)
add_definitions(-DENABLE_SERIAL_RS232_MODEM)
	else(${IS_SIMU})
		add_definitions(-DENABLE_SERIAL_ARDUINO)
		add_definitions(-DENABLE_SERIAL_ISS)
#		add_definitions(-DENABLE_SERIAL_RS232_MODEM)
		add_definitions(-DENABLE_MOTORS)		# needs serial arduino and serial ISS
		add_definitions(-DENABLE_GPS)			# needs serial arduino
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_CAM2)
		add_definitions(-DENABLE_TCPCAM)		# needs TCP and CAM1/CAM2
	endif(${IS_SIMU})
elseif(${IS_R2D2})
	if(${IS_SIMU})
		set(ENABLE_GRAPHVIZ ON)
		set(ENABLE_SDL ON)
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1
	else(${IS_SIMU})
		set(ENABLE_GRAPHVIZ OFF)
		set(ENABLE_SDL OFF)
		add_definitions(-DENABLE_SERIAL)
		add_definitions(-DENABLE_I2C)
		add_definitions(-DENABLE_MOTORS)	# needs serial
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1
	endif(${IS_SIMU})
elseif(${IS_COOKIE})
	if(${IS_SIMU})
		set(ENABLE_GRAPHVIZ ON)
		set(ENABLE_SDL ON)
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1
	else(${IS_SIMU})
		set(ENABLE_GRAPHVIZ OFF)
		set(ENABLE_SDL OFF)
		add_definitions(-DENABLE_SERIAL_POLOLU)
		add_definitions(-DENABLE_I2C)
		add_definitions(-DENABLE_SPI)
		add_definitions(-DENABLE_MOTORS)	# needs serial Pololu
		add_definitions(-DENABLE_TCP)
		add_definitions(-DENABLE_CAM1)
		add_definitions(-DENABLE_TCPCAM)	# needs TCP and CAM1
	endif(${IS_SIMU})
endif(${IS_LIRMIA})

include(${CMAKE_CURRENT_SOURCE_DIR}/src/core/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/vision/CMakeLists.txt)

find_package(OpenCV REQUIRED)
set(OTHER_LIBS
	${OpenCV_LIBS}
)

add_library(BETTERAVE_USER_COMMON
	src/interfaces/Camera_client.h		src/interfaces/Camera_client.cpp
	src/interfaces/Camera_server.h		src/interfaces/Camera_server.cpp
	src/interfaces/FSM.h			src/interfaces/FSM.cpp
	src/interfaces/FSMDraw.h		src/interfaces/FSMDraw.cpp
	src/interfaces/I2C.h			src/interfaces/I2C.cpp
	src/interfaces/Joystick.h		src/interfaces/Joystick.cpp
	src/interfaces/Serial.h			src/interfaces/Serial.cpp
	src/interfaces/SPI.h			src/interfaces/SPI.cpp
	src/interfaces/TCP_client.h		src/interfaces/TCP_client.cpp
	src/interfaces/TCP_server.h		src/interfaces/TCP_server.cpp
	src/parts/Clock.h			src/parts/Clock.cpp
	src/parts/Compass_CMPS10.h		src/parts/Compass_CMPS10.cpp
	src/parts/Depth_MS5803.h		src/parts/Depth_MS5803.cpp
	src/parts/Depth_BMP085.h		src/parts/Depth_BMP085.cpp
	src/parts/Echosonder.h			src/parts/Echosonder.cpp
	src/parts/Gps.h				src/parts/Gps.cpp
	src/parts/Gyro_ITG3200.h		src/parts/Gyro_ITG3200.cpp
	src/parts/Imu_UM6LT.h			src/parts/Imu_UM6LT.cpp
	src/parts/Imu_razor.h			src/parts/Imu_razor.cpp
	src/parts/Subscriber.h			src/parts/Subscriber.cpp
	src/interfaces/Maths_tools.h
)

if(${IS_LIRMIA})
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
	add_executable(lirmia src/mains/Lirmia.cpp)
	add_executable(camera_lirmia src/remote/remote_cam_lirmia.cpp)
	add_executable(remote_lirmia src/remote/remote_control_monitor_lirmia.cpp)
	add_executable(unit_testing_blobs src/units_testing/blobs.cpp)
	add_executable(unit_testing_pipeline src/units_testing/pipeline.cpp)
	add_executable(recam src/units_testing/local_cam_rec.cpp)
	target_link_libraries(BETTERAVE_USER_LIRMIA BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS ${LIBS} ${OTHER_LIBS})
	target_link_libraries(lirmia BETTERAVE_USER_LIRMIA)
	target_link_libraries(camera_lirmia BETTERAVE_USER_LIRMIA)
	target_link_libraries(remote_lirmia BETTERAVE_USER_LIRMIA)
	target_link_libraries(unit_testing_blobs BETTERAVE_USER_LIRMIA)
	target_link_libraries(unit_testing_pipeline BETTERAVE_USER_LIRMIA)
	target_link_libraries(recam BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS ${LIBS} ${OTHER_LIBS})
elseif(${IS_SAYAB})
	add_library(BETTERAVE_USER_SAYAB
		src/parts/sayab/Acoustic_modem.h	src/parts/sayab/Acoustic_modem.cpp
		src/parts/sayab/Autonomy.h		src/parts/sayab/Autonomy.cpp
		src/parts/sayab/Cameras.h		src/parts/sayab/Cameras.cpp
		src/parts/sayab/Logger.h		src/parts/sayab/Logger.cpp
		src/parts/sayab/Motors.h		src/parts/sayab/Motors.cpp
		src/parts/sayab/Remote_control.h	src/parts/sayab/Remote_control.cpp
		src/parts/sayab/Remote_monitor.h	src/parts/sayab/Remote_monitor.cpp
		src/parts/sayab/Simulator.h		src/parts/sayab/Simulator.cpp
		src/parts/sayab/State.h			src/parts/sayab/State.cpp
		src/parts/sayab/State_machine.h		src/parts/sayab/State_machine.cpp
	)
	add_executable(sayab src/mains/Sayab.cpp)
	add_executable(camera_sayab src/remote/remote_cam_sayab.cpp)
	add_executable(remote_sayab src/remote/remote_control_monitor_sayab.cpp)
	target_link_libraries(BETTERAVE_USER_SAYAB BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS ${LIBS} ${OTHER_LIBS})
	target_link_libraries(sayab BETTERAVE_USER_SAYAB)
	target_link_libraries(camera_sayab BETTERAVE_USER_SAYAB)
	target_link_libraries(remote_sayab BETTERAVE_USER_SAYAB)
elseif(${IS_R2D2})
	add_library(BETTERAVE_USER_R2D2
		src/parts/r2d2/Autonomy.h		src/parts/r2d2/Autonomy.cpp
		src/parts/r2d2/Cameras.h		src/parts/r2d2/Cameras.cpp
		src/parts/r2d2/Logger.h			src/parts/r2d2/Logger.cpp
		src/parts/r2d2/Motors.h			src/parts/r2d2/Motors.cpp
		src/parts/r2d2/Remote_control.h		src/parts/r2d2/Remote_control.cpp
		src/parts/r2d2/Remote_monitor.h		src/parts/r2d2/Remote_monitor.cpp
		src/parts/r2d2/State.h			src/parts/r2d2/State.cpp
		src/parts/r2d2/State_machine.h		src/parts/r2d2/State_machine.cpp
	)
	add_executable(r2d2 src/mains/R2D2.cpp)
	add_executable(camera_r2d2 src/remote/remote_cam_r2d2.cpp)
	add_executable(remote_r2d2 src/remote/remote_control_monitor_r2d2.cpp)
	target_link_libraries(BETTERAVE_USER_R2D2 BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS ${LIBS} ${OTHER_LIBS})
	target_link_libraries(r2d2 BETTERAVE_USER_R2D2)
	target_link_libraries(camera_r2d2 BETTERAVE_USER_R2D2)
	target_link_libraries(remote_r2d2 BETTERAVE_USER_R2D2)
elseif(${IS_COOKIE})
	add_library(BETTERAVE_USER_COOKIE
		src/parts/cookie/Autonomy.h		src/parts/cookie/Autonomy.cpp
		src/parts/cookie/Cameras.h		src/parts/cookie/Cameras.cpp
		src/parts/cookie/Logger.h		src/parts/cookie/Logger.cpp
		src/parts/cookie/Motors.h		src/parts/cookie/Motors.cpp
		src/parts/cookie/Remote_control.h	src/parts/cookie/Remote_control.cpp
		src/parts/cookie/Remote_monitor.h	src/parts/cookie/Remote_monitor.cpp
		src/parts/cookie/State.h		src/parts/cookie/State.cpp
		src/parts/cookie/State_machine.h	src/parts/cookie/State_machine.cpp
	)
	add_executable(cookie src/mains/Cookie.cpp)
	add_executable(camera_cookie src/remote/remote_cam_cookie.cpp)
	add_executable(remote_cookie src/remote/remote_control_monitor_cookie.cpp)
	target_link_libraries(BETTERAVE_USER_COOKIE BETTERAVE_USER_COMMON BETTERAVE_CORE VISION_LIBS ${LIBS} ${OTHER_LIBS})
	target_link_libraries(cookie BETTERAVE_USER_COOKIE)
	target_link_libraries(camera_cookie BETTERAVE_USER_COOKIE)
	target_link_libraries(remote_cookie BETTERAVE_USER_COOKIE)
endif(${IS_LIRMIA})
