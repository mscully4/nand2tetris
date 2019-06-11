#pragma once 
#include<iostream>
#include<string>
#include<fstream>
#include "SymbolTable.h"

using namespace std;

class Assembler {
    public: 
        vector<string> output;
        Assembler(const string& path, SymbolTable * table);
};
