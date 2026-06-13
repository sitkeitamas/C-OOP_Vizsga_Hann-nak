#ifndef RECORD_HPP
#define RECORD_HPP

// ============================================================
// RECORD.HPP - Polimorfizmus sablon
//
// Ezt akkor kell módosítani, ha a feladat azt kéri, hogy
// különböző típusú elemeket kezeljünk egységes interfészen.
//
// Példa: Ital (vodka, gin, ...) mindnek van neve és ára,
//        de eltérő extra mezőik vannak.
//
// TIPIKUS TEENDŐK:
// 1. A Record az érinthetetlen alap (virtual = 0 metódusok)
// 2. A konkrét osztályok (pl. VodkaRecord) öröklik a Recordot
// 3. A gyűjtemény (pl. Cocktail) unique_ptr<Record> vektort kezel
// ============================================================

#include <memory>
#include <string>
#include <vector>

// ===== ABSZTRAKT ALAP =====
class Record {
public:
    virtual ~Record() = default;
    virtual std::string title() const = 0;              // listában látható szöveg
    virtual std::vector<int> values() const { return {}; }  // int értékek (opcionális)
    virtual std::string name() const { return title(); }    // rövid azonosító
};

// ===== PÉLDA KONKRÉT OSZTÁLY =====
// // TODO: nevezd át és töltsd ki a feladat szerint!
class SampleRecord : public Record {
    std::string _name;
    int _value1;
    int _value2;
public:
    SampleRecord(std::string name, int value1, int value2)
        : _name(name), _value1(value1), _value2(value2) {}

    std::string title() const override
    {
        return _name + " | " + std::to_string(_value1) + " | " + std::to_string(_value2);
    }

    std::vector<int> values() const override
    {
        return {_value1, _value2};
    }

    std::string name() const override { return _name; }

    // Extra getterek a konkrét adatokhoz:
    int value1() const { return _value1; }
    int value2() const { return _value2; }
};

// ===== GYŰJTEMÉNY OSZTÁLY =====
class Collection {
    std::vector<std::unique_ptr<Record>> _records;
public:
    // Hozzáadás: col.add(std::make_unique<SampleRecord>("alma", 5, 3));
    bool add(std::unique_ptr<Record> record);
    void del(int index);
    // Módosítás: del(index) + add(make_unique<...>(...))
    // (abstract osztályt nem lehet másolni, nincs modify())

    std::vector<std::string> titles() const;
    const Record* at(int index) const;
    int size() const;
};

#endif
