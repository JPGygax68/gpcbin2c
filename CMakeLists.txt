cmake_minimum_required(VERSION 3.0)

#------------------------------------------------
# Project + basics
#

project(GPCBin2C)

add_executable(${PROJECT_NAME} bin2c.cpp)

#------------------------------------------------
# Special
#

# Force static runtime libraries

FOREACH(flag
  CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
  CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
  CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
  CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
  STRING(REPLACE "/MD" "/MT" "${flag}" "${${flag}}")
  SET("${flag}" "${${flag}} /EHsc")
ENDFOREACH()
 
#------------------------------------------------
# Export this build tree for use by other CMake projects.
#

# Create a config file for build tree usage:
set(TARGET_SET "${PROJECT_NAME}")
set(TARGETSET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TARGETSET_LIB_DIR     ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(TARGETSET_CMAKE_DIR   ${CMAKE_CURRENT_BINARY_DIR})
configure_file("project-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}-targets.cmake")

export(PACKAGE ${PROJECT_NAME})

#------------------------------------------------
# INSTALLATION
#

# Note: this requires CMAKE_INSTALL_PREFIX to be defined by the user

# Create a config file for the install tree:
set(TARGETSET_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(TARGETSET_LIB_DIR     ${CMAKE_INSTALL_PREFIX}/lib) # not needed with header-only libraries
set(TARGETSET_CMAKE_DIR   ${CMAKE_INSTALL_PREFIX}/CMake)
configure_file("project-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/InstallFiles/${PROJECT_NAME}-config.cmake"
  @ONLY)
  
install(TARGETS ${PROJECT_NAME} 
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

install(EXPORT ${PROJECT_NAME}-targets DESTINATION "cmake")

