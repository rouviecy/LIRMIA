cmake_minimum_required(VERSION 2.8)
project(betterave)

find_package(PkgConfig)

set(ENABLE_GRAPHVIZ ON)
set(ENABLE_PYTHON OFF)
set(ENABLE_JAVA OFF)
set(ENABLE_SDL ON)
set(JAVA_LIBMAWT_PATH /etc/java-config-2/current-system-vm/jre/lib/amd64/xawt)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/core/CMakeLists.txt)

add_library(BETTERAVE_USER
	src/interfaces/FSM.h		src/interfaces/FSM.cpp
	src/interfaces/FSMDraw.h	src/interfaces/FSMDraw.cpp
	src/interfaces/Serial.h		src/interfaces/Serial.cpp
	src/interfaces/TCP_client.h	src/interfaces/TCP_client.cpp
	src/interfaces/TCP_server.h	src/interfaces/TCP_server.cpp
	src/parts/Sensor.h		src/parts/Sensor.cpp
	src/parts/State.h		src/parts/State.cpp
	src/parts/State_machine.h	src/parts/State_machine.cpp
)

add_executable(lirmia src/mains/Lirmia.cpp)
target_link_libraries(lirmia ${LIBS} BETTERAVE_USER BETTERAVE_CORE)
