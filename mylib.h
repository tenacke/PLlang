#include <iostream>
#include <vector>

using namespace std;

void generate_llvm_code(string block);
void generate_procedure(const char* name, string block);
void generate_function(const char* name, const char* params, string block);
void declare_global_constant(const char* name, const char* value);
void declare_global_constant_array(const char* name, const char* values);
void declare_global_variable(const char* name);
void declare_global_variable_array(const char* name, const char* size);
void declare_local_constant(const char* name, const char* value);
void declare_local_constant_array(const char* name, const char* values);
void declare_local_variable(const char* name);
void declare_local_variable_array(const char* name, const char* size);
string wrap_expression(void);
// void wrap_statement(void);  
string store_variable(const char* name, const char* value);
string store_array_element(const char* name, const char* index, const char* value);

void add_condition(const char* condition);
const char* load_variable(const char* name);
const char* load_variable_from_array(const char* name, const char* index);
const char* do_operation(const char* op, const char* arg1, const char* arg2);
string procedure_call(const char* name);
const char* function_call(const char* name, const char* args);

string if_then(string condition, string then);
string if_else(string condition, string then, string els);
string while_loop(string condition, string block);
string read_variable(const char* name);
string write_variable(const char* name);
string writeln_variable(const char* name);
string break_loop();
string return_value(const char* value);
