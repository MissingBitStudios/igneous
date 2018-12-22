set(FASTNOISE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fastnoise)
file(GLOB FASTNOISE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/fastnoise/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/fastnoise/*.h
)
add_library(fastnoise STATIC ${FASTNOISE_SOURCES})
target_include_directories(fastnoise PUBLIC
  ${FASTNOISE_INCLUDE_DIR}
)