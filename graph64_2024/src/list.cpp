#include "list.hpp"
#include "graphics.hpp"
#include <algorithm>
#include <cctype>

using namespace genv;
using namespace std;

int List::_global_activation_counter = 0;

List::List(Lib* lib, int x, int y, int sx, int sy)
    : Widget(lib, x, y, sx, sy), _selected_index(-1), _scroll_offset(0),
      _activation_order(0), _sort_state(0), _sort_mode(DefaultOrder),
      _next_original_order(1), _scrollbar_dragging(false), _scrollbar_drag_offset(0),
      _capacity(-1), _weight_col(-1), _value_sort_col(-1), _value_sort_dir(0)
{
    set_draggable(true);
}

// ============================================================
// SZÖVEG AMI A LISTÁBAN LÁTHATÓ  ← CSAK EZT KELL MÓDOSÍTANI!
// ============================================================
std::string List::row_text(const ListItem& item) const
{
    // TODO: módosítsd a feladatnak megfelelően
    // Pl. ha VALUE_DIMENSION = 2 és a két érték: méret és ár:
    //   return item.name + " | méret: " + to_string(item.values[0]) + " | ár: " + to_string(item.values[1]);
    //
    // Általános megjelenítő (minden értéket mutat):
    string result = item.name;
    for (int i = 0; i < (int)item.values.size(); i++)
        result += " | " + to_string(i+1) + ": " + to_string(item.values[i]);
    return result;
}

// ===== SZŰRÉS: látható elemek indexei =====
vector<int> List::visible_indices() const
{
    vector<int> result;
    if (_filter_text.empty())
    {
        for (int i = 0; i < (int)_items.size(); i++) result.push_back(i);
        return result;
    }

    // Kis-nagybetű érzéketlen szűrés: név tartalmazza a szűrőszöveget
    string lower_filter = _filter_text;
    transform(lower_filter.begin(), lower_filter.end(), lower_filter.begin(),
              [](unsigned char c) { return (char)tolower(c); });

    for (int i = 0; i < (int)_items.size(); i++)
    {
        string lower_name = _items[i].name;
        transform(lower_name.begin(), lower_name.end(), lower_name.begin(),
                  [](unsigned char c) { return (char)tolower(c); });
        if (lower_name.find(lower_filter) != string::npos)
            result.push_back(i);
    }
    return result;
}

void List::draw() const
{
    vector<int> visible = visible_indices();
    bool scrollbarVisible = has_scrollbar();
    int row_width = scrollbarVisible ? _sx - 31 : _sx - 16;

    gout << color(50, 56, 82) << move_to(_x, _y) << box(_sx, _sy)
         << color(91, 104, 143) << move_to(_x + 2, _y + 2) << box(_sx - 4, _sy - 4)
         << color(35, 39, 58) << move_to(_x + 5, _y + 5) << box(_sx - 10, _sy - 10);

    int line_height = 60;
    int rows_on_screen = visible_rows();

    for (int i = 0; i < rows_on_screen; i++)
    {
        int vi = i + _scroll_offset;
        if (vi >= (int)visible.size()) break;

        int idx = visible[vi];  // tényleges index _items-ben
        int y = _y + 8 + i * line_height;
        const ListItem& it = _items[idx];

        // Szín prioritás: kijelölt (kék) > jelölt (arany) > normál (sötét)
        if (idx == _selected_index)
            gout << color(79, 128, 255) << move_to(_x + 8, y) << box(row_width, line_height - 4);
        else if (_marked_orders.count(it.original_order))
            gout << color(160, 110, 20) << move_to(_x + 8, y) << box(row_width, line_height - 4);
        else
            gout << color(43, 49, 73) << move_to(_x + 8, y) << box(row_width, line_height - 4);

        gout << color(it.text_r, it.text_g, it.text_b) << move_to(_x + 16, y + 21) << genv::text(row_text(it));
    }

    if (scrollbarVisible)
    {
        int track_x = scrollbar_x(); int track_y = scrollbar_y();
        int track_h = scrollbar_height();
        int thumb_y = scrollbar_thumb_y(); int thumb_h = scrollbar_thumb_height();

        gout << color(30, 34, 52) << move_to(track_x, track_y) << box(11, track_h)
             << color(68, 78, 112) << move_to(track_x + 2, track_y + 2) << box(7, track_h - 4)
             << color(122, 150, 230) << move_to(track_x + 1, thumb_y) << box(9, thumb_h);
    }
}

void List::handle(event ev)
{
    if (!is_clickable()) return;
    int line_height = 60;

    if (ev.type == ev_mouse && _scrollbar_dragging)
    {
        if (ev.button == -btn_left) { _scrollbar_dragging = false; return; }
        scroll_to_thumb_position(ev.pos_y - _scrollbar_drag_offset);
        return;
    }

    if (ev.type == ev_mouse && ev.button == btn_left && is_on_scrollbar(ev.pos_x, ev.pos_y))
    {
        if (is_on_scrollbar_thumb(ev.pos_x, ev.pos_y))
        { _scrollbar_dragging = true; _scrollbar_drag_offset = ev.pos_y - scrollbar_thumb_y(); }
        else
        { _scrollbar_dragging = true; _scrollbar_drag_offset = scrollbar_thumb_height() / 2;
          scroll_to_thumb_position(ev.pos_y - _scrollbar_drag_offset); }
        return;
    }

    if (ev.type == ev_mouse && ev.button == btn_left && is_selected(ev.pos_x, ev.pos_y))
    {
        vector<int> visible = visible_indices();
        int local_y = ev.pos_y - _y - 8;
        int vi = local_y / line_height + _scroll_offset;   // látható index
        if (vi >= 0 && vi < (int)visible.size())
        {
            _selected_index = visible[vi];  // tényleges _items index
            _activation_order = ++_global_activation_counter;
            if (_on_select) _on_select();
        }
        else _selected_index = -1;
    }

    if (ev.type == ev_mouse && ev.button == btn_wheelup && is_selected(ev.pos_x, ev.pos_y))
        if (_scroll_offset > 0) _scroll_offset--;
    if (ev.type == ev_mouse && ev.button == btn_wheeldown && is_selected(ev.pos_x, ev.pos_y))
        if (_scroll_offset < max_scroll_offset()) _scroll_offset++;
}

// ===== ALAP MŰVELETEK =====

bool List::add_item(const ListItem& item)
{
    if (item.name.empty()) return false;
    if (DUPLICATE_NAME_MODE == RejectDuplicateNames && index_of(item.name) >= 0) return false;
    if (!can_add(item)) return false;   // kapacitás ellenőrzés

    ListItem stored = item;
    stored.original_order = _next_original_order++;
    _items.push_back(stored);
    apply_sort_mode();
    clamp_scroll_offset();
    return true;
}

void List::remove_selected()
{
    if (_selected_index >= 0 && _selected_index < (int)_items.size())
    {
        _marked_orders.erase(_items[_selected_index].original_order);
        _items.erase(_items.begin() + _selected_index);
        _selected_index = -1;
        clamp_scroll_offset();
    }
}

void List::clear()
{
    _items.clear();
    _selected_index = -1;
    _scroll_offset = 0;
    _sort_state = 0;
    _sort_mode = DefaultOrder;
    _next_original_order = 1;
    _scrollbar_dragging = false;
    _marked_orders.clear();
    _value_sort_col = -1;
    _value_sort_dir = 0;
    // _filter_text és _capacity szándékosan marad (felhasználó állítja)
}

void List::clear_selection() { _selected_index = -1; }

// ===== LEKÉRDEZÉSEK =====

ListItem List::get_selected_item() const { return item_at_index(_selected_index); }
int List::selected_index() const { return _selected_index; }
int List::item_count() const { return (int)_items.size(); }
int List::visible_count() const { return (int)visible_indices().size(); }
vector<ListItem> List::items() const { return _items; }
int List::activation_order() const { return _activation_order; }

// ===== MÓDOSÍTÁS =====

void List::set_selected_index(int index)
{
    if (index >= 0 && index < item_count()) _selected_index = index;
    else _selected_index = -1;
}

bool List::update_selected(const ListItem& item)
{
    ListItem old = get_selected_item();
    if (_selected_index < 0 || old.name.empty() || item.name.empty()) return false;
    if (DUPLICATE_NAME_MODE == RejectDuplicateNames && old.name != item.name && index_of(item.name) >= 0)
        return false;

    ListItem stored = item;
    stored.original_order = old.original_order;
    _items[_selected_index] = stored;
    clamp_scroll_offset();
    return true;
}


bool List::move_selected_up()
{
    if (_selected_index <= 0 || _selected_index >= item_count()) return false;
    swap(_items[_selected_index], _items[_selected_index - 1]);
    _selected_index--;
    _sort_state = 0;
    if (_selected_index < _scroll_offset) _scroll_offset = _selected_index;
    clamp_scroll_offset();
    return true;
}

bool List::move_selected_down()
{
    if (_selected_index < 0 || _selected_index >= item_count() - 1) return false;
    swap(_items[_selected_index], _items[_selected_index + 1]);
    _selected_index++;
    _sort_state = 0;
    if (_selected_index >= _scroll_offset + visible_rows())
        _scroll_offset = _selected_index - visible_rows() + 1;
    clamp_scroll_offset();
    return true;
}

// ===== RENDEZÉS =====

int List::cycle_name_sort()
{
    int selectedOrder = get_selected_item().original_order;
    _value_sort_col = -1;
    _value_sort_dir = 0;
    _sort_state = (_sort_state + 1) % 3;

    if (_sort_state == 1)
        stable_sort(_items.begin(), _items.end(), [](const ListItem& a, const ListItem& b) {
            return a.name < b.name; });
    else if (_sort_state == 2)
        stable_sort(_items.begin(), _items.end(), [](const ListItem& a, const ListItem& b) {
            return a.name > b.name; });
    else
        stable_sort(_items.begin(), _items.end(), [](const ListItem& a, const ListItem& b) {
            return a.original_order < b.original_order; });

    restore_selection_by_order(selectedOrder);
    clamp_scroll_offset();
    return _sort_state;
}

void List::set_sort_mode(SortMode mode)
{
    _sort_mode = mode;
    _sort_state = (mode == AbcOrder) ? 1 : (mode == ReverseAbcOrder ? 2 : 0);
    apply_sort_mode();
}

void List::apply_sort_mode()
{
    int selectedOrder = get_selected_item().original_order;
    if (_sort_mode == AbcOrder)
        stable_sort(_items.begin(), _items.end(), [](const ListItem& a, const ListItem& b) {
            return a.name < b.name; });
    else if (_sort_mode == ReverseAbcOrder)
        stable_sort(_items.begin(), _items.end(), [](const ListItem& a, const ListItem& b) {
            return a.name > b.name; });
    else
        stable_sort(_items.begin(), _items.end(), [](const ListItem& a, const ListItem& b) {
            return a.original_order < b.original_order; });
    restore_selection_by_order(selectedOrder);
}

// Érték szerinti rendezés: cycle col → 1 (növekvő) → 2 (csökkenő) → 0 (ki) → ...
// Visszatér a rendezés irányával (0=ki, 1=növekvő, 2=csökkenő)
int List::cycle_value_sort(int col)
{
    int selectedOrder = get_selected_item().original_order;

    if (_value_sort_col != col)
    {
        // Új oszlop: növekvő rendezés
        _value_sort_col = col;
        _value_sort_dir = 1;
    }
    else
    {
        _value_sort_dir++;          // 1→2→3
        if (_value_sort_dir > 2)    // 3 = visszaáll alaprendezésre
        {
            _value_sort_col = -1;
            _value_sort_dir = 0;
            apply_sort_mode();
            restore_selection_by_order(selectedOrder);
            clamp_scroll_offset();
            return 0;
        }
    }

    int c = _value_sort_col;
    int d = _value_sort_dir;
    stable_sort(_items.begin(), _items.end(), [c, d](const ListItem& a, const ListItem& b) {
        int va = (c < (int)a.values.size()) ? a.values[c] : 0;
        int vb = (c < (int)b.values.size()) ? b.values[c] : 0;
        return d == 1 ? va < vb : va > vb;
    });

    restore_selection_by_order(selectedOrder);
    clamp_scroll_offset();
    return _value_sort_dir;
}

// ===== ÁTHELYEZÉS MÁSIK LISTÁBA =====

bool List::move_item_to(List& other, const std::string& name)
{
    int index = index_of(name);
    if (index < 0) return false;
    if (!other.add_item(_items[index])) return false;
    _marked_orders.erase(_items[index].original_order);
    _items.erase(_items.begin() + index);
    _selected_index = -1;
    clamp_scroll_offset();
    return true;
}

// ===== SZŰRÉS =====

void List::set_filter(const std::string& text)
{
    _filter_text = text;
    _scroll_offset = 0;
    clamp_scroll_offset();
}

std::string List::filter_text() const { return _filter_text; }

// ===== JELÖLÉS =====

void List::toggle_mark()
{
    if (_selected_index < 0 || _selected_index >= (int)_items.size()) return;
    int order = _items[_selected_index].original_order;
    if (_marked_orders.count(order)) _marked_orders.erase(order);
    else _marked_orders.insert(order);
}

void List::clear_marks() { _marked_orders.clear(); }

bool List::is_marked(int index) const
{
    if (index < 0 || index >= (int)_items.size()) return false;
    return _marked_orders.count(_items[index].original_order) > 0;
}

vector<ListItem> List::marked_items() const
{
    vector<ListItem> result;
    for (const auto& item : _items)
        if (_marked_orders.count(item.original_order)) result.push_back(item);
    return result;
}

// ===== KAPACITÁS =====

void List::set_capacity(int capacity, int weight_col)
{
    _capacity = capacity;
    _weight_col = weight_col;
}

int List::current_weight() const
{
    if (_capacity < 0) return 0;
    int total = 0;
    for (const auto& item : _items)
    {
        if (_weight_col < 0) total += 1;
        else if (_weight_col < (int)item.values.size()) total += item.values[_weight_col];
    }
    return total;
}

bool List::can_add(const ListItem& item) const
{
    if (_capacity < 0) return true;     // nincs korlát
    int weight = (_weight_col < 0) ? 1
                 : (_weight_col < (int)item.values.size() ? item.values[_weight_col] : 0);
    return current_weight() + weight <= _capacity;
}

int List::capacity() const { return _capacity; }

void List::set_on_select(std::function<void()> callback) { _on_select = callback; }

// ===== BELSŐ SEGÉDFÜGGVÉNYEK =====

ListItem List::item_at_index(int index) const
{
    if (index < 0 || index >= (int)_items.size()) return ListItem();
    return _items[index];
}

int List::index_of(const std::string& name) const
{
    for (int i = 0; i < (int)_items.size(); i++)
        if (_items[i].name == name) return i;
    return -1;
}

int List::index_of_original_order(int original_order) const
{
    if (original_order <= 0) return -1;
    for (int i = 0; i < (int)_items.size(); i++)
        if (_items[i].original_order == original_order) return i;
    return -1;
}

void List::restore_selection_by_order(int original_order)
{
    _selected_index = index_of_original_order(original_order);
}

int List::visible_rows() const { return max(1, (_sy - 12) / 60); }
int List::max_scroll_offset() const { return max(0, visible_count() - visible_rows()); }
void List::clamp_scroll_offset() { _scroll_offset = max(0, min(_scroll_offset, max_scroll_offset())); }
bool List::has_scrollbar() const { return visible_count() > visible_rows(); }
int List::scrollbar_x() const { return _x + _sx - 18; }
int List::scrollbar_y() const { return _y + 8; }
int List::scrollbar_height() const { return _sy - 16; }

int List::scrollbar_thumb_height() const
{
    if (!has_scrollbar()) return scrollbar_height();
    int cnt = visible_count();
    int thumb = (cnt > 0) ? (scrollbar_height() * visible_rows() / cnt) : scrollbar_height();
    return max(28, min(scrollbar_height(), thumb));
}

int List::scrollbar_thumb_y() const
{
    if (!has_scrollbar()) return scrollbar_y();
    int travel = scrollbar_height() - scrollbar_thumb_height();
    if (travel <= 0 || max_scroll_offset() <= 0) return scrollbar_y();
    return scrollbar_y() + travel * _scroll_offset / max_scroll_offset();
}

bool List::is_on_scrollbar(int x, int y) const
{
    return has_scrollbar() && x >= scrollbar_x() - 2 && x < scrollbar_x() + 13 &&
           y >= scrollbar_y() && y < scrollbar_y() + scrollbar_height();
}

bool List::is_on_scrollbar_thumb(int x, int y) const
{
    return is_on_scrollbar(x, y) && y >= scrollbar_thumb_y() &&
           y < scrollbar_thumb_y() + scrollbar_thumb_height();
}

void List::scroll_to_thumb_position(int thumb_y)
{
    if (!has_scrollbar()) { _scroll_offset = 0; return; }
    int travel = scrollbar_height() - scrollbar_thumb_height();
    if (travel <= 0) { _scroll_offset = 0; return; }
    int relative_y = max(0, min(thumb_y - scrollbar_y(), travel));
    _scroll_offset = (relative_y * max_scroll_offset() + travel / 2) / travel;
    clamp_scroll_offset();
}
