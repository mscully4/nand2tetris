#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <functional>
#include <cstring>
#include <cstdlib>
#include "HashTable/HashTable.h"

using namespace std;

void trim(string& s, const string& delimiters = " \f\n\r\t\v" )
{
    s.erase( s.find_last_not_of( delimiters ) + 1 ).erase( 0, s.erase( s.find_last_not_of( delimiters ) + 1 ).find_first_not_of( delimiters ) );
}

int main(int argc, char* argv[]) {
    HashTable<string, int> table(11);
    //load the constants into the Hash Table
    table.put("R0", 0);
    table.put("R1", 1);
    table.put("R2", 2);
    table.put("R3", 3);
    table.put("R4", 4);
    table.put("R5", 5);
    table.put("R6", 6);
    table.put("R7", 7);
    table.put("R8", 8);
    table.put("R9", 9);
    table.put("R10", 10);
    table.put("R11", 11);
    table.put("R12", 12);
    table.put("R13", 13);
    table.put("R14", 14);
    table.put("R15", 15);

    table.put("SP", 0);    
    table.put("LCL", 1);    
    table.put("ARG", 2);    
    table.put("THIS", 3);    
    table.put("THAT", 4);    
    table.put("SCREEN", 16384);    
    table.put("KBD", 24576);    

    ifstream infile, infile_copy;
    infile.open(argv[1]);
    infile_copy.open(argv[1]);
    ofstream outfile;
    vector<string> output;
    int location = 16, value = 0, line_c = 0;
    bool result;
    hash<string> hasher;
    if (infile_copy) {
        string line;
        while (getline(infile_copy, line)) {
            trim(line);
            if ((int)line[0] && (line[0] != '/' && line[0] != '(')) ++line_c;
            if (!(line[0] == '/' && line[1] =='/') && (int)line[0]) {
                if (line[0] == '(') {
                    line.erase(0, 1);
                    for (int locator = 0; locator < line.length(); ++locator) {
                        if (line[locator] == ')') {
                            line.erase(locator, line.length() - locator);
                            break;
                        }
                    }
                    trim(line);
                    table.put(line, line_c);
                    if (line == "END_EQ") {
                        int ds = 0;
                        table.get("END_EQ", ds);
                        //cout << ds << endl;
                    }
                }
            }
        }
    }
    //table.print_all();

    if (infile) {
        string line, address, command ;
        while (getline(infile, line)) {
            trim(line);
            //cout << line << endl;
            if (!(line[0] == '/' && line[1] == '/') && !isspace(line[0]) && (int)line[0]) {
                if (line[0] == '@') {
                    line.erase(0, 1);
                    if (isdigit(line[0])) {
                        address = bitset<16> (stoi(line, nullptr, 10)).to_string();
                        output.push_back(address);
                    } else {
                        bool result = table.get(line, value);
                        if (result) {
                            output.push_back(bitset<16>(value).to_string());    
                        } else {
                            table.put(line, location);
                            output.push_back(bitset<16>(location).to_string());
                            ++location;
                        }
                    }
                } else if (line[0] != '(') {
                    command = "1110000000000000";
                    for (int i=0; i<line.length(); ++i) {
                        if (line[i] == '/') {break;}
                        else if ((int)line[i] == (int)'=') {
                            //cout << line << " " << i << endl;
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
                                return 1;
                            }
                        }
                    }
                    //cout << command << endl;
                    output.push_back(command); 
                }   
            }
        }
    }
    //cout << argv[2] << endl;
    outfile.open(argv[2]);
    for (int j=0; j<output.size(); j++) {
        //cout << output[j] << endl;
        outfile << output[j] << endl;
    }
}
