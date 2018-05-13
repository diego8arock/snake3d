set(snake3dlib_srcs 
        pujOgre/Application.cxx
        pujOgre/Level.cxx
        pujOgre/Obstacle.cxx
        pujOgre/Player.cxx
        pujOgre/Snake.cxx
)
        
include_directories(
  ${OIS_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
link_directories(
  ${OGRE_LIBRARY_DIRS}
)
add_library(pujOgre SHARED ${snake3dlib_srcs})
target_link_libraries(pujOgre ${OGRE_LIBRARIES} ${OIS_LIBRARIES})
