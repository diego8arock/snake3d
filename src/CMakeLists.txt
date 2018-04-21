## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(
  ${OIS_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_BINARY_DIR}/lib
  )
link_directories(${OGRE_LIBRARY_DIRS})
add_executable(snake3d snake3d.cxx)
target_link_libraries(snake3d pujOgre)

## eof - $RCSfile$
