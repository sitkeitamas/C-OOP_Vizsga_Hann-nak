#include "view.hpp"

using namespace std;

void AppView::create_widgets(Lib* lib)
{
    titleLabel = new StaticText(lib, 20, 14, 860, 36,
                                "MVC widget keret (config.hpp – vizsgán módosítsd)",
                                {245, 247, 255}, {37, 64, 105});

    new StaticText(lib, 20, 58, 70, 26, "Név:", {235, 239, 255}, {52, 58, 86});
    nameInput = new Text(lib, 92, 56, 150, 28);

    new StaticText(lib, 252, 58, 55, 26, "Ért1:", {235, 239, 255}, {52, 58, 86});
    val0Input = new SetValue(lib, 308, 56, 80, 28, 0, 9999);

    new StaticText(lib, 398, 58, 55, 26, "Ért2:", {235, 239, 255}, {52, 58, 86});
    val1Input = new SetValue(lib, 454, 56, 80, 28, 0, 9999);

    if (UI_CATEGORY)
    {
        new StaticText(lib, 548, 58, 70, 26, "Tag:", {235, 239, 255}, {52, 58, 86});
        tagInput = new Text(lib, 618, 56, 120, 28);
    }

    int invY = 96;
    if (UI_INVENTORY)
    {
        new StaticText(lib, 20, invY, 120, 26, "Katalógus:",
                       {235, 239, 255}, {52, 58, 86});
        catalogDropdown = new Dropdown(lib, 140, invY - 2, 200, 30);
        new StaticText(lib, 352, invY, 90, 26, "Mennyiség:",
                       {235, 239, 255}, {52, 58, 86});
        qtyInput = new SetValue(lib, 442, invY - 2, 70, 30, 1, 99);
        stockDisplay = new StaticText(lib, 20, invY + 38, 860, 40, "",
                                      {235, 239, 255}, {43, 49, 73});
        invY += 88;
    }

    if (UI_MACHINE)
    {
        new StaticText(lib, 20, invY, 120, 26, "REC választó:",
                       {235, 239, 255}, {52, 58, 86});
        catalogDropdown = new Dropdown(lib, 140, invY - 2, 220, 30);
        new StaticText(lib, 372, invY, 90, 26, "Darab:",
                       {235, 239, 255}, {52, 58, 86});
        countInput = new SetValue(lib, 462, invY - 2, 70, 30, 1, 20);
        invY += 44;
        new StaticText(lib, 20, invY, 400, 26, "Gép (név  sorszám/méret)",
                       {245, 247, 255}, {37, 64, 105});
        machineList = new List(lib, 20, invY + 28, 400, 280);
        invY += 320;
    }

    if (UI_CATEGORY && categoryDropdown == nullptr)
    {
        new StaticText(lib, 548, 96, 90, 26, "Szűrő:",
                       {235, 239, 255}, {52, 58, 86});
        categoryDropdown = new Dropdown(lib, 618, 94, 120, 30);
    }

    int listY = UI_INVENTORY ? 230 : (UI_MACHINE ? 480 : 140);
    if (UI_TWO_LIST)
    {
        new StaticText(lib, 440, listY - 34, 180, 26, "Bal (katalógus)",
                       {245, 247, 255}, {37, 64, 105});
        new StaticText(lib, 660, listY - 34, 180, 26, "Jobb (kiválasztott)",
                       {245, 247, 255}, {37, 64, 105});
        leftList = new List(lib, 440, listY, 200, 360);
        rightList = new List(lib, 660, listY, 200, 360);
    }

    statusLabel = new StaticText(lib, 20, 620, 400, 28, "Kész.",
                                 {245, 247, 255}, {48, 57, 91});
    statsLabel = new StaticText(lib, 440, 620, 420, 28, "",
                                {235, 239, 255}, {52, 58, 86});
}

void AppView::wire_list_select_handlers(function<void()> handler)
{
    if (leftList) leftList->set_on_select(handler);
    if (rightList) rightList->set_on_select(handler);
    if (machineList) machineList->set_on_select(handler);
}

void AppView::set_status(const string& msg)
{
    if (statusLabel) statusLabel->set_text(msg);
}

string AppView::row_text(const Item& item) const
{
    string s = item.name;
    if ((int)item.vals.size() > VAL_STOCK)
        s += " | " + to_string(item.vals[VAL_STOCK]);
    if (UI_CATEGORY && !item.tag.empty())
        s += " [" + item.tag + "]";
    if (!item.assigned.empty())
        s += " -> " + item.assigned;
    return s;
}

void AppView::refresh_two_lists()
{
    if (!_model || !leftList || !rightList) return;

    if (leftList->selected_index() >= 0)
        _saved_left = leftList->get_selected_item().name;
    if (rightList->selected_index() >= 0)
        _saved_right = rightList->get_selected_item().name;

    leftList->clear();
    rightList->clear();

    for (int i = 0; i < (int)_model->items().size(); i++)
    {
        const Item& it = _model->items()[i];
        if (it.picked)
            rightList->add_item({row_text(it), {i}});
        else
            leftList->add_item({row_text(it), {i}});
    }

    for (int i = 0; i < leftList->item_count(); i++)
    {
        leftList->set_selected_index(i);
        if (leftList->get_selected_item().name == _saved_left) break;
        leftList->clear_selection();
    }
    for (int i = 0; i < rightList->item_count(); i++)
    {
        rightList->set_selected_index(i);
        if (rightList->get_selected_item().name == _saved_right) break;
        rightList->clear_selection();
    }
}

void AppView::refresh_inventory()
{
    if (!_model || !catalogDropdown || UI_MACHINE) return;

    _preserve_catalog = catalogDropdown->selected_item();
    auto names = _model->available_names();
    catalogDropdown->set_items(names);
    if (!_preserve_catalog.empty())
        catalogDropdown->update_selected(_preserve_catalog);
    else if (!names.empty())
        catalogDropdown->set_selected_index(0);
    else
        catalogDropdown->set_selected_index(-1);

    if (stockDisplay)
        stockDisplay->set_text(_model->stock_display_text());
}

void AppView::refresh_machine()
{
    if (!_model || !UI_MACHINE) return;

    if (catalogDropdown)
    {
        _preserve_catalog = catalogDropdown->selected_item();
        vector<string> labels;
        for (const auto& it : _model->items())
            if (!it.picked)
                labels.push_back(it.name + " (" + to_string(it.vals.empty() ? 0 : it.vals[VAL_STOCK]) + ")");
        catalogDropdown->set_items(labels);
        if (!_preserve_catalog.empty())
            catalogDropdown->update_selected(_preserve_catalog);
    }

    if (!machineList) return;

    if (machineList->selected_index() >= 0)
        _saved_machine = machineList->get_selected_item().name;

    machineList->clear();
    auto rows = _model->machine_display_rows();
    for (int i = 0; i < (int)rows.size(); i++)
        machineList->add_item({rows[i].text, {i}});

    for (int i = 0; i < machineList->item_count(); i++)
    {
        machineList->set_selected_index(i);
        if (machineList->get_selected_item().name == _saved_machine) break;
        machineList->clear_selection();
    }
}

void AppView::refresh_categories()
{
    if (!UI_CATEGORY || !categoryDropdown || !_model) return;
    _preserve_category = categoryDropdown->selected_item();
    auto tags = _model->all_tags();
    tags.insert(tags.begin(), "(mind)");
    categoryDropdown->set_items(tags);
    if (!_preserve_category.empty())
        categoryDropdown->update_selected(_preserve_category);
}

void AppView::update_stats()
{
    if (!statsLabel || !_model) return;
    statsLabel->set_text("Jobb: " + to_string(_model->picked_count()) +
                         " db | Össz: " + to_string(_model->picked_sum(VAL_STOCK)));
}

void AppView::refresh_all()
{
    refresh_categories();
    refresh_two_lists();
    refresh_inventory();
    refresh_machine();
    update_stats();
}

void AppView::fill_inputs_from_selection(int item_index)
{
    if (!_model || item_index < 0 || item_index >= (int)_model->items().size()) return;
    const Item& it = _model->items()[item_index];
    nameInput->set_text(it.name);
    val0Input->set_value(it.vals.size() > (size_t)VAL_STOCK ? it.vals[VAL_STOCK] : 0);
    val1Input->set_value(it.vals.size() > (size_t)VAL_SECOND ? it.vals[VAL_SECOND] : 0);
    if (tagInput) tagInput->set_text(it.tag);
}

void AppView::clear_name_input()
{
    if (nameInput) nameInput->clear();
}

string AppView::name_text() const { return nameInput ? nameInput->text() : ""; }
string AppView::tag_text() const { return tagInput ? tagInput->text() : ""; }
int AppView::val0_number() const { return val0Input ? val0Input->value() : 0; }
int AppView::val1_number() const { return val1Input ? val1Input->value() : 0; }
int AppView::sell_quantity() const { return qtyInput ? qtyInput->value() : 0; }
int AppView::insert_count() const { return countInput ? countInput->value() : 1; }

int AppView::selected_left_item_index() const
{
    if (!leftList || leftList->selected_index() < 0) return -1;
    ListItem li = leftList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int AppView::selected_right_item_index() const
{
    if (!rightList || rightList->selected_index() < 0) return -1;
    ListItem li = rightList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int AppView::selected_machine_row() const
{
    if (!machineList || machineList->selected_index() < 0) return -1;
    ListItem li = machineList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int AppView::active_item_index() const
{
    if (leftList && rightList &&
        leftList->selected_index() >= 0 && rightList->selected_index() >= 0)
        return (leftList->activation_order() > rightList->activation_order())
                   ? selected_left_item_index()
                   : selected_right_item_index();
    if (leftList && leftList->selected_index() >= 0)
        return selected_left_item_index();
    if (rightList && rightList->selected_index() >= 0)
        return selected_right_item_index();
    return -1;
}

int AppView::catalog_item_index() const
{
    if (!_model || !catalogDropdown) return -1;
    string sel = catalogDropdown->selected_item();
    if (sel.empty()) return -1;
    for (int i = 0; i < (int)_model->items().size(); i++)
    {
        const Item& it = _model->items()[i];
        if (it.name == sel) return i;
        if (sel.find(it.name) == 0) return i;
    }
    return -1;
}

string AppView::selected_category() const
{
    if (!categoryDropdown) return "";
    string s = categoryDropdown->selected_item();
    return s == "(mind)" ? "" : s;
}
