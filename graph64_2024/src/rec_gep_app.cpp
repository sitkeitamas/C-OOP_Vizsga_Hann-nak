#include "rec_gep_app.hpp"

RecGepApp::RecGepApp(int width, int height) : Lib(width, height)
{
    _controller.attach(this);
    _controller.seed_demo_data();
}
