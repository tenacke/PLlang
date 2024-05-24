#include "mylib.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>

int variable_count = 0;
int label_count = 0;
vector<string> llvmcode;
vector<string> expression_buffer;
vector<string> local_buffer;
stack<string> condition_stack;

map<string, string> symbol_table;
map<string, string> local_symbol_table;
map<string, string> arrey_symbol_table;

void generate_llvm_code(string block) {
    ofstream myout(string("output.ll"));
    myout << "; ModuleID = 'calculator'" << endl ; 
    myout << "declare i32 @printf(i8*, ...)" << endl ;
    myout << "declare i32 @scanf(i8*, ...)" << endl ;
    myout << "@read.str = constant [3 x i8] c\"%d\\00\"" << endl ;
    myout << "@print.str = constant [4 x i8] c\"%d\\0A\\00\"" << endl << endl ;

    for (size_t i = 0; i < llvmcode.size(); i++) {
         myout << llvmcode[i] << endl;
    }

    myout << "\ndefine i32 @main() {" << endl ;
    myout << block << endl ;
    myout << "\n  ret i32 0" << endl ;
    myout << "}" << endl ;
}

void generate_procedure(const char* name, string block) {
    string stm = "define void @" + string(name) + "() {";
    llvmcode.push_back("\n" + stm);
    for (size_t i = 0; i < local_buffer.size(); i++) {
        llvmcode.push_back(local_buffer[i]);
    }
    llvmcode.push_back(block);
    local_buffer.clear();
    local_symbol_table.clear();
    variable_count = 0;
    label_count = 0;
    llvmcode.push_back("\n  ret void");
    llvmcode.push_back("}");
}

void generate_function(const char* name, const char* args, string block) {
    string stm = "define i32 @" + string(name) + "(" + string(args) + ") {";
    llvmcode.push_back("\n" + stm);
    for (size_t i = 0; i < local_buffer.size(); i++) {
        llvmcode.push_back(local_buffer[i]);
    }
    llvmcode.push_back(block);
    local_buffer.clear();
    local_symbol_table.clear();
    variable_count = 0;
    label_count = 0;
    llvmcode.push_back("\n  ret i32 0");
    llvmcode.push_back("}");
}

string wrap_expression() {
    string stm = "";
    for (size_t i = 0; i < expression_buffer.size(); i++) {
        stm += expression_buffer[i] + "\n";
    }
    expression_buffer.clear();
    return stm;
}

// void wrap_statement() {
//     string stm = "";
//     for (size_t i = 0; i < statement_buffer.size(); i++) {
//         stm += statement_buffer[i] + "\n";
//     }
//     statement_buffer.clear();
//     statement_buffer.push_back(stm);
// }

void declare_global_constant(const char* name, const char* value) {
    string stm = "@" + string(name) + " = constant i32 " + string(value) + "";
    symbol_table[string(name)] = string("@" + string(name));
    llvmcode.push_back(stm);
}

void declare_global_constant_array(const char* name, const char* values) {
    
}

void declare_global_variable(const char* name) {
    string stm = "@" + string(name) + " = global i32 0";
    symbol_table[string(name)] = string("@" + string(name));
    llvmcode.push_back(stm);
}

void declare_global_variable_array(const char* name, const char* size) {
    string stm = "@" + string(name) + " = global [" + string(size) + " x i32] zeroinitializer";
    symbol_table[string(name)] = string("@" + string(name));
    arrey_symbol_table[string(name)] = "[" + string(size) + " x i32]";
    llvmcode.push_back(stm);
}

void declare_local_constant(const char* name, const char* value) {
    string new_name = "%" + to_string(++variable_count);
    string stm = new_name + " = alloca i32";
    local_symbol_table[string(name)] = new_name;
    local_buffer.push_back("  " + stm);
    local_buffer.push_back(store_variable(name, value));
}

void declare_local_constant_array(const char* name, const char* values) {
    string new_name = "%" + to_string(++variable_count);
    // string stm = new_name + " = alloca [ " + string(value) + " x i32]";
    local_symbol_table[string(name)] = new_name;
    // local_buffer.push_back("  " + stm);
}

void declare_local_variable(const char* name) {
    string new_name = "%" + to_string(++variable_count);
    string stm = new_name + " = alloca i32";
    local_symbol_table[string(name)] = new_name;
    local_buffer.push_back("  " + stm);
}

void declare_local_variable_array(const char* name, const char* size) {
    string new_name = "%" + to_string(++variable_count);
    string stm = new_name + " = alloca [" + string(size) + " x i32]";
    local_symbol_table[string(name)] = new_name;
    arrey_symbol_table[string(name)] = "[" + string(size) + " x i32]";
    local_buffer.push_back("  " + stm);
}

void add_condition(const char* condition) {
    condition_stack.push(string(condition));
}

string store_variable(const char* name, const char* value) {
    // look local symbol table first if not found look in global symbol table
    string var = local_symbol_table[string(name)];
    if (var.empty()) {
        var = symbol_table[string(name)];
    }
    string exp = wrap_expression();
    string stm = "store i32 " + string(value) + ", i32* " + var + "";
    return exp + "  " + stm;
}

const char* load_variable(const char* name) {
    string var = local_symbol_table[string(name)];
    if (var.empty()) {
        var = symbol_table[string(name)];
    }
    const char* new_name = strdup(string("%" + to_string(++variable_count)).c_str());
    string stm = string(new_name) + " = load i32, i32* " + var + "";
    expression_buffer.push_back("  " + stm);
    return new_name;
}

const char* load_variable_from_array(const char* name, const char* index) {
    string var = local_symbol_table[string(name)];
    if (var.empty()) {
        var = symbol_table[string(name)];
    }
    string type = arrey_symbol_table[string(name)];
    const char* new_name = strdup(string("%" + to_string(++variable_count)).c_str());
    string stm = string(new_name) + " = getelementptr inbounds" + type + ", i32* " + var + ", i32 " + index ;
    expression_buffer.push_back("  " + stm);
    return new_name;
}

const char* do_operation(const char* op, const char* arg1, const char* arg2) {
    const char* new_name = strdup(string("%" + to_string(++variable_count)).c_str());
    string stm = string(new_name) + " = "  + string(op) + " i32 " + string(arg1) + ", " + string(arg2) + "";
    expression_buffer.push_back("  " + stm);
    return new_name;
}

string procedure_call(const char* name) {
    string stm = "call void @" + string(name) + "()";
    return "  " + stm;
}

const char* function_call(const char* name, const char* args) {
    string stm = "call i32 " + string(name) + "(" + string(args) + ")";
    return name;
}

string if_then(string condition, string then) {
    string condition_var = condition_stack.top();
    condition_stack.pop();

    string entry_label = "if" + to_string(++label_count);
    string then_label = "if" + to_string(++label_count);
    string exit_label = "if" + to_string(++label_count);

    string if_statement = "  br label %" + entry_label + "\n";
    if_statement += "\n" + entry_label + ":\n";
    if_statement += condition;
    if_statement += "  br i1 " + condition_var + ", label %" + then_label + ", label %" + exit_label + "\n";
    if_statement += "\n" + then_label + ":\n";
    if_statement += then + "\n";
    if_statement += "  br label %" + exit_label + "\n";
    if_statement += "\n" + exit_label + ":\n";

    return if_statement;
}

string if_else(string condition, string statement, string else_statement) {
    string condition_var = condition_stack.top();
    condition_stack.pop();

    string entry_label = "if" + to_string(++label_count);
    string then_label = "if" + to_string(++label_count);
    string else_label = "if" + to_string(++label_count);
    string exit_label = "if" + to_string(++label_count);

    string if_statement = "  br label %" + entry_label + "\n";
    if_statement += "\n" + entry_label + ":\n";
    if_statement += condition;
    if_statement += "  br i1 " + condition_var + ", label %" + then_label + ", label %" + else_label + "\n";
    if_statement += "\n" + then_label + ":\n";
    if_statement += statement + "\n";
    if_statement += "  br label %" + exit_label + "\n";
    if_statement += "\n" + else_label + ":\n";
    if_statement += else_statement + "\n";
    if_statement += "  br label %" + exit_label + "\n";
    if_statement += "\n" + exit_label + ":\n";

    return if_statement;
}

string while_loop(string condition, string block) {
    string condition_var = condition_stack.top();
    condition_stack.pop();

    string entry_label = "while" + to_string(++label_count);
    string loop_label = "while" + to_string(++label_count);
    string exit_label = "while" + to_string(++label_count);

    string while_statement = "  br label %" + entry_label + "\n";
    while_statement += "\n" + entry_label + ":\n";
    while_statement += condition;
    while_statement += "  br i1 " + condition_var + ", label %" + loop_label + ", label %" + exit_label + "\n";
    while_statement += "\n" + loop_label + ":\n";
    while_statement += block + "\n";
    while_statement += "  br label %" + entry_label + "\n";
    while_statement += "\n" + exit_label + ":\n";

    return while_statement;
}

string read_variable(const char* name) {
    string var = local_symbol_table[string(name)];
    if (var.empty()) {
        var = symbol_table[string(name)];
    }
    string stm = "call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @read.str, i32 0, i32 0), i32* " + var + ")";
    return "  " + stm;
}

string write_variable(const char* name) {
    string var = local_symbol_table[string(name)];
    if (var.empty()) {
        var = symbol_table[string(name)];
    }
    string stm = "call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @read.str, i32 0, i32 0), i32* " + var + ")";
    return "  " + stm;
}

string writeln_variable(const char* name) {
    string var = local_symbol_table[string(name)];
    if (var.empty()) {
        var = symbol_table[string(name)];
    }
    string stm = "call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32* " + var + ")";
    return "  " + stm;
}

string break_loop() {
    cout << "break loop" << endl;
    return "  break";
}

string return_value(const char* value) {
    wrap_expression();
    string stm = "ret i32 " + string(value);
    return "  " + stm;
}

