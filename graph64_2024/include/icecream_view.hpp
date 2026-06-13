#ifndef ICECREAM_VIEW_HPP
#define ICECREAM_VIEW_HPP

#include "application.hpp"
#include "button.hpp"
#include "dropdown.hpp"
#include "icecream_model.hpp"
#include "setvalue.hpp"
#include "statictext.hpp"
#include <string>

class IceCreamView
{
    IceCreamModel* _model = nullptr;

    StaticText* titleLabel = nullptr;
    StaticText* stockDisplay = nullptr;
    StaticText* statusLabel = nullptr;
    Dropdown* flavorDropdown = nullptr;
    SetValue* qtyInput = nullptr;

    std::string _preserve_selection;

public:
    void bind(IceCreamModel* model) { _model = model; }
    void create_widgets(Lib* lib);
    void set_status(const std::string& msg);
    void refresh_all();

    std::string selected_flavor() const;
    int sell_quantity() const;
    void remember_selection();

    Button* sellButton = nullptr;
    Button* restockAllButton = nullptr;
    Button* restockSelectedButton = nullptr;
};

#endif
