#ifndef APP_HPP
#define APP_HPP

#include "application.hpp"
#include "controller.hpp"

// Bevihető MVC keret – vizsgán: config.hpp + row_text / szabályok finomhangolása.
class App : public Lib
{
    AppController _controller;

public:
    App(int width, int height);
};

#endif
