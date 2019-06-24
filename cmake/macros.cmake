macro(add_shaders ARG_TARGET)
  cmake_parse_arguments(ARG "" "OUTPUT" "SHADERS" ${ARGN})

  if(NOT TARGET ${ARG_TARGET})
    message(FATAL_ERROR "add_shaders: You must provide a valid target.")
  elseif(NOT ARG_OUTPUT)
    message(FATAL_ERROR "add_shaders: You must provide a valid output directory.")
  elseif(NOT ARG_SHADERS)
    message(FATAL_ERROR "add_shaders: You must provide shaders.")
  endif()

  foreach(SHADER ${ARG_SHADERS})
    if(${SHADER} MATCHES "/vs_[^/]+\\.sc$")
      add_shader(${SHADER} VERTEX OUTPUT ${ARG_OUTPUT} DX11_MODEL 5_0 GLSL 130)
    elseif(${SHADER} MATCHES "/fs_[^/]+\\.sc$")
      add_shader(${SHADER} FRAGMENT OUTPUT ${ARG_OUTPUT} DX11_MODEL 5_0 GLSL 130)
    endif()
  endforeach()

  target_sources(${ARG_TARGET} PRIVATE ${ARG_SHADERS})
endmacro()

macro(add_shader_directory ARG_TARGET ARG_INPUT_DIR ARG_OUTPUT_DIR)
  file(GLOB_RECURSE SHADERS ${ARG_INPUT_DIR})
  add_shaders(${ARG_TARGET} OUTPUT ${ARG_OUTPUT_DIR} SHADERS ${SHADERS})
endmacro()

macro(add_asset_shaders ARG_TARGET)
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

  unset(ASSET_COMMANDS)
  unset(ASSET_FILES)
  set(OUTDIR ${ARG_OUTPUT})
  file(MAKE_DIRECTORY ${OUTDIR})
  file(MAKE_DIRECTORY ${OUTDIR}/glsl)
  file(MAKE_DIRECTORY ${OUTDIR}/essl)
  file(MAKE_DIRECTORY ${OUTDIR}/dx9)
  file(MAKE_DIRECTORY ${OUTDIR}/dx11)
  file(MAKE_DIRECTORY ${OUTDIR}/metal)

  foreach(SHADER ${ARG_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    get_filename_component(SHADER ${SHADER} ABSOLUTE)
    if(${SHADER_NAME} MATCHES "^vs_")
      asset_shader(${SHADER} ${SHADER_NAME} VERTEX)
    elseif(${SHADER_NAME} MATCHES "^fs_")
      asset_shader(${SHADER} ${SHADER_NAME} FRAGMENT)
    endif()
  endforeach()

  file(WRITE ${ARG_HEADER})

  add_custom_command(TARGET ${ARG_TARGET}
    PRE_BUILD
    ${ASSET_COMMANDS}
    COMMAND "$<TARGET_FILE:bin2c>" ${ARG_HEADER} ${ASSET_FILES}
    DEPENDS
    shaderc
    DEPENDS
    bin2c
  )
endmacro()

macro(copy_directory ARG_TARGET ARG_ORIGIN_DIR ARG_DESTINATION_DIR)
  add_custom_command(TARGET ${ARG_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_ORIGIN_DIR} ${ARG_DESTINATION_DIR}
  )
endmacro()
