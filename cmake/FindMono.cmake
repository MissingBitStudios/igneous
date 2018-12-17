set(MONO_SEARCH_PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw
  /opt/local
  /opt/csw
  /opt
  /usr/bin
  /usr/local/bin
  C:\\Program Files\ (x86)
)

find_path(MONO_INCLUDE_DIR mono/jit/jit.h
  HINTS
    ENV MONO_ROOT
  PATH_SUFFIXES include/mono-2.0
  PATHS
  ${MONO_SEARCH_PATHS}
)

find_path(MONO_LIBRARY_DIR mono-2.0-sgen.lib
  HINTS
    ENV MONO_ROOT
  PATH_SUFFIXES lib
  PATHS
  ${MONO_SEARCH_PATHS}
)

find_library(MONO_LIBRARY mono-2.0-sgen
  HINTS
    ENV MONO_ROOT
  PATH_SUFFIXES lib
  PATHS
  ${MONO_SEARCH_PATHS}
)

find_path(MONO_BINARY_DIR mono-2.0-sgen.dll
  HINTS
    ENV MONO_ROOT
  PATH_SUFFIXES bin
  PATHS
  ${MONO_SEARCH_PATHS}
)

file(COPY ${MONO_BINARY_DIR}/mono-2.0-sgen.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${MONO_LIBRARY_DIR}/mono/4.7-api DESTINATION ${CMAKE_BINARY_DIR}/4.7-api)

mark_as_advanced(MONO_LIBRARY MONO_INCLUDE_DIR)