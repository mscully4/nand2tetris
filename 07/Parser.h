#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;

class Parser {
    public:
        vector<vector<string>> commands;
        ifstream infile;
        string line;
        vector<string> results;
        Parser(const string&, bool);
        void trim(string&, const string& = " \f\n\r\t\v");
};

