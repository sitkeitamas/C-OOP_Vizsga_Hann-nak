#ifndef SYNC_APP_HPP
#define SYNC_APP_HPP

#include "application.hpp"
#include "sync_controller.hpp"

// Általános F_sync MVC alkalmazás – referencia váz.
class SyncApp : public Lib
{
    SyncController _controller;

public:
    SyncApp(int width, int height);
};

#endif
