#ifndef ICECREAM_APP_HPP
#define ICECREAM_APP_HPP

#include "application.hpp"
#include "icecream_controller.hpp"

class IceCreamApp : public Lib
{
    IceCreamController _controller;

public:
    IceCreamApp(int width, int height);
};

#endif
