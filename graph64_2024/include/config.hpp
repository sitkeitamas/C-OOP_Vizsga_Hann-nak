#ifndef CONFIG_HPP
#define CONFIG_HPP

// ============================================================
// CONFIG.HPP - Általános beállítások
// ============================================================

// Hány int érték tartozik egy elemhez?
// Pl. 2: (méret, ár)   3: (R, G, B)   1: (darabszám)
static constexpr int VALUE_DIMENSION = 2;

// Megengedett-e azonos nevű elem? (ha nem, akkor add_item visszaadja a sikertelenséget)
enum DuplicateNameMode {
    RejectDuplicateNames,
    AllowDuplicateNames
};
static constexpr DuplicateNameMode DUPLICATE_NAME_MODE = RejectDuplicateNames;

// Áthelyezés jobbra: true = katalógus (bal listán marad), false = átvitel (balról eltűnik)
static constexpr bool MOVE_KEEP_ON_LEFT = true;

#endif
