#include "dropdown.hpp"
#include "graphics.hpp"
#include <algorithm>

using namespace genv;
using namespace std;

// ============================================================
// DROPDOWN.CPP – JAVÍTOTT VERZIÓ
// ============================================================

Dropdown::Dropdown(Lib* lib, int x, int y, int sx, int sy,
                   const vector<string>& items)
    : Widget(lib, x, y, sx, sy),
      _items(items),
      _selected_index(items.empty() ? -1 : 0),
      _scroll_offset(0),
      _opened(false),
      _row_height(sy),
      _visible_rows(5),
      _on_select(nullptr)
{
    set_draggable(true);   // FIX: ctrl+egérrel húzható
}

// ============================================================
// FIX: nyitott állapotban a lenyíló lista területét is lefedi.
// Az application.cpp open-dropdown prioritás logikájával együtt
// biztosítja, hogy kattintás máshova is bezárja a dropdownt.
// ============================================================
bool Dropdown::is_selected(int mx, int my) const
{
    bool on_header = (mx >= _x && mx < _x + _sx && my >= _y && my < _y + _sy);
    if (!_opened) return on_header;

    bool on_list = (mx >= _x && mx < _x + _sx &&
                    my >= _y + _sy &&
                    my < _y + _sy + _visible_rows * _row_height);
    return on_header || on_list;
}

// ============================================================
// MEGJELENÍTÉS
// ============================================================
void Dropdown::draw() const
{
    // --- Fejléc ---
    gout << color(50, 56, 82)  << move_to(_x, _y) << box(_sx, _sy)
         << color(38, 43, 64)  << move_to(_x + 2, _y + 2) << box(_sx - 4, _sy - 4);

    string shown = (_selected_index >= 0 && _selected_index < (int)_items.size())
                   ? _items[_selected_index] : "";
    gout << color(235, 239, 255)
         << move_to(_x + 8, _y + _sy / 2 - 10)
         << genv::text(shown);

    // Nyíl ikon jobb oldalon
    int ax = _x + _sx - 14;
    int ay = _y + _sy / 2 - 3;
    gout << color(180, 195, 240);
    if (_opened) {
        // Felfelé mutató háromszög (nyitva)
        gout << move_to(ax, ay + 6) << line(10, 0) << line(-5, -6) << line(-5, 6);
    } else {
        // Lefelé mutató háromszög (zárva)
        gout << move_to(ax, ay) << line(10, 0) << line(-5, 6) << line(-5, -6);
    }

    if (!_opened) return;

    // --- Lenyíló lista ---
    int list_y = _y + _sy;
    int list_h  = _visible_rows * _row_height;

    gout << color(30, 34, 52) << move_to(_x, list_y)         << box(_sx, list_h)
         << color(55, 62, 92) << move_to(_x + 1, list_y + 1) << box(_sx - 2, list_h - 2);

    for (int i = 0; i < _visible_rows; i++)
    {
        int idx   = _scroll_offset + i;
        if (idx >= (int)_items.size()) break;

        int row_y = list_y + i * _row_height;

        if (idx == _selected_index)
            gout << color(79, 128, 255) << move_to(_x + 2, row_y + 1) << box(_sx - 4, _row_height - 2);
        else
            gout << color(43, 49, 73)   << move_to(_x + 2, row_y + 1) << box(_sx - 4, _row_height - 2);

        gout << color(235, 239, 255)
             << move_to(_x + 8, row_y + _row_height / 2 - 10)
             << genv::text(_items[idx]);
    }
}

// ============================================================
// ESEMÉNYKEZELÉS
// ============================================================
void Dropdown::handle(event ev)
{
    if (!is_clickable()) return;
    if (ev.type != ev_mouse) return;

    bool on_header = (ev.pos_x >= _x && ev.pos_x < _x + _sx &&
                      ev.pos_y >= _y  && ev.pos_y < _y + _sy);
    bool on_list   = (ev.pos_x >= _x && ev.pos_x < _x + _sx &&
                      ev.pos_y >= _y + _sy &&
                      ev.pos_y < _y + _sy + _visible_rows * _row_height);

    // Fejlécre kattintás: nyit/zár toggle
    if (ev.button == btn_left && on_header)
    {
        _opened = !_opened;
        return;
    }

    if (!_opened) return;

    // Görgetés (bárhol, amíg nyitva van)
    int max_offset = max(0, (int)_items.size() - _visible_rows);
    if (ev.button == btn_wheelup && _scroll_offset > 0)
        { _scroll_offset--; return; }
    if (ev.button == btn_wheeldown && _scroll_offset < max_offset)
        { _scroll_offset++; return; }

    // Listán belüli kattintás: elem kiválasztása
    if (ev.button == btn_left && on_list)
    {
        int local_y = ev.pos_y - (_y + _sy);
        int idx     = local_y / _row_height + _scroll_offset;
        if (idx >= 0 && idx < (int)_items.size())
        {
            _selected_index = idx;
            if (_on_select) _on_select();
        }
        _opened = false;
        return;
    }

    // Kattintás a fejlécen és listán kívül: bezárás
    if (ev.button == btn_left)
    {
        _opened = false;
    }
}

// ============================================================
// FIX: update_selected – NÉV SZERINT KIVÁLASZTÁS
// Az eredeti implementáció az elemet ÁTNEVEZTE – ez bug volt.
// Most megkeresi a nevet és kijelöli. Ha nincs ilyen, nem crashel.
// ============================================================
void Dropdown::update_selected(const string& name)
{
    for (int i = 0; i < (int)_items.size(); i++)
    {
        if (_items[i] == name)
        {
            _selected_index = i;
            return;
        }
    }
    // Ha a név nem szerepel a listában: nem változtat semmit
}

// Átnevezés: a jelenleg kijelölt elem nevét módosítja
void Dropdown::rename_selected(const string& new_value)
{
    if (_selected_index >= 0 && _selected_index < (int)_items.size())
        _items[_selected_index] = new_value;
}

void Dropdown::add_item(const string& item)
{
    _items.push_back(item);
}

void Dropdown::remove_selected()
{
    if (_selected_index < 0 || _selected_index >= (int)_items.size()) return;
    _items.erase(_items.begin() + _selected_index);
    // Kijelölés kiigazítása (ne kerüljön a határon túlra)
    if (_selected_index >= (int)_items.size())
        _selected_index = (int)_items.size() - 1;
    _scroll_offset = 0;
}

void Dropdown::remove_item(const string& item)
{
    auto it = find(_items.begin(), _items.end(), item);
    if (it == _items.end()) return;

    int index = (int)(it - _items.begin());
    _items.erase(it);

    if (_selected_index == index)
        _selected_index = _items.empty() ? -1 : max(0, index - 1);
    else if (_selected_index > index)
        _selected_index--;

    // Görgetési offszet kiigazítása
    int max_offset = max(0, (int)_items.size() - _visible_rows);
    if (_scroll_offset > max_offset) _scroll_offset = max_offset;
}

void Dropdown::clear()
{
    _items.clear();
    _selected_index = -1;
    _scroll_offset  = 0;
    _opened         = false;
}

void Dropdown::close() { _opened = false; }

int Dropdown::selected_index() const { return _selected_index; }
bool Dropdown::is_opened() const { return _opened; }

string Dropdown::selected_item() const
{
    if (_selected_index >= 0 && _selected_index < (int)_items.size())
        return _items[_selected_index];
    return "";
}

// FIX: -1 megengedett → kijelölés törlése
void Dropdown::set_selected_index(int index)
{
    if (index < 0)                         { _selected_index = -1; return; }
    if (index < (int)_items.size())        { _selected_index = index; }
}

void Dropdown::set_items(const vector<string>& items)
{
    _items          = items;
    _selected_index = items.empty() ? -1 : 0;
    _scroll_offset  = 0;
    _opened         = false;
}

void Dropdown::set_on_select(function<void()> callback) { _on_select = callback; }