#ifndef ICECREAM_CONTROLLER_HPP
#define ICECREAM_CONTROLLER_HPP

#include "icecream_model.hpp"
#include "icecream_view.hpp"

class IceCreamController
{
    IceCreamModel _model;
    IceCreamView _view;

public:
    void attach(Lib* lib);
    void seed_demo_data();

    void on_sell();
    void on_restock_all();
    void on_restock_selected();
};

#endif
