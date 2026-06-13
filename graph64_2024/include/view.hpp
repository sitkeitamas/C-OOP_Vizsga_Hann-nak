#ifndef VIEW_HPP
#define VIEW_HPP

#include "application.hpp"
#include "button.hpp"
#include "config.hpp"
#include "dropdown.hpp"
#include "list.hpp"
#include "model.hpp"
#include "setvalue.hpp"
#include "statictext.hpp"
#include "text.hpp"
#include <functional>
#include <string>

// Nézet: csak widgetek + refresh. Üzleti logika a Model-ben, esemény a Controller-ben.
class AppView
{
    Model* _model = nullptr;

    StaticText* titleLabel = nullptr;
    StaticText* statusLabel = nullptr;
    StaticText* statsLabel = nullptr;
    StaticText* stockDisplay = nullptr;
    Text* nameInput = nullptr;
    Text* tagInput = nullptr;
    SetValue* val0Input = nullptr;
    SetValue* val1Input = nullptr;
    List* leftList = nullptr;
    List* rightList = nullptr;
    List* machineList = nullptr;
    Dropdown* catalogDropdown = nullptr;
    Dropdown* categoryDropdown = nullptr;
    SetValue* qtyInput = nullptr;
    SetValue* countInput = nullptr;

    std::string _saved_left;
    std::string _saved_right;
    std::string _saved_machine;
    std::string _preserve_catalog;
    std::string _preserve_category;

public:
    void bind(Model* model) { _model = model; }
    void create_widgets(Lib* lib);
    void wire_list_select_handlers(std::function<void()> handler);
    void set_status(const std::string& msg);
    void refresh_all();
    void fill_inputs_from_selection(int item_index);
    void clear_name_input();

    std::string name_text() const;
    std::string tag_text() const;
    int val0_number() const;
    int val1_number() const;
    int sell_quantity() const;
    int insert_count() const;
    int selected_left_item_index() const;
    int selected_right_item_index() const;
    int selected_machine_row() const;
    int active_item_index() const;
    int catalog_item_index() const;
    std::string selected_category() const;

    Button* addButton = nullptr;
    Button* editButton = nullptr;
    Button* delButton = nullptr;
    Button* moveUpButton = nullptr;
    Button* moveDownButton = nullptr;
    Button* moveRightButton = nullptr;
    Button* moveLeftButton = nullptr;
    Button* assignButton = nullptr;
    Button* releaseButton = nullptr;
    Button* sellButton = nullptr;
    Button* restockAllButton = nullptr;
    Button* restockSelectedButton = nullptr;
    Button* insertButton = nullptr;
    Button* removeMachineButton = nullptr;

private:
    void refresh_two_lists();
    void refresh_inventory();
    void refresh_machine();
    void refresh_categories();
    void update_stats();
    std::string row_text(const Item& item) const;
};

#endif
