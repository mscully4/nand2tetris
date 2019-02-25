#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

string get_current_path() {
    char temp[200];
    return (getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

int main(int argc, char * argv[]) {

    if (argc == 2) {
        string infile = get_current_path() + "/" + argv[1];
        string outfile = get_current_path() + "/boof.xml";
        struct stat buffer;
        if (stat(infile.c_str(), &buffer) == 0) {
            if (S_ISREG(buffer.st_mode)) {
                cout << "FILE" << endl;
                JackTokenizer tokenizer(infile);
                CompilationEngine engine(outfile, &tokenizer);
           } else if (S_ISDIR(buffer.st_mode)) {
                cout << "DIR" << endl;
            } else {
                cerr << "FILE/DIRECTORY DOES NOT EXIST" << endl;
            }
        }
    }
}

