#include "icecream_controller.hpp"
#include "button.hpp"

void IceCreamController::attach(Lib* lib)
{
    _view.bind(&_model);
    _view.create_widgets(lib);

    _view.sellButton = new Button(lib, 650, 64, 100, 34, "Kiad",
                                  [this]() { on_sell(); });
    _view.restockAllButton = new Button(lib, 20, 240, 180, 34, "Mind feltölt",
                                       [this]() { on_restock_all(); });
    _view.restockSelectedButton = new Button(lib, 210, 240, 200, 34, "Kiválasztott feltölt",
                                             [this]() { on_restock_selected(); });
}

void IceCreamController::seed_demo_data()
{
    _model.seed_defaults();
    _view.refresh_all();
}

void IceCreamController::on_sell()
{
    std::string name = _view.selected_flavor();
    int qty = _view.sell_quantity();

    if (!_model.can_sell(name, qty))
    {
        // Túl nagy érték / nincs kiválasztva → semmi nem történik (A feladat).
        return;
    }

    _view.remember_selection();
    _model.sell(name, qty);
    _view.refresh_all();
    _view.set_status("Kiadva: " + name + " × " + std::to_string(qty));
}

void IceCreamController::on_restock_all()
{
    if (!_model.restock_all())
    {
        _view.set_status("Nincs fagyi a készleten.");
        return;
    }
    _view.refresh_all();
    _view.set_status("Minden fagyi feltöltve (+10 db).");
}

void IceCreamController::on_restock_selected()
{
    std::string name = _view.selected_flavor();
    if (!_model.restock_selected(name))
    {
        _view.set_status("Válassz fagyit a feltöltéshez.");
        return;
    }
    _view.refresh_all();
    _view.set_status(name + " feltöltve (+10 db).");
}
