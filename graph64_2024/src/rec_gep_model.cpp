#include "rec_gep_model.hpp"

bool RecGepModel::add_rec(const std::string& name, int size)
{
    if (name.empty() || size < 1)
        return false;
    _rec.push_back({name, size});
    return true;
}

bool RecGepModel::insert_to_machine(int rec_index, int copy_count)
{
    if (rec_index < 0 || rec_index >= (int)_rec.size() || copy_count < 1)
        return false;
    const RecEntry& r = _rec[rec_index];
    for (int i = 0; i < copy_count; i++)
        _machine.push_back({r.name, r.size});
    return true;
}

std::vector<std::string> RecGepModel::rec_labels() const
{
    std::vector<std::string> labels;
    for (const auto& r : _rec)
        labels.push_back(r.name + "  (méret: " + std::to_string(r.size) + ")");
    return labels;
}

std::vector<MachineDisplayRow> RecGepModel::machine_rows() const
{
    std::vector<MachineDisplayRow> rows;
    for (int ci = 0; ci < (int)_machine.size(); ci++)
    {
        const MachineCopy& c = _machine[ci];
        for (int slot = 1; slot <= c.size; slot++)
        {
            MachineDisplayRow row;
            row.copy_index = ci;
            row.slot = slot;
            row.text = c.name + "  " + std::to_string(slot) + "/" + std::to_string(c.size);
            rows.push_back(row);
        }
    }
    return rows;
}

bool RecGepModel::remove_machine_row(int display_row_index)
{
    auto rows = machine_rows();
    if (display_row_index < 0 || display_row_index >= (int)rows.size())
        return false;
    int ci = rows[display_row_index].copy_index;
    if (ci < 0 || ci >= (int)_machine.size())
        return false;
    _machine.erase(_machine.begin() + ci);
    return true;
}

int RecGepModel::selected_rec_index(const std::string& label) const
{
    auto labels = rec_labels();
    for (int i = 0; i < (int)labels.size(); i++)
        if (labels[i] == label)
            return i;
    return -1;
}
