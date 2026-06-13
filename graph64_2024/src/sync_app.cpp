#include "sync_app.hpp"

SyncApp::SyncApp(int width, int height) : Lib(width, height)
{
    _controller.attach(this);
    _controller.seed_demo_data();
}
