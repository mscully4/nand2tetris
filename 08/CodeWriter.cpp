#include "CodeWriter.h"

CodeWriter::CodeWriter(const vector<vector<string>>& commands, string filename) {
    labels = 0;        
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
                    results.push_back("@" + filename + "." + commands[i][2]);
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
                    results.push_back("@" + filename + "." + commands[i][2]);
                    results.push_back("D=A");
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
                //this needs a label after the name of the function
                results.push_back("@" + commands[i][1]);
                results.push_back("0; JMP");
            } else if (commands[i][0] == "if-goto") {
                results.push_back("@SP");
                results.push_back("AM=M-1");
                results.push_back("D=M");
                //this needs a label after the name of the function
                results.push_back("@" + commands[i][1]);
                results.push_back("D; JNE");
            } 
            //Function Calling
            else if (commands[i][0] == "function") {
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
                results.push_back("@RA" + to_string(getStaticNumber(true)));
                results.push_back("D=A");
                results.push_back("@SP");
                results.push_back("AM=M+1");
                results.push_back("A=A-1");
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
                results.push_back("D=M-D");
                results.push_back("@ARG");
                results.push_back("M=D");

                //change LCL to the stack pointer and increment the stack pointer by the number of local variables
                results.push_back("@SP");
                results.push_back("D=M");
                results.push_back("@LCL");
                results.push_back("M=D");
                results.push_back("@" + commands[i][2]);
                results.push_back("D=A");
                results.push_back("@SP");
                results.push_back("D=M+D");

                //jump to the function
                results.push_back("@" + commands[i][1]);
                results.push_back("0; JMP");

                //provide a return label
                results.push_back("(RA" + to_string(getStaticNumber(false)) + ")");
            } else if (commands[i][0] == "return") {
                //put the return address in a temp variable
                results.push_back("@5"); 
                results.push_back("D=A");
                results.push_back("@LCL");
                results.push_back("A=M-D");
                results.push_back("D=M");
                results.push_back("@RET");
                results.push_back("M=D");

                //reposition the return value
                results.push_back("@SP");
                results.push_back("AM=M-1");
                results.push_back("D=M");
                results.push_back("M=0");
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
                results.push_back("A=M-1");
                results.push_back("D=M");
                results.push_back("M=0");
                results.push_back("@THAT");
                results.push_back("M=D");

                //restore the THIS of the caller
                results.push_back("@2");
                results.push_back("D=A");
                results.push_back("@LCL");
                results.push_back("A=M-D");
                results.push_back("D=M");
                results.push_back("M=0");
                results.push_back("@THIS");
                results.push_back("M=D");

                //restore the ARG of the caller
                results.push_back("@3");
                results.push_back("D=A");
                results.push_back("@LCL");
                results.push_back("A=M-D");
                results.push_back("D=M");
                results.push_back("M=0");
                results.push_back("@ARG");
                results.push_back("M=D");

                //restore the LCL of the caller
                results.push_back("@4");
                results.push_back("D=A");
                results.push_back("@LCL");
                results.push_back("A=M-D");
                results.push_back("D=M");
                results.push_back("M=0");
                results.push_back("@LCL");
                results.push_back("M=D");

                //go to the return address of the caller
                results.push_back("@RET");
                results.push_back("A=M");
                results.push_back("0; JMP");
            }
        }
    }


void CodeWriter::writer(const string & path) {
    outfile.open(path, ios_base::app);
    for (int x=0; x<results.size(); ++x) {
        outfile << results[x] << endl;
    }
    outfile.close();
}

int CodeWriter::getStaticNumber(bool inc) {
    int static number = 0;
    if (inc) {
        return ++number;
    } else {
        return number;
    }
}
