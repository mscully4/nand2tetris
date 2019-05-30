#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

#include "Parser.h"

using namespace std;

Parser::Parser(const string& path, bool init) {
    infile.open(path);
    if (init) {
        vector<string> init {"call", "Sys.init", "0"};
        this->commands.push_back(init);
    }
    if (infile) {
        while (getline(infile, line)) {
            if (!(line[0] == '/' && line[1] == '/') && !(isspace(line[0]))) {
                trim(line);
                std::istringstream iss(line);
                results = vector<string>(istream_iterator<string>{iss}, istream_iterator<string>());
                this->commands.push_back(results);
            }
        }
    }
}

void Parser::trim(string& s, const string& delimiters) {
    s.erase(s.find_last_not_of(delimiters) + 1).erase(0, s.erase(s.find_last_not_of(delimiters) + 1).find_first_not_of(delimiters));
}
