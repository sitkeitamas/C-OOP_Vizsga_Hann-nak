#include "rec_gep_view.hpp"

using namespace std;

void RecGepView::create_widgets(Lib* lib)
{
    new StaticText(lib, 20, 14, 860, 40,
                   "REC / gép listakezelés (MVC minta)",
                   {245, 247, 255}, {37, 64, 105});

    new StaticText(lib, 20, 68, 120, 28, "Név:", {235, 239, 255}, {52, 58, 86});
    nameInput = new Text(lib, 140, 68, 160, 28);
    new StaticText(lib, 20, 104, 120, 28, "Méret:", {235, 239, 255}, {52, 58, 86});
    sizeInput = new SetValue(lib, 140, 104, 90, 28, 1, 16);
    addRecButton = nullptr;

    new StaticText(lib, 20, 148, 120, 28, "REC választó:", {235, 239, 255}, {52, 58, 86});
    recDropdown = new Dropdown(lib, 140, 148, 260, 34);

    new StaticText(lib, 20, 192, 120, 28, "Darabszám:", {235, 239, 255}, {52, 58, 86});
    countInput = new SetValue(lib, 140, 192, 90, 28, 1, 20);

    new StaticText(lib, 20, 248, 380, 28, "Gép (sorok: név  sorszám/méret)",
                   {245, 247, 255}, {37, 64, 105});
    machineList = new List(lib, 20, 280, 380, 360);

    statusLabel = new StaticText(lib, 20, 650, 860, 28, "Kész.",
                                {245, 247, 255}, {48, 57, 91});
}

void RecGepView::set_status(const string& msg)
{
    if (statusLabel) statusLabel->set_text(msg);
}

void RecGepView::refresh_all()
{
    if (!_model || !recDropdown || !machineList) return;

    string prev_rec = recDropdown->selected_item();
    auto labels = _model->rec_labels();
    recDropdown->set_items(labels);
    if (!prev_rec.empty())
        recDropdown->update_selected(prev_rec);

    if (machineList->selected_index() >= 0)
        _saved_machine = machineList->get_selected_item().name;

    machineList->clear();
    auto rows = _model->machine_rows();
    for (int i = 0; i < (int)rows.size(); i++)
        machineList->add_item({rows[i].text, {i}});

    for (int i = 0; i < machineList->item_count(); i++)
    {
        machineList->set_selected_index(i);
        if (machineList->get_selected_item().name == _saved_machine) break;
        machineList->clear_selection();
    }
}

string RecGepView::name_text() const { return nameInput ? nameInput->text() : ""; }
int RecGepView::size_value() const { return sizeInput ? sizeInput->value() : 1; }
int RecGepView::count_value() const { return countInput ? countInput->value() : 1; }

int RecGepView::selected_rec_index() const
{
    if (!recDropdown || !_model) return -1;
    return _model->selected_rec_index(recDropdown->selected_item());
}

int RecGepView::selected_machine_row() const
{
    if (!machineList || machineList->selected_index() < 0) return -1;
    ListItem li = machineList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}
