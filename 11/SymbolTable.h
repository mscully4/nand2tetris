#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "HashTable/HashTable.h"

using namespace std;

class SymbolTable {
    public:
        HashTable<string, int> counters = HashTable<string, int>(11);
        HashTable<string, vector<string>> class_table = HashTable<string, vector<string>>(11);
        HashTable<string, vector<string>> subroutine_table = HashTable<string, vector<string>>(11);
        SymbolTable();
        void define(const string&, const string&, const string&);
        int varCount(const string& kind);
        string kindOf(const string& name);
        string typeOf(const string& name);
        int indexOf(const string& name);
        void startClass();
        void startSubroutine(const string& className);
};

