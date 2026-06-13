#ifndef REC_GEP_MODEL_HPP
#define REC_GEP_MODEL_HPP

#include <string>
#include <vector>

// REC / gép listakezelés – Minta A + H (példafeladat: Gép, rac kezelés).
struct RecEntry
{
    std::string name;
    int size = 1; // megjelenítésben a nevező (pl. 1/4)
};

struct MachineCopy
{
    std::string name;
    int size = 1;
};

struct MachineDisplayRow
{
    int copy_index = -1;
    int slot = 0;
    std::string text;
};

class RecGepModel
{
    std::vector<RecEntry> _rec;
    std::vector<MachineCopy> _machine;

public:
    const std::vector<RecEntry>& rec_entries() const { return _rec; }

    bool add_rec(const std::string& name, int size);
    bool insert_to_machine(int rec_index, int copy_count);
    bool remove_machine_row(int display_row_index);

    std::vector<std::string> rec_labels() const;
    std::vector<MachineDisplayRow> machine_rows() const;
    int selected_rec_index(const std::string& label) const;
};

#endif
