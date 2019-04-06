#include "VMWriter.h"
#include <string>

using namespace std;

VMWriter::VMWriter() {
}

void VMWriter::openFile(const string& path, const string& className) {
    this->output.open(path);
    this->className = className;
}

void VMWriter::writePush(const string& segment, const int& index) {
    if (segment == "var") {
        this->output << "push local " << index << endl;
    } else {
        this->output << "push " << segment << " " << index << endl;
    }
}

void VMWriter::writePop(const string& segment, const int& index) {
   if (segment == "var") {
        this->output << "pop local " << index << endl;        
    } else {
        this->output << "pop " << segment << " " << index << endl;

    }
}

void VMWriter::writeArithmetic(const string& command) {
    if (command == "+") {
        this->output << "add" << endl;
    } else if (command == "-") {
        this->output << "sub" << endl;
    } else if (command == "*") {
        this->output << "call Math.multiply 2" << endl;
    } else if (command == "/") {
        this->output << "call Math.divide 2" << endl;
    } else if (command == "neg") {
        this->output << "neg" << endl;
    } else if (command == "not" || command == "~") {
        this->output << "not" << endl;
    } else if (command == ">") {
        this->output << "gt" << endl;
    } else if (command == "<") {
        this->output << "lt" << endl;
    } else if (command == "=") {
        this->output << "eq" << endl;
    } else if (command == "&") {
        this->output << "and" << endl;
    } else if (command == "|") {
        this->output << "or" << endl;
    }
}

void VMWriter::writeLabel(const string& label) {
    this->output << "label " << label << endl;
}

void VMWriter::writeGoto(const string& label) {
    this->output << "goto " << label << endl;
}

void VMWriter::writeIf(const string& label) {
    this->output << "if-goto " << label << endl;
}

void VMWriter::writeCall(const string& name, const int& args) {
    this->output << "call " << name << " " << args << endl; 
}

void VMWriter::writeFunction(const string& name, const int& locals) {
    this->output << "function " << this->className << "." << name << " " << locals << endl; 
}

void VMWriter::writeReturn() {
    this->output << "return" << endl;
}
