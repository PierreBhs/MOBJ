# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/4-5>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

 cmake_minimum_required(VERSION 2.8.0)
 project(SCHEMATIC)
 
 set (CMAKE_CXX_FLAGS            "-std=c++11 -Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
 set (CMAKE_INSTALL_PREFIX       "../install" )

 # Detection de LibXml2.
find_package(LibXml2)
find_package(Qt4     REQUIRED)
include(${QT_USE_FILE})



    include_directories( ${SCHEMATIC_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR}
                       )

                    set( includes     Indentation.h
                                      Term.h
                                      Net.h
                                      Instance.h
                                      Point.h
                                      Node.h
                                      Cell.h
                                      XmlUtil.h
                                      Box.h
                                      Symbol.h
                                      Line.h
                                      Shape.h
                       )
                     set( mocIncludes  SaveCellDialog.h
                                      CellWidget.h
                                      CellViewer.h
                                      OpenCellDialog.h
                                      InstancesModel.h
                                      InstancesWidget.h
                       )

                    set( cpps         Indentation.cpp
                                      Term.cpp
                                      Net.cpp
                                      Instance.cpp
                                      Point.cpp
                                      Node.cpp
                                      Cell.cpp
                                      XmlUtil.cpp
                                      Main.cpp
                                      Box.cpp
                                      Symbol.cpp
                                      Line.cpp
                                      Shape.cpp
                                      ShapeXML.cpp
                                      SaveCellDialog.cpp
                                      CellWidget.cpp
                                      CellViewer.cpp
                                      OpenCellDialog.cpp
                                      InstancesModel.cpp
                                      InstancesWidget.cpp
                       )

qt4_wrap_cpp( mocCpps  ${mocIncludes} )

        add_executable (tme810  ${cpps} ${mocCpps} )
        target_link_libraries ( tme810  ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} )
               
               install ( TARGETS      tme810       DESTINATION bin )
               install ( FILES        ${includes}  DESTINATION include )
               install( FILES  ${includes} ${mocIncludes}  DESTINATION include )