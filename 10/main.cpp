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
        string infile = get_current_path() + "/" + argv[1];
        struct stat buffer;
        cout << infile.c_str() << endl;
        if (stat(infile.c_str(), &buffer) == 0) {
            if (S_ISREG(buffer.st_mode)) {
                string outfile = argv[1];
                outfile = get_current_path() + "/" + outfile.replace(outfile.length()-4, 4, ".xml");
                cout << "FILE" << endl;
                JackTokenizer tokenizer(infile);
                CompilationEngine engine(outfile, &tokenizer);
           } else if (S_ISDIR(buffer.st_mode)) {
                cout << "DIR" << endl;
                string destination = get_current_path() + "/" + argv[1];
                DIR * dir;
                struct dirent * ent;
                dir = opendir(argv[1]);
                while ((ent=readdir(dir)) != NULL) {
                    string x = (string)argv[1] + ent->d_name;
                    stat(x.c_str(), &buffer);
                    if (S_ISREG(buffer.st_mode)) {
                        int len = x.length();
                        if (x[len-5] == '.' && x[len-4] == 'j' && x[len-3] == 'a' && x[len-2] == 'c' && x[len-1] == 'k') {
                            cout << x << endl;
                            string in = string(get_current_path()) + string("/") + x;
                            string out = string(get_current_path()) + string("/") + x.replace(len-5, 5, ".xml");
                            JackTokenizer * tokenizer = new JackTokenizer(in);
                            CompilationEngine * engine = new CompilationEngine(out, tokenizer);
                            delete tokenizer;
                            delete engine;
                        }
                    }
                }
           } else {
                cerr << "FILE/DIRECTORY DOES NOT EXIST" << endl;
            }
        }
    }
}

