// sprawdzarka do zadania Hanoj
// autor Piotr Guzinski
// zlozonosc czasowa O(odpowiedz + n)
// zlozonosc pamieciowa O(n)

// sprawdzarka nie weryfikuje poprawnosci wejscia oraz pierwszej linii wyjscia.
// sprawdzarka dziala z programem "Toster"

#include <bits/stdc++.h>
#define F(i, a, b) for(int i = (int)a; i <= (int)b; i++)
#define pii pair<int,int>
#define f first
#define s second
#define be(x) x.begin(), x.end()
#define V vector
#define pb push_back
using namespace std;

const int maxm = 1'000'009; // sugeruje zmniejszyc maksymalne m dla mniejszych testow w celu poprawienia wydajnosci
deque <int> stos[maxm];


int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    F(i, 1, m){
        int x;
        cin >> x;
        F(j, 1, x){
            int a;
            cin >> a;
            stos[i].pb(a);
        }
    }

    int ans;
    cin >> ans;

    if(ans < -1){
        cout << "I Wynik nie moze byc ujemny XDDDD\n";
        return 0;
    }

    if(ans == -1){
        // W zwiazku z faktem ze sprawdzarka nie weryfikuje pierwszej linii wyjscia zakladam ze wynik jest poprawny
        cout << "C\n";
        return 0;
    }

    F(i, 1, ans){
        int a, b;
        cin >> a >> b;
        if(a <= 0 || b <= 0 || a > m || b > m){
            cout << "I Twoj program chce korzystac z nieistniejacego stosu (iteracja: " << i << ")\n";
            return 0;
        }
        if(stos[a].size() == 0){
            cout << "I Twoj program probuje przeniesc element z pustego stosu (nr iteracji: " << i << " nr stosu: " << a <<")\n";
            cout << a << " " << b << "\n";
            return 0;
        }
        int x = stos[a].front();
        stos[a].pop_front();
        if(stos[b].size() > 0 && stos[b].back() > x){
            cout << "I Przeniesienie tego elementu na stos " << b << " sprawi, ze przestanie on byc posortowany (itercja: " << i << ")\n";
            return 0;
        }
        stos[b].pb(x);
    }
    int last = 0;

    F(i, 1, m){
        if(stos[i].size() > 0){
            if(last > 0){
                cout << "I Na jednym stosie nie ma wszystkich klockow (istnieja klocki na stosach: " << last << " i " << i << ")\n";
                return 0;
            }
            last = i;
        }
    }

    F(i, 1, stos[last].size()-1){
        if(stos[last][i] != stos[last][i-1] + 1){
            cout << "I Na ostatnim stosie elementy sa nieposortowane\n";
            return 0;
        }
    }

    cout << "C\n";
    return 0;
}