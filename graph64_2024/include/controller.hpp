#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "model.hpp"
#include "view.hpp"

class AppController
{
    Model _model;
    AppView _view;

public:
    AppView& view() { return _view; }
    Model& model() { return _model; }

    void attach(Lib* lib);
    void seed_demo_data();

    void on_add();
    void on_modify();
    void on_delete();
    void on_move_up();
    void on_move_down();
    void on_move_right();
    void on_move_left();
    void on_assign();
    void on_release();
    void on_sell();
    void on_restock_selected();
    void on_restock_all();
    void on_insert_to_machine();
    void on_remove_from_machine();
    void on_list_select();
    void on_category_change();
};

#endif
