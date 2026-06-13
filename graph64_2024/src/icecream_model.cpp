#include "icecream_model.hpp"
#include <algorithm>

void IceCreamModel::seed_defaults()
{
    _flavors = {
        {"vanília", 5},
        {"csoki", 8},
        {"eper", 3},
        {"pistácia", 6},
    };
}

bool IceCreamModel::add_flavor(const std::string& name, int stock)
{
    if (name.empty()) return false;
    for (const auto& f : _flavors)
        if (f.name == name) return false;
    _flavors.push_back({name, stock});
    return true;
}

bool IceCreamModel::remove_flavor(const std::string& name)
{
    auto it = std::find_if(_flavors.begin(), _flavors.end(),
                           [&](const IceCreamFlavor& f) { return f.name == name; });
    if (it == _flavors.end()) return false;
    _flavors.erase(it);
    return true;
}

std::vector<std::string> IceCreamModel::available_names() const
{
    std::vector<std::string> names;
    for (const auto& f : _flavors)
        if (f.stock > 0)
            names.push_back(f.name);
    return names;
}

std::string IceCreamModel::stock_display_text() const
{
    if (_flavors.empty()) return "Készlet: (üres)";
    std::string text = "Készlet: ";
    for (size_t i = 0; i < _flavors.size(); i++)
    {
        if (i > 0) text += "  |  ";
        text += _flavors[i].name + ": " + std::to_string(_flavors[i].stock) + " db";
    }
    return text;
}

int IceCreamModel::stock_of(const std::string& name) const
{
    for (const auto& f : _flavors)
        if (f.name == name) return f.stock;
    return 0;
}

bool IceCreamModel::can_sell(const std::string& name, int qty) const
{
    if (name.empty() || qty <= 0) return false;
    return qty <= stock_of(name);
}

bool IceCreamModel::sell(const std::string& name, int qty)
{
    if (!can_sell(name, qty)) return false;
    for (auto& f : _flavors)
        if (f.name == name)
        {
            f.stock -= qty;
            return true;
        }
    return false;
}

bool IceCreamModel::restock_all()
{
    if (_flavors.empty()) return false;
    for (auto& f : _flavors)
        f.stock += kRestockAmount;
    return true;
}

bool IceCreamModel::restock_selected(const std::string& name)
{
    if (name.empty()) return false;
    for (auto& f : _flavors)
        if (f.name == name)
        {
            f.stock += kRestockAmount;
            return true;
        }
    return false;
}
