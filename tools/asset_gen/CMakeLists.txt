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

macro(add_asset_shaders ARG_TARGET)
  if(IGNEOUS_ASSET_GEN)
    if(NOT TARGET ${ARG_TARGET})
      message(FATAL_ERROR "add_asset_shaders: You must provide a valid target.")
    endif()

    cmake_parse_arguments(ARG "" "" "HEADER" ${ARGN})

    unset(ASSET_COMMANDS)
    unset(HEADER)

    foreach(SHADER ${ARG_HEADER})
      if(${SHADER} MATCHES "\.h$|\.hpp$")
        if(HEADER)
          list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:bin2c>" ${HEADER} ${ASSET_FILES})
        endif()
        unset(ASSET_FILES)
        set(HEADER ${SHADER})
      elseif(${SHADER} MATCHES "\.sc$")
        if(NOT HEADER)
          message(FATAL_ERROR "add_asset_shaders: You must provide an output header before any shaders.")
        endif()
        get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
        if(${SHADER_NAME} MATCHES "^vs_")
          set(DX_TYPE "vs")
          set(TYPE "VERTEX")
        elseif(${SHADER_NAME} MATCHES "^fs_")
          set(DX_TYPE "ps")
          set(TYPE "FRAGMENT")
        else()
          message(FATAL_ERROR "add_asset_shaders: Unknown shader type: ${SHADER_NAME}")
        endif()
        set(BASE_OPTIONS FILE ${SHADER} INCLUDES ${BGFX_DIR}/src ${TYPE})

        # glsl
        set(OPTIONS ${BASE_OPTIONS} OUTPUT ${ASSET_BINARY_DIR}/glsl/${SHADER_NAME}.bin LINUX PROFILE 120)
        shaderc_parse(OUT ${OPTIONS})
        list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
        list(APPEND ASSET_FILES ${ASSET_BINARY_DIR}/glsl/${SHADER_NAME}.bin ${SHADER_NAME}_glsl)

        # gles
        set(OPTIONS ${BASE_OPTIONS} OUTPUT ${ASSET_BINARY_DIR}/essl/${SHADER_NAME}.bin ANDROID PROFILE 120)
        shaderc_parse(OUT ${OPTIONS})
        list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
        list(APPEND ASSET_FILES ${ASSET_BINARY_DIR}/essl/${SHADER_NAME}.bin ${SHADER_NAME}_essl)

        # dx9
        set(OPTIONS ${BASE_OPTIONS} OUTPUT ${ASSET_BINARY_DIR}/dx9/${SHADER_NAME}.bin WINDOWS PROFILE ${DX_TYPE}_3_0)
        shaderc_parse(OUT ${OPTIONS})
        list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
        list(APPEND ASSET_FILES ${ASSET_BINARY_DIR}/dx9/${SHADER_NAME}.bin ${SHADER_NAME}_dx9)

        # dx11
        set(OPTIONS ${BASE_OPTIONS} OUTPUT ${ASSET_BINARY_DIR}/dx11/${SHADER_NAME}.bin WINDOWS PROFILE ${DX_TYPE}_4_0)
        shaderc_parse(OUT ${OPTIONS})
        list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
        list(APPEND ASSET_FILES ${ASSET_BINARY_DIR}/dx11/${SHADER_NAME}.bin ${SHADER_NAME}_dx11)

        # metal
        set(OPTIONS ${BASE_OPTIONS} OUTPUT ${ASSET_BINARY_DIR}/metal/${SHADER_NAME}.bin OSX PROFILE metal)
        shaderc_parse(OUT ${OPTIONS})
        list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${OUT})
        list(APPEND ASSET_FILES ${ASSET_BINARY_DIR}/metal/${SHADER_NAME}.bin ${SHADER_NAME}_metal)

        target_sources(${ARG_TARGET} PRIVATE ${SHADER})
        source_group("Asset Shader Files" FILES ${SHADER})
      endif()
    endforeach()

    if(HEADER)
      list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:bin2c>" ${HEADER} ${ASSET_FILES})
    else()
      message(FATAL_ERROR "add_asset_shaders: You must provide an output header.")
    endif()
    
    add_custom_target(
      ${ARG_TARGET}_assets ALL
      ${ASSET_COMMANDS}
      DEPENDS shaderc
      DEPENDS bin2c
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set_target_properties(${ARG_TARGET}_assets PROPERTIES FOLDER "assets" )
    add_dependencies(${ARG_TARGET} ${ARG_TARGET}_assets)
  endif()
endmacro()

if(IGNEOUS_ASSET_GEN)
  set(ASSET_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated CACHE INTERNAL "")
  file(MAKE_DIRECTORY ${ASSET_BINARY_DIR})
  file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/glsl)
  file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/essl)
  file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/dx9)
  file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/dx11)
  file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/metal)

  add_executable(bin2c
    bin2c.cpp
    header.h
  )

  set_target_properties(bin2c PROPERTIES FOLDER "igneous/tools")
endif()
