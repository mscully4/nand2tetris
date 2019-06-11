#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SymbolTable.h"
#include "Assembler.h"
#include "trim.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        SymbolTable table;
        ifstream infile;
        infile.open(argv[1]);
        vector<string> output;
        int value = 0, line_c = 0;
        if (infile) {
            string line;
            while(getline(infile, line)) {
                trim(line);
                if ((int)line[0] && (line[0] != '/' && line[0] != '(')) ++line_c;
                if (line[0] == '(') {
                    line.erase(0, 1);
                    line.erase(line.length() - 1, line.length());
                    trim(line);
                    table.define(line, line_c);
                    /*if (line == "END_EQ") {
                        cout << table.addressOf("END_EQ") << endl;
                    }*/
                }
            }
        }
        Assembler assembler(argv[1], &table);
    }
}
