#include "record.hpp"

using namespace std;

// ============================================================
// RECORD.CPP - Collection implementáció
//
// FONTOS: az abstract Record osztályt nem lehet másolni,
// ezért csak az unique_ptr<Record> alapú add() működik.
// A del() + add() kombináció helyettesíti a modify()-t.
// ============================================================

bool Collection::add(std::unique_ptr<Record> record)
{
    if (!record) return false;
    _records.push_back(std::move(record));
    return true;
}

void Collection::del(int index)
{
    if (index >= 0 && index < (int)_records.size())
        _records.erase(_records.begin() + index);
}

// Módosítás: del(index) + add(make_unique<XRecord>(...))
// Nincs önálló modify(), mert abstract osztályt nem lehet másolni.

vector<string> Collection::titles() const
{
    vector<string> result;
    for (const auto& r : _records) result.push_back(r->title());
    return result;
}

const Record* Collection::at(int index) const
{
    if (index < 0 || index >= (int)_records.size()) return nullptr;
    return _records[index].get();
}

int Collection::size() const { return (int)_records.size(); }
