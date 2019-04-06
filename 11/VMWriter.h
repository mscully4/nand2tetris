#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

class VMWriter {
    public:
        ofstream output;
        string className;
        VMWriter();
        void openFile(const string& path, const string& className);
        void writePush(const string& segment, const int& index);
        void writePop(const string& segment, const int& index);
        void writeArithmetic(const string& command);
        void writeLabel(const string& label);
        void writeGoto(const string& label);
        void writeIf(const string& label);
        void writeCall(const string& name, const int& args);
        void writeFunction(const string& name, const int& locals);
        void writeReturn();
};

