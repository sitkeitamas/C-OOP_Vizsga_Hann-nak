# graph64 MVC demók (`feature/mvc-demos` ág)

Code::Blocks projekt: `GraphicsProject.cbp`

## Cél

Bevihető **Model – View – Controller – App** réteg a tanár `genv` widgetjei fölé, plusz két mintamegoldás.

## Fájlstruktúra

| Modul | include | src | Leírás |
|-------|---------|-----|--------|
| **F_sync váz** | `sync_*` | `sync_*` | Általános kétlistás MVC (referencia) |
| **REC / gép** | `rec_gep_*` | `rec_gep_*` | Listakezelés – Gép, rac kezelés feladat |
| **Fagyi** | `icecream_*` | `icecream_*` | Készlet + kiadás + megjelenítés – fagyi A |

Régi `window.hpp/cpp` megmaradt referenciának; az új kód nem használja.

## Build (Code::Blocks)

1. Nyisd meg `GraphicsProject.cbp`-t Windows alatt (SDL2 DLL-ek a projekt mappában).
2. Válaszd a targetet:
   - **Debug** – alapértelmezett `main.cpp` → REC/gép
   - **RecGepDemo** – `main_list.cpp`
   - **IceCreamDemo** – `main_icecream.cpp`
   - **SyncDemo** – `main_sync.cpp`

## MVC szétválasztás

- **Model** – adat + szabályok (`can_sell`, `insert_to_machine`, …)
- **View** – widgetek + `refresh_*` (nincs üzleti logika)
- **Controller** – `on_*` eseménykezelők → model → view.refresh
- **App** – `Lib` leszármazott, összerakja a controllert

## Vizsgán

Ne új `Cipo` osztály — a meglévő mintákból másold az `Item`/modell mezőket és a validációkat.
