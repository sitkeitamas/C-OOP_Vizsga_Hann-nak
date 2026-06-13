#ifndef SYNC_MODEL_HPP
#define SYNC_MODEL_HPP

#include <string>
#include <vector>

// F_sync minta – általános kétlistás adatmodell (vizsgabevihető váz).
struct SyncItem
{
    std::string name;
    int value = 0;
    bool picked = false; // false = bal, true = jobb
};

class SyncModel
{
    std::vector<SyncItem> _items;

public:
    const std::vector<SyncItem>& items() const { return _items; }

    bool validate_unique_name(const std::string& name, int skip_index = -1) const;
    bool add_item(const std::string& name, int value);
    bool modify_item(int index, const std::string& name, int value);
    bool remove_item(int index);
    bool move_item_up(int index);
    bool move_item_down(int index);
    bool move_item_to_right(int index);
    bool move_item_to_left(int index);

    int warehouse_index(int index) const;
    int stats_picked_count() const;
    int stats_picked_sum() const;
};

#endif
