#ifndef LIST_HPP
#define LIST_HPP

// ============================================================
// LIST.HPP - DEFAULT SABLON (A_VEC_VEK)
// ListItem: name + vector<int> values <- VALUE_DIMENSION
//
// legtöbb feladatra alkalmas alap
//
// CSERE HA:
//   → B_map_int: ha készletszerű (neve + mérete + darabszám van)
//   → C_map_string: ha szöveges kategória (neve + típusa)
//   → D_time: ha időpontos (neve + kezdés + tartam, átfedés kell)
// ============================================================

#include "config.hpp"
#include "widgets.hpp"
#include <string>
#include <vector>
#include <set>
#include <functional>

enum SortMode { DefaultOrder, AbcOrder, ReverseAbcOrder };

// ===== ELEM STRUKTÚRA =====
struct ListItem
{
    std::string name;
    std::vector<int> values;    // VALUE_DIMENSION darab int
    int original_order;         // belső sorszám (rendezéshez), nem kell módosítani
    int text_r, text_g, text_b; // szöveg színe (alapértelmezett: fehéres)

    ListItem() : original_order(0), text_r(235), text_g(239), text_b(255) {}
    ListItem(std::string name, std::vector<int> values, int original_order = 0,
             int text_r = 235, int text_g = 239, int text_b = 255)
        : name(name), values(values), original_order(original_order),
          text_r(text_r), text_g(text_g), text_b(text_b) {}
};

class List : public Widget
{
protected:
    std::vector<ListItem> _items;
    int _selected_index;
    int _scroll_offset;
    int _activation_order;
    int _sort_state;
    SortMode _sort_mode;
    int _next_original_order;
    bool _scrollbar_dragging;
    int _scrollbar_drag_offset;
    static int _global_activation_counter;
    std::function<void()> _on_select;

    // ===== SZŰRÉS =====
    std::string _filter_text;   // ha nem üres: csak az egyező nevű elemek látszanak

    // ===== JELÖLÉS (szín toggle) =====
    std::set<int> _marked_orders;   // original_order értékek a jelölt elemeknél

    // ===== KAPACITÁS =====
    int _capacity;      // -1 = korlátlan; egyébként max. összesúly
    int _weight_col;    // -1 = minden elem súlya 1; 0+ = values[weight_col] a súly

    // ===== ÉRTÉK SZERINTI RENDEZÉS =====
    int _value_sort_col;  // -1 = inaktív; 0+ = values[col] szerint rendez
    int _value_sort_dir;  // 0 = ki, 1 = növekvő, 2 = csökkenő

    // --- Segédfüggvények ---
    ListItem item_at_index(int index) const;
    int index_of(const std::string& name) const;
    int index_of_original_order(int original_order) const;
    void restore_selection_by_order(int original_order);
    void apply_sort_mode();
    std::string row_text(const ListItem& item) const;  // ← IDE ÍRD A SAJÁT FORMÁTUMOT!
    int visible_rows() const;
    int max_scroll_offset() const;
    void clamp_scroll_offset();
    bool has_scrollbar() const;
    int scrollbar_x() const;
    int scrollbar_y() const;
    int scrollbar_height() const;
    int scrollbar_thumb_height() const;
    int scrollbar_thumb_y() const;
    bool is_on_scrollbar(int x, int y) const;
    bool is_on_scrollbar_thumb(int x, int y) const;
    void scroll_to_thumb_position(int mouse_y);

public:
    List(Lib* lib, int x, int y, int sx, int sy);

    void draw() const override;
    void handle(genv::event ev) override;

    // ===== ALAP MŰVELETEK =====
    bool add_item(const ListItem& item);   // false = dupla nevet nem fogad el (ha tiltva) VAGY kapacitás túllépés
    void remove_selected();
    void clear();
    void clear_selection();

    // ===== LEKÉRDEZÉSEK =====
    ListItem get_selected_item() const;
    int selected_index() const;
    int item_count() const;          // összes elem (szűréstől függetlenül)
    int visible_count() const;       // szűrt, látható elemek száma
    std::vector<ListItem> items() const;
    int activation_order() const;

    // ===== MÓDOSÍTÁS =====
    void set_selected_index(int index);
    bool update_selected(const ListItem& item);   // false = sikertelen

    // ===== FEL/LE MOZGATÁS =====
    bool move_selected_up();
    bool move_selected_down();

    // ===== RENDEZÉS =====
    int cycle_name_sort();    // 0=eredeti, 1=abc, 2=fordított abc
    void set_sort_mode(SortMode mode);
    int cycle_value_sort(int col);   // col=értékoszlop; 0=ki, 1=növekvő, 2=csökkenő

    // ===== ÁTHELYEZÉS MÁSIK LISTÁBA =====
    bool move_item_to(List& other, const std::string& name);

    // ===== SZŰRÉS =====
    void set_filter(const std::string& text);   // üres = szűrés ki
    std::string filter_text() const;
    std::vector<int> visible_indices() const;   // aktuálisan látható elemek indexei _items-ben

    // ===== JELÖLÉS (szín toggle) =====
    void toggle_mark();                           // kijelölt elem jelölését kapcsolja
    void clear_marks();
    bool is_marked(int index) const;              // index _items-beli tényleges index
    std::vector<ListItem> marked_items() const;   // az összes jelölt elem

    // ===== KAPACITÁS =====
    // set_capacity(10)       → max 10 elem (mindegyik súlya 1)
    // set_capacity(100, 1)   → max súly 100, ahol a súly values[1]
    void set_capacity(int capacity, int weight_col = -1);
    bool can_add(const ListItem& item) const;
    int current_weight() const;
    int capacity() const;

    void set_on_select(std::function<void()> callback);
};

#endif
