#include "sync_model.hpp"

bool SyncModel::validate_unique_name(const std::string& name, int skip_index) const
{
    for (int i = 0; i < (int)_items.size(); i++)
        if (i != skip_index && _items[i].name == name)
            return false;
    return true;
}

bool SyncModel::add_item(const std::string& name, int value)
{
    if (name.empty() || !validate_unique_name(name))
        return false;
    _items.push_back({name, value, false});
    return true;
}

bool SyncModel::modify_item(int index, const std::string& name, int value)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked)
        return false;
    if (name.empty() || !validate_unique_name(name, index))
        return false;
    _items[index].name = name;
    _items[index].value = value;
    return true;
}

bool SyncModel::remove_item(int index)
{
    if (index < 0 || index >= (int)_items.size())
        return false;
    _items.erase(_items.begin() + index);
    return true;
}

bool SyncModel::move_item_up(int index)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked)
        return false;
    for (int i = index - 1; i >= 0; i--)
        if (!_items[i].picked)
        {
            std::swap(_items[index], _items[i]);
            return true;
        }
    return false;
}

bool SyncModel::move_item_down(int index)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked)
        return false;
    for (int i = index + 1; i < (int)_items.size(); i++)
        if (!_items[i].picked)
        {
            std::swap(_items[index], _items[i]);
            return true;
        }
    return false;
}

bool SyncModel::move_item_to_right(int index)
{
    if (index < 0 || index >= (int)_items.size() || _items[index].picked)
        return false;
    _items[index].picked = true;
    return true;
}

bool SyncModel::move_item_to_left(int index)
{
    if (index < 0 || index >= (int)_items.size() || !_items[index].picked)
        return false;
    _items[index].picked = false;
    return true;
}

int SyncModel::warehouse_index(int index) const
{
    return (index >= 0 && index < (int)_items.size() && !_items[index].picked) ? index : -1;
}

int SyncModel::stats_picked_count() const
{
    int n = 0;
    for (const auto& it : _items)
        if (it.picked) n++;
    return n;
}

int SyncModel::stats_picked_sum() const
{
    int s = 0;
    for (const auto& it : _items)
        if (it.picked) s += it.value;
    return s;
}
