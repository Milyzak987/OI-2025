#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read input file content from stdin
    int n;
    cin >> n;
    vector<string> pages(n);
    for(int i=0;i<n;i++) cin >> pages[i];

    string line;
    cin >> line; // consume newline after last page
    string ops;
    cin >> ops; // read ops from program output

    unordered_set<string> visited;
    string field;
    string lastVisited;

    for(char c: ops){
        if(c>='a' && c<='z'){
            field.push_back(c);
        } else if(c=='B'){
            if(!field.empty()) field.pop_back();
        } else if(c=='E'){
            if(find(pages.begin(), pages.end(), field)==pages.end()){
                cout << "I Invalid page visited: " << field << '\n';
                return 0;
            }
            if(visited.count(field)){
                cout << "I Page visited twice: " << field << '\n';
                return 0;
            }
            visited.insert(field);
            lastVisited = field;
            field.clear();
        } else if(c=='T'){
            if(lastVisited.empty()) continue;
            if(field.empty()){
                field = lastVisited;
            } else {
                // autocomplete only if field is prefix
                if(lastVisited.substr(0, field.size())==field){
                    field = lastVisited;
                }
            }
        } else {
            cout << "I Invalid character in ops: " << c << '\n';
            return 0;
        }
    }

    if(visited.size()!=n){
        cout << "I Not all pages visited. Visited " << visited.size() << " out of " << n << '\n';
        return 0;
    }

    cout << "C" << '\n';
    return 0;
}
