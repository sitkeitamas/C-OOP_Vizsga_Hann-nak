#ifndef MODEL_HPP
#define MODEL_HPP

#include "config.hpp"
#include <string>
#include <vector>

// Egyetlen adatstruktúra – vizsgán csak a mezők jelentését és a szabályokat írd át.
struct Item
{
    std::string name;
    std::vector<int> vals;       // VALUE_DIMENSION db (készlet, méret, idő…)
    std::string tag;             // kategória
    std::string assigned;        // hozzárendelés (apply/release)
    bool picked = false;         // két listás: false=bal, true=jobb
};

class Model
{
    std::vector<Item> _items;

    void ensure_vals(Item& it) const;

public:
    const std::vector<Item>& items() const { return _items; }

    void seed_demo(DemoSeed seed);

    // --- Alap CRUD ---
    bool validate_unique_name(const std::string& name, int skip_index = -1) const;
    bool add_item(const std::string& name, int val0, int val1 = 0,
                  const std::string& tag = "");
    bool modify_item(int index, const std::string& name, int val0, int val1 = 0,
                     const std::string& tag = "");
    bool remove_item(int index);

    // --- Két listás (F_sync / rec–gép alap) ---
    bool move_item_up(int index);
    bool move_item_down(int index);
    bool move_item_to_right(int index);
    bool move_item_to_left(int index);

    // --- Hozzárendelés (opcionális) ---
    bool assign(int left_index, int right_index);
    bool release(int index);

    // --- Készlet / kiadás (fagyi, cipő C) ---
    bool can_sell(int index, int qty) const;
    bool sell(int index, int qty);
    bool restock(int index, int amount = RESTOCK_AMOUNT);
    bool restock_all(int amount = RESTOCK_AMOUNT);
    std::vector<std::string> available_names() const;
    std::string stock_display_text() const;

    // --- Kategória ---
    std::vector<std::string> all_tags() const;
    std::vector<int> indices_with_tag(const std::string& tag) const;

    // --- Idő (vals[VAL_STOCK]=kezdés, vals[VAL_SECOND]=tartam perc) ---
    bool time_overlap(int index_a, int index_b) const;

    // --- Gép lista megjelenítés: picked elemek sorokra bontva (név slot/méret) ---
    struct DisplayRow
    {
        int item_index;
        int sub_index;
        std::string text;
    };
    std::vector<DisplayRow> machine_display_rows() const;
    bool remove_machine_group(int display_row_index);

    // --- REC felvétel + berakás gépbe ---
    bool add_rec(const std::string& name, int size);
    bool insert_rec_to_machine(int rec_index, int copy_count);

    // --- Statisztika ---
    int picked_count() const;
    int picked_sum(int val_index = VAL_STOCK) const;
};

#endif
