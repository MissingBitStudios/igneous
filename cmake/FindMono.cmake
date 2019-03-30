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

if(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/mono-2.0-sgen.dll)
	message("Copying mono files...")
	file(COPY ${MONO_BINARY_DIR}/mono-2.0-sgen.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)
	file(COPY ${MONO_LIBRARY_DIR} DESTINATION ${CMAKE_BINARY_DIR}/bin/mono)
	file(COPY ${MONO_LIBRARY_DIR}/../etc DESTINATION ${CMAKE_BINARY_DIR}/bin/mono)
	message("Done copying mono files!")
endif(NOT EXISTS ${CMAKE_BINARY_DIR}/mono-2.0-sgen.dll)

mark_as_advanced(MONO_LIBRARY MONO_INCLUDE_DIR)