
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
    bool render( std::string _name = "crichton" );
    void write_defaults( std::ofstream &_output );
    void convert_meshes(); // Do this only once
    void write_meshes( std::ofstream &_output );
    void write_current_state( std::ofstream &_output );
    
    bool render_baked_states( std::string _filename );

  private:
    std::shared_ptr<dart::simulation::World> mWorld;

};
