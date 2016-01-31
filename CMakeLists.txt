cmake_minimum_required( VERSION 2.8 )

set( CMAKE_CXX_FLAGS "-std=gnu++0x" )

find_package( DART REQUIRED )
include_directories( ${DART_INCLUDE_DIRS} )


add_library( lighter SHARED lighter.cpp lighter.h )
target_link_libraries( lighter ${DART_LIBRARIES} )
set_target_properties( lighter PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )

add_executable( render_single_scene render_single_scene.cpp )
target_link_libraries( render_single_scene lighter )
set_target_properties( render_single_scene PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

