#include "sync_view.hpp"
#include <functional>
#include <string>

using namespace std;

void SyncView::create_widgets(Lib* lib)
{
    titleLabel = new StaticText(lib, 20, 14, 760, 40,
                                "F_sync MVC váz (általános kétlista)",
                                {245, 247, 255}, {37, 64, 105});
    new StaticText(lib, 20, 68, 95, 28, "Neve:", {235, 239, 255}, {52, 58, 86});
    nameInput = new Text(lib, 118, 68, 145, 28);
    new StaticText(lib, 20, 100, 95, 28, "Értéke:", {235, 239, 255}, {52, 58, 86});
    valueInput = new SetValue(lib, 118, 100, 100, 28, 0, 9999);

    statusLabel = new StaticText(lib, 20, 510, 245, 32, "Kész.", {245, 247, 255}, {48, 57, 91});
    statsLabel = new StaticText(lib, 20, 546, 760, 28, "", {235, 239, 255}, {52, 58, 86});

    new StaticText(lib, 278, 60, 215, 28, "Raktár", {245, 247, 255}, {37, 64, 105});
    new StaticText(lib, 538, 60, 215, 28, "Kiválasztott", {245, 247, 255}, {37, 64, 105});
    warehouseList = new List(lib, 278, 94, 215, 408);
    pickedList = new List(lib, 538, 94, 215, 408);
}

void SyncView::wire_list_select_handlers(std::function<void()> handler)
{
    if (warehouseList) warehouseList->set_on_select(handler);
    if (pickedList) pickedList->set_on_select(handler);
}

void SyncView::set_status(const string& msg)
{
    if (statusLabel) statusLabel->set_text(msg);
}

string SyncView::row_text(const SyncItem& item) const
{
    return item.name + " | " + to_string(item.value);
}

void SyncView::refresh_lists()
{
    if (!_model || !warehouseList || !pickedList) return;

    if (warehouseList->selected_index() >= 0)
        _saved_left = warehouseList->get_selected_item().name;
    if (pickedList->selected_index() >= 0)
        _saved_right = pickedList->get_selected_item().name;

    warehouseList->clear();
    pickedList->clear();

    for (int i = 0; i < (int)_model->items().size(); i++)
    {
        const SyncItem& it = _model->items()[i];
        string txt = row_text(it);
        if (!it.picked)
            warehouseList->add_item({txt, {i}});
        else
            pickedList->add_item({txt, {i}});
    }

    for (int i = 0; i < warehouseList->item_count(); i++)
    {
        warehouseList->set_selected_index(i);
        if (warehouseList->get_selected_item().name == _saved_left) break;
        warehouseList->clear_selection();
    }
    for (int i = 0; i < pickedList->item_count(); i++)
    {
        pickedList->set_selected_index(i);
        if (pickedList->get_selected_item().name == _saved_right) break;
        pickedList->clear_selection();
    }

    update_stats();
}

void SyncView::update_stats()
{
    if (!statsLabel || !_model) return;
    statsLabel->set_text("Jobb: " + to_string(_model->stats_picked_count()) +
                         " db | Összérték: " + to_string(_model->stats_picked_sum()));
}

void SyncView::refresh_all()
{
    refresh_lists();
}

void SyncView::fill_inputs_from_selection(int item_index)
{
    if (!_model || item_index < 0 || item_index >= (int)_model->items().size()) return;
    const SyncItem& it = _model->items()[item_index];
    nameInput->set_text(it.name);
    valueInput->set_value(it.value);
}

void SyncView::clear_name_input()
{
    if (nameInput) nameInput->clear();
}

string SyncView::name_text() const { return nameInput ? nameInput->text() : ""; }
int SyncView::value_number() const { return valueInput ? valueInput->value() : 0; }

int SyncView::selected_warehouse_item_index() const
{
    if (!warehouseList || warehouseList->selected_index() < 0) return -1;
    ListItem li = warehouseList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int SyncView::selected_picked_item_index() const
{
    if (!pickedList || pickedList->selected_index() < 0) return -1;
    ListItem li = pickedList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int SyncView::active_item_index() const
{
    if (warehouseList && pickedList &&
        warehouseList->selected_index() >= 0 && pickedList->selected_index() >= 0)
        return (warehouseList->activation_order() > pickedList->activation_order())
                   ? selected_warehouse_item_index()
                   : selected_picked_item_index();
    if (warehouseList && warehouseList->selected_index() >= 0)
        return selected_warehouse_item_index();
    if (pickedList && pickedList->selected_index() >= 0)
        return selected_picked_item_index();
    return -1;
}
