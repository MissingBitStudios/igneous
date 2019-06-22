add_library(stb STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_connected_components.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_c_lexer.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_divide.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_dxt.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_easy_font.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_herringbone_wang_tile.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_resize.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_write.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_leakcheck.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_perlin.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_rect_pack.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_sprintf.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_textedit.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_tilemap_editor.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_truetype.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_vorbis.c
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_voxel_render.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stretchy_buffer.h
)

target_include_directories(stb PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/stb
)
