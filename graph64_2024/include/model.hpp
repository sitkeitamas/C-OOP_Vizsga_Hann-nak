#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>

// Az egyetlen adatstruktúra – minden feladathoz elég.
// Nem használt mezőket hagyd üresen / 0-n / false-on.
struct Item
{
    std::string name;              // megjelenő név vagy jelölés (pl. "c4")
    std::vector<int> vals;         // számok: koordináták, ár, RGB, stb.
    std::string tag;               // szöveges mező: kategória, típus

    bool picked = false;           // két listás feladatnál: jobb oldalon van-e
};

#endif
