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
    this->classFields = 0;
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
}

void CompilationEngine::compile_class_var_dec() {
    string token;
    int fields = 0;
    while (true) {
        token = tokenizer->tokens[tokenizer->iterator];
        write(token, tokenizer->tokenType(token));
        if (token == "static" || token == "field") {
            string boof = tokenizer->tokens[tokenizer->iterator + 2];
            table.define(tokenizer->tokens[tokenizer->iterator + 2], tokenizer->tokens[tokenizer->iterator + 1], token);
            if (token == "field") this->classFields++;
            int counter = 3;
            while(tokenizer->tokens[tokenizer->iterator + counter] == ",") {
                boof = tokenizer->tokens[tokenizer->iterator + counter + 1];
                table.define(tokenizer->tokens[tokenizer->iterator + counter + 1], tokenizer->tokens[tokenizer->iterator + 1], token);
                if (token == "field") this->classFields++;
                counter += 2;
            }
        }
        if (token == ";" && (tokenizer->tokens[tokenizer->iterator + 1] != "static" || tokenizer->tokens[tokenizer->iterator + 1] != "field")) {
            tokenizer->advance();
            break;
        } 
        tokenizer->advance();
    }
}

void CompilationEngine::compile_subroutine() {
    table.startSubroutine(className);

    string token = tokenizer->tokens[tokenizer->iterator]; 
    string subroutineType = token;
    
    //we only need to pass this as the first argument for methods, not constructors
    if (subroutineType == "method") table.define("this", this->className, "argument");
    

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
    compile_subroutine_body(name, subroutineType);
    
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

void CompilationEngine::compile_subroutine_body(string& subroutineName, string& type) {
    int localVars = 0;
    
    //we need to count the number of local variables for write the function 
    while (tokenizer->tokens[tokenizer->iterator] == "var") {
        localVars += compile_var_dec();
        tokenizer->advance();
    }
    writer.writeFunction(subroutineName, localVars);

    if (type == "constructor") {
        writer.writePush("constant", this->classFields);
        writer.writeCall("Memory.alloc", 1);
        writer.writePop("pointer", 0);
    } else if (type == "method") {
        writer.writePush("argument", 0);
        writer.writePop("pointer", 0);
    }

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
            compile_let();
        } else if (token == "while") {
            compile_while();
        } else if (token == "if") {
            compile_if();
        } else if (token == "do") {
            compile_do();
        } else if (token == "return") {
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
    bool isArray = false;
    do {
        string token = tokenizer->tokens[tokenizer->iterator];
        if (token == "[") {
            writer.writePush(table.kindOf(varName), table.indexOf(varName));
            compile_expression();
            writer.writeArithmetic("+");
            writer.writePop("temp", 0);
            isArray = true;
        } 
        tokenizer->advance(); 
    } while(tokenizer->tokens[tokenizer->iterator-1] != "=");
    
    //moving on to the actual expressions
    compile_expression();

    //if we are assigning to an array index, we need to point pointer 1 at the location saved in temp 0
    //Then we can pop the value to the array at location index using that 0
    if (isArray) {
        writer.writePush("temp", 0);
        writer.writePop("pointer", 1);
        writer.writePop("that", 0);
    } 
    //in the case of a field, we use this instead of field
    if (table.kindOf(varName) == "field") {
        writer.writePop("this", table.indexOf(varName));
    } else {
        writer.writePop(table.kindOf(varName), table.indexOf(varName));
    }
}


void CompilationEngine::compile_expression() {
    token = tokenizer->tokens[tokenizer->iterator];
    while (token != ";" && token != "]" && token != ")") {
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
            } else if (token == "-" && tokenizer->tokenType(tokenizer->tokens[tokenizer->iterator - 1]) == "symbol" && tokenizer->tokens[tokenizer->iterator - 1] != ")") {
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
                    tokenizer->advance();
                }
                //write past the (
                tokenizer->advance();
                
                //this will be the arguments
                int args = compile_expression_list();

                //advance past the )
                tokenizer->advance();
                
                if (table.kindOf(origin) == "field") {
                    int index = table.indexOf(origin);
                    origin = table.typeOf(origin);
                    writer.writePush("this", index);
                    args++;
                }

                //write the call to the function
                writer.writeCall(origin + "." + methodName, args);
            } else if (token == "[") {
                //advance past the bracket
                tokenizer->advance();
                
                //handle the expression inside the brackets
                compile_expression();
                
                //add the index to the base of the array, set pointer 1 to the address of the element, access the entry using the that 0
                writer.writeArithmetic("+");
                writer.writePop("pointer", 1);
                writer.writePush("that", 0);

                tokenizer->advance();
            } 
        } else if (tokenizer->tokenType(token) == "stringConstant") {
            writer.writePush("constant", token.length() - 2);
            writer.writeCall("String.new", 1);
            for (int x=1; x<token.length() - 1; ++x) {
                writer.writePush("constant", (int)token[x]);
                writer.writeCall("String.appendChar", 2);
            }
            tokenizer->advance();   
        } else if (tokenizer->tokenType(token) == "symbol") {
            if (token == "(") {
                //write the (
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            
                compile_expression();
            
                //write the (
                token = tokenizer->tokens[tokenizer->iterator];
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
            } else if (token == "~") {
                write(token, tokenizer->tokenType(token));
                tokenizer->advance();
                compile_term();
                //tokenizer->advance();
            }
        } else if (tokenizer->tokenType(token) == "integerConstant") {
            writer.writePush("constant", stoi(token));
            tokenizer->advance();  
        } else if (tokenizer->tokenType(token) == "keyword") {
            if (token == "true" || token == "false" || token == "null") {
                if (token == "true") {
                    writer.writePush("constant", 0);
                    writer.writeArithmetic("not");
                } else if (token == "false") {
                    writer.writePush("constant", 0);
                }
                //also need null
                tokenizer->advance();
            } else if (token == "this") {
                writer.writePush("pointer", 0);
                tokenizer->advance();
            }
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
    //keep track of the number of arguments
    int args = 0;

    //advance past the do token 
    tokenizer->advance();
   
    //store the name of the function being called 
    string name;
    
    //this will only execute if a method outside the class is being called, otherwise it is a function or a method within the class
    if (tokenizer->tokens[tokenizer->iterator + 1] == ".") {
        //store the class the function resides in
        if (table.typeOf(tokenizer->tokens[tokenizer->iterator]) != "NONE") {
            name = table.typeOf(tokenizer->tokens[tokenizer->iterator]);   
            if (table.kindOf(tokenizer->tokens[tokenizer->iterator]) == "field") {
                writer.writePush("this", table.indexOf(tokenizer->tokens[tokenizer->iterator]));
            } else {
                writer.writePush("local", table.indexOf(tokenizer->tokens[tokenizer->iterator]));
            }
            args++;     
        } else {
            name = tokenizer->tokens[tokenizer->iterator];
        }

        tokenizer->advance();

        for (int i=0; i<2; ++i) {
            name += tokenizer->tokens[tokenizer->iterator];
            tokenizer->advance();
        }
    } else {
        name = this->className + "." + tokenizer->tokens[tokenizer->iterator];
        tokenizer->advance();
    
        //push the object that the method is being called on onto the stack
        writer.writePush("pointer", 0);
        args++;
    }

    //advance past the opening parenthesis
    tokenizer->advance();
    
    //handle the arguments
    args += compile_expression_list();
  
     
    //now that our arguments are on the stack, call the funciton
    writer.writeCall(name, args);

    //advance past the closing parenthesis 
    while((token = tokenizer->tokens[tokenizer->iterator]) != ";") {
        tokenizer->advance();
    }


    writer.writePop("temp", 0);
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
