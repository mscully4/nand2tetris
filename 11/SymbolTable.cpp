#include <iostream>
#include <string>
#include "SymbolTable.h"
using namespace std;

SymbolTable::SymbolTable() {
    this->counters.put("static", 0);
    this->counters.put("field", 0);
    this->counters.put("argument", 0);
    this->counters.put("var", 0);
}

void SymbolTable::define(const string& name, const string& type, const string& kind) {
    int y;
    counters.get(kind, y);
    vector<string> x = {type, kind, to_string(y)};
    if (kind == "static" || kind == "field") {
        class_table.put(name, x);
    } else if (kind == "argument" || kind == "var") {
        subroutine_table.put(name, x);
    } else {
        cerr << "INVALID KIND" << endl;
    }
    counters.put(kind, ++y);
}

int SymbolTable::varCount(const string& kind) {
    int x;
    counters.get(kind, x);
    return x;
}

string SymbolTable::kindOf(const string& name) {
    vector<string> x;
    subroutine_table.get(name, x);
    if (x.empty()) {
        class_table.get(name, x);
    }
    if (!x.empty()) {
        return x[1];
    } else {
        //cerr << "NO KIND FOUND IN MEMORY" << endl;
        return "NONE";
    }
}

string SymbolTable::typeOf(const string& name) {
    string kind = kindOf(name);
    vector<string> x;
    if (kind == "static" || kind == "field") {
        class_table.get(name, x);
    } else if (kind == "argument" || kind == "var") {
        subroutine_table.get(name, x);
    }

    if (!x.empty()) {
        return x[0];
    } else {
        //cerr << "NO TYPE FOUND IN MEMORY" << endl;
        return "NONE";
    }
    return x[0]; 
}

int SymbolTable::indexOf(const string& name) {
    string kind = kindOf(name);
    vector<string> x;
    if (kind == "static" || kind == "field") {
        class_table.get(name, x);
    } else if (kind == "argument" || kind == "var") {
        subroutine_table.get(name, x);
    }

    if (!x.empty()) {
        return stoi(x[2]);
    } else {
        //cerr << "NO INDEX FOUND IN MEMORY" << endl;
        return -1;
    }
}

void SymbolTable::startSubroutine(const string& className) {
    subroutine_table = HashTable<string, vector<string>>(11);
    counters.put("argument", 0);
    counters.put("var", 0);
    //this->define("this", className, "argument"); 
}

void SymbolTable::startClass() {
    class_table = HashTable<string, vector<string>>(11);
    counters.put("static", 0);
    counters.put("field", 0);
}
