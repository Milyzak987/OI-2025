#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Używamy long double dla precyzji geometrycznej
using Real = long double;
const Real PI = acos(-1.0L);
const Real TWO_PI = 2.0L * PI;
const Real EPS = 1e-11L; // Margines błędu dla floatów

struct Kitten {
    long long x, y;
    Real angle;  // Kąt pozycyjny (gdzie kot siedzi)
    Real spread; // Połowa kąta widzenia (odchylenie stycznych)
};

int main() {
    // Szybkie wejście/wyjście (choć przy N^2 to niewiele pomoże)
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long R_in;
    if (!(cin >> N >> R_in)) return 0;
    Real R = (Real)R_in;

    vector<Kitten> kittens(N);

    // KROK 1: Obliczenie parametrów dla każdego kota
    for (int i = 0; i < N; ++i) {
        cin >> kittens[i].x >> kittens[i].y;

        // Dystans od środka (0,0)
        Real dist = sqrt((Real)kittens[i].x * kittens[i].x + (Real)kittens[i].y * kittens[i].y);

        // Kąt pozycyjny (atan2 zwraca wartość w zakresie -PI do PI)
        kittens[i].angle = atan2((Real)kittens[i].y, (Real)kittens[i].x);
        
        // Jeśli atan2 zwróci ujemny kąt, zamieniamy go na zakres [0, 2PI) dla wygody
        if (kittens[i].angle < 0) kittens[i].angle += TWO_PI;

        // Spread (odchylenie stycznych)
        // Jeśli kot jest (teoretycznie) na granicy koła lub w środku (zgodnie z zadaniem nie jest), spread = 0
        if (dist > R) {
            kittens[i].spread = acos(R / dist);
        } else {
            kittens[i].spread = 0.0;
        }
    }

    long long count = 0;

    // KROK 2: Sprawdzanie "Każdy z Każdym" (O(N^2))
    // Dwie pętle zagnieżdżone - to jest ta "wolna" część
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            
            // Obliczamy najkrótszą odległość kątową na okręgu między kotem i a kotem j
            Real angle_diff = abs(kittens[i].angle - kittens[j].angle);
            
            // Jeśli różnica jest większa niż PI (180 stopni), to znaczy że krótsza droga jest "w drugą stronę"
            if (angle_diff > PI) {
                angle_diff = TWO_PI - angle_diff;
            }

            // Warunek widoczności oparty na stycznych:
            // Jeśli odległość kątowa między kotami jest mniejsza niż suma ich "zasięgów" stycznych,
            // to znaczy, że ich pola widzenia zachodzą na siebie -> widzą się.
            // Odejmujemy EPS, bo jeśli linia tylko "dotyka" koła, to się nie liczy (nierówność ostra).
            if (angle_diff < (kittens[i].spread + kittens[j].spread - EPS)) {
                count++;
            }
        }
    }

    cout << count << endl;

    return 0;
}
