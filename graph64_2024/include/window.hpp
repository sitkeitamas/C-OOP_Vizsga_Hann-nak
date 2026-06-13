#ifndef WINDOW_HPP
#define WINDOW_HPP

// ============================================================
// F_SYNC - WINDOW.HPP
//
// "refresh_lists" minta: a Lista widgetek CSAK megjelenítők.
// Az igazi adat: _items (vector<Item>) – ez az egyetlen forrás.
// Minden módosítás után refresh_lists() újraépíti a widgeteket.
//
// Tartalmazza:
//   fill_inputs_from_selection() – kijelölésből feltölti az inputokat
//   modify_item()                – kijelölt elem módosítása
//   move_up() / move_down()      – elem sorrendjének cseréje
//
// OPCIONÁLIS – apply / release minta (hozzárendelős feladatokhoz):
//   Ha a feladat nem egyszerű "áthelyezés" hanem "hozzárendelés",
//   azaz a bal lista egy eleme egy mezőn keresztül hivatkozik a jobb
//   lista egy elemére (pl. orvos ↔ műtét), -> az apply/release
//   minta a move_to_right/move_to_left helyett.
// ============================================================

#include "application.hpp"
#include "button.hpp"
#include "setvalue.hpp"
#include "statictext.hpp"
#include "text.hpp"
#include "list.hpp"
#include <vector>
#include <string>

// ===== AZ EGYETLEN ADATSTRUKTÚRA =====
struct Item {
    std::string name;
    int value;
    bool picked;    // melyik listán van (false=bal, true=jobb)
};

class Window : public Lib {
protected:
    std::vector<Item> _items;

    StaticText *titleLabel;
    StaticText *nameLabel;
    StaticText *leftListLabel;
    StaticText *rightListLabel;
    StaticText *statusLabel;
    StaticText *statsLabel;

    Text     *nameInput;
    SetValue *valueInput;

    Button *addButton;
    Button *editButton;
    Button *delButton;
    Button *moveUpButton;
    Button *moveDownButton;
    Button *moveRightButton;
    Button *moveLeftButton;
    // Hozzárendelős feladathoz (apply/release minta):
    // Button *applyButton;    // "Hozzárendel" gomb
    // Button *releaseButton;  // "Felszabadít" gomb

    // ===== LISTÁK =====
    List *warehouseList;
    List *pickedList;

    // ===== SEGÉDFÜGGVÉNYEK =====
    void refresh_lists();
    void update_stats();
    void fill_inputs_from_selection();   // kijelölésből feltölti az inputokat
    int  warehouse_item_index() const;   // kijelölt warehouseList elem → _items index
    int  picked_item_index()    const;   // kijelölt pickedList elem → _items index
    int  active_item_index()    const;   // bármelyik listán kijelölt → _items index

public:
    Window(int, int);

    void add_item();
    void modify_item();      // kijelölt elem módosítása az inputok alapján
    void delete_item();
    void move_up();          // kijelölt elem feljebb (bal listán belül)
    void move_down();        // kijelölt elem lejjebb (bal listán belül)
    void move_to_right();
    void move_to_left();

    void apply();
    void release();
};

#endif
