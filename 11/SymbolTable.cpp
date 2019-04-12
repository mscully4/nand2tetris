#include <iostream>
#include <string>
#include <map>
#include "SymbolTable.h"
using namespace std;

SymbolTable::SymbolTable() {
    counters1.insert(pair<string, int>("static", 0));
    counters1.insert(pair<string, int>("field", 0));
    counters1.insert(pair<string, int>("argument", 0));
    counters1.insert(pair<string, int>("var", 0));
}

void SymbolTable::define(const string& name, const string& type, const string& kind) {
    map<string, int>::iterator itr;
    itr = counters1.find(kind);
    int z = itr->second;
    vector<string> n = {type, kind, to_string(z)};
    if (kind == "static" || kind == "field") {
        classTable.insert(pair<string, vector<string>>(name, n));
    } else if (kind == "argument" || kind == "var") {
        subroutineTable.insert(pair<string, vector<string>>(name, n));
    } else {
        cerr << "INVALID KIND" << endl;
    }
    counters1[kind]++;
}

int SymbolTable::varCount(const string& kind) {
    return counters1.find(kind)->second;
}

string SymbolTable::kindOf(const string& name) {
    map<string, vector<string>>::iterator itr;
    itr = subroutineTable.find(name);
    if (itr != subroutineTable.end()) {
        return itr->second[1];
    } else if (classTable.find(name) != classTable.end()) {
        itr = classTable.find(name);
        return itr->second[1];
    } else {
        return "NONE";
    }
}

string SymbolTable::typeOf(const string& name) {
    map<string, vector<string>>::iterator itr;
    string kind = kindOf(name);
    vector<string> x;
    if (kind == "static" || kind == "field") {
        x = classTable.find(name)->second;
    } else if (kind == "argument" || kind == "var") {
        x = subroutineTable.find(name)->second;
    }

    if (!x.empty()) {
        return x[0];
    } else {
        return "NONE";
    }
}

int SymbolTable::indexOf(const string& name) {
    map<string, vector<string>>::iterator itr;
    string kind = kindOf(name);
    vector<string> x;
    if (kind == "static" || kind == "field") {
        x = classTable.find(name)->second;
    } else if (kind == "argument" || kind == "var") {
        x = subroutineTable.find(name)->second;
    }

    if (!x.empty()) {
        return stoi(x[2]);
    } else {
        return -1;
    }
}

void SymbolTable::startSubroutine(const string& className) {
    subroutineTable = map<string, vector<string>>();
    counters1["argument"] = 0;
    counters1["var"] = 0;
    //this->define("this", className, "argument"); 
}

void SymbolTable::resetClass() {
    classTable = map<string, vector<string>>();
}
