#ifndef ICECREAM_MODEL_HPP
#define ICECREAM_MODEL_HPP

#include <string>
#include <vector>

// Fagyi alapfeladat (A) – Minta E + F: készlet, kiadás, megjelenítés.
struct IceCreamFlavor
{
    std::string name;
    int stock = 0;
};

class IceCreamModel
{
    std::vector<IceCreamFlavor> _flavors;
    static constexpr int kRestockAmount = 10;

public:
    const std::vector<IceCreamFlavor>& flavors() const { return _flavors; }

    void seed_defaults();
    bool add_flavor(const std::string& name, int stock = kRestockAmount);
    bool remove_flavor(const std::string& name);

    std::vector<std::string> available_names() const;
    std::string stock_display_text() const;

    bool can_sell(const std::string& name, int qty) const;
    bool sell(const std::string& name, int qty);
    bool restock_all();
    bool restock_selected(const std::string& name);

    int stock_of(const std::string& name) const;
};

#endif
