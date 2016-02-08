#include "lighter.h"
#include <dart/utils/urdf/DartLoader.h>
#include <aoi/utils/robotData.h>
#include <golems/global/robot_global.h>

int main( int argc, char* argv[] ) {

  // Load a world
  dart::utils::DartLoader mDl;
  std::shared_ptr<dart::simulation::World> mW( new dart::simulation::World() );
  mW = mDl.parseWorld("/home/ana/Research/commonData/scenes/crichton/crichton_heuristic_v2.urdf");
 // mW = mDl.parseWorld("/home/ana/Research/commonData/scenes/baxter/baxter_world_fullpaths.urdf");
  
  // Init simulation global variables
  setHardCodedStructures();
  
  // Set a robot subject
  subject_t mSubject;
  setSubject( mSubject,
              mW.get(),
              LWA4_ROBOT ); //BAXTER_ROBOT//LWA4_ROBOT

  Eigen::VectorXd la(7); la << -45, -45, 0, -60, 0, 75, 0;
  Eigen::VectorXd ra(7); ra << 45, 45, 0, 60, 0, -75, 0;

  la = la * 3.1416/180.0;
  ra = ra * 3.1416/180.0;


  mSubject.limb[0].arm->setPositions( mSubject.limb[0].armDofs, la );
  mSubject.limb[1].arm->setPositions( mSubject.limb[1].armDofs, ra );

  mSubject.update();  
  
  // Create lighter object
  Lighter mLighter;
  mLighter.addWorld(mW);
  mLighter.render("an2");
  return 0;
}
