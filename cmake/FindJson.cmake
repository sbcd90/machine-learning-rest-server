# Locates the tensorFlow library and include directories.

include(FindPackageHandleStandardArgs)
unset(JSON_FOUND)

find_path(Json_INCLUDE_DIR
	  NAMES
	  nlohmann/
	  HINTS
	  /usr/local/include/nlohmann)

find_package_handle_standard_args(Json DEFAULT_MSG Json_INCLUDE_DIR)

if(JSON_FOUND)
	set(Json_INCLUDE_DIRS ${Json_INCLUDE_DIR})
endif()
