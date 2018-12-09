set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)
file(GLOB STB_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/stb/*.c
)
add_library(stb STATIC ${STB_SOURCES})
target_include_directories(stb PUBLIC
  ${STB_INCLUDE_DIR}
)