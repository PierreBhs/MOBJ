# Install script for directory: /home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "../install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tme810" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tme810")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tme810"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/build/tme810")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tme810" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tme810")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tme810")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Indentation.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Term.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Net.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Instance.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Point.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Node.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Cell.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/XmlUtil.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Box.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Symbol.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Line.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Shape.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Indentation.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Term.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Net.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Instance.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Point.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Node.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Cell.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/XmlUtil.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Box.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Symbol.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Line.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/Shape.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/SaveCellDialog.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/CellWidget.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/CellViewer.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/OpenCellDialog.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/InstancesModel.h"
    "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/src/InstancesWidget.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/pierre/Documents/Master/M1/MOBJ/MOBJ/TME8/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
