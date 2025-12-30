# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appwe_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appwe_autogen.dir/ParseCache.txt"
  "appwe_autogen"
  )
endif()
