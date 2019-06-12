#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

string get_current_path() {
    char temp[200];
    return (getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

int main(int argc, char * argv[]) {

    if (argc == 2) {
        string infile, outfile, fileName;
        fileName = argv[1];
        infile = get_current_path() + "/" + fileName;
        cout << infile.c_str() << endl;
        struct stat buffer;
        if (stat(infile.c_str(), &buffer) == 0) {
            if (S_ISREG(buffer.st_mode)) {
                cout << "FILE" << endl;
                outfile = get_current_path() + "/" + fileName.substr(0, fileName.length() - 5) + ".vm";
                JackTokenizer tokenizer(infile);
                CompilationEngine engine(outfile, &tokenizer);
            } else if (S_ISDIR(buffer.st_mode)) {
                cout << "DIR" << endl;
                string destination = get_current_path() + "/" + fileName.substr(0, fileName.length() - 5) + ".vm"; 
                DIR * dir;
                struct dirent * ent;
                if ((dir = opendir(argv[1])) != NULL) {
                    struct stat buffer;
                    while ((ent=readdir(dir)) != NULL) {
                        string x = (string)argv[1] + ent->d_name;
                        stat(x.c_str(), &buffer);
                        if (S_ISREG(buffer.st_mode)) {
                            int len = x.length();
                            if (x[len-5] == '.' && x[len-4] == 'j' && x[len-3] == 'a' && x[len-2] == 'c' && x[len-1] == 'k') {
                                string in = string(get_current_path()) + string("/") + x;
                                string out = string(get_current_path()) + string("/") + x.replace(len-5, 5, ".vm");
                                cout << in << endl;
                                JackTokenizer * tokenizer = new JackTokenizer(in);
                                CompilationEngine * engine = new CompilationEngine(out, tokenizer);
                                delete tokenizer;
                                delete engine;
                            }
                        }
                    }
                }
            } else {
                cerr << "FILE/DIRECTORY DOES NOT EXIST" << endl;
            }
        }
    }
}

