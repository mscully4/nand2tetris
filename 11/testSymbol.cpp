#include "SymbolTable.h"

using namespace std;

int main() {
    SymbolTable table;
    table.define("chork", "int", "var");
    cout << table.varCount("var") << endl;
    cout << table.typeOf("chork") << " " << table.indexOf("chork") << " " << table.kindOf("chork") << endl;
    table.define("piss", "int", "var");
    cout << table.typeOf("piss") << " " << table.indexOf("piss") << " " << table.kindOf("piss") << endl;
    table.startSubroutine("BOOF");


    table.define("piss", "boolean", "argument");
    cout << table.typeOf("piss") << " " << table.indexOf("piss") << " " << table.kindOf("piss") << endl;
    table.define("chork", "int", "argument");
    cout << table.typeOf("chork") << " " << table.indexOf("chork") << " " << table.kindOf("chork") << endl;
}
