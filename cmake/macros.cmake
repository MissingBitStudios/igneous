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

macro(copy_directory ARG_TARGET ARG_ORIGIN_DIR ARG_DESTINATION_DIR)
  add_custom_command(TARGET ${ARG_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARG_ORIGIN_DIR} ${ARG_DESTINATION_DIR}
  )
endmacro()
