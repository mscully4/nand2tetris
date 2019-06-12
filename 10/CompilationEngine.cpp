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
        } else if (token == "<") {
            output = " &lt;";
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
 
    token = tokenizer->tokens[tokenizer->iterator];
    //write the parenthesis
    write(token, tokenizer->tokenType(token));
    tokenizer->advance();
    //if there are parameters, compile parameter list 
    if (token != ")") {
        compile_parameter_list();
    } 
    
    //token = tokenizer->tokens[tokenizer->iterator];
    //write(token, tokenizer->tokenType(token));
    tokenizer->advance();

    //compile the subroutine body
    compile_subroutine_body();   
    
    outfile << "</subroutineDec>" << endl;

    token = tokenizer->tokens[tokenizer->iterator];
    if (token == "}") {
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
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
    while (true) {
        token = tokenizer->tokens[tokenizer->iterator];
        if (token == "var") {
            compile_var_dec();
        } else if (token == "let" || token == "if" || token == "else" || token == "while" || token == "do" || token == "return") {
            compile_statement();
        }
        
        if (tokenizer->tokens[tokenizer->iterator] == "}") {
            break;   
        }
        tokenizer->advance();
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
    string token;
    while ((token = tokenizer->tokens[tokenizer->iterator]) != "}") {
        if (token == "let") {
            compile_let();
        } else if (token == "while") {
            compile_while();
        } else if (token == "if") {
            compile_if();
        } else if (token == "do") {
            compile_do();
        } else if (token == "return") {
            compile_return();
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
    //keep looping until the end of the expression is reached
    while(token != ";" && token != "]" && token != ")") {
        //compile term for identifiers
        if (tokenizer->tokenType(token) == "identifier") {
            compile_term();
        } else if (tokenizer->tokenType(token) == "integerConstant") {
            //compile term for integers
            compile_term();
        } else if (tokenizer->tokenType(token) == "stringConstant") {
            //compile term for strings
            compile_term();
        } else if (tokenizer->tokenType(token) == "keyword") {
            //compile term for keywords
            compile_term();
        } else if (tokenizer->tokenType(token) == "symbol") {
            //if the expression begins with a parenthesis
            if (token == "(") { 
                //compile the term within the parenthesis
                compile_term();
                //then advance
                tokenizer->advance();
            } 
            //if the expression begins with a negative sign
            else if (token == "-") {
                //if the negation is taking place within parenthesis
                if (tokenizer->tokens[tokenizer->iterator - 1] == "(") {
                    //compile the term within the parenthesis
                    compile_term();
                    //then advance
                    tokenizer->advance();
                } 
                //otherwise a single value is being negated
                else {
                    //write the negative sign
                    write(token, tokenizer->tokenType(token));
                    tokenizer->advance();
                    
                    //handle the actual term
                    compile_term();
                }
            } 
            //if the expression begins with a not sign
            else if (token == "~") {
                //compile the term, but don't advance
                compile_term();
            } 
            //if the expression begins with a comma
            else if (token == ",") {
                //this is only being called with paramters or arguments so just break
                break;
            }
            else {
                //otherwise just write and advance
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            }
        }
        token = tokenizer->tokens[tokenizer->iterator];
    }
    outfile << "</expression>" << endl; 
}

void CompilationEngine::compile_term() {
    outfile << "<term>" << endl;
    string token = tokenizer->tokens[tokenizer->iterator];
    //if the term is an identifier
    if (tokenizer->tokenType(token) == "identifier") {
        //write the first token and advance to the next
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
        token = tokenizer->tokens[tokenizer->iterator];
        //if the next token is a period, then this is a method or field
        if (token == ".") {
            if (tokenizer->tokens[tokenizer->iterator+2] == "(") {
                //write the method information
                while ((token = tokenizer->tokens[tokenizer->iterator]) != "(") {
                    write(token, tokenizer->tokenType(token));
                    tokenizer->advance();
                }
                //write the (
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
                
                //compile the arguments
                compile_expression_list();

                //write the )
                token = tokenizer->tokens[tokenizer->iterator];
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            } 
            //this is a field
            else if (tokenizer->tokens[tokenizer->iterator+2] == ")") {
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            }
        } 
        //otherwise this is a field array
        else if (token == "[") {
            //write the opening bracket
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            
            //compile the expression inside
            compile_expression();
            
            //write the closing bracket
            token = tokenizer->tokens[tokenizer->iterator];
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
        }
    } else if (tokenizer->tokenType(token) == "integerConstant") {
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    } else if (tokenizer->tokenType(token) == "stringConstant") {
        //write the string without the quotation marks
        write(token.substr(1, token.length() - 2), tokenizer->tokenType(token));
        tokenizer->advance();
    } else if (tokenizer->tokenType(token) == "keyword") {
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
    } else if (tokenizer->tokenType(token) == "symbol") {
        //if the first token is a open parenthesis
        if (token == "(") {
                //write the opening parenthesis
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
                //compile the expression
                compile_expression();
                //write the closing parenthesis
                token = tokenizer->tokens[tokenizer->iterator];
                write(token, tokenizer->tokenType(token));
                token = tokenizer->tokens[tokenizer->iterator];
        } 
        //if the first token is the not operator
        else if (token == "~") {
                tokenizer->advance();
                //compile whatever comes next
                compile_term();
        } 
        //if the first token is the negative sign, then we have a negative int
        else if (token == "-") {
            //negative int, will be exactly two tokens
            //write the -
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            token = tokenizer->tokens[tokenizer->iterator];
            
            //write the value
            outfile << "<term>" << endl;
            write(token, tokenizer->tokenType(token));
            outfile << "</term>" << endl;
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
    
    //need to take care of the expression
    compile_expression();    

    //write the closing parenthesis of the condition
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();

    //write the opening curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();
  
    //compile the statements within the loop 
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
    
    //compile the logic expression
    compile_expression();
    
    //write the closing parenthesis of the condition 
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();

    //write the opening curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    tokenizer->advance();

    //compile the statements within the if
    compile_statement();

    //write the closing curly brace
    write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
    
    //if there is an else
    if (tokenizer->tokens[tokenizer->iterator + 1] == "else") {
        tokenizer->advance();
        token = tokenizer->tokens[tokenizer->iterator];
        
        //write the else keyword
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();

        //write the opening curly brace
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        tokenizer->advance();
       
        //compile the statements within the else
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
    
    //write until we reach the parameters
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
   
    //write the closing parenthesis 
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
    tokenizer->advance();
    
    //advance until you reach the end of the line
    //while(tokenizer->tokens[tokenizer->iterator] != ";") {
    //    token = tokenizer->tokens[tokenizer->iterator];
    //    cout << token << endl;
    //    write(token, tokenizer->tokenType(token));
    //    tokenizer->advance();
    //}
    
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
        //keep going until the closing parenthesis is reached
        while ((token = tokenizer->tokens[tokenizer->iterator]) != ")") {
            //compile the expression
            compile_expression();
            //if there is a comma, write it and advance
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
        //compile the return value
        compile_expression();
    }

    //writing the final semicolon
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));

    outfile << "</returnStatement>" << endl;
}
