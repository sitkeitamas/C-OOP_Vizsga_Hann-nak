#ifndef CONFIG_HPP
#define CONFIG_HPP

// ============================================================
// CONFIG.HPP – Vizsgán EZT módosítsd leginkább (mezők + szabályok).
// ============================================================

// --- Adatmodell ---
static constexpr int VALUE_DIMENSION = 2; // vals[] hossza (méret, ár / készlet / idő…)

enum DuplicateNameMode { RejectDuplicateNames, AllowDuplicateNames };
static constexpr DuplicateNameMode DUPLICATE_NAME_MODE = RejectDuplicateNames;

static constexpr bool MOVE_KEEP_ON_LEFT = true; // true = katalógus, false = átvitel

// vals[] indexek (vizsgán átnevezhető kommenttel)
static constexpr int VAL_STOCK  = 0; // készlet / első számmező
static constexpr int VAL_SECOND = 1; // második szám (ár, időtartam…)
static constexpr int RESTOCK_AMOUNT = 10;

// --- UI: mely panel aktív (vizsgán kapcsold be a feladathoz) ---
static constexpr bool UI_TWO_LIST     = true;  // két listás F_sync (csapat, kiválasztás…)
static constexpr bool UI_INVENTORY    = true;  // fagyi/cipő: dropdown + kiadás + készlet sor
static constexpr bool UI_MACHINE      = false; // REC/gép: katalógus dropdown + gép lista
static constexpr bool UI_CATEGORY     = false; // tag mező + kategória szűrés
static constexpr bool UI_ASSIGN       = false; // apply / release gombok

// --- Demo induló adat (beadáskor DemoSeed::None) ---
enum class DemoSeed { None, TwoList, Inventory, RecGep };
static constexpr DemoSeed DEMO_SEED = DemoSeed::Inventory;

#endif
