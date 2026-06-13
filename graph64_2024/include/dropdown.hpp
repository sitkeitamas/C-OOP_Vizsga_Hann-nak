#ifndef DROPDOWN_HPP
#define DROPDOWN_HPP

// ============================================================
// DROPDOWN.HPP – JAVÍTOTT VERZIÓ
//
// Javítások az eredetihez képest:
//  1. set_draggable(true) → ctrl+egérrel húzható
//  2. is_selected() override → nyitott állapotban a lenyíló
//     lista területét is lefeedi (kattintás nem kerül más widgetre)
//  3. update_selected(name) JAVÍTVA: most tényleg NÉV SZERINT
//     KIVÁLASZT (az original verziója átnevezte – ez bug volt)
//  4. rename_selected(new_value) → az elem tényleges átnevezéséhez
//  5. set_on_select(callback) → eseménykezelő hozzáadása
//  6. set_selected_index(-1) engedélyezve → töröli a kijelölést
//  7. Görgetés / remove_item javítva
// ============================================================

#include "widgets.hpp"
#include <vector>
#include <string>
#include <functional>

class Dropdown : public Widget
{
protected:
    std::vector<std::string> _items;
    int _selected_index;
    int _scroll_offset;
    bool _opened;
    int _row_height;
    int _visible_rows;
    std::function<void()> _on_select;

public:
    Dropdown(Lib* lib, int x, int y, int sx, int sy,
             const std::vector<std::string>& items = {});

    // Nyitott állapotban a lenyíló lista területét is lefedi
    bool is_selected(int mx, int my) const override;
    void draw() const override;
    void handle(genv::event ev) override;

    void add_item(const std::string& item);
    void remove_selected();
    void remove_item(const std::string& item);

    // update_selected: NÉV SZERINT kiválasztja az elemet
    //   pl. dd->update_selected("jazz") → kijelöli a "jazz" nevű sort
    void update_selected(const std::string& name);

    // rename_selected: a JELENLEG kijelölt elem nevét átírja
    void rename_selected(const std::string& new_value);

    void clear();
    void close();   // programozottan bezárja

    int         selected_index() const;
    std::string selected_item()  const;

    void set_selected_index(int index);   // -1 = kijelölés törlése
    void set_items(const std::vector<std::string>& items);

    bool is_opened() const;

    // Callback: kiválasztáskor hívódik
    void set_on_select(std::function<void()> callback);
};

#endif