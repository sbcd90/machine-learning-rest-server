# Locates the tensorFlow library and include directories.

include(FindPackageHandleStandardArgs)
unset(PROXYGEN_FOUND)

find_path(Proxygen_INCLUDE_DIR
	  NAMES
	  proxygen/external
	  proxygen/httpclient
	  proxygen/httpserver
	  proxygen/lib
	  HINTS
	  /usr/local/include/proxgen)

find_library(Proxygen_LIBRARY NAMES proxygenhttpserver
	     HINTS
	     /usr/lib
	     /usr/local/lib)

find_package_handle_standard_args(Proxygen DEFAULT_MSG Proxygen_INCLUDE_DIR Proxygen_LIBRARY)

if(PROXYGEN_FOUND)
	set(Proxygen_LIBRARIES ${Proxygen_LIBRARY})
	set(Proxygen_INCLUDE_DIRS ${Proxygen_INCLUDE_DIR})
endif()

mark_as_advanced(Proxygen_INCLUDE_DIR Proxygen_LIBRARY)
