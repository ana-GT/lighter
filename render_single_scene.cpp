#include "lighter.h"
#include <dart/utils/urdf/DartLoader.h>

int main( int argc, char* argv[] ) {

  // Load a world
  dart::utils::DartLoader mDl;
  std::shared_ptr<dart::simulation::World> mW( new dart::simulation::World() );
  mW = mDl.parseWorld("/home/ana/Research/commonData/scenes/crichton/crichton_world_fullpaths.urdf");

  // Create lighter object
  Lighter mLighter;
  mLighter.addWorld(mW);
  mLighter.render();
  return 0;
}
