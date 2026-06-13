#include "rec_gep_controller.hpp"
#include "button.hpp"

void RecGepController::attach(Lib* lib)
{
    _view.bind(&_model);
    _view.create_widgets(lib);

    _view.addRecButton = new Button(lib, 320, 104, 120, 28, "REC felvétel",
                                    [this]() { on_add_rec(); });
    _view.insertButton = new Button(lib, 250, 192, 150, 34, "Berakás gépbe",
                                    [this]() { on_insert(); });
    _view.removeButton = new Button(lib, 420, 236, 180, 34, "Sor törlése gépből",
                                    [this]() { on_remove(); });
}

void RecGepController::seed_demo_data()
{
    _model.add_rec("nev1", 4);
    _model.add_rec("nev2", 4);
    _view.refresh_all();
}

void RecGepController::on_add_rec()
{
    if (!_model.add_rec(_view.name_text(), _view.size_value()))
    {
        _view.set_status("REC felvétel sikertelen (név üres vagy méret < 1).");
        return;
    }
    _view.refresh_all();
    _view.set_status("REC elem felvéve.");
}

void RecGepController::on_insert()
{
    int rec_idx = _view.selected_rec_index();
    if (!_model.insert_to_machine(rec_idx, _view.count_value()))
    {
        _view.set_status("Berakás sikertelen (válassz REC elemet és darabszámot).");
        return;
    }
    _view.refresh_all();
    _view.set_status("Berakva a gépbe.");
}

void RecGepController::on_remove()
{
    if (!_model.remove_machine_row(_view.selected_machine_row()))
    {
        _view.set_status("Válassz sort a gép listából!");
        return;
    }
    _view.refresh_all();
    _view.set_status("A kijelölt elem összes sora törölve.");
}
