#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sys/stat.h>
#include<unistd.h>
#include<cctype>
#include<vector>
#include<algorithm>
#include "JackTokenizer.h"

using namespace std;

void trim(string& s, const string& delimiters = " \f\n\r\t\v" )
{
    s.erase(s.find_last_not_of(delimiters) + 1).erase(0, s.erase(s.find_last_not_of(delimiters) + 1).find_first_not_of(delimiters));
}

void remove_comments(string& boof) {
    string chork;
    for (int i=0; i<boof.length(); ++i) {
        if (boof[i] == '/' && boof[i-1] == '/') {
            boof = boof.substr(0, i-1);
        }
    }
}

void split(string& s, vector<string>& tokens) {
    vector<int> positions {};
    string delimiters = " ,.-~;*()[]{}\"";
    for (int i=0; i<s.length(); ++i) {
        if (delimiters.find(s[i]) != string::npos) {
            positions.push_back(i);
        }
    }
    string buffer;
    for (int j=0; j<positions.size(); ++j) {
        if (s[positions[j]] == '"') {
            string literal = s.substr(positions[j], s.find('"', positions[j]+1) - positions[j] + 1);
            tokens.push_back(literal);
            ++j;
            while(s[positions[j]] != '"') {
                ++j;
            };
        } else if (s[positions[j]] == ' ') {  
            buffer = s.substr((j == 0) ? 0 : (positions[j-1] + 1), (j == 0) ? positions[j] : positions[j] - positions[j-1] - 1);
            if (buffer.length() > 0) {
                tokens.push_back(buffer);
            }
        } else if (s[positions[j]] == ',' || s[positions[j]] == ';' || s[positions[j]] == '(' || s[positions[j]] == ')' || s[positions[j]] == '.' || s[positions[j]] == '[' || s[positions[j]] == ']') {
            buffer = s.substr((j == 0) ? 0 : (positions[j-1] + 1), (j == 0) ? positions[j] : positions[j] - positions[j-1] - 1);
            if (buffer.length() > 0) tokens.push_back(buffer); 
            buffer = s.substr(positions[j], 1);
            if (buffer.length() > 0) tokens.push_back(buffer); 
        } else {
            tokens.push_back(s.substr(positions[j], 1));
        }
    }
}

JackTokenizer::JackTokenizer(const string& path) {
    cout << "JACK" << endl;
    ifstream infile(path);
    string line;
    int counter = 0;
    if (infile) {
        while (getline(infile, line)) {
            remove_comments(line);
            trim(line);
            if (!line.empty() && (line.find("/*") == string::npos)) {
                split(line, tokens);
            } else if (!line.empty()) {
                //block comment opener found
                if (line.find("*/") != string::npos) {
                    //block comment closes on the same line it opened
                    line.erase(line.find("/*"), line.find("*/")+2);
                    //block comment is removed from line
                    if (!line.empty()) {
                        //the block comment has been removed from the line, but there is actual code left on the line
                        split(line, tokens);
                        cout << line << endl;
                    }
                } else {
                    //block comment that spans multiple lines
                    //get new line until we find the closing bracket for the block comment
                    while (getline(infile, line)) {
                        //found it, now stop reading new lines
                        if (line.find("*/") != string::npos) {
                            break;
                        }
                    } 
                    line.erase(0, line.find("*/") + 2);
                    if (!line.empty()) {
                        split(line, tokens);
                    }
                }
            }
            ++counter;
        }
    }
};

string JackTokenizer::tokenType(string token) {
    vector<string> keywords{"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};
    string type;
    if (isdigit(token[0]) == 1) {
        type = "integerConstant";
    } else if (token[0] == '"' && token[token.length() - 1] == '"') {
        type = "stringConstant";
    } else if (find(keywords.begin(), keywords.end(), token) != keywords.end()) {
        type = "keyword";
    } else if (token.length() == 1 || token == "&lt" || token == "&gt") {
        int ascii = token[0];
        if ((ascii > 122 && ascii < 127) || (ascii > 58 && ascii < 63) || (ascii > 39 && ascii < 48) || (ascii > 90 && ascii < 94) || ascii == 38 || token == "&lt" || token == "&rt") {
            type = "symbol";
        } else {
            type = "identifier";
        }
    } else {
        type = "identifier";
    }
    return type;
}

bool JackTokenizer::has_more_tokens() {
    if (iterator > tokens.size() - 1) {
        return false;
    } else {
        return true;
    }
}

void JackTokenizer::advance() {
    this->iterator += 1;
   // cout << tokens[this->iterator] << endl;
}


