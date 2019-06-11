#include <iostream>
#include <string>
#include <map>
#include "SymbolTable.h"

using namespace std;

SymbolTable::SymbolTable() {
    //loading all the constants into the hash table
    this->define("R0", 0);
    this->define("R1", 1);
    this->define("R2", 2);
    this->define("R3", 3);
    this->define("R4", 4);
    this->define("R5", 5);
    this->define("R6", 6);
    this->define("R7", 7);
    this->define("R8", 8);
    this->define("R9", 9);
    this->define("R10", 10);
    this->define("R11", 11);
    this->define("R12", 12);
    this->define("R13", 13);
    this->define("R14", 14);
    this->define("R15", 15);

    this->define("SP", 0);
    this->define("LCL", 1);
    this->define("ARG", 2);
    this->define("THIS", 3);
    this->define("THAT", 4);
    this->define("SCREEN", 16384);
    this->define("KBD", 24576);

}

void SymbolTable::define(const string& key, const int& value) {
    table.insert(pair<string, int>(key, value));
}

int SymbolTable::addressOf(const string& name) {
    map<string, int>::iterator itr;
    itr = table.find(name);
    if (itr != table.end()) {
        int value = itr->second;
        return value;
    } else {
        return -1;
    }
}
