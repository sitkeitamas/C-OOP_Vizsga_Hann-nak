# MVC widget keret (bevihető fájlok)

Csak az `include/` és `src/` mappák (a `graphics.hpp` / graphicslib kivételével) – egy **újrafelhasználható MVC váz** vizsgára.

**Hanna számára részletes útmutató:** [`IMPLEMENTATION.md`](IMPLEMENTATION.md) — mit csináltunk, mit nézz meg, példafeladatok mentén.

## Fájlok

| Fájl | Szerep |
|------|--------|
| `config.hpp` | **Vizsgán leginkább ezt módosítsd** – mezők, UI panel kapcsolók, demo seed |
| `model.hpp` / `model.cpp` | Adat (`Item`) + üzleti szabályok |
| `view.hpp` / `view.cpp` | Widgetek, `refresh_*`, megjelenítés |
| `controller.hpp` / `controller.cpp` | Eseménykezelők (`on_add`, `on_sell`, …) |
| `app.hpp` / `app.cpp` | `Lib` leszármazott, összekötés |
| `util.hpp` | Segéd (sakktábla, szöveg) – opcionális |
| `record.hpp` | Perzisztencia sablon – opcionális |
| Widget fájlok | `button`, `list`, `text`, … – tanár eredeti API |

## Vizsga workflow

1. **config.hpp** – kapcsold be a feladathoz illő panelt:
   - `UI_TWO_LIST` – két listás szinkron (F_sync, csapat…)
   - `UI_INVENTORY` – készlet + kiadás + feltöltés (fagyi, cipő C)
   - `UI_MACHINE` – REC/gép: dropdown + gép lista sorokkal
   - `UI_CATEGORY` – tag mező + szűrés
   - `UI_ASSIGN` – apply / release
2. **DEMO_SEED** → `DemoSeed::None` beadáskor
3. Szükség esetén **`row_text()`** a `view.cpp`-ben, vagy model szabályok finomhangolása

## Példa beállítások

**Fagyi (A):**
```cpp
static constexpr bool UI_TWO_LIST = false;
static constexpr bool UI_INVENTORY = true;
static constexpr DemoSeed DEMO_SEED = DemoSeed::None;
```

**REC/gép:**
```cpp
static constexpr bool UI_TWO_LIST = false;
static constexpr bool UI_INVENTORY = false;
static constexpr bool UI_MACHINE = true;
static constexpr DemoSeed DEMO_SEED = DemoSeed::None;
```

**Két listás alap (F_sync):**
```cpp
static constexpr bool UI_TWO_LIST = true;
static constexpr bool UI_INVENTORY = false;
static constexpr DemoSeed DEMO_SEED = DemoSeed::None;
```

## Fordítás

Code::Blocks: `GraphicsProject.cbp` → Debug target.

Mac (SDL2 Homebrew):
```bash
cd graph64_2024
g++ -std=c++17 -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib \
  src/*.cpp graphicslib/graphics.cpp -lSDL2 -lSDL2_ttf -o bin/AppDemo
```
