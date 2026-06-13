// ============================================================
// F_SYNC - WINDOW_PELDA.CPP
//
// refresh_lists minta – teljes példa, fill_inputs, modify, move_up/down
//
// TESTRESZABÁSI LÉPÉSEK:
//   1. Item struktúra (window.hpp-ban)
//   2. refresh_lists()
//   3. fill_inputs_from_selection() mezőit mód.
//   4. modify_item() logika mód.
// ============================================================

#include "window.hpp"
using namespace std;

Window::Window(int XX, int YY) : Lib(XX, YY)
{
    titleLabel = new StaticText(this, 20, 14, 760, 40,
                                "Szinkron lista (F_sync minta)", {245,247,255}, {37, 64, 105});

    nameLabel = new StaticText(this, 20, 68, 95, 28, "Neve:",    {235,239,255}, {52, 58, 86});
    nameInput = new Text      (this, 118, 68, 145, 28);

    new StaticText(this, 20, 100, 95, 28, "Értéke:", {235,239,255}, {52, 58, 86});
    valueInput = new SetValue (this, 118, 100, 100, 28, 0, 9999);

    addButton  = new Button(this, 20,  138, 110, 34, "Hozzáad",  [this](){ add_item(); });
    editButton = new Button(this, 135, 138, 110, 34, "Módosít",  [this](){ modify_item(); });
    delButton  = new Button(this, 20,  178, 110, 34, "Töröl",    [this](){ delete_item(); });

    moveUpButton   = new Button(this, 135, 178, 53, 34, "Fel ▲", [this](){ move_up(); });
    moveDownButton = new Button(this, 192, 178, 53, 34, "Le ▼",  [this](){ move_down(); });

    statusLabel = new StaticText(this, 20, 510, 245, 32, "Kész.", {245,247,255}, {48, 57, 91});
    statsLabel  = new StaticText(this, 20, 546, 760, 28, "",      {235,239,255}, {52, 58, 86});

    leftListLabel  = new StaticText(this, 278, 60, 215, 28, "Raktár",     {245,247,255}, {37, 64, 105});
    rightListLabel = new StaticText(this, 538, 60, 215, 28, "Kiválasztott", {245,247,255}, {37, 64, 105});

    warehouseList = new List(this, 278, 94, 215, 408);
    pickedList    = new List(this, 538, 94, 215, 408);

    warehouseList->set_on_select([this](){ fill_inputs_from_selection(); });
    pickedList->set_on_select   ([this](){ fill_inputs_from_selection(); });

    moveRightButton = new Button(this, 500, 240, 30, 42, ">", [this](){ move_to_right(); });
    moveLeftButton  = new Button(this, 500, 295, 30, 42, "<", [this](){ move_to_left(); });

    // Tesztelemek (töröld éles feladatnál):
    _items.push_back({"Alma",  150, false});
    _items.push_back({"Körte", 200, false});
    _items.push_back({"Banán",  80, false});
    refresh_lists();
}

// ============================================================
// REFRESH_LISTS – az F_sync minta lényege
// ============================================================
void Window::refresh_lists()
{
    // Kijelölések mentése (a teljes szöveg alapján)
    string sel_w = (warehouseList->selected_index() >= 0)
                   ? warehouseList->get_selected_item().name : "";
    string sel_p = (pickedList->selected_index() >= 0)
                   ? pickedList->get_selected_item().name : "";

    warehouseList->clear();
    pickedList->clear();

    for (int i = 0; i < (int)_items.size(); i++)
    {
        const Item& it = _items[i];
        string txt = it.name + " | " + to_string(it.value);

        if (!it.picked)
            warehouseList->add_item({txt, {i}});   // values[0] = _items index
        else
            pickedList->add_item({txt, {i}});
    }

    // Kijelölés visszaállítása szöveg szerint
    for (int i = 0; i < warehouseList->item_count(); i++) {
        warehouseList->set_selected_index(i);
        if (warehouseList->get_selected_item().name == sel_w) break;
        warehouseList->clear_selection();
    }
    for (int i = 0; i < pickedList->item_count(); i++) {
        pickedList->set_selected_index(i);
        if (pickedList->get_selected_item().name == sel_p) break;
        pickedList->clear_selection();
    }

    update_stats();
}

// ============================================================
// STATISZTIKA
// ============================================================
void Window::update_stats()
{
    int cnt = 0, sum = 0;
    for (const auto& it : _items)
        if (it.picked) { cnt++; sum += it.value; }
    statsLabel->set_text("Jobb: " + to_string(cnt) + " db | Összérték: " + to_string(sum));
}

// ============================================================
// SEGÉDFÜGGVÉNYEK: _items index meghatározása
// A lista values[0]-ban tároljuk az _items-beli indexet.
// ============================================================
int Window::warehouse_item_index() const
{
    if (warehouseList->selected_index() < 0) return -1;
    ListItem li = warehouseList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int Window::picked_item_index() const
{
    if (pickedList->selected_index() < 0) return -1;
    ListItem li = pickedList->get_selected_item();
    return li.values.empty() ? -1 : li.values[0];
}

int Window::active_item_index() const
{
    // Amelyik listán utoljára kattintottak, azt adjuk vissza
    if (warehouseList->selected_index() >= 0 && pickedList->selected_index() >= 0)
        return (warehouseList->activation_order() > pickedList->activation_order())
               ? warehouse_item_index() : picked_item_index();
    if (warehouseList->selected_index() >= 0) return warehouse_item_index();
    if (pickedList->selected_index() >= 0)    return picked_item_index();
    return -1;
}

// ============================================================
// FILL_INPUTS_FROM_SELECTION
// Kattintáskor feltölti az inputokat a kijelölt elem adataival.
// ============================================================
void Window::fill_inputs_from_selection()
{
    int idx = active_item_index();
    if (idx < 0) return;
    nameInput->set_text(_items[idx].name);
    valueInput->set_value(_items[idx].value);
}

// ============================================================
// HOZZÁADÁS
// ============================================================
void Window::add_item()
{
    string name = nameInput->text();
    if (name.empty()) { statusLabel->set_text("A név nem lehet üres."); return; }

    // Dupla ellenőrzés
    for (const auto& it : _items)
        if (it.name == name) { statusLabel->set_text("Ilyen nevű elem már van."); return; }

    // TODO: módosítani az Item inicializálást!
    _items.push_back({name, valueInput->value(), false});
    refresh_lists();
    nameInput->clear();
    statusLabel->set_text("Elem hozzáadva: " + name);
}

// ============================================================
// MÓDOSÍTÁS
// Csak bal listán (picked == false) lévő elemet lehet módosítani.
// ============================================================
void Window::modify_item()
{
    int idx = warehouse_item_index();
    if (idx < 0) { statusLabel->set_text("Válassz elemet a bal listából!"); return; }

    string name = nameInput->text();
    if (name.empty()) { statusLabel->set_text("A név nem lehet üres."); return; }

    // Dupla ellenőrzés (az éppen módosított elemet kihagyjuk)
    for (int i = 0; i < (int)_items.size(); i++)
        if (i != idx && _items[i].name == name)
            { statusLabel->set_text("Ilyen nevű elem már van."); return; }

    _items[idx].name  = name;
    _items[idx].value = valueInput->value();

    refresh_lists();
    statusLabel->set_text("Módosítva: " + name);
}

// ============================================================
// TÖRLÉS
// ============================================================
void Window::delete_item()
{
    int idx = active_item_index();
    if (idx < 0) { statusLabel->set_text("Nincs kijelölt elem."); return; }

    string name = _items[idx].name;
    _items.erase(_items.begin() + idx);
    refresh_lists();
    statusLabel->set_text("Törölve: " + name);
}

// ============================================================
// FEL MOZGATÁS (csak bal lista, picked == false elemek közt)
// ============================================================
void Window::move_up()
{
    int idx = warehouse_item_index();
    if (idx < 0) { statusLabel->set_text("Válassz elemet a bal listából!"); return; }

    // Az előző nem-picked elemet keressük _items-ben
    for (int i = idx - 1; i >= 0; i--)
    {
        if (!_items[i].picked)
        {
            swap(_items[idx], _items[i]);
            refresh_lists();
            statusLabel->set_text("Feljebb mozgatva.");
            return;
        }
    }
    statusLabel->set_text("Már a lista tetején van.");
}

// ============================================================
// LE MOZGATÁS (csak bal lista, picked == false elemek közt)
// ============================================================
void Window::move_down()
{
    int idx = warehouse_item_index();
    if (idx < 0) { statusLabel->set_text("Válassz elemet a bal listából!"); return; }

    // A következő nem-picked elemet keressük _items-ben
    for (int i = idx + 1; i < (int)_items.size(); i++)
    {
        if (!_items[i].picked)
        {
            swap(_items[idx], _items[i]);
            refresh_lists();
            statusLabel->set_text("Lejjebb mozgatva.");
            return;
        }
    }
    statusLabel->set_text("Már a lista alján van.");
}

// ============================================================
// ÁTHELYEZÉS JOBBRA
// TODO: opcionálisan feltétel ellenőrzés ide!
// ============================================================
void Window::move_to_right()
{
    int idx = warehouse_item_index();
    if (idx < 0) { statusLabel->set_text("Válassz elemet a bal listából!"); return; }

    // TODO: feltétel ellenőrzés:
    // if (/* feltétel */) { statusLabel->set_text("..."); return; }

    _items[idx].picked = true;
    refresh_lists();
    statusLabel->set_text("Áthelyezve jobbra: " + _items[idx].name);
}

// ============================================================
// ÁTHELYEZÉS BALRA
// ============================================================
void Window::move_to_left()
{
    int idx = picked_item_index();
    if (idx < 0) { statusLabel->set_text("Válassz elemet a jobb listából!"); return; }

    _items[idx].picked = false;
    refresh_lists();
    statusLabel->set_text("Visszahelyezve balra: " + _items[idx].name);
}

// ============================================================
// APPLY – HOZZÁRENDELÉS  (opcionális, apply/release minta)
// ============================================================
// Mikor: ha a feladat NEM egyszerű áthelyezés, hanem
// a bal lista egyik eleme egy mezőn keresztül "hivatkozik" a jobb
// lista egyik elemére (pl. orvos kap műtétet, tanuló kap termet).
//
// TESTRESZABÁS (3 lépés):
//   1. Az Item struktúrában legyen egy "link" mező, pl.:
//         std::string assigned;   // a hozzárendelt jobb oldali elem neve
//
//   2. Feltétel (opcionális) – mikor szabad összekötni?
//      Pl.: if (_items[idx_left].level < _items[idx_right].required_level)
//               { statusLabel->set_text("Nem elég tapasztalt."); return; }
//
//   3. Az összekötés – melyik elem melyik mezője hivatkozzon mire:
//      Pl.: _items[idx_right].assigned = _items[idx_left].name;
//      VAGY: _items[idx_left].assigned = _items[idx_right].name;
//
// A konstruktorban:
//   applyButton   = new Button(this, X, Y, W, H, "Hozzárendel", [this](){ apply(); });
//   releaseButton = new Button(this, X, Y, W, H, "Felszabadít", [this](){ release(); });
// ============================================================

void Window::apply()
{
    int idx_left  = warehouse_item_index();
    int idx_right = picked_item_index();
    if (idx_left < 0 || idx_right < 0)
        { statusLabel->set_text("Mindkét listából válassz elemet!"); return; }

    // TODO: feltétel ellenőrzés (ha szükséges):
    // if (/* pl. _items[idx_left].exp < _items[idx_right].required_exp * /)
    //     { statusLabel->set_text("A feltétel nem teljesül."); return; }

    // TODO: összekötést – melyik elem melyik mezőjébe kerüljön mit:
    // _items[idx_right].assigned = _items[idx_left].name;   // jobb oldal kapja a bal nevét
    // VAGY:
    // _items[idx_left].assigned = _items[idx_right].name;   // bal oldal kapja a jobb nevét

    refresh_lists();
    statusLabel->set_text("Hozzárendelve.");
}


// ============================================================
// RELEASE – HOZZÁRENDELÉS BONTÁSA  (opcionális, apply/release minta)
// ============================================================
// TESTRESZABÁS (2 lépés):
//   1.  melyik lista elemét kell felszabadítani (bal vagy jobb)?
//      Alapértelmezés: jobb oldali (picked_item_index).
//      Ha bal oldali kell: cserélni le warehouse_item_index()-re.
//
//   2. melyik mező értékét kell törölni:
//      Pl.: _items[idx].assigned = "";
// ============================================================

void Window::release()
{
    // TODO: melyik listából kell a kijelölés:
    int idx = picked_item_index();          // jobb listából
    // int idx = warehouse_item_index();    // bal listából (ha azt kell)

    if (idx < 0) { statusLabel->set_text("Válassz elemet!"); return; }

    // TODO: felszabadítás – melyik mezőt kell törölni:
    // _items[idx].assigned = "";

    refresh_lists();
    statusLabel->set_text("Hozzárendelés bontva.");
}
