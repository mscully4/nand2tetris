#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

#include "Parser.h"
#include "CodeWriter.h"

using namespace std;

string get_current_path() {
    char temp[200];
    return (getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

int main(int argc, char * argv[]) {
    if (argc == 2) {
        string path = get_current_path() + "/" + argv[1];
        struct stat buffer;
        if (stat(path.c_str(), &buffer) == 0) {
            if (S_ISREG(buffer.st_mode)) {
                cout << "FILE" << endl << endl;
                Parser parser(path, false);
                CodeWriter codewriter(parser.commands, "lajds");
                string destination = ((string)argv[1]);
                destination = get_current_path() + "/" + destination.replace(destination.length() - 3, 3, ".vm");
                codewriter.writer(destination);
            } else if (S_ISDIR(buffer.st_mode)) {
                cout << "DIRECTORY" << endl;
                string destination = ((string)argv[1]);
                if (destination.back() == '/') destination.pop_back();
                string out;
                for (int i=destination.length(); i>0; --i) {
                    if (destination[i] == '/') {
                        out = destination.substr(i, destination.length() - i);
                        break;
                    }
                }
                cout << destination << out << endl;
                destination = get_current_path() + "/" + destination + out + ".asm";
                cout << destination << endl;
                //write the bootstrap code into the destination file
                ofstream outfile;
                outfile.open(destination);
                outfile << "@256\nD=A\n@SP\nM=D\n" << endl;
                outfile.close();
                //translate the Sys.vm file first
                Parser sys_parser((string)argv[1] + "Sys.vm", true);
                CodeWriter sys_codewriter(sys_parser.commands, "SYS");
                sys_codewriter.writer(destination);
                //then move on until everything else
                DIR * dir;
                struct dirent * ent;
                if ((dir = opendir(argv[1])) != NULL) {
                   struct stat buffer;
                    while ((ent=readdir(dir)) != NULL) {
                        string x = (string)argv[1] + ent->d_name;
                        stat(x.c_str(), &buffer);
                        if (S_ISREG(buffer.st_mode)) {
                            if (string(1, x[x.length()-3]) + string(1, x[x.length()-2]) + string(1, x[x.length()-1]) == ".vm" &&
                                string(1, x[x.length()-6]) + string(1, x[x.length()-5]) + string(1, x[x.length()-4]) != "Sys") {
                                string filename = "";
                                for (int i=x.length(); i > 0; --i) {
                                    if (x[i] == '/') {
                                        for (int j=i+1; j<x.length()-3; ++j) {
                                            filename += x[j];
                                        }
                                        break;
                                    }
                                }
                                Parser parser(x, false);
                                CodeWriter codewriter(parser.commands, filename);
                                codewriter.writer(destination);
                            }
                        }
                    }
                }
            }
        }
    } else {
        cerr << "Invalid Input" << endl;
        return 1;
    }
};
