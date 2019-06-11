#pragma once
#include<iostream>
#include<string>
#include<map>

using namespace std;

class SymbolTable {
    public:
        map<string, int> table;
        SymbolTable();
        void define(const string& key, const int& value);
        int addressOf(const string& name);
};

