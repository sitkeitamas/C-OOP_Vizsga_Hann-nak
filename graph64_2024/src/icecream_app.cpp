#include "icecream_app.hpp"

IceCreamApp::IceCreamApp(int width, int height) : Lib(width, height)
{
    _controller.attach(this);
    _controller.seed_demo_data();
}
