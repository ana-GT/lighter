
#pragma once

// Creates a pov rendered image from a DART world
#include <string>
#include <memory>

namespace dart { 
 namespace simulation {
   class World;
 }
} 

class Lighter {

  public: 
    Lighter();
    void addWorld( std::shared_ptr<dart::simulation::World> _world );
    bool render( std::string _name = "crichton.png" );
    void add_defaults( std::ofstream &_output );
    void add_meshes( std::ofstream &_output );
  private:
    std::shared_ptr<dart::simulation::World> mWorld;

};
