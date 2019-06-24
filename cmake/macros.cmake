macro(add_shaders ARG_TARGET)
  cmake_parse_arguments(ARG "" "OUTPUT" "SHADERS" ${ARGN})

  if(NOT TARGET ${ARG_TARGET} OR NOT IS_DIRECTORY ${ARG_OUTPUT} OR NOT ARG_SHADERS)
    message(FATAL_ERROR "add_shaders: You must provide a valid target, output directory, and shaders.")
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

macro(copy_directory ARG_TARGET ARG_ORIGIN_DIR ARG_DESTINATION_DIR)
  add_custom_command(TARGET ${ARG_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_ORIGIN_DIR} ${ARG_DESTINATION_DIR}
  )
endmacro()
