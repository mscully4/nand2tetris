#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

class JackTokenizer {
    public:
        vector<string> tokens;
        int iterator = 0;
        JackTokenizer(const string& path);
        string tokenType(string token);
        bool has_more_tokens();
        void advance();
};

