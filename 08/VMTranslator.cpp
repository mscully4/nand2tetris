#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cctype>
#include <sstream>
#include <iterator>

using namespace std;

string get_current_path() {
    char temp[200];
    return (getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

void trim(string& s, const string& delimiters = " \f\n\r\t\v" )
{
    s.erase(s.find_last_not_of(delimiters) + 1).erase(0, s.erase(s.find_last_not_of(delimiters) + 1).find_first_not_of(delimiters));
}


class Parser {
    public:
        vector<vector<string>> commands;
        Parser(const string& path) {
            ifstream infile;
            infile.open(path);
            string line;
            if (infile) {
                while (getline(infile, line)) {
                    if (!(line[0] == '/' && line[1] == '/') && !(isspace(line[0]))) {
                        trim(line);
                        std::istringstream iss(line);
                        vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());
                        this->commands.push_back(results);
                    }
                }
            }
        }
};

class CodeWriter {
    public:
        vector<string> results;
        int labels = 0;
        CodeWriter(const vector<vector<string>>& commands) {
            for (int i=0; i<commands.size(); i++) {
                if (commands[i][0] == "push") {
                    if (commands[i][1] == "local") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@LCL");
                        results.push_back("A=M+D");
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("A=M");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                    } else if (commands[i][1] == "argument") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@ARG");
                        results.push_back("A=M+D");
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("A=M");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                    } else if (commands[i][1] == "static") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@R16");
                        results.push_back("A=A+D");
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                        results.push_back("A=M-1");
                        results.push_back("M=D");
                    } else if (commands[i][1] == "this") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@THIS");
                        results.push_back("A=M+D");
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("A=M");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                    } else if (commands[i][1] == "that") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@THAT");
                        results.push_back("A=M+D");
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("A=M");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                    } else if (commands[i][1] == "temp") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@R5");
                        results.push_back("A=A+D");
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("A=M");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                    } else if (commands[i][1] == "pointer") {
                        if (commands[i][2] == "0") {
                            results.push_back("@THIS");
                        } else if (commands[i][2] == "1") {
                            results.push_back("@THAT");
                        }
                        results.push_back("D=M");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                        results.push_back("A=M-1");
                        results.push_back("M=D");
                    }
                    else if (commands[i][1] == "constant") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@SP");
                        results.push_back("A=M");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("M=M+1");
                    }
                } else if (commands[i][0] == "pop") {
                    if (commands[i][1] == "local") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@LCL");
                        results.push_back("D=M+D");
                        results.push_back("@R13");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R13");
                        results.push_back("A=M");
                        results.push_back("M=D");
                    } else if (commands[i][1] == "argument") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@ARG");
                        results.push_back("D=M+D");
                        results.push_back("@R14");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R14");
                        results.push_back("A=M");
                        results.push_back("M=D");
                    } else if (commands[i][1] == "static") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@R16");
                        results.push_back("D=A+D");
                        results.push_back("@R15");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R15");
                        results.push_back("A=M");
                        results.push_back("M=D");
                    } else if (commands[i][1] == "this") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@THIS");
                        results.push_back("D=M+D");
                        results.push_back("@R13");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R13");
                        results.push_back("A=M");
                        results.push_back("M=D");
                    } else if (commands[i][1] == "that") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@THAT");
                        results.push_back("D=M+D");
                        results.push_back("@R14");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R14");
                        results.push_back("A=M");
                        results.push_back("M=D");
                    } else if (commands[i][1] == "pointer") {
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R3");
                        if (commands[i][2] == "1") {
                            results.push_back("A=A+1");
                        }
                        results.push_back("M=D");
                    } else if (commands[i][1] == "temp") {
                        results.push_back("@" + commands[i][2]);
                        results.push_back("D=A");
                        results.push_back("@R5");
                        results.push_back("D=A+D");
                        results.push_back("@R15");
                        results.push_back("M=D");
                        results.push_back("@SP");
                        results.push_back("AM=M-1");
                        results.push_back("D=M");
                        results.push_back("@R15");
                        results.push_back("A=M");
                        results.push_back("M=D");
                    }
                }
                else if (commands[i][0] == "add") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("M=M+D");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                }
                else if (commands[i][0] == "sub") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("M=M-D");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                }
                else if (commands[i][0] == "neg") {
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("M=-M");
                }
                else if (commands[i][0] == "and") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("M=M&D");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                }
                else if (commands[i][0] == "or") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("M=M|D");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                }
               else if (commands[i][0] == "not") {
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("M=!M");
                }
                else if (commands[i][0] == "eq") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("MD=M-D");
                    results.push_back("M=-1");
                    results.push_back("@END" + to_string(labels));
                    results.push_back("D; JEQ");
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("M=0");
                    results.push_back("(END" + to_string(labels++) + ")");
                }
                else if (commands[i][0] == "gt") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("MD=M-D");
                    results.push_back("M=-1");
                    results.push_back("@END" + to_string(labels));
                    results.push_back("D; JGT");
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("M=0");
                    results.push_back("(END" + to_string(labels++) + ")");
                }
                else if (commands[i][0] == "lt") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("MD=M-D");
                    results.push_back("M=-1");
                    results.push_back("@END" + to_string(labels));
                    results.push_back("D; JLT");
                    results.push_back("@SP");
                    results.push_back("A=M-1");
                    results.push_back("M=0");
                    results.push_back("(END" + to_string(labels++) + ")");
                } else if (commands[i][0] == "label") {
                    results.push_back("(" + commands[i][1] + ")");
                } else if (commands[i][0] == "goto") {
                    results.push_back("@" + commands[i][1]);
                    results.push_back("0; JMP");
                } else if (commands[i][0] == "if-goto") {
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@" + commands[i][1]);
                    results.push_back("D; JNE");
                } else if (commands[i][0] == "function") {
                    results.push_back("(" + commands[i][1] + ")");
                    results.push_back("@SP");
                    results.push_back("A=M");
                    int iterations = stoi(commands[i][2]);
                    for (int i=0; i<iterations; ++i) {
                        results.push_back("M=0");
                        results.push_back("A=A+1");
                    }
                } else if (commands[i][0] == "call") {
                    //storing the return address
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                    results.push_back("D=M-1");
                    results.push_back("A=D");
                    results.push_back("M=D");
                    
                    results.push_back("@LCL");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                    results.push_back("A=M-1");
                    results.push_back("M=D");

                    results.push_back("@ARG");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                    results.push_back("A=M-1");
                    results.push_back("M=D");
                    
                    results.push_back("@THIS");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                    results.push_back("A=M");
                    results.push_back("M=D");
                    
                    results.push_back("@THAT");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("M=M+1");
                    results.push_back("A=M");
                    results.push_back("M=D");

                    results.push_back("@" + commands[i][2]);
                    results.push_back("D=A");
                    results.push_back("@5");
                    results.push_back("D=D+A");
                    results.push_back("@SP");
                    results.push_back("D=A-D");
                    results.push_back("@ARG");
                    results.push_back("M=D");

                    results.push_back("@SP");
                    results.push_back("D=A");
                    results.push_back("@LCL");
                    results.push_back("M=D");

                    results.push_back("@" + commands[i][1]);
                    
                    results.push_back("(" + commands[i][1] + "$" + commands[i][2] + ")");
                 } else if (commands[i][0] == "return") {
                    //store the return address in R15
                    results.push_back("@5");
                    results.push_back("D=A");
                    results.push_back("@LCL");
                    results.push_back("A=M-D");
                    results.push_back("D=M");
                    results.push_back("@R15");
                    results.push_back("M=D");
        
                    //reposition the return value
                    results.push_back("@SP");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("@ARG");
                    results.push_back("A=M");
                    results.push_back("M=D");

                    //restore the SP of the caller
                    results.push_back("@ARG");
                    results.push_back("D=M");
                    results.push_back("@SP");
                    results.push_back("M=D+1");

                    //restore the THAT of the caller
                    results.push_back("@LCL");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("M=0");
                    results.push_back("@THAT");
                    results.push_back("M=D");

                    //restore the THIS of the caller
                    results.push_back("@LCL");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("M=0");
                    results.push_back("@THIS");
                    results.push_back("M=D");

                    //restore the ARG of the caller
                    results.push_back("@LCL");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("M=0");
                    results.push_back("@ARG");
                    results.push_back("M=D");

                    //restore the LCL of the caller
                    results.push_back("@LCL");
                    results.push_back("AM=M-1");
                    results.push_back("D=M");
                    results.push_back("M=0");
                    results.push_back("@LCL");
                    results.push_back("M=D");

                    //go to the return address of the caller
                    results.push_back("@R15");
                    results.push_back("A=M");
                }
           }
            for (int j=0; j<results.size(); ++j) {
                cout << results[j] << endl;
            }    
        }

        void writer(const string & path) {
            ofstream outfile;
            outfile.open(path, ios_base::app);
            for (int x=0; x<results.size(); ++x) {
                outfile << results[x] << endl;
            }
            outfile.close();
        }
};

int main(int argc, char * argv[]) {
    if (argc == 3) {
        string path = get_current_path() + "/" + argv[1];
        struct stat buffer;
        if (stat(path.c_str(), &buffer) == 0) {
            if (S_ISREG(buffer.st_mode)) {
                cout << "FILE" << endl << endl;
                Parser parser(path);
                CodeWriter codewriter(parser.commands);
                string destination = get_current_path() + "/" + argv[2];
                codewriter.writer(destination);
            } else if (S_ISDIR(buffer.st_mode)) {
                cout << "DIRECTORY" << endl;
                string destination = get_current_path() + "/" + argv[2];
                //write the bootstrap code into the destination file
                ofstream outfile;
                outfile.open(destination);
                outfile << "@256\nD=A\n@SP\nM=D\n" << "@SP\nAM=M+1\nM=0\n" << "@SP\nAM=M+1\nM=0\n" << "@SP\nAM=M+1\nM=0\n" << "@SP\nAM=M+1\nM=0\n" << "@SP\nAM=M+1\nM=0\n" << endl;
                outfile.close();
                //translate the Sys.vm file first
                Parser sys_parser((string)argv[1] + "Sys.vm");
                CodeWriter sys_codewriter(sys_parser.commands);
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
                            if (string(1, x[x.length()-3]) + string(1, x[x.length()-2]) + string(1, x[x.length()-1]) == ".vm" && x != "Sys.vm") {
                                Parser parser(x);
                                //CodeWriter codewriter(parser.commands);
                                //codewriter.writer(get_current_path() + "/" + argv[2]);
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
