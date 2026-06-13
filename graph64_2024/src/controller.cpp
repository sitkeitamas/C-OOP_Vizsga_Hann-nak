#include "controller.hpp"
#include "button.hpp"
#include "config.hpp"

void AppController::attach(Lib* lib)
{
    _view.bind(&_model);
    _view.create_widgets(lib);

    int btnY = UI_INVENTORY ? 168 : 92;
    int moveY = UI_INVENTORY ? 204 : 128;

    _view.addButton = new Button(lib, 20, btnY, 95, 30, "Hozzáad",
                                 [this]() { on_add(); });
    _view.editButton = new Button(lib, 120, btnY, 95, 30, "Módosít",
                                  [this]() { on_modify(); });
    _view.delButton = new Button(lib, 220, btnY, 95, 30, "Töröl",
                                 [this]() { on_delete(); });

    if (UI_TWO_LIST)
    {
        _view.moveUpButton = new Button(lib, 20, moveY, 45, 30, "Fel",
                                        [this]() { on_move_up(); });
        _view.moveDownButton = new Button(lib, 68, moveY, 45, 30, "Le",
                                          [this]() { on_move_down(); });
        _view.moveRightButton = new Button(lib, 640, 300, 28, 36, ">",
                                           [this]() { on_move_right(); });
        _view.moveLeftButton = new Button(lib, 640, 345, 28, 36, "<",
                                          [this]() { on_move_left(); });
    }

    if (UI_ASSIGN)
    {
        _view.assignButton = new Button(lib, 120, moveY, 90, 30, "Apply",
                                        [this]() { on_assign(); });
        _view.releaseButton = new Button(lib, 215, moveY, 90, 30, "Release",
                                         [this]() { on_release(); });
    }

    if (UI_INVENTORY && !UI_MACHINE)
    {
        _view.sellButton = new Button(lib, 520, 94, 80, 30, "Kiad",
                                      [this]() { on_sell(); });
        _view.restockSelectedButton = new Button(lib, 605, 94, 100, 30, "Feltölt",
                                                 [this]() { on_restock_selected(); });
        _view.restockAllButton = new Button(lib, 710, 94, 100, 30, "Mind feltölt",
                                            [this]() { on_restock_all(); });
    }

    if (UI_MACHINE)
    {
        _view.insertButton = new Button(lib, 540, 94, 90, 30, "Berak",
                                        [this]() { on_insert_to_machine(); });
        _view.removeMachineButton = new Button(lib, 635, 94, 90, 30, "Kivesz",
                                               [this]() { on_remove_from_machine(); });
    }

    _view.wire_list_select_handlers([this]() { on_list_select(); });
}

void AppController::seed_demo_data()
{
    _model.seed_demo(DEMO_SEED);
    _view.refresh_all();
}

void AppController::on_list_select()
{
    _view.fill_inputs_from_selection(_view.active_item_index());
}

void AppController::on_category_change()
{
    _view.refresh_all();
}

void AppController::on_add()
{
    if (UI_MACHINE)
    {
        if (!_model.add_rec(_view.name_text(), _view.val0_number()))
        {
            _view.set_status("REC felvétel sikertelen (név üres vagy duplikált).");
            return;
        }
    }
    else if (!_model.add_item(_view.name_text(), _view.val0_number(), _view.val1_number(),
                              _view.tag_text()))
    {
        _view.set_status(_view.name_text().empty() ? "A név nem lehet üres."
                                                   : "Ilyen nevű elem már van.");
        return;
    }
    _view.refresh_all();
    _view.clear_name_input();
    _view.set_status("Hozzáadva.");
}

void AppController::on_modify()
{
    int idx = _view.selected_left_item_index();
    if (idx < 0) idx = _view.active_item_index();
    if (!_model.modify_item(idx, _view.name_text(), _view.val0_number(), _view.val1_number(),
                            _view.tag_text()))
    {
        _view.set_status(idx < 0 ? "Válassz elemet!" : "Módosítás sikertelen.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Módosítva.");
}

void AppController::on_delete()
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

void AppController::on_move_up()
{
    int idx = _view.selected_left_item_index();
    if (!_model.move_item_up(idx))
    {
        _view.set_status(idx < 0 ? "Válassz bal oldali elemet!" : "Nem mozgatható feljebb.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Feljebb.");
}

void AppController::on_move_down()
{
    int idx = _view.selected_left_item_index();
    if (!_model.move_item_down(idx))
    {
        _view.set_status(idx < 0 ? "Válassz bal oldali elemet!" : "Nem mozgatható lejjebb.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Lejjebb.");
}

void AppController::on_move_right()
{
    int idx = _view.selected_left_item_index();
    if (!_model.move_item_to_right(idx))
    {
        _view.set_status("Válassz bal oldali elemet!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Jobbra.");
}

void AppController::on_move_left()
{
    int idx = _view.selected_right_item_index();
    if (!_model.move_item_to_left(idx))
    {
        _view.set_status("Válassz jobb oldali elemet!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Balra.");
}

void AppController::on_assign()
{
    int left = _view.selected_left_item_index();
    int right = _view.selected_right_item_index();
    if (!_model.assign(left, right))
    {
        _view.set_status("Apply: válassz bal ÉS jobb elemet!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Hozzárendelve.");
}

void AppController::on_release()
{
    int idx = _view.selected_right_item_index();
    if (!_model.release(idx))
    {
        _view.set_status("Release: válassz jobb oldali elemet!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Feloldva.");
}

void AppController::on_sell()
{
    int idx = _view.catalog_item_index();
    int qty = _view.sell_quantity();
    if (!_model.sell(idx, qty))
    {
        _view.set_status("Kiadás sikertelen (nincs készlet vagy rossz mennyiség).");
        return;
    }
    _view.refresh_all();
    _view.set_status("Kiadva.");
}

void AppController::on_restock_selected()
{
    int idx = _view.catalog_item_index();
    if (!_model.restock(idx))
    {
        _view.set_status("Feltöltés sikertelen.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Feltöltve.");
}

void AppController::on_restock_all()
{
    if (!_model.restock_all())
    {
        _view.set_status("Nincs mit feltölteni.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Mind feltöltve.");
}

void AppController::on_insert_to_machine()
{
    int idx = _view.catalog_item_index();
    if (!_model.insert_rec_to_machine(idx, _view.insert_count()))
    {
        _view.set_status("Berakás sikertelen.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Berakva a gépbe.");
}

void AppController::on_remove_from_machine()
{
    if (!_model.remove_machine_group(_view.selected_machine_row()))
    {
        _view.set_status("Válassz sort a gép listából!");
        return;
    }
    _view.refresh_all();
    _view.set_status("Kivéve a gépből.");
}
