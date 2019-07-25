set(OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${OUTDIR})
file(MAKE_DIRECTORY ${OUTDIR}/glsl)
file(MAKE_DIRECTORY ${OUTDIR}/essl)
file(MAKE_DIRECTORY ${OUTDIR}/dx9)
file(MAKE_DIRECTORY ${OUTDIR}/dx11)
file(MAKE_DIRECTORY ${OUTDIR}/metal)

macro(asset_shader FILE VAR)
	set(ARGS ${ARGN})
	set(BASE_OPTIONS FILE ${FILE} INCLUDES ${BGFX_DIR}/src ${ARGS})
	list(FIND ARGS VERTEX IND)
	if(IND EQUAL -1)
		set(TYPE "ps")
	else()
		set(TYPE "vs")
	endif()

	# glsl
	set(OPTIONS ${BASE_OPTIONS} OUTPUT ${OUTDIR}/glsl/${VAR}.bin LINUX PROFILE 120)
	shaderc_parse(OUT ${OPTIONS})
	list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
	list(APPEND ASSET_FILES ${OUTDIR}/glsl/${VAR}.bin ${VAR}_glsl)

	# gles
	set(OPTIONS ${BASE_OPTIONS} OUTPUT ${OUTDIR}/essl/${VAR}.bin ANDROID PROFILE 120)
	shaderc_parse(OUT ${OPTIONS})
	list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
	list(APPEND ASSET_FILES ${OUTDIR}/essl/${VAR}.bin ${VAR}_essl)

	# dx9
	set(OPTIONS ${BASE_OPTIONS} OUTPUT ${OUTDIR}/dx9/${VAR}.bin WINDOWS PROFILE ${TYPE}_3_0)
	shaderc_parse(OUT ${OPTIONS})
	list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
	list(APPEND ASSET_FILES ${OUTDIR}/dx9/${VAR}.bin ${VAR}_dx9)

	# dx11
	set(OPTIONS ${BASE_OPTIONS} OUTPUT ${OUTDIR}/dx11/${VAR}.bin WINDOWS PROFILE ${TYPE}_4_0)
	shaderc_parse(OUT ${OPTIONS})
	list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
	list(APPEND ASSET_FILES ${OUTDIR}/dx11/${VAR}.bin ${VAR}_dx11)

	# metal
	set(OPTIONS ${BASE_OPTIONS} OUTPUT ${OUTDIR}/metal/${VAR}.bin OSX PROFILE metal)
	shaderc_parse(OUT ${OPTIONS})
	list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
	list(APPEND ASSET_FILES ${OUTDIR}/metal/${VAR}.bin ${VAR}_metal)
endmacro()

macro(asset FILE VAR)
	list(APPEND ASSET_FILES ${FILE} ${VAR})
endmacro()

macro(add_asset_shaders ARG_TARGET)
  if(IGNEOUS_ASSET_GEN)
    cmake_parse_arguments(ARG "" "OUTPUT;HEADER" "SHADERS" ${ARGN})

    if(NOT TARGET ${ARG_TARGET})
      message(FATAL_ERROR "add_shaders: You must provide a valid target.")
    elseif(NOT ARG_OUTPUT)
      message(FATAL_ERROR "add_shaders: You must provide an output directory.")
    elseif(NOT ARG_HEADER)
      message(FATAL_ERROR "add_shaders: You must provide a header path.")
    elseif(NOT ARG_SHADERS)
      message(FATAL_ERROR "add_shaders: You must provide shaders.")
    endif()

    get_filename_component(ARG_OUTPUT ${ARG_OUTPUT} ABSOLUTE)
    get_filename_component(ARG_HEADER ${ARG_HEADER} ABSOLUTE)

    unset(ASSET_COMMANDS)
    unset(ASSET_FILES)

    foreach(SHADER ${ARG_SHADERS})
      get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
      get_filename_component(SHADER ${SHADER} ABSOLUTE)
      if(${SHADER_NAME} MATCHES "^vs_")
        asset_shader(${SHADER} ${SHADER_NAME} VERTEX)
      elseif(${SHADER_NAME} MATCHES "^fs_")
        asset_shader(${SHADER} ${SHADER_NAME} FRAGMENT)
      endif()
    endforeach()

    if(NOT EXISTS ${ARG_HEADER})
      file(WRITE ${ARG_HEADER})
    endif()

    add_custom_target(
      ${ARG_TARGET}_assets ALL
      ${ASSET_COMMANDS}
      COMMAND "$<TARGET_FILE:bin2c>" ${ARG_HEADER} ${ASSET_FILES}
      DEPENDS shaderc
      DEPENDS bin2c
   )
    set_target_properties(${ARG_TARGET}_assets PROPERTIES FOLDER "assets")
    add_dependencies(${ARG_TARGET} ${ARG_TARGET}_assets)
  endif()
endmacro()

add_executable(bin2c
	bin2c.cpp
)

set_target_properties(bin2c PROPERTIES FOLDER "assets")
