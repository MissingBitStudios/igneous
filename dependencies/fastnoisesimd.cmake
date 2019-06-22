add_library(fastnoisesimd STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/ARM/cpu-features.c
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/ARM/cpu-features.h
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD.h
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_internal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_internal.h
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_neon.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_sse2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp
)

target_include_directories(fastnoisesimd PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/fastnoisesimd
)
