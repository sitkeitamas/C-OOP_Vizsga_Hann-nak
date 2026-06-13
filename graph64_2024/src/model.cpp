#include "model.hpp"
#include <algorithm>

void Model::ensure_vals(Item& it) const
{
    if ((int)it.vals.size() < VALUE_DIMENSION)
        it.vals.resize(VALUE_DIMENSION, 0);
}

void Model::seed_demo(DemoSeed seed)
{
    _items.clear();
    switch (seed)
    {
    case DemoSeed::TwoList:
        add_item("Alma", 150);
        add_item("Körte", 200);
        add_item("Banán", 80);
        break;
    case DemoSeed::Inventory:
        add_item("vanília", 5);
        add_item("csoki", 8);
        add_item("eper", 3);
        add_item("pistácia", 6);
        break;
    case DemoSeed::RecGep:
        add_rec("nev1", 4);
        add_rec("nev2", 4);
        break;
    default:
        break;
    }
}

bool Model::validate_unique_name(const std::string& name, int skip_index) const
{
    if (DUPLICATE_NAME_MODE == AllowDuplicateNames) return true;
    for (int i = 0; i < (int)_items.size(); i++)
        if (i != skip_index && _items[i].name == name)
            return false;
    return true;
}

bool Model::add_item(const std::string& name, int val0, int val1, const std::string& tag)
{
    if (name.empty() || !validate_unique_name(name)) return false;
    Item it;
    it.name = name;
    it.vals = {val0, val1};
    it.tag = tag;
    it.picked = false;
    ensure_vals(it);
    _items.push_back(it);
    return true;
}

bool Model::modify_item(int index, const std::string& name, int val0, int val1,
                          const std::string& tag)
{
    if (index < 0 || index >= (int)_items.size()) return false;
    if (UI_TWO_LIST && _items[index].picked) return false;
    if (name.empty() || !validate_unique_name(name, index)) return false;
    _items[index].name = name;
    _items[index].vals = {val0, val1};
    ensure_vals(_items[index]);
    _items[index].tag = tag;
    return true;
}

bool Model::remove_item(int index)
{
    if (index < 0 || index >= (int)_items.size()) return false;
    _items.erase(_items.begin() + index);
    return true;
}

bool Model::move_item_up(int index)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked) return false;
    for (int i = index - 1; i >= 0; i--)
        if (!_items[i].picked)
        {
            std::swap(_items[index], _items[i]);
            return true;
        }
    return false;
}

bool Model::move_item_down(int index)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked) return false;
    for (int i = index + 1; i < (int)_items.size(); i++)
        if (!_items[i].picked)
        {
            std::swap(_items[index], _items[i]);
            return true;
        }
    return false;
}

bool Model::move_item_to_right(int index)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked) return false;
    _items[index].picked = true;
    if (!MOVE_KEEP_ON_LEFT)
        ; // picked=true already moves visually; keep on left if MOVE_KEEP_ON_LEFT
    return true;
}

bool Model::move_item_to_left(int index)
{
    if (index < 0 || index >= (int)_items.size() || !_items[index].picked) return false;
    _items[index].picked = false;
    return true;
}

bool Model::assign(int left_index, int right_index)
{
    if (left_index < 0 || right_index < 0) return false;
    if (left_index >= (int)_items.size() || right_index >= (int)_items.size()) return false;
    _items[right_index].assigned = _items[left_index].name;
    return true;
}

bool Model::release(int index)
{
    if (index < 0 || index >= (int)_items.size()) return false;
    _items[index].assigned.clear();
    return true;
}

bool Model::can_sell(int index, int qty) const
{
    if (index < 0 || index >= (int)_items.size() || qty <= 0) return false;
    ensure_vals(const_cast<Item&>(_items[index]));
    return qty <= _items[index].vals[VAL_STOCK];
}

bool Model::sell(int index, int qty)
{
    if (!can_sell(index, qty)) return false;
    _items[index].vals[VAL_STOCK] -= qty;
    return true;
}

bool Model::restock(int index, int amount)
{
    if (index < 0 || index >= (int)_items.size()) return false;
    ensure_vals(_items[index]);
    _items[index].vals[VAL_STOCK] += amount;
    return true;
}

bool Model::restock_all(int amount)
{
    if (_items.empty()) return false;
    for (auto& it : _items)
    {
        ensure_vals(it);
        it.vals[VAL_STOCK] += amount;
    }
    return true;
}

std::vector<std::string> Model::available_names() const
{
    std::vector<std::string> names;
    for (const auto& it : _items)
    {
        ensure_vals(const_cast<Item&>(it));
        if (it.vals[VAL_STOCK] > 0)
            names.push_back(it.name);
    }
    return names;
}

std::string Model::stock_display_text() const
{
    if (_items.empty()) return "Készlet: (üres)";
    std::string text = "Készlet: ";
    for (size_t i = 0; i < _items.size(); i++)
    {
        if (i > 0) text += "  |  ";
        ensure_vals(const_cast<Item&>(_items[i]));
        text += _items[i].name + ": " + std::to_string(_items[i].vals[VAL_STOCK]) + " db";
    }
    return text;
}

std::vector<std::string> Model::all_tags() const
{
    std::vector<std::string> tags;
    for (const auto& it : _items)
        if (!it.tag.empty() &&
            std::find(tags.begin(), tags.end(), it.tag) == tags.end())
            tags.push_back(it.tag);
    return tags;
}

std::vector<int> Model::indices_with_tag(const std::string& tag) const
{
    std::vector<int> idx;
    for (int i = 0; i < (int)_items.size(); i++)
        if (tag.empty() || _items[i].tag == tag)
            idx.push_back(i);
    return idx;
}

bool Model::time_overlap(int index_a, int index_b) const
{
    if (index_a < 0 || index_b < 0 || index_a >= (int)_items.size() ||
        index_b >= (int)_items.size())
        return false;
    const Item& a = _items[index_a];
    const Item& b = _items[index_b];
    if ((int)a.vals.size() < 2 || (int)b.vals.size() < 2) return false;
    int a0 = a.vals[VAL_STOCK], a1 = a.vals[VAL_SECOND];
    int b0 = b.vals[VAL_STOCK], b1 = b.vals[VAL_SECOND];
    return a0 < b0 + b1 && b0 < a0 + a1;
}

std::vector<Model::DisplayRow> Model::machine_display_rows() const
{
    std::vector<DisplayRow> rows;
    for (int i = 0; i < (int)_items.size(); i++)
    {
        if (!_items[i].picked) continue;
        ensure_vals(const_cast<Item&>(_items[i]));
        int size = std::max(1, _items[i].vals[VAL_STOCK]);
        for (int slot = 1; slot <= size; slot++)
        {
            DisplayRow r;
            r.item_index = i;
            r.sub_index = slot;
            r.text = _items[i].name + "  " + std::to_string(slot) + "/" +
                     std::to_string(size);
            rows.push_back(r);
        }
    }
    return rows;
}

bool Model::remove_machine_group(int display_row_index)
{
    auto rows = machine_display_rows();
    if (display_row_index < 0 || display_row_index >= (int)rows.size()) return false;
    int idx = rows[display_row_index].item_index;
    return remove_item(idx);
}

bool Model::add_rec(const std::string& name, int size)
{
    return add_item(name, size, 0);
}

bool Model::insert_rec_to_machine(int rec_index, int copy_count)
{
    if (rec_index < 0 || rec_index >= (int)_items.size() || copy_count < 1) return false;
    if (_items[rec_index].picked) return false;
    const Item& r = _items[rec_index];
    for (int c = 0; c < copy_count; c++)
    {
        Item copy;
        copy.name = r.name;
        copy.vals = {r.vals.empty() ? 1 : r.vals[VAL_STOCK], 0};
        ensure_vals(copy);
        copy.picked = true;
        _items.push_back(copy);
    }
    return true;
}

int Model::picked_count() const
{
    int n = 0;
    for (const auto& it : _items)
        if (it.picked) n++;
    return n;
}

int Model::picked_sum(int val_index) const
{
    int s = 0;
    for (const auto& it : _items)
        if (it.picked && val_index < (int)it.vals.size())
            s += it.vals[val_index];
    return s;
}
