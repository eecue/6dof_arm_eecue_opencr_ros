# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_eecue_6dof_arm_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED eecue_6dof_arm_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(eecue_6dof_arm_FOUND FALSE)
  elseif(NOT eecue_6dof_arm_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(eecue_6dof_arm_FOUND FALSE)
  endif()
  return()
endif()
set(_eecue_6dof_arm_CONFIG_INCLUDED TRUE)

# output package information
if(NOT eecue_6dof_arm_FIND_QUIETLY)
  message(STATUS "Found eecue_6dof_arm: 0.0.0 (${eecue_6dof_arm_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'eecue_6dof_arm' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${eecue_6dof_arm_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(eecue_6dof_arm_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${eecue_6dof_arm_DIR}/${_extra}")
endforeach()
