set(lib_name Utilities)
###############################################
set(${lib_name}_srcs
  JsonSerializationHelpers.hpp
  RateLimiter.cpp
  RateLimiter.hpp
  Sha256.cpp
  Sha256.hpp
  SpdlogInit.cpp
  SpdlogInit.hpp
  Utility.cpp
  Utility.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
###############################################
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name}
  PUBLIC Boost::json spdlog::spdlog
  PRIVATE cpr::cpr OpenSSL::SSL
  )
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/UtilitiesLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
xpSourceListAppend(${${lib_name}_srcs})
