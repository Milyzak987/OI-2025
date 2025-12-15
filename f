/*
 * Rozwiązanie zadania Chatownicy (cha)
 * Autor: Gemini
 */

#include "chalib.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int main() {
    [span_1](start_span)// Pobranie liczby mieszkańców[span_1](end_span)
    int n = daj_n();

    // Jeśli n=1, nie można utworzyć żadnej pary.
    if (n < 2) {
        odpowiedz({});
        return 0;
    }

    // Tworzymy wektor z numerami mieszkańców 1..n
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);

    // Losowe przetasowanie jest kluczowe przeciwko testom adaptacyjnym
    mt19937 rng(1337); 
    shuffle(p.begin(), p.end(), rng);

    // Listy na znalezione pary
    vector<pair<int, int>> friends;
    vector<pair<int, int>> enemies;

    int queries_used = 0;
    int max_queries = n; [span_2](start_span)// Limit zapytań dla najtrudniejszego podzadania[span_2](end_span)

    // FAZA 1: Wstępne parowanie
    // Zużywa floor(n/2) zapytań.
    for (int i = 0; i + 1 < n; i += 2) {
        [span_3](start_span)// Sprawdzenie relacji[span_3](end_span)
        if (zapytaj(p[i], p[i+1])) {
            friends.push_back({p[i], p[i+1]});
        } else {
            enemies.push_back({p[i], p[i+1]});
        }
        queries_used++;
    }

    // FAZA 2: Hill Climbing / Mieszanie
    // Próbujemy zamienić pary mniejszościowe na większościowe
    // Potrzebujemy 2 zapytań na każdą próbę wymiany.
    
    while (queries_used + 1 < max_queries && !friends.empty() && !enemies.empty()) {
        // Pobieramy po jednej parze z każdego zbioru
        pair<int, int> p_friend = friends.back();
        friends.pop_back();
        
        pair<int, int> p_enemy = enemies.back();
        enemies.pop_back();
        
        int u = p_friend.first;
        int v = p_friend.second; // u i v są przyjaciółmi
        
        int x = p_enemy.first;
        int y = p_enemy.second;  // x i y są wrogami
        
        // Sprawdzamy relacje "na krzyż"
        bool res1 = zapytaj(u, x);
        queries_used++;
        
        // Zabezpieczenie przed przekroczeniem limitu w połowie procesu
        if (queries_used == max_queries) {
            // Nie możemy wykonać drugiego zapytania.
            // Ratujemy co mamy na podstawie pierwszego wyniku.
            if (res1) { // Przyjaciele
                friends.push_back({u, x});
                // Pary (v, y) nie znamy, przepada (lub można by ją dać do friends/enemies losowo, ale to ryzyko)
            } else { // Wrogowie
                enemies.push_back({u, x});
            }
            break; 
        }
        
        bool res2 = zapytaj(v, y);
        queries_used++;
        
        // Rozdzielamy nowe pary do odpowiednich list
        if (res1) friends.push_back({u, x});
        else enemies.push_back({u, x});
        
        if (res2) friends.push_back({v, y});
        else enemies.push_back({v, y});
        
        // Logika: 
        // Jeśli res1 == res2 == true (Przyjaciele), zamieniliśmy 1 parę E i 1 parę F na 2 pary F. (Zysk dla F)
        // Jeśli res1 == res2 == false (Wrogowie), zamieniliśmy 1 parę E i 1 parę F na 2 pary E. (Zysk dla E)
        // W przeciwnym razie liczby par się nie zmieniają, ale tasujemy wierzchołki.
    }

    [span_4](start_span)// Zwracamy większy zbiór par[span_4](end_span)
    if (friends.size() >= enemies.size()) {
        odpowiedz(friends);
    } else {
        odpowiedz(enemies);
    }

    return 0;
}
