//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Common.h"
#include "App.h"
#include "Input.h"

// TODO: new cannot be overloaded here, because it is overloaded by bullet (lame)
//#define new new(_NORMAL_BLOCK,__FILE__,__LINE

CINDER_APP_NATIVE(gen::App, ci::app::RendererGl);    // registers main() function
