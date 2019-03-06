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

/*void split(string& s, char delimiter, vector<string>& tokens) {
    vector<string> chork;
    int pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        chork.push_back(token);
        s.erase(0, pos + 1);
    }
    if (s[s.length() - 1] == ';') {
        chork.push_back(s.substr(0, s.length() - 1));
        chork.push_back(";");
    } else {
        chork.push_back(s); 
    }
    
    for (int j=0; j<chork.size(); ++j) {
        string boof = chork[j];
        if (boof[0] == '(') {
            tokens.push_back("(");
            if (boof.length() > 1 && boof[1] == '-') {
                tokens.push_back("-");
                tokens.push_back(boof.substr(2, boof.length() - 3));
            } else {
                tokens.push_back(boof.substr(1, boof.length() - 2));
            }
            tokens.push_back(")");
        } else if (boof[boof.length() - 1] == ')') {
            //cout << boof << endl;
            int period, loc;
            if ((period = boof.find('.')) != string::npos) {
                tokens.push_back(boof.substr(0, period));
                boof.erase(0, period + 1);
                tokens.push_back(".");
            }
            if ((loc = boof.find('(')) != string::npos) {
                if (boof.find('(') + 1 != boof.find(')')) {
                    int lparen = boof.find('('), rparen = boof.find(')');
                    tokens.push_back(boof.substr(0, boof.find('(')));    
                    tokens.push_back(boof.substr(lparen, 1));    
                    tokens.push_back(boof.substr(lparen + 1, rparen - lparen - 1));
                    tokens.push_back(boof.substr(rparen, 1)); 
                } else {
                    tokens.push_back(boof.substr(0, loc));
                    boof.erase(0, loc + 1); 
                    tokens.push_back("(");
                    //figure out a way to parse parameters
                    tokens.push_back(")");
                }
            } else {
                tokens.push_back(boof.substr(0, boof.length() - 1));
                tokens.push_back(")");
            }
        } else if (boof[0] == '"') {
            if (boof.find(1, '"') != string::npos) {
                tokens.push_back(boof);
            } else {
                string str = boof.substr(0, boof.length());
                while (true) {
                    ++j;
                    str += " ";
                    if (chork[j].find('"') != string::npos) {
                        str += chork[j].substr(0, chork[j].length());
                        break;
                    } else {
                        str += chork[j];
                    }
                }
                tokens.push_back(str);
            }
        } else if (boof.find('[') != string::npos) {
            tokens.push_back(boof.substr(0, boof.find('[')));
            tokens.push_back(boof.substr(boof.find('['), boof.find('[')));
            tokens.push_back(boof.substr(boof.find('[') + 1, boof.find(']') - 2));
            tokens.push_back(boof.substr(boof.find(']'), boof.find(']')));
        } else if (boof.find(',') != string::npos) {
            tokens.push_back(boof.substr(0, boof.find(',')));
            tokens.push_back(",");
        } else if (boof.find('(') != string::npos) {
            if (boof.find('.') != string::npos) {
                int period = boof.find('.'), lparen = boof.find('(');
                cout << boof.substr(0, period) << endl;
                cout << boof.substr(period, 1) << endl;
                cout << boof.substr(period+1, lparen) << endl;
            } else {
                tokens.push_back(boof.substr(0, boof.find('(')));
                tokens.push_back("(");
                tokens.push_back(boof.substr(boof.find('(') + 1, boof.length() - 1));
            }
        } else {
            tokens.push_back(boof);
        }
    }
//    for (int i=0; i<tokens.size(); ++i) cout << tokens[i] << endl;
}*/

void split(string& s, vector<string>& tokens) {
    vector<int> positions {};
    string delimiters = " ,.-;*()[]{}\"";
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
    ifstream infile(path);
    string line;
    int counter = 0;
    if (infile) {
        while (getline(infile, line)) {
            remove_comments(line);
            trim(line);
            if (!line.empty() && !(line[0] == '/' && line[1] == '/') && !(line[0] == '/' && line[1] == '*')) {
                split(line, tokens);
            }
            ++counter;
            if (counter > 300) break;
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
    } else if (token.length() == 1) {
        int ascii = token[0];
        if ((ascii > 122 && ascii < 127) || (ascii > 58 && ascii < 63) || (ascii > 39 && ascii < 48) || (ascii > 90 && ascii < 94) || ascii == 38 || ascii == 40 || ascii == 41) {
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
}


