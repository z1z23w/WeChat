# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/WeChatServer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/WeChatServer_autogen.dir/ParseCache.txt"
  "WeChatServer_autogen"
  )
endif()
