#include<string>
#include<iostream>
#include<vector>
#include<bitset>
#include<map>
#include "Assembler.h"
#include "SymbolTable.h"
#include "trim.h"

using namespace std;

Assembler::Assembler(const string& path, SymbolTable * table) {
    ifstream infile;
    infile.open(path);
    if (infile) {
        string line, address, command;
        int location = 16, value = 0;
        while (getline(infile, line)) {
            trim(line);
            if (!(line[0] == '/' && line[1] == '/') && !isspace(line[0]) && (int)line[0]) {
                if (line[0] == '@') {
                    line.erase(0, 1);
                    if (isdigit(line[0])) {
                        address = bitset<16> (stoi(line, nullptr, 10)).to_string();
                        cout << address << endl;
                        output.push_back(address);
                    } else {
                        value = table->addressOf(line);
                        if (value >= 0) {
                            output.push_back(bitset<16>(value).to_string());
                        } else {
                            table->define(line, location);
                            address = bitset<16>(location).to_string();
                            cout << address << endl;
                            output.push_back(address);
                            ++location;
                        }
                    }
                } else if (line[0] != '(') {
                    command = "1110000000000000";
                    for (int i=0; i<line.length(); ++i) {
                        if (line[i] == '/') {
                            break;
                        }
                        else if (line[i] == '=') {
                            for (int z=0; z<i; ++z) {
                                if (line[z] == 'A') {
                                    command[10] = '1';
                                } else if (line[z] == 'D') {
                                    command[11] = '1';
                                } else if (line[z] == 'M') {
                                    command[12] = '1'; 
                                } else if (line[z] == '/') {
                                    break;
                                }         
                            }
                            for (int q=i+1; q<line.length(); q++) {
                                if (line[q] == '/') break;
                                if (line[q] == 'M' || line[q+1] == 'M') {
                                    command[3] = '1';
                                }
                                if ((line[q] == 'M' || line[q] == 'A')) {
                                    command[4] = '1';
                                    command[5] = '1';
                                }
                                if (line[q] == 'D') {
                                    command[6] = '1';
                                    command[7] = '1';
                                }       
                                if (line[q] == '+') {
                                    command[8] = '1';
                                    if (line[q+1] == 'A' || line[q+1] =='D' || line[q+1] == 'M') {
                                        command[4] = '0';
                                        command[5] = '0';
                                        command[6] = '0';
                                        command[7] = '0';
                                        break;
                                    } else if (line[q+1] == '1') {
                                        if (line[q-1] == 'M' || line[q-1] == 'A') {
                                            command[4] = '1';
                                            command[5] = '1';
                                            command[6] = '0';
                                            command[7] = '1';
                                            command[9] = '1';
                                            break;
                                        } else if (line[q-1] == 'D') {
                                            command[4] = '0';
                                            command[5] = '1';
                                            command[6] = '1';
                                            command[7] = '1';
                                            command[9] = '1';
                                            break;
                                        }
                                    }
                                }
                                if (line[q] == '-') {
                                    command[8] = '1';
                                    if ((line[q-1] == 'A' || line[q-1] == 'M') && line[q+1] == 'D') {
                                        command[4] = '0';
                                        command[5] = '0';
                                        command[6] = '0';
                                        command[7] = '1';
                                        command[9] = '1';
                                        break;
                                    } else if (line[q-1] == 'D' && (line[q+1] == 'A' || line[q+1] == 'M')) {
                                        command[4] = '0';
                                        command[5] = '1';
                                        command[6] = '0';
                                        command[7] = '0';
                                        command[9] = '1';
                                        break;
                                    } else if (line[q+1] == '1' && line[q-1] == '=') {
                                        command[4] = '1';
                                        command[5] = '1';
                                        command[6] = '1';
                                        break; 
                                    } else if (line[q+1] == '1' && line[q-1] != '=') {
                                        if (line[q-1] == 'M' || line[q-1] == 'A') {    
                                            command[4] = '1';
                                            command[5] = '1';
                                            command[6] = '0';
                                            command[7] = '0';
                                            break; 
                                        } else if (line[q-1] == 'D') {
                                            command[4] = '0';
                                            command[5] = '0';
                                            command[6] = '1';
                                            command[7] = '1';
                                            break;
                                        }
                                    } 
                                }
                                 
                                if (line[q] == '0') {
                                    command[4] = '1';
                                    command[6] = '1';
                                    command[8] = '1';
                                }
                                if (line[q] == '1') {
                                    command[4] = '1';
                                    command[5] = '1';
                                    command[6] = '1';
                                    command[7] = '1';
                                    command[8] = '1';
                                    command[9] = '1';
                                    break;
                                }
                                
                                if (line[q] == '!') {
                                    command[9] = '1';
                                }
                                if (line[q] == '|') {
                                    command[4] = '0';
                                    command[5] = '1';
                                    command[6] = '0';
                                    command[7] = '1';
                                    command[8] = '0';
                                    command[9] = '1';
                                    break;
                                }
                                if (line[q] == '&') {
                                    command[4] = '0';
                                    command[5] = '0';
                                    command[6] = '0';
                                    command[7] = '0';
                                    command[8] = '0';
                                    command[9] = '0';
                                    break;
                                }
                            }
                        }
                        if (line[i] == ';') {
                            if (line[i-1] == 'M' || line[i-1] == 'A') {
                                    command[4] = '1';
                                    command[5] = '1';
                            }
                            if (line[i-1] == 'D') {
                                command[6] = '1';
                                command[7] = '1';
                            }
                            if (line[i-1] == '0') {
                                command[4] = '1';
                                command[6] = '1';
                                command[8] = '1';
                            } 
                            if (line[i+2] == 'G') {
                                command[15] = '1';
                                if (line[i+3] == 'E') {
                                    command[14] = '1';
                                }
                            } else if (line[i+2] == 'L') {
                                command[13] = '1';
                                if (line[i+3] == 'E') {
                                    command[14] = '1';
                                }
                            } else if (line[i+2] == 'E') {
                                command[14] = '1';
                            } else if (line[i+2] == 'N') {
                                command[13] = '1';
                                command[15] = '1';
                            } else if (line[i+2] == 'M') {
                                command[13] = '1';
                                command[14] = '1';
                                command[15] = '1';
                            } else {
                                cerr << "Invalid Syntax" << endl;
                                break;
                            }
                        }
                    }
                    output.push_back(command);
                }
            }
        }
    }
    ofstream outfile;
    outfile.open(path.substr(0, path.length() - 3) + "hack");
    for (int i=0; i<output.size(); ++i) {
        outfile << output[i] << endl;
    }
}
