#include<iostream>
#include<string>
#include<vector>
#include "HashTable.h"
using namespace std;

int main() {
    HashTable<string, vector<string>> x = HashTable<string, vector<string>>(11);
    vector<string> y = {"BOOF", "CHORK", "PISS"};
    x.put("TEST", y);
    vector<string> z;
    x.get("TEST", z);
    for (int i=0; i<z.size(); ++i) cout << z[i] << endl;
}
