cmake_minimum_required( VERSION 2.8 )

set( CMAKE_CXX_FLAGS "-std=gnu++0x" )

find_package( DART REQUIRED )
include_directories( ${DART_INCLUDE_DIRS} )
message( STATUS "DART Libraries: ${DART_LIBRARIES} " )

include_directories( /home/ana/Research/Code/GM )
link_directories( /home/ana/Research/Code/GM/lib )
include_directories( /home/ana/Research )

#-- MLPACK on its own
set( MLPACK_INCLUDE_DIRS /usr/local/include /usr/include/armadillo_bits )
set( MLPACK_LIBRARY_DIRS /usr/local/lib )
set( MLPACK_LIBRARIES mlpack ${LIBXML2_LIBRARIES} )

include_directories( ${MLPACK_INCLUDE_DIRS} )
link_directories( ${MLPACK_LIBRARY_DIRS} )

add_library( lighter SHARED lighter.cpp lighter.h )
target_link_libraries( lighter ${DART_LIBRARIES} dart-core  )
set_target_properties( lighter PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

add_executable( render_single_scene render_single_scene.cpp )
target_link_libraries( render_single_scene lighter utils  ${MLPACK_LIBRARIES}  )
set_target_properties( render_single_scene PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

