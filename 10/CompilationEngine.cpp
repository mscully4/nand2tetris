#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cctype>
#include<vector>
#include<algorithm>
#include "CompilationEngine.h"
#include "JackTokenizer.h"

using namespace std;

CompilationEngine::CompilationEngine(const string& path, JackTokenizer * tokenizer) {
    this->tokenizer = tokenizer;
    this->outfile.open(path);
    compile_class();
};

void CompilationEngine::write(const string& token, const string& type) {
    bool alt = false;
    string output = token;
    if (type == "symbol") {
        if (token == ">") {
            output = "&gt;";
            alt = true;
        } else if (token == "<") {
            output = " &lt;";
            alt = true;
        } else if (token == "&") {
            output = "&amp;";
        }
    }
    outfile << "<" + type + "> " + output + " </" + type + ">" << endl;
}

void CompilationEngine::compile_class() {
    outfile << "<class>" << endl;
    while (tokenizer->has_more_tokens()) {
        token = tokenizer->tokens[tokenizer->iterator];
        if (token == "static" || token == "field") {
            compile_class_var_dec();
        } else if (token == "function" || token == "constructor" || token == "method") {
            compile_subroutine();
        }
        else {
            write(token, tokenizer->tokenType(token));
            if (tokenizer->has_more_tokens()) {
                tokenizer->advance();
            } else {
                break;
            }   
        }
    }
    //outfile << tokenizer->tokens[tokenizer->iterator] << endl;
    outfile << "</class>" << endl;
}

void CompilationEngine::compile_class_var_dec() {
    outfile << "<classVarDec>" << endl;
    string token;
    while (true) {
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        if (token == ";" && (tokenizer->tokens[tokenizer->iterator + 1] != "static" || tokenizer->tokens[tokenizer->iterator + 1] != "field")) {
            tokenizer->advance();
            break;
        } 
        tokenizer->advance();
        
    }
    outfile << "</classVarDec>" << endl;
}

void CompilationEngine::compile_subroutine() {
    outfile << "<subroutineDec>" << endl;
    for (int i=0; i<3; ++i) {
        string token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    }
 
    bool params = true, body = true;
    int curly = 1;
    while (true) {
        token = tokenizer->tokens[tokenizer->iterator];
        if (token == "(" && params == true) {
            params = false;
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            compile_parameter_list();
        } else if (token == "{" && body == true) {
            body = false;
            compile_subroutine_body();
        }
        
        if (!(body || params)) {
            outfile << "</subroutineDec>" << endl;
            break;
        } else if (tokenizer->has_more_tokens()) {
            tokenizer->advance();
        } else {
            break;
        }
    }
}

void CompilationEngine::compile_parameter_list() {
    outfile << "<parameterList>" << endl;
    string token;
    while ((token = tokenizer->tokens[tokenizer->iterator]) != ")") {
        write(token, tokenizer->tokenType(token));
        if (tokenizer->has_more_tokens()) tokenizer->advance();    
    }
    
    outfile << "</parameterList>" << endl; 
    
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
    //tokenizer->advance();
}

void CompilationEngine::compile_subroutine_body() {
    //writing the opening bracket
    outfile << "<subroutineBody>" << endl;
    //write the first curly brace
    string token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
    tokenizer->advance();

    //we are looking for the last curly brace as the end of the subroutine body
    int curly = 1;
    while (curly > 0) {
        token = tokenizer->tokens[tokenizer->iterator];
        if (token == "{") {
            ++curly;
        } else if (token == "}") {
            --curly;
        } else if (token == "var") {
            compile_var_dec();
        } else if (token == "let" || token == "if" || token == "else" || token == "while" || token == "do" || token == "return") {
            compile_statement();
        }

        if (tokenizer->has_more_tokens() && token != "return" && tokenizer->tokens[tokenizer->iterator] != "}") {
            tokenizer->advance();
        } else {
            break;
        }
    }
    //write the last curly brace
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
    tokenizer->advance(); 
    //write the ending bracket
    outfile << "</subroutineBody>" << endl;

}

void CompilationEngine::compile_var_dec() {
    outfile << "<varDec>" << endl;
    while((token = tokenizer->tokens[tokenizer->iterator]) != ";") {
        
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    }
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    outfile << "</varDec>" << endl;
}

void CompilationEngine::compile_statement() {
    outfile << "<statements>" << endl;
    vector<string> statements {"let", "if", "else", "while", "do", "return"};
    while (true) {
        string token = tokenizer->tokens[tokenizer->iterator];
        //outfile << token << endl;
        if (token == "let") {
            //outfile << "LET" << endl;
            compile_let();
        } else if (token == "while") {
            //outfile << "WHILE" << endl;
            compile_while();
        } else if (token == "if") {
            //outfile << "IF" << endl;
            compile_if();
        } else if (token == "do") {
            //outfile << "DO" << endl;
            compile_do();
        } else if (token == "return") {
            //cout  << "RETURN" << endl;
            compile_return();
        } else if (token == "}") {
            break;
        }
        if (tokenizer->has_more_tokens()) tokenizer->advance();
    }
    outfile << "</statements>" << endl;
}

void CompilationEngine::compile_let() {
    outfile << "<letStatement>" << endl;
    //getting the let declarations out of the way
    string token = tokenizer->tokens[tokenizer->iterator];
    do {
        token = tokenizer->tokens[tokenizer->iterator];
        if (token == "[") {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            compile_expression();
        } 
            write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
            tokenizer->advance(); 
        
    } while(tokenizer->tokens[tokenizer->iterator - 1] != "=");
    //moving on to the actual expressions
    compile_expression();
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    //tokenizer->advance();
    outfile << "</letStatement>" << endl;
}


void CompilationEngine::compile_expression() {
    outfile << "<expression>" << endl;
    token = tokenizer->tokens[tokenizer->iterator];
    while(token != ";" && token != "]" && token != ")") {
        //cout << token << endl;
        //this is only true when a function is being called
        if (token == "(" && tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator - 1]) == "identifier") {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            compile_expression_list();
            //not sure if this should be here or at the end of compile_expression_list
            //tokenizer->advance();
            token = tokenizer->tokens[tokenizer->iterator];
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
        } else {
            if (tokenizer->tokenType(token) != "symbol" || token == "(") {
                compile_term();
                token = tokenizer->tokens[tokenizer->iterator];
            } else if (token == "-") {
                if (tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator - 1]) == "identifier") {
                    cout << "BOOF" << endl;
                    //write the negative sign
                    write(token, tokenizer->tokenType(token));
                    tokenizer->advance();
                    
                    //handle the actual term
                    compile_term();
                    token = tokenizer->tokens[tokenizer->iterator];
                } else {
                    //cout << "CHORK" << endl;
                    compile_term();
                    tokenizer->advance();
                    token = tokenizer->tokens[tokenizer->iterator];
                }
            } else if (token == "~") {
                compile_term();
                token = tokenizer->tokens[tokenizer->iterator];
            } else if (token == ",") {
                break;
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
                token = tokenizer->tokens[tokenizer->iterator];
            } else {
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
                token = tokenizer->tokens[tokenizer->iterator];
            }
        }
    }
    //outfile << tokenizer->tokens[tokenizer->iterator] << " " << token << endl;
    outfile << "</expression>" << endl; 
}

void CompilationEngine::compile_term() {
    outfile << "<term>" << endl;

    string token = tokenizer->tokens[tokenizer->iterator];
    if (tokenizer->tokenType(token) == "identifier") {
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
        token = tokenizer->tokens[tokenizer->iterator];
        if (token == ".") {
            //this is a method
            while ((token = tokenizer->tokens[tokenizer->iterator]) != "(") {
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            }
            //write the (
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            
            //this will be the arguments
            compile_expression_list();

            //write the )
            token = tokenizer->tokens[tokenizer->iterator];
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
        } else if (token == "[") {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            
            compile_expression();
            
            token = tokenizer->tokens[tokenizer->iterator];
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
        } 
    } else if (token == "(") {
        //write the (
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
        
        compile_expression();
        
        //write the (
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    } else if (token == "-") {
        //negative int, will be exactly two tokens
        //write the -
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
        token = tokenizer->tokens[tokenizer->iterator];

        //write the value
        outfile << "<term>" << endl;
        write(token, tokenizer->tokenType(token));
        outfile << "</term>" << endl;
    } else if (token == "~") {
        //cout << token << endl;
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
        compile_term();
        //tokenizer->advance();
    } else {
        //int, string or keyword
        //these will always be one token
        if (token[0] == '"') {
            //cout << token << endl;
            write(token.substr(1, token.length() - 2), tokenizer->tokenType(token));
            tokenizer->advance();
        } else {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
        }
    }

    outfile << "</term>" << endl;
}

void CompilationEngine::compile_while() {
    //opening bracket
    outfile << "<whileStatement>" << endl;
    
    //write while and opening parenthesis
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();
   // cout << tokenizer->tokens[tokenizer->iterator] << endl; 
    
    //need to take care of the expression
    compile_expression();    
 
    //write the closing parenthesis of the condition
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();

    //write the opening curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();
   
    compile_statement();
        
    //write the closing curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
 
    //closing bracket
    outfile << "</whileStatement>" << endl;
}

void CompilationEngine::compile_if() {
    outfile << "<ifStatement>" << endl;
    
    //write the if and the opening parenthesis
    for (int i=0; i<2; ++i) {    
        write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
        tokenizer->advance();
    }

    compile_expression();
    
    //write the closing parenthesis of the condition 
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();

    //write the opening curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();

    compile_statement();

    //write the closing curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    
    if (tokenizer->tokens[tokenizer->iterator + 1] == "else") {
        tokenizer->advance();
        token = tokenizer->tokens[tokenizer->iterator];
        
        //outfile << "ELSE" << endl;
        //write the else keyword
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();

        //write the opening curly brace
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
       
        compile_statement(); 

        //write the closing curly brace
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        //tokenizer->advance();
    }
    outfile << "</ifStatement>" << endl;
}

void CompilationEngine::compile_do() {
    outfile << "<doStatement>" << endl;
    
    //print until we reach the parameters
    string token;
    while ((token = tokenizer->tokens[tokenizer->iterator]) != "(") {
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    }

    //write the opening parenthesis
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
    tokenizer->advance();
    
    //handle the arguments
    compile_expression_list();
    
    while(tokenizer->tokens[tokenizer->iterator] != ";") {
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    }
    //write the ending semicolon
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));

    outfile << "</doStatement>" << endl;
}

void CompilationEngine::compile_expression_list() {
    outfile << "<expressionList>" << endl;

    token = tokenizer->tokens[tokenizer->iterator];
    //if the list is not empty
    if (token != ")") {
        while ((token = tokenizer->tokens[tokenizer->iterator]) != ")") {
            compile_expression();
            if ((token = tokenizer->tokens[tokenizer->iterator]) == ",") {
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            }
        }
    }
    outfile << "</expressionList>" << endl;
}

void CompilationEngine::compile_return() {
    outfile << "<returnStatement>" << endl;

    
    //write the return keyword    
    string token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
    tokenizer->advance();
    
    //check if there is a return value
    token = tokenizer->tokens[tokenizer->iterator];
    string type = tokenizer->tokenType(token);
    if (type == "keyword" || type == "identifier") {
        compile_expression();
    }

    //writing the final semicolon
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));

    outfile << "</returnStatement>" << endl;
}
