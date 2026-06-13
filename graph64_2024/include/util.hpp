#ifndef UTIL_HPP
#define UTIL_HPP

// Szintaxis-segéd – vizsgán nem kell fejben tartani a C++ részleteket.
// Nem kötelező mindet használni; csak ami kell a feladathoz.

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

// --- Szöveg / szám ---
inline std::string u_to_string(int n) { return std::to_string(n); }

inline bool u_empty(const std::string& s) { return s.empty(); }

inline std::string u_lower(std::string s)
{
    for (char& c : s) c = (char)std::tolower((unsigned char)c);
    return s;
}

// --- Rendezés (példa – másold át ha kell) ---
// std::sort(v.begin(), v.end(), [](const Item& a, const Item& b){ return a.name < b.name; });
// std::stable_sort(...);

// --- Sakktábla (huszár feladat) ---
inline std::string chess_notation(int file, int rank)
{
    return std::string(1, char('a' + file)) + std::to_string(rank);
}

inline bool chess_parse_file(const std::string& s, int& file_out)
{
    if (s.size() != 1) return false;
    char c = s[0];
    if (c >= 'A' && c <= 'H') c = char(c - 'A' + 'a');
    if (c < 'a' || c > 'h') return false;
    file_out = c - 'a';
    return true;
}

inline bool chess_is_knight_move(int f1, int r1, int f2, int r2)
{
    int df = std::abs(f2 - f1);
    int dr = std::abs(r2 - r1);
    return (df == 2 && dr == 1) || (df == 1 && dr == 2);
}

#endif
