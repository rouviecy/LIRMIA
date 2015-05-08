cmake_minimum_required(VERSION 2.8)
project(betterave)

find_package(PkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

if(${ENABLE_GRAPHVIZ})
	pkg_check_modules(GVC REQUIRED libgvc)
	if(GVC_FOUND)
		add_definitions(-DENABLE_GRAPHVIZ)
		add_definitions(-DHAVE_STRING_H) # patch for broken >=graphviz-2.30 headers
		include_directories(${GVC_INCLUDE_DIRS})
		link_directories(${GVC_LIBRARY_DIRS})
		set(LIBS ${LIBS} ${GVC_LIBRARIES})
	endif(GVC_FOUND)
endif(${ENABLE_GRAPHVIZ})

if(${ENABLE_SDL})
	pkg_search_module(SDL2 REQUIRED sdl2)
	if(SDL2_FOUND)
		add_definitions(-DENABLE_SDL)
		set(LIBS ${LIBS} ${SDL2_LIBRARIES})
	endif(SDL2_FOUND)
endif(${ENABLE_SDL})

if(${ENABLE_JAVA})
	find_package(Java REQUIRED)
	find_package(JNI REQUIRED)
	if(JNI_FOUND)
		include(UseJava)
		set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.6" "-target" "1.6")
		file(GLOB JAVA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/parts/java/*.java")
		add_jar(Java_parts ${JAVA_SOURCES})
		get_target_property(_jarFile Java_parts JAR_FILE)
		get_target_property(_classDir Java_parts CLASSDIR)
		add_definitions(-DENABLE_JAVA)
		include_directories(${JNI_INCLUDE_DIRS})
		find_library(JAVA_MAWT_LIBRARY mawt ${JAVA_LIBMAWT_PATH})
		set(LIBS ${LIBS} ${JNI_LIBRARIES} ${JAVA_JVM_LIBRARY} ${JAVA_AWT_LIBRARY} ${JAVA_MAWT_LIBRARY})
		add_library(BETTERAVE_CORE
			src/core/CoreJava.h		src/core/CoreJava.cpp
			src/core/ComJava.h		src/core/ComJava.cpp
		)
	endif(JNI_FOUND)
endif(${ENABLE_JAVA})

if(${ENABLE_PYTHON})
	find_package(PythonLibs REQUIRED)
	if(PYTHONLIBS_FOUND)
		add_definitions(-DENABLE_PYTHON)
		include_directories(${PYTHON_INCLUDE_DIRS})
		set(LIBS ${LIBS} ${PYTHON_LIBRARIES})
		add_library(BETTERAVE_CORE
			src/core/ComPython.h		src/core/ComPython.cpp
		)
	endif(PYTHONLIBS_FOUND)
endif(${ENABLE_PYTHON})

add_library(BETTERAVE_CORE
	src/core/ComDraw.h		src/core/ComDraw.cpp
	src/core/ComThread.h		src/core/ComThread.cpp
	src/core/Maestro.h		src/core/Maestro.cpp
	src/core/Share.h		src/core/Share.cpp
)
