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
    cout << "INIT" << endl;
    this->tokenizer = tokenizer;
    this->outfilePath = path;
    this->whileCounter = 0;
    this->ifCounter = 0; 
    
    compile_class();
};

void CompilationEngine::write(const string& token, const string& type) {
    /*bool alt = false;
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
    outfile << "<" + type + "> " + output + " </" + type + ">" << endl;*/
}

void CompilationEngine::compile_class() {
    this->className = tokenizer->tokens[tokenizer->iterator + 1];
    writer.openFile(outfilePath, className);
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
    table.resetClass();
}

void CompilationEngine::compile_class_var_dec() {
    outfile << "<classVarDec>" << endl;
    string token;
    while (true) {
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        if (token == "static" || token == "field") {
            string boof = tokenizer->tokens[tokenizer->iterator + 2];
            table.define(tokenizer->tokens[tokenizer->iterator + 2], tokenizer->tokens[tokenizer->iterator + 1], token);
            writer.writePop(table.kindOf(boof), table.indexOf(boof));
            int counter = 3;
            while(tokenizer->tokens[tokenizer->iterator + counter] == ",") {
                boof = tokenizer->tokens[tokenizer->iterator + counter + 1];
                table.define(tokenizer->tokens[tokenizer->iterator + counter + 1], tokenizer->tokens[tokenizer->iterator + 1], token);
                writer.writePop(table.kindOf(boof), table.indexOf(boof));
                counter += 2;
            }
        }
        if (token == ";" && (tokenizer->tokens[tokenizer->iterator + 1] != "static" || tokenizer->tokens[tokenizer->iterator + 1] != "field")) {
            tokenizer->advance();
            break;
        } 
        tokenizer->advance();
        
    }
    outfile << "</classVarDec>" << endl;
}

void CompilationEngine::compile_subroutine() {
    table.startSubroutine(className);
    //advance past the function token
    tokenizer->advance();
    //advance past the return type token
    string type = tokenizer->tokens[tokenizer->iterator];
    tokenizer->advance();
    //store the function's name
    string name = tokenizer->tokens[tokenizer->iterator];
    tokenizer->advance();

    //we need to write the function name

    //advance past the opening parenthesis
    tokenizer->advance();
    
    //compile the paramater list        
    compile_parameter_list();
    
    //advance past the closing parenthesis
    tokenizer->advance();
    
    //advance past the opening curly brace
    tokenizer->advance();

    //compile the body of the subroutine
    compile_subroutine_body(name);
    
    //advance past the closing curly brace
    tokenizer->advance();

    writer.output << endl;
}

void CompilationEngine::compile_parameter_list() {
    string token;
    int counter = 0;
    while ((token = tokenizer->tokens[tokenizer->iterator]) != ")") {
        write(token, tokenizer->tokenType(token));
        if (counter % 3 == 0) {
            string kitten = tokenizer->tokens[tokenizer->iterator + 1];
            table.define(tokenizer->tokens[tokenizer->iterator + 1], token, "argument");
        }
        if (tokenizer->has_more_tokens()) tokenizer->advance();
        ++counter;    
    }
    
    
    token = tokenizer->tokens[tokenizer->iterator];
    write(token, tokenizer->tokenType(token));
}

void CompilationEngine::compile_subroutine_body(string& subroutineName) {
    int localVars = 0;
    
    //we need to count the number of local variables for write the function 
    while (tokenizer->tokens[tokenizer->iterator] == "var") {
        localVars += compile_var_dec();
        tokenizer->advance();
    }
    writer.writeFunction(subroutineName, localVars);

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
}

int CompilationEngine::compile_var_dec() {
    string kind = tokenizer->tokens[tokenizer->iterator], type = tokenizer->tokens[tokenizer->iterator+1];
    int declarations = 0;
    while((token = tokenizer->tokens[tokenizer->iterator]) != ";") {
        if (token != kind && token != type && token != ",") {
            table.define(token, type, kind);
            declarations++;
        }
        tokenizer->advance();
    }
    return declarations;
}

void CompilationEngine::compile_statement() {
    outfile << "<statements>" << endl;
    vector<string> statements {"let", "if", "else", "while", "do", "return"};
    while (true) {
        string token = tokenizer->tokens[tokenizer->iterator];
        if (token == "let") {
            //cout << "LET" << endl;
            compile_let();
        } else if (token == "while") {
            //cout << "WHILE" << endl;
            compile_while();
        } else if (token == "if") {
            //cout << "IF" << endl;
            compile_if();
        } else if (token == "do") {
            //cout << "DO" << endl;
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
    //getting the let declarations out of the way
    string token = tokenizer->tokens[tokenizer->iterator], varName = tokenizer->tokens[tokenizer->iterator + 1];
    do {
        if (token == "[") {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            compile_expression();
        } 
            write(tokenizer->tokens[tokenizer->iterator], tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator]));
            tokenizer->advance(); 
        
    } while(tokenizer->tokens[tokenizer->iterator-1] != "=");
    
    //moving on to the actual expressions
    compile_expression();

    //pop the value onto the stack
    writer.writePop(table.kindOf(varName), table.indexOf(varName));
}


void CompilationEngine::compile_expression() {
    outfile << "<expression>" << endl;
    token = tokenizer->tokens[tokenizer->iterator];
    while(token != ";" && token != "]" && token != ")") {
        //this is only true when a method is being called
        if (tokenizer->tokenType(token) == "identifier" && tokenizer->tokens[tokenizer->iterator + 1] == "." && tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator + 2]) == "identifier") {
            compile_term();
        } else {
            if (tokenizer->tokenType(token) == "integerConstant") {
                writer.writePush("constant", stoi(token));
                tokenizer->advance();
                token = tokenizer->tokens[tokenizer->iterator];
            } else if (tokenizer->tokenType(token) == "keyword" && (token == "true" || token == "false" || token == "null")) {
                compile_term();
                token = tokenizer->tokens[tokenizer->iterator];        
            } else if (tokenizer->tokenType(token) != "symbol" || token == "(") {
                compile_term();
                token = tokenizer->tokens[tokenizer->iterator];
            } else if (token == "-" && tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator - 1]) == "symbol") {
                //this will execute on negative numbers but not on subtraction operations
                //compile the term after the negative sign and then write the negation
                tokenizer->advance();
                compile_term();
                writer.writeArithmetic("neg");
                token = tokenizer->tokens[tokenizer->iterator];
            } else if (token == ",") {
                break;
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
                token = tokenizer->tokens[tokenizer->iterator];
            } else if ((tokenizer->tokenType(token) == "symbol") && (token == "+" || token == "-" || token == "*" || token == "/" || token == "<" || token == ">" || token == "=" || token == "~" || token == "&" || token == "|")) {
                //store the symbol for later and advance past it
                string symbol = tokenizer->tokens[tokenizer->iterator];
                tokenizer->advance();
                token = tokenizer->tokens[tokenizer->iterator];
                //the token will be the token after the operator, we need to compile this as an expression
                compile_expression();
                
                writer.writeArithmetic(symbol);
                token = tokenizer->tokens[tokenizer->iterator];
            } else {
                tokenizer->advance();
                token = tokenizer->tokens[tokenizer->iterator];
            }
        }
    }
    outfile << "</expression>" << endl; 
}

    void CompilationEngine::compile_term() {

        string token = tokenizer->tokens[tokenizer->iterator];
        if (tokenizer->tokenType(token) == "identifier") {
            if (tokenizer->tokens[tokenizer->iterator + 1] != ".") writer.writePush(table.kindOf(token), table.indexOf(token)); 
            tokenizer->advance();
            token = tokenizer->tokens[tokenizer->iterator];
            if (token == ".") {
                //this is a method
                string origin = tokenizer->tokens[tokenizer->iterator - 1], methodName = tokenizer->tokens[tokenizer->iterator + 1];
                while ((token = tokenizer->tokens[tokenizer->iterator]) != "(") {
                    write(token, tokenizer->tokenType(token));
                    tokenizer->advance();
                }
                //write past the (
                tokenizer->advance();
                
                //this will be the arguments
                int args = compile_expression_list();

                //advance past the )
                tokenizer->advance();
            
                //write the call to the function
                writer.writeCall(origin + "." + methodName, args);
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
        } else if (token == "~") {
            write(token, tokenizer->tokenType(token));
            tokenizer->advance();
            compile_term();
            //tokenizer->advance();
        } else if (tokenizer->tokenType(token) == "integerConstant") {
            writer.writePush("constant", stoi(token));
            tokenizer->advance();  
        } else if (token == "true" || token == "false" || token == "null") {
            if (token == "true") {
                writer.writePush("constant", 0);
                writer.writeArithmetic("not");
            } else if (token == "false") {
                writer.writePush("constant", 0);
            }
            //also need null
            tokenizer->advance();
        } else {
            //int, string or keyword
            //these will always be one token
            if (token[0] == '"') {
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
    int counter = ++whileCounter; 
    string startLabel = "WHILE_START" + to_string(counter);
    writer.writeLabel(startLabel);

    //advance past the while token and the opening parenthesis
    tokenizer->advance();
    tokenizer->advance();
    
    //need to take care of the expression
    compile_expression();    
    writer.writeArithmetic("not");
    writer.writeIf("WHILE_END" + to_string(counter));

    //advance past the closing parenthesis of the condition and the opening curly brace
    tokenizer->advance();
    tokenizer->advance();
   
    compile_statement();

    //need a goto statement to return back to the beginning of the loop
    writer.writeGoto("WHILE_START" + to_string(counter));
        
    //write the ending label
    writer.writeLabel("WHILE_END" + to_string(counter));

}

void CompilationEngine::compile_if() {
    int counter = ++ifCounter;
    //advance past the if token and the opening parenthesis
    tokenizer->advance();
    tokenizer->advance();
    
    compile_expression();
   
    writer.writeIf("IF-TRUE" + to_string(counter));
    writer.writeGoto("IF-FALSE" + to_string(counter));
    writer.writeLabel("IF-TRUE" + to_string(counter));

    //advance past the closing parenthesis of the condition and the opening curly brace 
    tokenizer->advance();
    tokenizer->advance();

    //handle the actual statements inside the loop
    compile_statement();

    //after the compilation of the statements, jump to the end of the if statement
    writer.writeGoto("IF-END" + to_string(counter));

    //if the expression is false, jump here
    writer.writeLabel("IF-FALSE" + to_string(counter));

    //if there is an else statement there will be commands after the IF-FALSE label, otherwise the if statement is over
    if (tokenizer->tokens[tokenizer->iterator + 1] == "else") {
        //advance to the else token
        tokenizer->advance();
       
        //advance to the opening curly brace 
        tokenizer->advance();
        
        //advance to beginning of the statement
        tokenizer->advance();
       
        compile_statement(); 

    }
    writer.writeLabel("IF-END" + to_string(counter));
    //the token at this point will be the closing curly brace
}

void CompilationEngine::compile_do() {
    //advance past the do token 
    tokenizer->advance();
   
    //store the name of the function being called 
    string name;
    //this will only execute if a method outside the class is being called, otherwise it is a function or a method within the class
    if (tokenizer->tokens[tokenizer->iterator + 1] == ".") {
        //store the class the function resides in
        name = tokenizer->tokens[tokenizer->iterator];
        tokenizer->advance();
        
        for (int i=0; i<2; ++i) {
            name += tokenizer->tokens[tokenizer->iterator];
            tokenizer->advance();
        }
    } else {
        name = tokenizer->tokens[tokenizer->iterator];
        tokenizer->advance();
    }
    
    //advance past the opening parenthesis
    tokenizer->advance();
    
    //handle the arguments
    int args = compile_expression_list();
  
     
    //now that our arguments are on the stack, call the funciton
    writer.writeCall(name, args);

    //advance past the closing parenthesis 
    while((token = tokenizer->tokens[tokenizer->iterator]) != ";") {
        tokenizer->advance();
    }

    //final token is a semicolon
    
}

int CompilationEngine::compile_expression_list() {

    int args = 0;
    token = tokenizer->tokens[tokenizer->iterator];
    //if the list is not empty
    if (token != ")") {
        while ((token = tokenizer->tokens[tokenizer->iterator]) != ")") {
            compile_expression();
            args += 1;
            if ((token = tokenizer->tokens[tokenizer->iterator]) == ",") {
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            }
        }
    }
    return args;
}

void CompilationEngine::compile_return() {
    //check if there is a return value
    tokenizer->advance();
    token = tokenizer->tokens[tokenizer->iterator];
    
    //if not we need to push 0 onto the stack
    if (token == ";") {
        writer.writePush("constant", 0);
    } else {
        compile_expression();
    }
    writer.writeReturn(); 
}
