#include "sync_controller.hpp"
#include "button.hpp"

void SyncController::attach(Lib* lib)
{
    _view.bind(&_model);
    _view.create_widgets(lib);

    _view.addButton = new Button(lib, 20, 138, 110, 34, "Hozzáad",
                                 [this]() { on_add(); });
    _view.editButton = new Button(lib, 135, 138, 110, 34, "Módosít",
                                  [this]() { on_modify(); });
    _view.delButton = new Button(lib, 20, 178, 110, 34, "Töröl",
                                 [this]() { on_delete(); });
    _view.moveUpButton = new Button(lib, 135, 178, 53, 34, "Fel",
                                    [this]() { on_move_up(); });
    _view.moveDownButton = new Button(lib, 192, 178, 53, 34, "Le",
                                      [this]() { on_move_down(); });
    _view.moveRightButton = new Button(lib, 500, 240, 30, 42, ">",
                                       [this]() { on_move_right(); });
    _view.moveLeftButton = new Button(lib, 500, 295, 30, 42, "<",
                                      [this]() { on_move_left(); });

    _view.wire_list_select_handlers([this]() { on_list_select(); });
}

void SyncController::seed_demo_data()
{
    _model.add_item("Alma", 150);
    _model.add_item("Körte", 200);
    _model.add_item("Banán", 80);
    _view.refresh_all();
}

void SyncController::on_list_select()
{
    _view.fill_inputs_from_selection(_view.active_item_index());
}

void SyncController::on_add()
{
    if (!_model.add_item(_view.name_text(), _view.value_number()))
    {
        _view.set_status(_view.name_text().empty() ? "A név nem lehet üres."
                                                   : "Ilyen nevű elem már van.");
        return;
    }
    _view.refresh_all();
    _view.clear_name_input();
    _view.set_status("Elem hozzáadva.");
}

void SyncController::on_modify()
{
    int idx = _view.selected_warehouse_item_index();
    if (!_model.modify_item(idx, _view.name_text(), _view.value_number()))
    {
        _view.set_status(idx < 0 ? "Válassz elemet a bal listából!"
                                 : "Módosítás sikertelen.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Módosítva.");
}

void SyncController::on_delete()
{
    int idx = _view.active_item_index();
    if (!_model.remove_item(idx))
    {
        _view.set_status("Nincs kijelölt elem.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Törölve.");
}

void SyncController::on_move_up()
{
    int idx = _view.selected_warehouse_item_index();
    if (!_model.move_item_up(idx))
    {
        _view.set_status(idx < 0 ? "Válassz elemet a bal listából!"
                                 : "Már a lista tetején van.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Feljebb mozgatva.");
}

void SyncController::on_move_down()
{
    int idx = _view.selected_warehouse_item_index();
    if (!_model.move_item_down(idx))
    {
        _view.set_status(idx < 0 ? "Válassz elemet a bal listából!"
                                 : "Már a lista alján van.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Lejjebb mozgatva.");
}

void SyncController::on_move_right()
{
    int idx = _view.selected_warehouse_item_index();
    if (!_model.move_item_to_right(idx))
    {
        _view.set_status("Válassz elemet a bal listából!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Áthelyezve jobbra.");
}

void SyncController::on_move_left()
{
    int idx = _view.selected_picked_item_index();
    if (!_model.move_item_to_left(idx))
    {
        _view.set_status("Válassz elemet a jobb listából!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Visszahelyezve balra.");
}
