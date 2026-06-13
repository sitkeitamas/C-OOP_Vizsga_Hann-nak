#ifndef REC_GEP_APP_HPP
#define REC_GEP_APP_HPP

#include "application.hpp"
#include "rec_gep_controller.hpp"

class RecGepApp : public Lib
{
    RecGepController _controller;

public:
    RecGepApp(int width, int height);
};

#endif
