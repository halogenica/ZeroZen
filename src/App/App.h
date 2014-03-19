
#pragma once

#include "Common.h"

namespace gen
{
    
class App : public ci::app::AppNative
{
        
public:
	void prepareSettings(Settings *settings);
    void setup();
    void shutdown();
    void update();
    void draw();
    void resize();

    void ToggleFullscreen();

protected:
    void InitGamepad();
    void InitBullet();              // initialize bullet physics
    void UpdateTimestep(float dt);  // updates the simulation
    void DestroyBullet();           // free memory used by bullet physics
};

}