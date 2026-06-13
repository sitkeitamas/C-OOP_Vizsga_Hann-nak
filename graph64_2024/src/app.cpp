#include "app.hpp"

App::App(int width, int height) : Lib(width, height)
{
    _controller.attach(this);
    _controller.seed_demo_data();
}
