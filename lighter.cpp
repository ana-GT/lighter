#include "lighter.h"

#include <dart/simulation/World.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/MeshShape.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h> // for exec

Lighter::Lighter() :
 mWorld( new dart::simulation::World() ) {

}


void Lighter::addWorld( std::shared_ptr<dart::simulation::World> _world ) {
  mWorld = _world;
  int numSkels = mWorld->getNumSkeletons();
  printf("World loaded has %d skeletons \n", numSkels );

}

bool Lighter::render( std::string _filename ) {

  // Create temporal file to render
  std::ofstream output("temporal_crichton.pov");
  // Add defaults
  add_defaults( output );
  // Get meshes
  add_meshes( output );
  
  output.close();
  return true;
}

void Lighter::add_defaults( std::ofstream &_output ) {

  // Add includes
  _output << " #include \"colors.inc\" \n";
  _output << " #include \"textures.inc\" \n";
  // Camera
  // -- Right handed: http://www.f-lohmueller.de/pov_tut/a_geo/a_geo85e.htm
  _output << " camera { \n ";
  _output << " \t angle 90 \n";
  _output << " \t right -x*4/3 \n";
  _output << " \t sky <0,0,1> \n";
  _output << " \t location <1.5,0,1.5> \n";
  _output << " \t look_at <0,0,1> \n";
  _output << " \t aperture 0.025 \n";
  _output << " \t focal_point <0,0,1.0> \n";
  _output << " \t blur_samples 40 \n";
  _output << " } \n\n";
  // Background
  _output << " background { Pink } \n";
  // Light source
  _output << " light_source {<0,0,3> White } \n\n";
  // Floor
  _output << " plane { z, -0.01 \n";
  _output << " \t texture { PinkAlabaster } \n";
  _output << " }\n\n";
  // Wall  
  _output << " plane { x, -1.5 \n";
  _output << " \t  pigment { rgb<0.5 0.5 0.1> } \n";
  _output << " }\n\n";

}



void Lighter::add_meshes( std::ofstream &_output ) {

  std::map<std::string, std::string> meshes_pov;
  std::map<std::string, std::string>::iterator it;
  dart::dynamics::SkeletonPtr skel;
  dart::dynamics::BodyNodePtr node;
  dart::dynamics::ShapePtr shape;
  int nsh;

  // 1. Get meshes all stored in a map

  for( int i = 0; i < mWorld->getNumSkeletons(); ++i ) {

    skel = mWorld->getSkeleton(i);
    for( int j = 0; j < skel->getNumBodyNodes(); ++j ) {
      node = skel->getBodyNode(j);
      nsh = node->getNumVisualizationShapes();      
      for( int k = 0; k < nsh; ++k ) {
	shape = node->getVisualizationShape(k);
	if( shape->getShapeType() == dart::dynamics::Shape::MESH ) {
	  std::string path = ((dart::dynamics::MeshShape*)shape.get())->getMeshPath();
	  size_t ind = path.rfind("/", path.length() );
	  std::string mesh_name = path.substr( ind+1, path.length() - ind );
	  ind = mesh_name.rfind(".", mesh_name.length() );
	  mesh_name = mesh_name.substr( 0, ind );

	  // Store if it is not there already
	  it = meshes_pov.find(mesh_name);
	  if( it == meshes_pov.end() ) {
	    char inc_name[50]; sprintf( inc_name, "%s.inc", mesh_name.c_str() );
	    meshes_pov[mesh_name] = inc_name;
	    std::cout << "Mesh name: "<< mesh_name << std::endl;
	    // We call blender
	    char cmd[400];
	    sprintf( cmd, "blender --background --python /home/ana/Research/lighter/dae_to_inc.py -- %s \n",
		     path.c_str() );
	    FILE* fp = NULL; fp = popen(cmd, "r");
	    if( fp == NULL ) { printf("Error with handler \n"); }
	    printf("Called popen...\n");
	    _output << " #include \""<<inc_name<<"\" \n";
	    int status;
	    usleep(2*1e6);
	    status = pclose(fp);
	    if( status == -1 ) { printf("Status was bad \n"); }

	    // Write .inc name 
	  } // end if

	} // if shape is mesh
      } // for all shapes
    } // for body nodes
  } // for skeletons

  //*****************************************************
  // Draw each skeleton 
  
  for( int i = 0; i < mWorld->getNumSkeletons(); ++i ) {
    
    skel = mWorld->getSkeleton(i);  
    for( int j = 0; j < skel->getNumBodyNodes(); ++j ) {
      node = skel->getBodyNode(j);
      nsh = node->getNumVisualizationShapes();
      
      for( int k = 0; k < nsh; ++k ) {
	
	shape = node->getVisualizationShape(k);
	if( shape->getShapeType() == dart::dynamics::Shape::MESH ) {
	  std::string path = ((dart::dynamics::MeshShape*)shape.get())->getMeshPath();
	  size_t ind = path.rfind("/", path.length() );
	  std::string mesh_name = path.substr( ind+1, path.length() - ind );
	  ind = mesh_name.rfind(".", mesh_name.length() );
	  mesh_name = mesh_name.substr( 0, ind );
	  std::cout << "Mesh name: "<< mesh_name << std::endl;
	  
	  // Get transformation
	  Eigen::Isometry3d Tf = node->getWorldTransform();
	  // Get matrix in pov terms
	  Eigen::Matrix4d tf = Tf.matrix();
	  char mat[200];
	  sprintf(mat," %f,%f,%f, %f,%f,%f, %f,%f,%f, %f,%f,%f",
		  tf(0,0), tf(1,0), tf(2,0), tf(0,1), tf(1,1), tf(2,1),
		  tf(0,2), tf(1,2), tf(2,2), tf(0,3), tf(1,3),  tf(2,3) );

	  // Add mesh
	  _output << "object { " << mesh_name << "\n matrix <"<< mat << "> } \n";
	  
	} // if MESH

      } // for shapes
    } // for num nodes
  } // for skeletons
}
