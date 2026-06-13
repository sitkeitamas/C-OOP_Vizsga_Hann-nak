#ifndef REC_GEP_CONTROLLER_HPP
#define REC_GEP_CONTROLLER_HPP

#include "rec_gep_model.hpp"
#include "rec_gep_view.hpp"

class RecGepController
{
    RecGepModel _model;
    RecGepView _view;

public:
    void attach(Lib* lib);
    void seed_demo_data();

    void on_add_rec();
    void on_insert();
    void on_remove();
};

#endif
