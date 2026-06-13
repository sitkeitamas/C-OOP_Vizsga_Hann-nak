#ifndef SYNC_CONTROLLER_HPP
#define SYNC_CONTROLLER_HPP

#include "sync_model.hpp"
#include "sync_view.hpp"

class SyncController
{
    SyncModel _model;
    SyncView _view;

public:
    SyncView& view() { return _view; }
    SyncModel& model() { return _model; }

    void attach(Lib* lib);
    void seed_demo_data();

    void on_add();
    void on_modify();
    void on_delete();
    void on_move_up();
    void on_move_down();
    void on_move_right();
    void on_move_left();
    void on_list_select();
};

#endif
