set(ASSET_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated CACHE INTERNAL "")
file(MAKE_DIRECTORY ${ASSET_BINARY_DIR})
file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/glsl)
file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/essl)
file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/dx9)
file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/dx11)
file(MAKE_DIRECTORY ${ASSET_BINARY_DIR}/metal)

macro(add_asset_shaders ARG_TARGET)
  if(IGNEOUS_ASSET_GEN)
    if(NOT TARGET ${ARG_TARGET})
      message(FATAL_ERROR "add_asset_shaders: You must provide a valid target.")
    endif()

    unset(ASSET_COMMANDS)

    #while(condition)
      cmake_parse_arguments(ARG "" "HEADER" "SHADERS" ${ARGN})

      if(NOT ARG_HEADER)
        message(FATAL_ERROR "add_asset_shaders: You must provide a header path.")
      elseif(NOT ARG_SHADERS)
        message(FATAL_ERROR "add_asset_shaders: You must provide shaders.")
      endif()

      unset(ASSET_FILES)

      foreach(SHADER ${ARG_SHADERS})
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
      endforeach()

      if(NOT EXISTS ${ARG_HEADER})
        file(WRITE ${ARG_HEADER})
      endif()

      list(APPEND ASSET_COMMANDS COMMAND "$<TARGET_FILE:bin2c>" ${ARG_HEADER} ${ASSET_FILES})
    #endwhile()
    
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
  add_executable(bin2c
    bin2c.cpp
    strings.h
  )

  set_target_properties(bin2c PROPERTIES FOLDER "assets")
endif()
