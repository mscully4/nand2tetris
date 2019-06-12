#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<map>

using namespace std;

class SymbolTable {
    public:
        map<string, int> counters1;
        map<string, vector<string>> classTable;
        map<string, vector<string>> subroutineTable;

        SymbolTable();
        void define(const string&, const string&, const string&);
        int varCount(const string& kind);
        string kindOf(const string& name);
        string typeOf(const string& name);
        int indexOf(const string& name);
        void resetClass();
        void startSubroutine(const string& className);
};

