# Implementáció — mit csináltunk, mit nézz Hanna

> **Célközönség:** Hanna — vizsgára bevihető `graph64_2024` keret  
> **Kapcsolódó:** `README-FRAMEWORK.md` (rövid technikai összefoglaló), `Hanna_vizsga_kontextus.md` (teljes háttér)

---

## 1. Mi volt a cél?

A tanár azt kéri, hogy vizsga **előtt** feltölts egy **általános, újrafelhasználható** fájlcsomagot (`include/` + `src/`). Vizsgán **csak ezt** használhatod, és **minimális módosítással** kell a konkrét feladathoz igazítani.

Hanna pontosítása:

> Az `include` és `src` mappában lévő fájlok (kivéve `graphics.hpp`) legyenek **MVC alapú widget-keretrendszer**. Ne legyenek feladat-specifikus osztályok (`Cipo`, `FagyiBolt` stb.). Vizsgán gyorsan lehessen összerakni: két listás kezelés, kategória, időpont, készlet…

**Amit ez jelent:** nem egy konkrét feladat megoldása van beleadva, hanem egy **belső mini-keretrendszer**, amiből feladatonként 2–4 dolgot kell átállítani.

---

## 2. Mit csináltunk? (előtte → utána)

| Előtte (rossz irány) | Utána (bevihető irány) |
|----------------------|------------------------|
| `window.hpp` — minden egy osztályban | `model` / `view` / `controller` / `app` szétválasztva |
| Külön `sync_*`, `icecream_*`, `rec_gep_*` demók | **Egy** egységes csomag |
| Feladatnevek a kódban (`Cipo`, `Szinkezelo` a `regiek/`-ben) | Általános `Item` struktúra + `config.hpp` |
| Logika a widget rajzolásában | Logika a **Model**-ben, megjelenítés a **View**-ban |

**Törölve / nem bevihető:** `window.hpp`, `window.cpp`, feladat-specifikus demo fájlok, Hanna régi `regiek/` kódja (azt **ne** másold vissza).

**Érintetlen:** tanár `graphicslib/`, `include/graphics.hpp`, alap widgetek (`button`, `list`, `text`, …).

---

## 3. A fájlok — mit nézz meg és miért

### Első körben (ezt tanuld meg fejből)

| Fájl | Mit csinál | Vizsgán |
|------|------------|---------|
| **`include/config.hpp`** | UI panelek be/ki, mezők jelentése, demo adat | **Leginkább ezt módosítod** |
| **`include/model.hpp`** + **`src/model.cpp`** | Adat (`Item`) + szabályok (CRUD, készlet, két lista, gép…) | Szabály finomhangolás, ha kell |
| **`include/view.hpp`** + **`src/view.cpp`** | Widgetek, `refresh_*`, lista szöveg (`row_text`) | Megjelenítés formátuma |
| **`include/controller.hpp`** + **`src/controller.cpp`** | Gomb/lista események → model → view frissítés | Ritkán kell hozzányúlni |
| **`include/app.hpp`** + **`src/app.cpp`** | Összerakja a controllert | Általában nem kell |
| **`src/main.cpp`** | `App app(...); app.event_loop();` | Nem kell változtatni |

### Második körben (ha a feladat kéri)

| Fájl | Mikor kell |
|------|------------|
| **`include/util.hpp`** | Huszár/sakktábla, szöveg segéd |
| **`include/record.hpp`** + **`src/record.cpp`** | Polimorf elemek (pl. fagyi C — sablonok) |
| Alap widget `.hpp` / `.cpp` | Csak ha új widget típus kell (ritka) |

### Mit **ne** nézz vizsgára

- `regiek/` — tanár ezt kritizálta (túl konkrét, rossz architektúra)
- `graphicslib/` belső részletei — tanár libje, ne módosítsd

---

## 4. Hogyan működik az MVC? (egy mondatban)

**Esemény** (gomb/lista) → **Controller** (`on_*`) → **Model** (adat + szabály) → **View** (`refresh_*`) → widgetek frissülnek.

```
[ Felhasználó kattint ]
        ↓
controller.on_sell()
        ↓
model.sell(index, qty)     ← itt dől el, hogy szabad-e
        ↓
view.refresh_all()         ← dropdown, statictext, listák újratöltése
```

**Szabály:** a View **nem** dönt (pl. nem ellenőrzi a készletet). A Controller **vékony** (ne legyen benne 50 sor logika).

---

## 5. Az adatmodell — `Item`

Minden feladat **ugyanazt** a struktúrát használja; vizsgán a mezők **jelentését** értelmezed át:

```cpp
struct Item {
    std::string name;        // név (fagyi íz, cipő név, karakter név…)
    std::vector<int> vals;   // számmezők (készlet, méret, idő…)
    std::string tag;         // kategória (klán, típus…)
    std::string assigned;    // hozzárendelés (apply/release feladatok)
    bool picked;             // két listás: false=bal, true=jobb
};
```

A `config.hpp`-ban:

- `VAL_STOCK` — melyik `vals[]` index a „fő” szám (pl. készlet)
- `VAL_SECOND` — második szám (ár, időtartam perc, másik méret…)
- `DUPLICATE_NAME_MODE` — egy név csak egyszer, vagy többször is lehet (cipő: ugyanaz a név, más méret)

---

## 6. UI panelek — `config.hpp` kapcsolók

| Kapcsoló | Mit ad a képernyőre | Model metódusok |
|----------|---------------------|-----------------|
| `UI_TWO_LIST` | Bal + jobb lista, Fel/Le, `<` `>` | `move_item_*`, CRUD |
| `UI_INVENTORY` | Dropdown (csak készleten lévők), mennyiség, Kiad/Feltölt, készletsor | `sell`, `restock`, `available_names` |
| `UI_MACHINE` | REC dropdown + darabszám + gép lista (`név 1/4`) | `add_rec`, `insert_rec_to_machine`, `remove_machine_group` |
| `UI_CATEGORY` | Tag mező + szűrő dropdown | `all_tags`, `indices_with_tag` |
| `UI_ASSIGN` | Apply / Release gombok | `assign`, `release` |

**Beadáskor mindig:**

```cpp
static constexpr DemoSeed DEMO_SEED = DemoSeed::None;
```

**Teszteléshez** (otthon): `DemoSeed::Inventory`, `TwoList`, `RecGep`.

---

## 7. Példafeladatok → hogyan használd a keretet

A `példafeladatok/` mappában lévő **leadott mintafeladatok**. A vizsga valószínűleg hasonló logikát kér, de **graph64 widgetekkel** (nem wxWidgets).

---

### 7.1 Fagylalka — `példafeladatok/leiras-1.txt`

| Feladatrész | Mit kér | Keretben mi van kész | Mit kell még vizsgán |
|-------------|---------|----------------------|----------------------|
| **A** (10p) | Kiadás, csak nem kifogyott fagyi, kifogyáskor eltűnik | `UI_INVENTORY`, `sell()`, `available_names()` | `config`: `UI_TWO_LIST=false`, `UI_INVENTORY=true` |
| **B** (10p) | Új fagyi hozzáad/töröl | `add_item`, `remove_item`, CRUD gombok | `DUPLICATE_NAME_MODE = RejectDuplicateNames` |
| **C** (25p) | Sablonok (banánhajó), csak eredeti összetevő | **Nincs kész** — polimorf/logika kell | `record.hpp` vagy extra model szabály |
| **D** (15p) | Új nap, statisztika, százalék | Részben: `restock_all`, `stock_display_text` | Statisztika számítás + reset — `model.cpp`-ben pár sor |

**Ajánlott `config.hpp` (A+B):**

```cpp
static constexpr bool UI_TWO_LIST  = false;
static constexpr bool UI_INVENTORY = true;
static constexpr bool UI_MACHINE   = false;
static constexpr DemoSeed DEMO_SEED = DemoSeed::None;
```

**Tesztelted:** fut, kb. jó — apró UI finomítások nem kritikusak vizsgára.

**Hova nézz:** `controller.on_sell()` → `model.sell()` → `view.refresh_inventory()`.

---

### 7.2 Cipőbolt készlet — `példafeladatok/leiras.txt`

| Feladatrész | Mit kér | Keret illeszkedés |
|-------------|---------|-------------------|
| **A** | Név egyszer, méretek a név mellett | `DUPLICATE_NAME_MODE = AllowDuplicateNames`; `name`+`vals[0]`=méret |
| **B** | Szerkesztés, méretenkénti törlés | CRUD + `remove_item` — szűkítés: csak egy méret törlése külön metódus kell |
| **C** | Készletszám, „Elkelt”, 0-nál törlés | `UI_INVENTORY` + `sell()` után ha `vals[0]==0` → `remove_item` |
| **D** | Szűrés cipőméretre, „Mind” | `UI_CATEGORY` vagy külön dropdown + `indices_with_tag` / szűrő a view-ban |

**Ajánlott kiindulás:**

```cpp
static constexpr DuplicateNameMode DUPLICATE_NAME_MODE = AllowDuplicateNames;
static constexpr bool UI_INVENTORY = true;
static constexpr bool UI_TWO_LIST = false;
// vals[0] = készlet, vals[1] = cipőméret (címke a listában row_text-ben)
```

**Vizsgán valószínűleg:** `row_text()` és `sell()` utáni automatikus törlés átírása a `model.cpp`-ben.

---

### 7.3 Gép / REC kezelés — `példafeladatok/Gép, rac kezelés.txt`

| Mit kér | Keretben |
|---------|----------|
| Rec lista: név + méret | `add_rec(name, size)` — bal oldali logika |
| Gépbe berakás darabszámmal | `insert_rec_to_machine()` |
| Megjelenés: `nev1 1/4` | `machine_display_rows()` + `view.refresh_machine()` |
| Kivétel: egy sor → az elem **összes** sora törlődik | `remove_machine_group()` |

**Ajánlott `config.hpp`:**

```cpp
static constexpr bool UI_TWO_LIST  = false;
static constexpr bool UI_INVENTORY = false;
static constexpr bool UI_MACHINE   = true;
static constexpr DemoSeed DEMO_SEED = DemoSeed::None;
```

**Hova nézz:** `model.cpp` — `insert_rec_to_machine`, `machine_display_rows`, `remove_machine_group`.

---

### 7.4 Karakter + klán — `példafeladatok/feladat.txt`

| Feladatrész | Minta | Keret |
|-------------|-------|-------|
| **A** | Skillpontok +/-, szabad pontok | `SetValue` + model validáció (még nincs külön metódus — vizsgán `model.cpp`) |
| **B** | Csapatlista, törlés, visszatöltés szerkesztésre | `UI_TWO_LIST` + `on_list_select` → `fill_inputs_from_selection` |
| **C** | Saját klánok (név + követelmények) | `UI_CATEGORY` + `tag` mező |
| **D** | Mentés/betöltés | `record.hpp` / külön `storage` — **nincs kész** |

**Ajánlott kiindulás (B):**

```cpp
static constexpr bool UI_TWO_LIST = true;
static constexpr bool UI_INVENTORY = false;
static constexpr bool UI_ASSIGN = false;  // vagy true, ha „hozzárendelés” jellegű
```

---

### 7.5 Huszár bábu lépései — `példafeladatok/Huszár bábu lépései.txt`

| Mit kér | Keret |
|---------|-------|
| Lépés rögzítés (X,Y → `a2`) | `add_item` + `util.hpp` `chess_notation` |
| LÉP gomb → aktuális pozíció | model metódus + controller `on_*` |
| Szűrés: csak huszár-lépések | `chess_is_knight_move` a `util.hpp`-ban |
| Új lépés → szűrés megszűnik | view `refresh` + filter flag a modelben |

**UI:** inkább **egy lista** + input mezők; két listás panel nem kell.

```cpp
static constexpr bool UI_TWO_LIST = false;
static constexpr bool UI_INVENTORY = false;
```

**Vizsgán:** új szűrési logika a `model.cpp`-ben, megjelenítés a `view.cpp`-ben — a keret vázat adja, a sakkspecifikus rész **hozzáírás**.

---

## 8. Vizsga napján — lépésről lépésre

1. **Olvasd el a feladatot** — melyik minta? (két lista / készlet / gép / kategória / idő / fájl)
2. **`config.hpp`** — kapcsold be a megfelelő `UI_*` paneleket, `DEMO_SEED = None`
3. **`Item` mezők jelentése** — kommenteld át magadnak (`vals[0]` = készlet vagy méret?)
4. **`view.cpp` → `row_text()`** — lista/dropdown szöveg a feladat szerint
5. **`model.cpp`** — csak a **eltérő szabályok** (pl. cipő: 0 készlet → törlés; fagyi C: sablon)
6. **Fordítás** Code::Blocks-ban (`GraphicsProject.cbp` → Debug)
7. **Ne nevezz** feladat-specifikus osztályt — maradj `Item` + config

---

## 9. Mi van már kész vs. mit kell még hozzáírni

| Kész a keretben | Vizsgán gyakran még kell |
|-----------------|--------------------------|
| MVC váz, event loop | Konkrét validációs szabályok |
| Két listás CRUD + mozgatás | Kapacitás / speciális feltétel jobbra lépésnél |
| Készlet kiadás + feltöltés | Automatikus törlés 0-nál |
| REC/gép sorok | — |
| Kategória mező + tag listázás | Szűrés a listán |
| `time_overlap()` váz | Időpontos feladat UI |
| `assign` / `release` | Orvos–műtét jellegű feladat |
| Demo seed otthoni teszthez | **D feladat:** fájl mentés/betöltés |
| | **Fagyi C:** polimorf sablonok (`record.hpp`) |
| | **Statisztika / százalék** (fagyi D) |

---

## 10. Gyors ellenőrzőlista (tanár szempontjából)

- [ ] Nincs `class Cipo` / `class Fagyi` / hasonló a bevitt kódban
- [ ] Adatmódosítás után mindig `view.refresh_*` hívódik
- [ ] Üzleti szabály a **Model**-ben van, nem a View-ban
- [ ] `config.hpp`-ban `DEMO_SEED = None` beadáskor
- [ ] Egy helyen van az adat: `Model::_items`
- [ ] Lista szöveg egy helyen: `row_text()` (vagy egy `refresh` metódus)

---

## 11. Futtatás

**Windows (vizsga):** Code::Blocks → `GraphicsProject.cbp` → Debug → Run.

**Mac (otthon, SDL2 Homebrew):**

```bash
cd graph64_2024
g++ -std=c++17 -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib \
  src/*.cpp graphicslib/graphics.cpp -lSDL2 -lSDL2_ttf -o bin/AppDemo
./bin/AppDemo
```

---

## 12. Ha elakadsz vizsgán

1. **Ne írj új osztályt** — keress meglévő `model.*` metódust vagy másold egy `on_*` mintát
2. **Státusz sor** — `view.set_status("...")` a controllerben hibánál (már benne van)
3. **Csak a feladathoz kellő panel** — kapcsold ki a többi `UI_*`-ot a `config.hpp`-ban (kevesebb zavar)
4. **A régi `regiek/` kód nem minta** — csak ötletadó, architektúrára ne építs

---

*Utolsó frissítés: 2026.06. — egyesített MVC keret (`feature/mvc-demos` branch)*
