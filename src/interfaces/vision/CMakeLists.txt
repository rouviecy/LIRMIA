cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

add_library(VISION_LIBS
	src/interfaces/vision/mapping/Arbre.h		src/interfaces/vision/mapping/Arbre.cpp
	src/interfaces/vision/mapping/Carte.h		src/interfaces/vision/mapping/Carte.cpp
	src/interfaces/vision/mapping/Multimap.h	src/interfaces/vision/mapping/Multimap.cpp
	src/interfaces/vision/Blobs.h			src/interfaces/vision/Blobs.cpp
	src/interfaces/vision/Body.h			src/interfaces/vision/Body.cpp
	src/interfaces/vision/Calibrate.h		src/interfaces/vision/Calibrate.cpp
	src/interfaces/vision/Flux_cam.h		src/interfaces/vision/Flux_cam.cpp
	src/interfaces/vision/Gui.h			src/interfaces/vision/Gui.cpp
	src/interfaces/vision/Instruments.h		src/interfaces/vision/Instruments.cpp
	src/interfaces/vision/IO_file.h			src/interfaces/vision/IO_file.cpp
	src/interfaces/vision/Reco.h			src/interfaces/vision/Reco.cpp
	src/interfaces/vision/Tracking.h		src/interfaces/vision/Tracking.cpp
	src/interfaces/vision/Transfo.h			src/interfaces/vision/Transfo.cpp
	src/interfaces/vision/Utils.h			src/interfaces/vision/Utils.cpp
	src/interfaces/vision/struct_HSV_bound.h
	src/interfaces/vision/struct_wrap_bound.h
	src/interfaces/vision/struct_nom.h
	src/interfaces/vision/struct_vignette.h
)
