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

void write(const string& token, const string& type) {
    cout << "<" + type + "> " + token + " </" + type + ">" << endl;
}


CompilationEngine::CompilationEngine(const string& path, JackTokenizer * tokenizer) {
    this->tokenizer = tokenizer;
    ofstream outfile(path);
    compile_class();
    /*while(tokenizer->has_more_tokens()) {
        cout << tokenizer->tokens[tokenizer->iterator] << endl;
        tokenizer->advance();
    }*/
};

void CompilationEngine::compile_class() {
    cout << "<class>" << endl;
    while (tokenizer->has_more_tokens()) {
        token = tokenizer->tokens[tokenizer->iterator];
        //cout << tokenizer->iterator << endl;
        if (token == "{" && tokenizer->tokens[tokenizer->iterator + 1] == "static") {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            compile_class_var_dec();
        } else if (token == "function" || token == "constructor" || token == "method") {
            compile_subroutine();
        }
        else {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
                
        }
    }
    cout << "</class>" << endl;
}

void CompilationEngine::compile_class_var_dec() {
    cout << "<ClassVarDec>" << endl;
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
    cout << "</ClassVarDec>" << endl;
}

void CompilationEngine::compile_subroutine() {
    cout << "<subroutineDec>" << endl;
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
            cout << "</subroutineDec>" << endl;
            break;
        } else if (tokenizer->has_more_tokens()) {
            tokenizer->advance();
        } else {
            break;
        }
    }
}

void CompilationEngine::compile_parameter_list() {
    cout << "<ParameterList>" << endl;
    while (true) {
        string token = tokenizer->tokens[tokenizer->iterator];
        if (token == ")") {
            cout << "</ParameterList>" << endl;
            write(token, tokenizer->tokenType(token));
            break;
        }
        write(token, tokenizer->tokenType(token));
        if (tokenizer->has_more_tokens()) tokenizer->advance();    
    }
}

void CompilationEngine::compile_subroutine_body() {
    //writing the opening bracket
    cout << "<SubroutineBody>" << endl;
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
        tokenizer->advance();
    }
    //write the last curly brace
    write(token, tokenizer->tokenType(token));
    //write the ending bracket
    cout << "</SubroutineBody>" << endl;

}

void CompilationEngine::compile_var_dec() {
    cout << "<varDec>" << endl;
    while((token = tokenizer->tokens[tokenizer->iterator]) != ";") {
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    }
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    cout << "</varDec>" << endl;
}

void CompilationEngine::compile_statement() {
    cout << "<statements>" << endl;
    vector<string> statements {"let", "if", "else", "while", "do", "return"};
    while (true) {
        string token = tokenizer->tokens[tokenizer->iterator];
        if (token == "let") {
            compile_let();
        }
        //(find(statements.begin(), statements.end(), tokenizer->tokens[tokenizer->iterator]) != statements.end());
        if (token == "return" && tokenizer->tokens[tokenizer->iterator + 1] == ";") break;
        tokenizer->advance();
    }
    cout << "</statements>" << endl;
}

void CompilationEngine::compile_let() {
    cout << "<letStatement>" << endl;
    //getting the let declarations out of the way
    do {
        write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
        tokenizer->advance(); 
    } while(tokenizer->tokens[tokenizer->iterator - 1] != "=");
    //moving on to the actual expressions
    compile_expression();
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    cout << "</letStatement>" << endl;
}


void CompilationEngine::compile_expression() {
    cout << "<expression>" << endl;
    string token;
    while((token = tokenizer->tokens[tokenizer->iterator]) != ";") {
        tokenizer->advance();
    }
    cout << "</expression>" << endl; 
}
/*void CompilationEngine::compile_let() {
void CompilationEngine::compile_let() {
void CompilationEngine::compile_let() {
void CompilationEngine::compile_let() {
*/
