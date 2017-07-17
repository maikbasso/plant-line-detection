# Install script for directory: /home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam.so.0.1.6"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam.so.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/build/src/libraspicam.so.0.1.6"
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/build/src/libraspicam.so.0.1"
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/build/src/libraspicam.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam.so.0.1.6"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam.so.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/build/src/libraspicam_cv.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so"
         OLD_RPATH "/opt/ros/kinetic/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libraspicam_cv.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/raspicam" TYPE FILE FILES
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/src/raspicamtypes.h"
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/src/raspicam.h"
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/src/raspicam_cv.h"
    "/home/maik/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE - UFRGS/Artigos/PlantLineDetection/plant-line-detection/C++/libs/RaspiCam/raspicam-0.1.6/src/raspicam_still_cv.h"
    )
endif()

