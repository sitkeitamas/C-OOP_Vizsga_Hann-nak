# Hanna C++ OOP vizsga — kontextus és eddigi következtetések

> Források:
> - [ChatGPT beszélgetés](https://chatgpt.com/share/6a2d4511-bc4c-83eb-a8b6-1f805c2adb33) (2026.06., telefonbeszélgetés után)
> - `graph64_2024/` — tanár library + vizsgabevihető framework + Hanna korábbi próbálkozásai
> - `példafeladatok/` — minta BevProg2 géptermi ZH leírások
>
> **Státusz:** A library azonosítva (`graph64_2024`). Az `include/` + `src/` mappa **egyesített MVC keretre** lett rendezve (`model` / `view` / `controller` / `app`). A **konkrét vizsgafeladat szövege** még **nem ismert**.

---

## Helyzet röviden

Hanna nem tud egyedül felkészülni a vizsgára. A feladat lényegét nehezen tudja elmagyarázni; segítséget kér a felkészüléshez. A vizsga valószínűleg **nem a C++ szintaxisa** miatt akad el, hanem az **absztrakció**, a **MVC**, és a **vizsgabevihető keretrendszer** miatt.

---

## Amit eddig tudunk

| Téma | Részlet |
|------|---------|
| **Tárgy jellege** | BevProg2-s stílusú géptermi ZH: widget-alapú GUI, adatkezelés, A–B–C–D feladatrészek |
| **Library neve** | **`graph64_2024`** — PPKE/Flugi-féle `genv::gout` / `genv::gin` API, SDL2, Code::Blocks |
| **Vizsga szabály** | Vizsga előtt **fájl-szettet** kell feltölteni/bevinni; a vizsgán **csak ezt** használhatják |
| **Fájltípusok** | `include/` (.h, .hpp) + `src/` (.cpp) felosztás |
| **MVC** | Elvárás: adat ↔ megjelenítés ↔ interakció szétválasztása |
| **Tanár kritikája** | Hanna beadott fájljai **túl gyakorlófeladat-specifikusak**, nem elég **általánosak** |
| **Hanna állapota** | Nem tudta elvégezni a vizsgát; a `regiek/` mappában vannak sikertelen próbálkozásai |
| **Jelenlegi sablon** | `include/` + `src/` — **egyesített MVC widget keret** (`config.hpp` + model/view/controller/app) |

---

## Hanna pontosítása (2026.06.)

> Az `include` és `src` mappában lévő fájlokat (kivéve `graphics.hpp`, mert az a grafikus lib része) **úgy kell átalakítani**, hogy egy **újrafelhasználható MVC alapú widget-keretrendszert** alkossanak. Vizsgán **minimális módosításokkal** lehessen gyorsan működő alkalmazást építeni (két listás készletkezelés, kategória- vagy időpont alapú adatkezelés stb.). **Csak e két mappában** kell dolgozni.

**Megvalósítás:** nincs külön `sync_*` / `icecream_*` / `rec_gep_*` / `window.*` — helyette egy bevihető csomag:

| Fájl | Szerep |
|------|--------|
| `config.hpp` | UI panel kapcsolók (`UI_TWO_LIST`, `UI_INVENTORY`, `UI_MACHINE`, …) + demo seed |
| `model.hpp` / `model.cpp` | `Item` + összes üzleti szabály |
| `view.hpp` / `view.cpp` | Widgetek + `refresh_*` |
| `controller.hpp` / `controller.cpp` | `on_*` eseménykezelők |
| `app.hpp` / `app.cpp` + `main.cpp` | `Lib` leszármazott, belépési pont |

Részletek: `graph64_2024/README-FRAMEWORK.md`. **Hanna implementációs útmutató:** `graph64_2024/IMPLEMENTATION.md`.

---

## A `graph64_2024` projekt felépítése

| Mappa / fájl | Szerep |
|--------------|--------|
| `graphicslib/` | Tanár **graphics.hpp** — alap rajzolás, események (`gout`, `gin`, SDL2) |
| `include/` + `src/` | **Bevihető MVC keret**: widgetek + `model` / `view` / `controller` / `app` |
| `include/config.hpp` | **Vizsgán fő testreszabási pont** — mezők, UI panelek, demo seed |
| `include/model.hpp` + `src/model.cpp` | `Item` + CRUD, két lista, készlet, gép, kategória, idő |
| `include/view.hpp` + `src/view.cpp` | Megjelenítés, `refresh_*`, `row_text()` |
| `include/controller.hpp` + `src/controller.cpp` | Eseménykezelők |
| `include/app.hpp` + `src/app.cpp` | Alkalmazás összerakás |
| `include/record.hpp` | Polimorfizmus sablon (`Record`, `Collection`) — összetett feladatokhoz |
| `include/util.hpp` | Segédfüggvények, pl. sakktábla/huszár (`chess_is_knight_move`) |
| `regiek/` | Hanna **korábbi, sikertelen** próbálkozásai (régi API, feladat-specifikus osztályok) |
| `Fonts/`, `SDL2.dll`, `lib/` | Futáshoz szükséges erőforrások (Code::Blocks / Windows) |

### Vizsgamodell (a kód alapján)

1. **Előre** feltöltesz egy **általános keretrendszert** (widgetek + minták).
2. **Vizsgán** csak ezt használod → a konkrét feladathoz a `config.hpp`-t és esetleg a model/view szabályokat igazítod.
3. A hallgató **nem SDL-t ír**, hanem widgeteket rak össze a tanár API-ján.

---

## A jelenlegi framework — egyesített MVC

A `Model` osztály **feladat-független**, egy adatforrás: `std::vector<Item> _items`.

- **View:** csak widgetek + `refresh_*` (nincs üzleti logika)
- **Controller:** vékony `on_*` metódusok → model → view.refresh
- **config.hpp:** mely UI panel aktív (két lista, készlet, gép, kategória, assign)

Támogatott minták egy csomagban:

| Panel (`config.hpp`) | Feladattípus |
|----------------------|--------------|
| `UI_TWO_LIST` | F_sync, csapat, bal↔jobb lista |
| `UI_INVENTORY` | Fagyi/cipő készlet, kiadás, feltöltés |
| `UI_MACHINE` | REC/gép: dropdown + sorok (név slot/méret) |
| `UI_CATEGORY` | `tag` mező + szűrés |
| `UI_ASSIGN` | apply / release hozzárendelés |

### Testreszabási pontok (vizsgán)

| Hol | Mit |
|-----|-----|
| `config.hpp` | UI panelek, `VALUE_DIMENSION`, `VAL_STOCK`/`VAL_SECOND` jelentés, demo seed → `None` |
| `model.hpp` — `Item` | Mezők jelentése (`vals[]`, `tag`, `assigned`, `picked`) |
| `view.cpp` — `row_text()` | Lista/dropdown szöveg formátuma |
| `model.cpp` | Validáció, `move_to_right` feltételek, `time_overlap()` |

---

## Hanna korábbi próbálkozásai (`regiek/`)

### 1. `cipo.hpp` / `cipo.cpp` — cipőbolt gyakorló

**Jó irány:** név + méret, duplikált névnél méret hozzáadása, lista frissítés.

**Problémák:**
- Egy **`Cipo` osztály örökli a `Widget`-et** és belül tartja az egész UI-t → **feladat-specifikus monolit**, nem általános keret
- A régi `Window` csak **egy widgetet** kezel (`widget[0]->handle/draw`) — nem valódi event loop
- `delete_cipo()` **egész cipőt töröl**, nem csak egy méretet (a példafeladat szerint méretenként kellene)
- `show_warning()` közvetlenül `gout`-ra ír — nem status label

### 2. `szinkezelo.hpp` / `szinkezelo.cpp` — színkezelő (félkész)

**Súlyos hibák:**
- Minden widget **ugyanarra a koordinátára** van rakva
- `use()` **befejezetlen**, szintaktikailag hibás: `szoveg->setColor(text, background)`
- RGB feleslegesen `string`-ként tárolva
- Két külön lista (`eloter`, `hatter`), de **ugyanazt a `szin` vektort** mutatják — nincs szétválasztás
- Copy-paste hibák (pl. „Nincs kijelölt **cipő**” színkezelőnél)

### 3. Régi `regiek/window.cpp`

Csak `widget[0]`-t hív — **nem működő alkalmazás-keret**. Pont az ellenkezője annak, amit a tanár vár.

### Összefoglaló: miért mondta a tanár, hogy „túl gyakorlófeladat-specifikus”?

| Tanár elvárása | Hanna régi kódja |
|----------------|------------------|
| `Shape`, `Scene`, `List` minták | `Cipo`, `Szinkezelo` — konkrét feladatnevek |
| Adat ↔ view szétválasztás (`refresh_lists`) | Logika + widget egy osztályban |
| Újrahasznosítható `Window` sablon | Minden feladathoz új „Widget-gyerek” |
| MVC-s gondolkodás | Nincs Model/View/Controller réteg |

A **`graph64_2024/include/` + `src/`** mappa viszont **pont azt csinálja, amit a tanár kér** — valószínűleg tanár vagy segítő **javított sablonja**, nem Hanna saját munkája.

---

## Technikai megállapítások (jelenlegi kód)

### Fordítás

A régi `window.hpp` / `window.cpp` **eltávolítva** — helyette `model/view/controller/app`. Mac-en tesztelve: `bin/AppDemo` (SDL2).

### Egyetlen `Item` definíció

`model.hpp`: `name`, `vals[]`, `tag`, `assigned`, `picked` — minden réteg ezt használja.

### API inkompatibilitás

A `regiek/` más API-t használ (`Window*` vs `Lib*`, `Display` vs `List`, `get_text()` vs `text()`) — **nem kompatibilis** az új frameworkkel. Ne keverjük össze a két verziót.

### Git / build megjegyzés

A `.gitignore` kiszűri a `*.a` fájlokat → a `graph64_2024/lib/` statikus library-k **nincsenek a repóban**. Code::Blocks alatt linkeléskor lokálisan szükségesek lehetnek.

---

## Mit jelent az include / source felosztás?

- **Header (.h, .hpp)** → *mit tud a kód?* (deklaráció, interfész)
- **Source (.cpp)** → *hogyan csinálja?* (implementáció)
- Az `#include` nem „betölt”, hanem **szövegesen bemásol** a fordítás idején
- Célok: gyorsabb fordítás, modularitás, encapsulation

---

## Mi a valószínű vizsgamodell? (ChatGPT + kód összevetés)

### 1. Tanár API-ja

`genv::gout`, `genv::gin`, widgetek: `Button`, `Text`, `SetValue`, `List`, `Dropdown`, `StaticText`.

Analógia:
- **Java:** `JPanel`, `paint()`, `ActionListener`
- **wxWidgets / BevProg2:** ugyanaz a feladatlogika, más widget API

### 2. Bevihető „framework”, nem konkrét megoldás

**Rossz:**
```cpp
class SnakeGame {};
class Homework3Button {};
class Cipo {};        // Hanna régi kódja
class Szinkezelo {};  // Hanna régi kódja
```

**Jó:**
```cpp
class Widget {};
class List {};
class Window : public Lib {};  // F_sync minta
// + Item adatstruktúra, refresh_lists()
```

### 3. MVC elvárás

**Rossz:** logika a View-ban (`draw()`-ban `if (player.hp < 10)`).

**Jó:** modell dönt, view csak rajzol (`view.render(model)`).

### 4. Klasszikus OOP-csapda

> **Nem az számít, hogy működik-e, hanem hogy a következő feladatból mennyit lehet újrahasználni.**

Hanna valószínűleg meg tudta oldani a gyakorlókat, de vizsgán kiderült: **konkrét megoldásokat** épített, nem **komponenseket**.

---

## Példafeladatok — részletes összefoglaló

Közös minta: **widget-alapú géptermi ZH**, A–B–C–D feladatrészek; ahol ki van mondva: **A alapfeladat nélkül a vizsga érvénytelen**.

A vizsga **nagy valószínűséggel** ilyen feladatok egyike (vagy hasonló), de a **wxWidgets helyett `graph64` widgetjeivel**.

### 1. `feladat.txt` — Karakter + klán (2018.06.28)

| Rész | Pont | Lényeg |
|------|------|--------|
| **A** | 10p | Skillpontok +/-; szabad pontok helyes kezelése; klánlista frissítése |
| **B** | 20p | Karakterek csapatlistába; törlés; szerkesztésre visszatöltés |
| **C** | 20p | Saját klánok (név + minimumkövetelmények) |
| **D** | 10p | Mentés/betöltés (3+3+4p A/B/C lefedettség szerint) |

### 2. `leiras.txt` — Cipőbolt készlet (2017)

| Rész | Pont | Lényeg |
|------|------|--------|
| **A** | 10p | Cipők listába; név egyszer; méretek a név mellett; törlésnél minden méret |
| **B** | 10p | Szerkesztés: méretenkénti törlés |
| **C** | 20p | Készletszám; „Elkelt” gomb; 0-nál méret/név törlődik |
| **D** | 20p | Szűrés cipőméretre; „Mind” opció |

→ Hanna `regiek/cipo.cpp` kódja **ehhez a feladathoz** közelít, de rossz absztrakciós szinten.

### 3. `leiras-1.txt` — Fagylalka (2017)

| Rész | Pont | Lényeg |
|------|------|--------|
| **A** | 10p | Kiadás számbeállítóval; csak nem kifogyott fagyik; kifogyáskor elem eltűnik |
| **B** | 10p | Új fagyi hozzáadása/törlése |
| **C** | 25p | Sablonok (pl. banánhajó); csak eredeti fagyi összetevő |
| **D** | 15p | „Új nap”; statisztika; százalékos arányok; reset |

### 4. `Gép, rac kezelés.txt` — Rövid gyakorló

- Két lista: **rec** és **gép**
- Rec-be: név + méret; gépbe berakás darabszámmal
- Megjelenés: `nev1 1/4`, `nev2 2/4` (méret = sorok száma)
- Kivétel: egy sor kijelölése → az adott elem **összes** sora törlődik

→ Jó illeszkedés az **F_sync kétlistás mintához** + kapacitás/súly logika (`List::set_capacity`).

### 5. `Huszár bábu lépései.txt` — Rövid gyakorló

- Új lépés X, Y → listában pl. `a2`, `d6`
- LÉP gomb → aktuális pozíció
- Aktuális pozíció **szűrő**: csak huszár-szerű lépések
- Új lépés felvétele → szűrés megszűnik

→ `util.hpp` sakktábla segédfüggvényei **erre a feladattípusra** készültek.

---

## Tipikus hibák, amire figyelni kell

- View-ban van üzleti logika (Controller helyett View)
- Osztálynevek túl konkrétak (`Cipo`, `Szinkezelo` helyett általános `Item` + `Window`)
- Feladat-specifikus Widget-gyerek az egész UI-val (`regiek/` minta)
- Hiányzó `refresh_lists()` — adat és megjelenítés összekeverése
- Utility kód szétszórva konkrét osztályokban
- Nem MVC-s szétválasztás

---

## Amit még **ki kell deríteni** (prioritás)

1. **Konkrét vizsgafeladat** szövege (melyik példafeladathoz hasonló, vagy teljesen más?)
2. **Tanár szóbeli visszajelzése** írásban / emlékezetből
3. **Meddig jutott el** Hanna a vizsgán, hol akadt el
4. A `graph64_2024` sablont **ki adta** (tanár / segítő / Hanna)?
5. **Tárgy pontos neve** és kurzus (BevProg2? Grafika? OOP külön?)

---

## Következő lépések

1. Hanna írja össze: **konkrét feladat**, eddigi kód, hol akadt el, tanár visszajelzése
2. Javítani a **fordítási hibát** (`apply`/`release` deklaráció)
3. A megfelelő példafeladat alapján **testreszabni** a F_sync sablont (`Item`, `refresh_lists`, validációk)
4. **Ne** a `regiek/` mintát követni — az a tanár által kritizált megközelítés

---

## Nyitott kérdések

- Melyik példafeladat (vagy kombináció) volt / lesz a vizsgán?
- Van-e **fájlkezelés** (D feladat) követelmény?
- Kell-e **polimorfizmus** (`record.hpp`) vagy elég az egyszerű `Item`?
- Kell-e **Dropdown** / **Checkbox**, vagy elég List + Button + SetValue?

---

## Mit tud a `graph64` library?

A **`graph64_2024`** a tanár **BevProg2-s grafikus csomagja** (ITK *Libgraphics*, Flugi-féle `genv` API, SDL2). Hanna **nem ezt írja meg** vizsgán — **erre** épít egy **bevihető, MVC-szerű réteget**, amiből bármelyik ZH-feladatot gyorsan összerakja.

### Három szint

```
┌─────────────────────────────────────────────────┐
│  Vizsgafeladat (cipő, fagyi, gép/rec, huszár…)   │  ← csak vizsgán: Item mezők + szabályok
├─────────────────────────────────────────────────┤
│  Hanna MVC / keret (Model, View, Controller, App)│  ← ezt viszi be, általános legyen
├─────────────────────────────────────────────────┤
│  Widget framework (Lib, Button, List, …)           │  ← már a repóban (include/ + src/)
├─────────────────────────────────────────────────┤
│  Tanár library: genv (gout, gin, SDL2)           │  ← graphicslib/, nem módosítandó
└─────────────────────────────────────────────────┘
```

### 1. szint — tanár API (`genv`)

Alacsony szintű rajzolás + események. **Vizsgán közvetlenül nem kell használni** — a widgetek belsejében fut.

| API | Mit csinál |
|-----|------------|
| `gout.open(w, h)` | Ablak megnyitása |
| `gout << color(r,g,b) << move_to(x,y) << box / line / text` | Rajzolás |
| `gout << font("LiberationSans-Regular.ttf", 16)` | Betűtípus |
| `gout << refresh` | Képernyő frissítés |
| `gin >> ev` | Esemény olvasás (billentyű, egér, timer) |
| `ev.type` | `ev_key`, `ev_mouse`, `ev_timer` |
| `ev.keycode`, `ev.pos_x/y`, `ev.button` | Billentyű / egér adat |
| `canvas`, `stamp` | Off-screen rajzolás (haladó) |

**Nincs benne:** gomb, lista, számbeállító — csak „vászon + egér + billentyű”.

### 2. szint — widget framework (bevihető alap)

A repó `include/` + `src/` mappája. Minden widget a `Lib`-re regisztrálódik; az `application.cpp` futtatja az **event loop**-ot.

**Alap:** `Lib` (ablak + widgetlista + event loop), `Widget` (ős: `draw()`, `handle(ev)`).

**Input widgetek:**

| Widget | Mit tud |
|--------|---------|
| `Button` | Felirat + callback kattintásra |
| `Text` | Egysoros szövegbevitel (fókusz, max hossz) |
| `SetValue` | Szám +/- gombokkal, min–max tartomány |
| `CheckBox` | Be/ki kapcsoló |
| `Dropdown` | Lenyíló választó; nyitott állapot prioritást kap |

**Megjelenítő widgetek:**

| Widget | Mit tud |
|--------|---------|
| `StaticText` | Címke / státusz / figyelmeztetés; színezhető |
| `List` | Görgethető, kijelölhető lista — **a legerősebb eszköz** |

**`List` extra képességei:** szűrés név szerint, rendezés, fel/le mozgatás, másik listába áthelyezés, kapacitás/súly (`set_capacity`), jelölés színnel, `set_on_select` callback, `ListItem` = `name` + `vector<int> values`.

**Event loop (`Lib::event_loop`):** háttér törlés → esemény → widget `handle` → minden widget `draw` → refresh. Ctrl+bal egér: widget húzás. Nyitott `Dropdown`: minden egéreseményt ő kap.

**Mit nem tud a graph64:** beépített MVC, fájlkezelés, automatikus adatkötés — ezek Hanna rétegébe kerülnek.

### 3. szint — amit Hanna épít (MVC)

A jelenlegi `Window` + F_sync minta **közelít** ehhez, de még keveri a rétegeket. Vizsgabevitelhez érdemes szétbontani:

| Réteg | Feladat | Mit **nem** csinálhat |
|-------|---------|----------------------|
| **Model** | Adat + üzleti szabályok | Widgetet nem ismer |
| **View** | Megjelenítés, `refresh` | Döntést nem hoz (nincs `if` szabály a `draw`-ban) |
| **Controller** | Esemény → model → view refresh | Nem tárol tartós adatot |
| **App** (`Lib` leszármazott) | Layout, widgetek, callback bekötés | Nem tartalmaz feladat-specifikus logikát |

**Alapelv:** *egy helyen legyen az adat, minden widget csak tükrözze.*

```
Esemény (gomb / kattintás)
    → Controller
        → Model (adat + szabály)
            → View.refresh (listák, címkék, dropdown)
```

---

## Javasolt bevihető architektúra (`include/` + `src/`)

Hanna **nem egy feladatot** visz be, hanem egy **kis belső keretrendszert**. Vizsgán csak az adatmodell mezői és a szabályok változnak.

### `include/` — fájlok és szerepük

**Platform (tanár + alap widgetek) — változtatás nélkül vagy minimálisan:**

| Fájl | Szerep |
|------|--------|
| `graphics.hpp`, `graphicslib/` | Tanár `genv` API |
| `widgets.hpp`, `button.hpp`, `text.hpp`, `setvalue.hpp`, `statictext.hpp`, `list.hpp`, `dropdown.hpp`, `checkbox.hpp` | Alap widgetek |
| `application.hpp` | `Lib` + event loop |
| `config.hpp` | Feladatfüggő konstansok (`VALUE_DIMENSION`, dupla név tiltás, stb.) |
| `util.hpp` | Általános segéd (pl. sakktábla — csak ha kell) |

**Hanna saját, bevihető réteg (ez a lényeg):**

| Fájl | Szerep |
|------|--------|
| **`model.hpp`** | `Item` (vagy `Record`) + **üzleti szabályok** deklarációi |
| **`view.hpp`** | Widget-hivatkozások + **csak megjelenítés** metódusok |
| **`controller.hpp`** | Eseménykezelő metódusok (vékony réteg) |
| **`app.hpp`** | `class App : public Lib` — összerakás |
| **`record.hpp`** *(opc.)* | Polimorf elemek (fagyi sablon, összetett C) |
| **`storage.hpp`** *(opc.)* | Mentés/betöltés (D feladat) |

A `window.hpp` / `window.cpp` helyett most **`model/view/controller/app`** van — ez a bevihető csomag.

### `src/` — implementációk

| Fájl | Mit implementál |
|------|------------------|
| Meglévő widget `.cpp`-k, `application.cpp` | Platform |
| **`model.cpp`** | CRUD, validáció, szűrés, kapacitás, hozzárendelés |
| **`view.cpp`** | `refresh_*`, `fill_from_selection`, státusz szövegek |
| **`controller.cpp`** | `on_*` metódusok — csak összekötés |
| **`app.cpp`** | Layout, gombok callbackjei |
| **`storage.cpp`** *(opc.)* | `save()` / `load()` |
| **`main.cpp`** | Csak: `App app(800,600); app.event_loop();` |

### Mit **ne** vigyen be

| Kerülendő | Helyette |
|-----------|----------|
| `class Cipo`, `class FagyiBolt`, `class Szinkezelo` | Általános `Item` + `config.hpp` |
| Widget-gyerek az egész UI-val (`regiek/`) | `App` + külön `View` |
| Logika `draw()` / listában | `model.can_*()`, `model.apply_*()` |
| Minden feladathoz új fájlstruktúra | Ugyanaz az App váz, vizsgán csak model szabályok |

---

## Absztrakt eljárások (minták) — MVC szinteken

Ezek **konkrét metódusnevek javaslatok** — nem kell mindet implementálni, de a bevihető csomag ezekből épül fel. Vizsgán a feladathoz **2–4 minta** kombinációja elegendő.

Minden mintánál ugyanaz a séma:

- **Model:** adat + szabály
- **View:** `refresh_*` (widgetek frissítése a model alapján)
- **Controller:** `on_*` (esemény → model → view.refresh)

---

### Minta A — `F_sync` (két listás alap)

**Mire jó:** gép/rec, sok A–B feladat, csapatlista, raktár ↔ kiválasztott.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `items()` · `add_item()` · `modify_item()` · `remove_item()` · `move_item_up()` · `move_item_down()` · `move_item_to_right()` · `move_item_to_left()` · `find_by_name()` · `validate_unique_name()` |
| **View** | `refresh_lists()` · `row_text(item)` · `update_stats()` · `set_status(message)` |
| **Controller** | `on_add()` · `on_modify()` · `on_delete()` · `on_move_up()` · `on_move_down()` · `on_move_right()` · `on_move_left()` |

**Adat:** `Item` mezők: `name`, `values[]`, `picked` (bal=false / jobb=true).

**Szabály példák:** dupla név tiltás; csak bal listáról módosítható; jobbra csak ha feltétel teljesül.

**Mintafeladatok:** Gép/rec kezelés · Karakter csapat (B) · F_sync sablon a repóban.

---

### Minta B — `SelectionSync` (kijelölés ↔ input szinkron)

**Mire jó:** szerkesztés (B feladat), visszatöltés karakterszerkesztőbe.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `item_at(index)` · `active_index()` · `can_modify(index)` |
| **View** | `fill_inputs_from_selection()` · `clear_inputs()` |
| **Controller** | `on_list_select()` → view.fill + model.active_index frissítés |

**Szabály:** kijelöléskor inputok = model adatai; módosítás csak engedélyezett állapotból.

**Mintafeladatok:** Cipőbolt B · Karakter szerkesztés visszatöltés · F_sync `fill_inputs_from_selection()`.

---

### Minta C — `AssignRelease` (hozzárendelés, nem áthelyezés)

**Mire jó:** bal elem „kapcsolódik” jobb elemhez (orvos–műtét, tanuló–terem).

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `assign(left_idx, right_idx)` · `release(idx)` · `can_assign(left, right)` · `linked_name(item)` |
| **View** | `refresh_lists()` — szövegben megjelenik a link (pl. `"A → B"`) |
| **Controller** | `on_assign()` · `on_release()` |

**Adat:** `Item.assigned` vagy hasonló mező.

**Mintafeladatok:** Nem szerepel expliciten a példafeladatokban, de a `window.cpp` kommentjei erre utalnak.

---

### Minta D — `SingleCatalog` (egy fő lista + alatta értékek)

**Mire jó:** cipőbolt A (név + méretek), egyszerű katalógus.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `add_entry(name, value)` · `append_value(name, value)` · `remove_entry(name)` · `remove_value(name, value)` · `values_for(name)` |
| **View** | `refresh_main_list()` · `format_entry(name, values)` |
| **Controller** | `on_add()` · `on_delete()` |

**Szabály:** egy név egyszer szerepel; ugyanahhoz a névhez több érték (méret) tartozhat.

**Mintafeladatok:** Cipőbolt A.

---

### Minta E — `Inventory` (készlet + kiadás/eladás)

**Mire jó:** cipőbolt C, fagyi A.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `set_stock(name, key, qty)` · `sell(name, key, qty)` · `can_sell(name, key, qty)` · `remove_if_empty()` |
| **View** | `refresh_main_list()` · `refresh_available_dropdown()` — csak ami nem fogyott ki |
| **Controller** | `on_sell()` · `on_restock()` · `on_restock_selected()` |

**Szabály:** túl nagy mennyiség → `can_sell` false, semmi nem történik; nulla készlet → elem eltűnik a választékból (nem üres hely).

**Mintafeladatok:** Cipőbolt C · Fagyi A.

---

### Minta F — `DynamicOptions` (dropdown a modelből)

**Mire jó:** klánlista skillpont változásra, méret szűrő, fagyi választék.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `available_options(context)` · `recompute_options()` |
| **View** | `refresh_dropdown()` · `refresh_filter_dropdown()` |
| **Controller** | `on_skill_change()` · `on_filter_change()` → model.recompute → view.refresh |

**Szabály:** dropdown elemei **mindig** a model `available_options()` kimenetéből jönnek.

**Mintafeladatok:** Karakter/klán A · Cipőbolt D (méret szűrő + „Mind") · Fagyi A/B.

---

### Minta G — `FilteredMoves` (szűrő logika aktuális állapotra)

**Mire jó:** huszár bábu lépései.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `add_move(x, y)` · `set_current_position(move)` · `clear_filter()` · `legal_moves_from(pos)` · `filtered_moves()` · `all_moves()` |
| **View** | `refresh_move_list()` — szűrt vagy teljes lista |
| **Controller** | `on_add_move()` · `on_step()` · `on_select_move()` |

**Segéd:** `util.hpp` — `chess_notation()`, `chess_is_knight_move()`.

**Szabály:** új lépés felvétele → szűrés kikapcsol; LÉP gomb → aktuális pozíció frissül → szűrés be.

**Mintafeladatok:** Huszár bábu lépései.

---

### Minta H — `CapacityRows` (kapacitás / sorok száma megjelenítés)

**Mire jó:** gép/rec — méret = sorok száma, darabszám = ismétlődés.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `insert_to_machine(name, size, count)` · `remove_machine_group(name, size)` · `machine_display_rows()` |
| **View** | `refresh_machine_list()` — pl. `"nev 2/4"` formátum |
| **Controller** | `on_insert()` · `on_remove_row()` |

**Segéd:** `List::set_capacity()` — összesített súly/kapacitás.

**Mintafeladatok:** Gép, rac kezelés.

---

### Minta I — `PolymorphicCatalog` (polimorf elemek)

**Mire jó:** fagyi C — sablonok, összetett összetevők.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `Collection` + `Record` leszármazottak · `can_serve(record)` · `serve(record)` · `add_record()` · `remove_record()` |
| **View** | `refresh_catalog()` — `record->title()` szöveggel |
| **Controller** | `on_serve()` · `on_add_type()` · `on_delete_type()` |

**Fájl:** `record.hpp` / `record.cpp` — csak ha tényleg kell (C feladat).

**Mintafeladatok:** Fagyi B/C.

---

### Minta J — `Statistics` (eladási statisztika)

**Mire jó:** fagyi D.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `record_sale(item)` · `start_new_day()` · `reset_statistics()` · `totals()` · `yesterday_totals()` · `percent_breakdown()` |
| **View** | `refresh_stats_display()` |
| **Controller** | `on_new_day()` · `on_reset_stats()` |

**Mintafeladatok:** Fagyi D.

---

### Minta K — `Persistence` (mentés / betöltés)

**Mire jó:** karakter D, általános D feladatrész.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `serialize()` · `deserialize()` — vagy külön `Storage` osztály |
| **View** | `refresh_all()` mentés/betöltés után |
| **Controller** | `on_save()` · `on_load()` |

**Szabály:** a **modellt** mentjük, nem a widget állapotát.

**Mintafeladatok:** Karakter D · Fájlkezelés általában D részpont.

---

### Minta L — `CustomDefinitions` (felhasználó által definiált típusok)

**Mire jó:** karakter C — custom klánok.

| Réteg | Eljárások |
|-------|-----------|
| **Model** | `add_definition(name, requirements)` · `remove_definition(name)` · `matching_definitions(item)` · `all_definitions()` |
| **View** | `refresh_definition_list()` · `refresh_matching_dropdown()` |
| **Controller** | `on_add_definition()` · `on_delete_definition()` |

**Mintafeladatok:** Karakter/klán C.

---

## Mintafeladat → minta kombináció (gyors táblázat)

| Példafeladat | Fő minták (Model szinten) |
|--------------|---------------------------|
| **Karakter + klán** | A + B + F + L + K |
| **Cipőbolt** | D + B + E + F + K *(D rész)* |
| **Fagylalka** | E + F + I + J + K *(D)* |
| **Gép, rac kezelés** | A + H |
| **Huszár lépései** | G |

Vizsgán: **olvasd a feladatot → jelöld be a sorokat → csak azokhoz a mintákhoz írd át az `Item` mezőket és a model szabályokat.**

---

## Minimális beadandó csomag (ha kevés idő van)

**Kötelező:**
- Meglévő widgetek + `Lib` + event loop
- `model.hpp/cpp` — `Item` + Minta A alap CRUD
- `view.hpp/cpp` — listák + `refresh_lists()` + Minta B
- `controller.hpp/cpp` — vékony `on_*` összekötés
- `app.hpp/cpp` — layout
- `main.cpp`

**Érdemes opcionálisan bevinni (készen, de lehet üres implementáció):**
- Minta C (`assign` / `release`) — ha van hozzárendelős gyakorlat
- Minta E + F — készlet + dropdown (cipő/fagyi)
- Minta G + `util.hpp` — huszár
- Minta K — `storage.hpp/cpp` váz (D feladat)
- Minta I — `record.hpp` váz (fagyi C)

---

## Vizsga napján — Hanna workflow (6 lépés)

1. Feladat elolvasása → **melyik minta?** (fenti táblázat)
2. `Item` / `config.hpp` mezők átírása
3. `model.cpp`: hiányzó szabályok (`can_*`, `validate_*`)
4. `view.cpp`: `row_text()` / `refresh_*` formátum
5. `controller.cpp`: hiányzó gomb + `on_*` callback
6. Teszt: üres input, dupla név, kifogyás, szűrés, mentés

**Egy mondatban:** *Ne új programot írj — csak az `Item` mezőit és a model szabályait igazítsd a feladathoz; a View és Controller váz változatlan marad.*

---

## Összefoglaló Hanna számára

| Kérdés | Válasz |
|--------|--------|
| Mit ad a tanár? | `genv` — rajzolás + események |
| Mit viszel be? | Widgetek + **Model / View / Controller / App** + absztrakt minták (A–L) |
| Mit **nem** viszel be? | `Cipo`, `Szinkezelo`, feladat-specifikus Widget-gyerekek |
| Mi a vizsga lényege? | Ugyanaz a keret, más adat és szabályok |
| Miért MVC? | A tanár az **újrahasznosíthatóságot** méri, nem azt, hogy „működik-e egyszer” |

---

*Utolsó frissítés: 2026-06-13 — graph64 képességek, MVC architektúra és absztrakt eljárás-minták (A–L) beépítve.*
