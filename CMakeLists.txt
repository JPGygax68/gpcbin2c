cmake_minimum_required(VERSION 3.0)

#------------------------------------------------
# Project + basics
#

project(gpcbin2c)

add_executable(${PROJECT_NAME} bin2c.cpp)

#------------------------------------------------
# Conan package manager
#
 
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (0)
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

install(FILES
  "${PROJECT_BINARY_DIR}/InstallFiles/${PROJECT_NAME}-config.cmake"
  DESTINATION "cmake" COMPONENT dev)
endif(0)