#include "icecream_view.hpp"

using namespace std;

void IceCreamView::create_widgets(Lib* lib)
{
    titleLabel = new StaticText(lib, 20, 14, 860, 40,
                                "Fagylalka – készlet és kiadás (MVC, A alapfeladat)",
                                {245, 247, 255}, {37, 64, 105});

    new StaticText(lib, 20, 68, 160, 28, "Fagyi választó:",
                   {235, 239, 255}, {52, 58, 86});
    flavorDropdown = new Dropdown(lib, 180, 64, 220, 34);

    new StaticText(lib, 420, 68, 120, 28, "Mennyiség:",
                   {235, 239, 255}, {52, 58, 86});
    qtyInput = new SetValue(lib, 540, 64, 90, 34, 1, 99);

    stockDisplay = new StaticText(lib, 20, 120, 860, 52, "",
                                  {235, 239, 255}, {43, 49, 73});

    statusLabel = new StaticText(lib, 20, 190, 860, 32, "Kész.",
                                 {245, 247, 255}, {48, 57, 91});
}

void IceCreamView::set_status(const string& msg)
{
    if (statusLabel) statusLabel->set_text(msg);
}

void IceCreamView::remember_selection()
{
    if (flavorDropdown) _preserve_selection = flavorDropdown->selected_item();
}

void IceCreamView::refresh_all()
{
    if (!_model || !flavorDropdown || !stockDisplay) return;

    remember_selection();

    auto available = _model->available_names();
    flavorDropdown->set_items(available);

    if (!_preserve_selection.empty())
        flavorDropdown->update_selected(_preserve_selection);
    else if (!available.empty())
        flavorDropdown->set_selected_index(0);
    else
        flavorDropdown->set_selected_index(-1);

    stockDisplay->set_text(_model->stock_display_text());
}

string IceCreamView::selected_flavor() const
{
    return flavorDropdown ? flavorDropdown->selected_item() : "";
}

int IceCreamView::sell_quantity() const
{
    return qtyInput ? qtyInput->value() : 0;
}
