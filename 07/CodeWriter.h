#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

class CodeWriter {
    public:
        int labels;
        vector<string> results;
        ofstream outfile;
        CodeWriter(const vector<vector<string>>& commands, string filename);
        void writer(const string & path);
        int getStaticNumber(bool);
};

