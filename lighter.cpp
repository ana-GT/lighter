#include "lighter.h"

#include <dart/simulation/World.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/MeshShape.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

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
  // Camera
  _output << " camera { \n \t location <10,0,0> \n";
  _output << " \t look_at 0 \n";
  _output << " \t angle 45 \n";
  _output << " } \n\n";
  // Light source
  _output << " light_source {<500,500,-1000> White } \n\n";
  // Floor
  _output << " plane { y, -1.5 \n";
  _output << " \t pigment { checker Black White } \n";
  _output << " }\n\n";  

  //
  _output << "#include \"/home/ana/Software/stltools/arm_4_link.inc\" \n";
  _output << "m_Exported \n";
}



void Lighter::add_meshes( std::ofstream &_output ) {
  for( int i = 0; i < mWorld->getNumSkeletons(); ++i ) {
    dart::dynamics::SkeletonPtr skel = mWorld->getSkeleton(i);
    printf("Skel %d has %d body nodes \n", i, skel->getNumBodyNodes() );
    for( int j = 0; j < skel->getNumBodyNodes(); ++j ) {
      dart::dynamics::BodyNodePtr node = skel->getBodyNode(j);
      int nsh = node->getNumVisualizationShapes();
      for( int k = 0; k < nsh; ++k ) {
	dart::dynamics::ShapePtr shape = node->getVisualizationShape(k);
	if( shape->getShapeType() == dart::dynamics::Shape::MESH ) {
	  std::cout << "Path: "<< ((dart::dynamics::MeshShape*)shape.get())->getMeshPath() << std::endl;
	} 
        // Get transformation
	Eigen::Isometry3d Tf = node->getWorldTransform();
	// Get matrix in pov terms
	Eigen::Matrix4d tf = Tf.matrix();
	char mat[200];
	sprintf(mat,"%f,%f,%f, %f,%f,%f, %f,%f,%f, %f,%f,%f",
		tf(0,0), tf(1,0), tf(2,0), tf(0,1), tf(1,1), tf(2,1),
		tf(0,2), tf(1,2), tf(2,2), tf(0,3), tf(1,3),  tf(2,3) );

	// Add sphere
	/*
	_output << "sphere { \n";
	_output << "\t <0,0,0>, 0.1 \n";
	_output << "\t texture { pigment {color Red} } \n";
	_output << "\t matrix <"<<mat<<"> \n";
	_output << "}\n\n";*/
      }
      printf("\t  Bn %d has %d shapes \n", j, nsh);
    }
  }
}
