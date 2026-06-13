#ifndef SYNC_VIEW_HPP
#define SYNC_VIEW_HPP

#include "application.hpp"
#include "button.hpp"
#include "list.hpp"
#include "setvalue.hpp"
#include "statictext.hpp"
#include "sync_model.hpp"
#include "text.hpp"
#include <functional>
#include <string>

class SyncView
{
    SyncModel* _model = nullptr;

    StaticText* titleLabel = nullptr;
    StaticText* statusLabel = nullptr;
    StaticText* statsLabel = nullptr;
    Text* nameInput = nullptr;
    SetValue* valueInput = nullptr;
    List* warehouseList = nullptr;
    List* pickedList = nullptr;

    std::string _saved_left;
    std::string _saved_right;

public:
    void bind(SyncModel* model) { _model = model; }

    void create_widgets(Lib* lib);
    void wire_list_select_handlers(std::function<void()> handler);
    void set_status(const std::string& msg);
    void refresh_all();
    void fill_inputs_from_selection(int item_index);
    void clear_name_input();

    std::string name_text() const;
    int value_number() const;
    int selected_warehouse_item_index() const;
    int selected_picked_item_index() const;
    int active_item_index() const;

    Button* addButton = nullptr;
    Button* editButton = nullptr;
    Button* delButton = nullptr;
    Button* moveUpButton = nullptr;
    Button* moveDownButton = nullptr;
    Button* moveRightButton = nullptr;
    Button* moveLeftButton = nullptr;

private:
    void refresh_lists();
    void update_stats();
    std::string row_text(const SyncItem& item) const;
};

#endif
