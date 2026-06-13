# Hanna C++ OOP vizsga — kontextus és eddigi következtetések

> Forrás: [ChatGPT beszélgetés](https://chatgpt.com/share/6a2d4511-bc4c-83eb-a8b6-1f805c2adb33) (2026.06., telefonbeszélgetés után)
>
> **Státusz:** A konkrét vizsgafeladat még **nem ismert** — ezt a dokumentumot addig frissítjük, amíg ki nem derül.

---

## Helyzet röviden

Hanna nem tud egyedül felkészülni a vizsgára. A feladat lényegét nehezen tudja elmagyarázni; segítséget kér a felkészüléshez. A vizsga valószínűleg nem a C++ szintaxisa miatt akad el, hanem az **absztrakció**, a **MVC**, és a **vizsgabevihető keretrendszer** miatt.

---

## Amit eddig tudunk

| Téma | Részlet |
|------|---------|
| **Tárgy jellege** | Egyetemi grafika / OOP / szoftvertervezés jellegű |
| **Tanár library-je** | Saját grafikus library (OpenGL-re épülhet, vagy teljesen saját) |
| **Vizsga szabály** | Vizsga előtt **fájl-szettet** kell feltölteni/bevinni; a vizsgán **csak ezt** használhatják |
| **Fájltípusok** | `include/` (.h, .hpp) + `source/` (.cpp) felosztás |
| **MVC** | Szó esik a Model–View–Controller mintáról |
| **Tanár kritikája** | A beadott fájlok **túl gyakorlófeladat-specifikusak**, nem elég **általánosak** |
| **Hanna állapota** | Nem tudta elvégezni a vizsgát; pontos library név, feladat és fájlszerkezet még hiányzik |

---

## Mit jelent az include / source felosztás?

- **Header (.h, .hpp)** → *mit tud a kód?* (deklaráció, interfész)
- **Source (.cpp)** → *hogyan csinálja?* (implementáció)
- Az `#include` nem „betölt”, hanem **szövegesen bemásol** a fordítás idején
- Célok: gyorsabb fordítás, modularitás, encapsulation
- Osztálynál: `.h`-ban osztály deklaráció, `.cpp`-ben metódusok implementációja

---

## Mi a valószínű vizsgamodell?

### 1. Tanár API-ja (nem pixelt rajzol a hallgató)

Tipikus elemek: `Window`, `Canvas`, `Line`, `Circle`, `Button`, `Event`, `Draw()`, `MouseClick()` stb.

Analógia:
- **Java:** `JPanel`, `paint()`, `ActionListener`
- **JS:** `canvas`, `requestAnimationFrame()`

### 2. Bevihető „framework”, nem konkrét megoldás

**Rossz** (túl specifikus):
```cpp
class SnakeGame {};
class Homework3Button {};
class DrawHouseTask {};
```

**Jó** (újrahasznosítható építőkockák):
```cpp
class GameObject {};
class Scene {};
class Renderer {};
class InputHandler {};
class Shape {};
class Widget {};
class Drawable {};
class EventHandler {};
```

### 3. MVC elvárás

```
MODEL (adat)  →  VIEW (megjelenítés)  →  CONTROLLER (interakció)
```

**Rossz:** logika a View-ban
```cpp
void draw() {
   if (player.hp < 10)
      drawRed();
}
```

**Jó:** modell dönt, view csak rajzol
```cpp
model.move();
view.render(model);
controller.handleInput();
```

### 4. Tipikus bevihető fájlszerkezet

```
include/
  Model.h, View.h, Controller.h
  Shape.h, Scene.h, Window.h

source/
  Model.cpp, View.cpp, Controller.cpp
  Shape.cpp, Scene.cpp, Window.cpp
```

Benne legyen (általánosan): eseménykezelés, objektumlista, kirajzolási ciklus, koordinátakezelés, állapotfrissítés — **nem** konkrét játék/feladat.

---

## A tanár kritikájának valószínű értelme

> „Túl gyakorlófeladat-specifikus, nem elég általános.”

Ez klasszikus egyetemi OOP-csapda (Pascal/C++ korszak is):

1. A hallgató megtanulja a nyelvet
2. Megoldja a gyakorlót
3. A tanár: *„Jó, de ez nem objektumorientált.”*

A rejtett tananyag:

> **Nem az számít, hogy működik-e, hanem hogy a következő feladatból mennyit lehet újrahasználni.**

Hanna valószínűleg meg tudta oldani a gyakorlókat, de vizsgán kiderült: **konkrét megoldásokat** épített, nem **komponenseket**.

---

## Tipikus hibák, amire figyelni kell

- View-ban van üzleti logika (Controller helyett View)
- Osztálynevek túl konkrétak (feladat-specifikusak)
- Hiányzó ősosztály / interfész (pl. `Shape` helyett `RedCircleForLab3`)
- Utility kód szétszórva konkrét osztályokban
- Nem MVC-s szétválasztás

---

## Amit még **ki kell deríteni** (prioritás)

1. **Library neve** és dokumentációja / header fájljai
2. **Konkrét vizsgafeladat** vagy mintafeladat szövege
3. **Hanna jelenlegi fájlszerkezete** (mit vitt be / mit írt eddig)
4. **Meddig jutott el** a feladatban
5. **Tanár konkrét visszajelzései** (ha vannak írásban)
6. **Tárgy pontos neve** és kurzus anyag (BevProg2? Grafika? OOP külön?)

---

## Következő lépések

1. Hanna írja össze: library név, feladat, eddigi kód, hol akadt el
2. Ha megvan a feladat → 10–15 perc alatt eldönthető: tanár elvárása, absztrakciós hiba, optimális bevihető framework
3. A `példafeladatok/` mappában lévő BevProg2-szerű feladatok (karakter/klán, cipőbolt, fagyi stb.) **lehetnek hasonló stílusúak**, de a vizsga valószínűleg **saját grafikus library**-re épül, nem wxWidgetsre

---

## Projektben lévő példafeladatok (referencia)

| Fájl | Tárgy / téma |
|------|----------------|
| `példafeladatok/feladat.txt` | Karakter + klán + skillpontok (2018) |
| `példafeladatok/leiras.txt` | Cipőbolt készlet (2017) |
| `példafeladatok/leiras-1.txt` | Fagylalka készlet + sablonok (2017) |
| `példafeladatok/Gép, rac kezelés.txt` | REC/gép lista kezelés |
| `példafeladatok/Huszár bábu lépései.txt` | Sakktábla lépések szűrése |

Ezek widget-alapú, A–B–C–D feladatrészes géptermi ZH-k — hasznos minták a **funkcionális követelmények** és **pontozás** szempontjából, de **nem biztos**, hogy Hanna vizsgája pont ezek közül valamelyik.

---

## Nyitott kérdések

- Mi a tanár filozófiája: tervezési minták? komponensek? event loop? öröklődés? vagy „10 hasznos osztály”?
- A vizsgafeladat **interaktív grafika** (egér, animáció) vagy **statikus rajz**?
- Van-e **fájlkezelés** (mentés/betöltés) követelmény?

---

*Utolsó frissítés: 2026-06-13 — a konkrét feladat azonosítása folyamatban.*
