#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include "JackTokenizer.h"

using namespace std;

class CompilationEngine {
    public:
        string token;
        JackTokenizer * tokenizer;
        CompilationEngine(const string& path, JackTokenizer * tokenizer);
        ofstream outfile;
        void write(const string& token, const string& type);
        void compile_class();
        void compile_class_var_dec();
        void compile_subroutine();
        void compile_parameter_list();
        void compile_subroutine_body();
        void compile_var_dec();
        void compile_statement();
        void compile_let();
        void compile_expression();
        void compile_term();
        void compile_while();
        void compile_if();
        void compile_do();
        void compile_expression_list();
        void compile_return();
};

