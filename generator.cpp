#include "defs.h"
#include "generator.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

bool error = false;

void set_error() { error = true; }

extern FILE *yyin;
extern int yyparse();
void generate_llvm_code();

GlobalBlock *global_block;

string filename;
stack<LoopStatement *> loop_stack;
map<string, string> var_symbol_table;
map<string, string> array_size_table;
vector<string> literals;
int literal_count = 0;
int variable_count = 0;
int break_count = 0;

int main(int argc, char *argv[]) {
  bool optimize = false;
  // Usage: mypl [-O] <input_file>
  if (argc < 2) {
    cout << "Usage: mypl [-O] <input_file>\n";
    exit(1);
  } else if (argc == 3 && strcmp(argv[1], "-O") != 0) {
    cout << "Usage: mypl [-O] <input_file>\n";
    exit(1);
  } else if (argc == 3) {
    optimize = true;
    filename = argv[2];
    yyin = fopen(argv[2], "r");
  } else {
    filename = argv[1];
    yyin = fopen(argv[1], "r");
  }
  filename = filename.substr(0, filename.find_last_of("."));
  yyparse();
  fclose(yyin);
  if (error) {
    cout << "error: failed to parse " << filename << endl;
    return 1;
  }
  generate_llvm_code();

  if (optimize) {
    if (system(("opt -S -O3 " + filename + ".ll -o " + filename + ".ll")
                   .c_str()) != 0) {
      cout << "error: failed to optimize ir" << endl;
      return 1;
    };
    system(("llvm-as " + filename + ".ll -o " + filename + ".bc").c_str());
    system(("llc " + filename + ".bc -o " + filename + ".s").c_str());
  } else {
    if (system(("opt -S -O0 " + filename + ".ll -o " + filename + ".ll")
                   .c_str()) != 0) {
      cout << "error: failed to validate ir" << endl;
      return 1;
    }
    system(("llvm-as " + filename + ".ll -o " + filename + ".bc").c_str());
    system(("llc " + filename + ".bc -o " + filename + ".s").c_str());
  }
}

string create_variable() { return "%" + to_string(variable_count++); }
string create_label() { return to_string(variable_count++); }
string reset_variable_count() { return to_string(variable_count = 0); }
string create_break_var() { return "#" + to_string(break_count++); }

void *create_id(char *ID) {
  IDNode *idNode = new IDNode(string(ID));
  return idNode;
}

void *create_num(char *NUM) {
  NumNode *numNode = new NumNode(string(NUM));
  return numNode;
}

string get_variable(string text) { return var_symbol_table[text]; }

void set_variable(string text, string variable) {
  var_symbol_table[text] = variable;
}

string get_array_size(string text) { return array_size_table[text]; }

void set_array_size(string text, string size) { array_size_table[text] = size; }

void generate_llvm_code() {
  fstream file;
  file.open(filename + ".ll", ios::out);
  string llvm_code = global_block->generate_llvm();
  file << llvm_code;
  cout << "llvm ir code generated to " << filename << ".ll" << endl;
}

void create_program(void *globalBlock) {
  global_block = (GlobalBlock *)globalBlock;
}

void *create_global_block(void *globalConstDecl, void *globalVarDecl,
                          void *funcDecl, void *procDecl, void *statement) {
  GlobalBlock *global_block = new GlobalBlock(globalConstDecl, globalVarDecl,
                                              funcDecl, procDecl, statement);
  return global_block;
}

void *create_simple_block(void *localConstDecl, void *localVarDecl,
                          void *statement) {
  LocalBlock *block = new LocalBlock(localConstDecl, localVarDecl, statement);
  return block;
}

void *resolve_global_const_assignment(void *globalConstAssList) {
  GlobalConstDecl *globalConstDecl = new GlobalConstDecl(globalConstAssList);
  return globalConstDecl;
}

void *add_global_const_assignment(void *globalConstAssign) {
  GlobalConstAssignmentList *globalConstAssList =
      new GlobalConstAssignmentList(globalConstAssign);
  return globalConstAssList;
}

void *append_global_const_assignment(void *globalConstAssList,
                                     void *globalConstAssign) {
  GlobalConstAssignmentList *globalConstAssignList =
      (GlobalConstAssignmentList *)globalConstAssList;
  globalConstAssignList->append((GlobalConstNode *)globalConstAssign);
  return globalConstAssignList;
}

void *create_global_const_assignment(void *ID, void *NUM) {
  GlobalConstNode *globalConstNode = new GlobalConstAssignment(ID, NUM);
  return globalConstNode;
}

void *create_global_const_array_assignment(void *ID, void *numList) {
  GlobalConstNode *globalConstNode =
      new GlobalConstArrayAssignment(ID, numList);
  return globalConstNode;
}

void *resolve_local_const_assignment(void *constAssList) {
  LocalConstDecl *localConstDecl = new LocalConstDecl(constAssList);
  return localConstDecl;
}

void *add_local_const_assignment(void *constAssign) {
  LocalConstAssignmentList *localConstAssList =
      new LocalConstAssignmentList(constAssign);
  return localConstAssList;
}

void *append_local_const_assignment(void *constAssList, void *constAssign) {
  LocalConstAssignmentList *localConstAssignList =
      (LocalConstAssignmentList *)constAssList;
  localConstAssignList->append((LocalConstNode *)constAssign);
  return localConstAssignList;
}

void *create_local_const_assignment(void *ID, void *NUM) {
  LocalConstNode *localConstNode = new LocalConstAssignment(ID, NUM);
  return localConstNode;
}

void *create_local_const_array_assignment(void *ID, void *numList) {
  LocalConstNode *localConstNode = new LocalConstArrayAssignment(ID, numList);
  return localConstNode;
}

void *add_num_list(void *NUM) {
  NumList *numList = new NumList(NUM);
  return numList;
}

void *append_num_list(void *numList, void *NUM) {
  NumList *numListPtr = (NumList *)numList;
  numListPtr->append((NumNode *)NUM);
  return numListPtr;
}

void *resolve_global_var_assignment(void *globalVarDeclList) {
  GlobalVarDecl *globalVarDecl = new GlobalVarDecl(globalVarDeclList);
  return globalVarDecl;
}

void *add_global_var_assignment(void *globalVariableDec) {
  GlobalVarAssignmentList *globalVarAssList =
      new GlobalVarAssignmentList(globalVariableDec);
  return globalVarAssList;
}

void *append_global_var_assignment(void *globalVarDeclList,
                                   void *globalVariableDec) {
  GlobalVarAssignmentList *globalVarAssignList =
      (GlobalVarAssignmentList *)globalVarDeclList;
  globalVarAssignList->append((GlobalVarNode *)globalVariableDec);
  return globalVarAssignList;
}

void *create_global_var_assignment(void *ID) {
  GlobalVarNode *globalVarNode = new GlobalVarAssignment(ID);
  return globalVarNode;
}

void *create_global_var_array_assignment(void *ID, void *NUM) {
  GlobalVarNode *globalVarNode = new GlobalVarArrayAssignment(ID, NUM);
  return globalVarNode;
}

void *resolve_local_var_assignment(void *varDeclList) {
  LocalVarDecl *localVarDecl = new LocalVarDecl(varDeclList);
  return localVarDecl;
}

void *add_local_var_assignment(void *variableDec) {
  LocalVarAssignmentList *localVarAssList =
      new LocalVarAssignmentList(variableDec);
  return localVarAssList;
}

void *append_local_var_assignment(void *varDeclList, void *variableDec) {
  LocalVarAssignmentList *localVarAssignList =
      (LocalVarAssignmentList *)varDeclList;
  localVarAssignList->append((LocalVarNode *)variableDec);
  return localVarAssignList;
}

void *create_local_var_assignment(void *ID) {
  LocalVarNode *localVarNode = new LocalVarAssignment(ID);
  return localVarNode;
}

void *create_local_var_array_assignment(void *ID, void *NUM) {
  LocalVarNode *localVarNode = new LocalVarArrayAssignment(ID, NUM);
  return localVarNode;
}

void *append_proc_decl(void *procDecl, void *ID, void *block) {
  ProcDecl *newProcDecl = new ProcDecl(procDecl, ID, block);
  return newProcDecl;
}

void *append_func_decl(void *funcDecl, void *ID, void *paramList, void *block) {
  FuncDecl *newFuncDecl = new FuncDecl(funcDecl, ID, paramList, block);
  return newFuncDecl;
}

void *create_param_list(void *ID) {
  ParamList *paramList = new ParamList(ID);
  return paramList;
}

void *append_param_list(void *paramList, void *ID) {
  ParamList *paramListPtr = (ParamList *)paramList;
  paramListPtr->append((IDNode *)ID);
  return paramListPtr;
}

void *create_store_statement(void *ID, void *expression) {
  Statement *storeStatement = new StoreStatement(ID, expression);
  return storeStatement;
}

void *create_store_array_statement(void *ID, void *index_expression,
                                   void *expression) {
  Statement *storeArrayStatement =
      new StoreArrayStatement(ID, index_expression, expression);
  return storeArrayStatement;
}

void *create_call_statement(void *ID) {
  Statement *callStatement = new CallStatement(ID);
  return callStatement;
}

void *resolve_statement_list(void *statementList) {
  BlockStatement *stmtList = new BlockStatement(statementList);
  return stmtList;
}

void *create_if_statement(void *condition, void *then_statement,
                          void *else_statement) {
  Statement *ifStatement =
      new IfStatement(condition, then_statement, else_statement);
  return ifStatement;
}

void *create_while_statement(void *condition, void *statement) {
  Statement *whileStatement = new WhileStatement(condition, statement);
  return whileStatement;
}

void *create_for_statement(void *ID, void *start_expression,
                           void *end_expression, void *statement) {
  Statement *forStatement =
      new ForStatement(ID, start_expression, end_expression, statement);
  return forStatement;
}

void *create_read_statement(void *ID) {
  Statement *readStatement = new ReadStatement(ID);
  return readStatement;
}

void *create_write_statement(void *ID) {
  Statement *writeStatement = new WriteStatement(ID);
  return writeStatement;
}

void *create_writeln_statement(void *ID) {
  Statement *writelnStatement = new WritelnStatement(ID);
  return writelnStatement;
}

void *create_break_statement() {
  Statement *breakStatement = new BreakStatement();
  return breakStatement;
}

void *create_return_statement(void *expression) {
  Statement *returnStatement = new ReturnStatement(expression);
  return returnStatement;
}

void *create_statement_list(void *statement) {
  StatementList *stmtList = new StatementList(statement);
  return stmtList;
}

void *append_statement_list(void *statementList, void *statement) {
  StatementList *stmtListPtr = (StatementList *)statementList;
  stmtListPtr->append((Statement *)statement);
  return stmtListPtr;
}

void *create_odd_condition(void *expression) {
  OddCondition *oddCondition = new OddCondition(expression);
  return oddCondition;
}

void *create_eq_condition(void *expression_1, void *expression_2) {
  EqCondition *eqCondition = new EqCondition(expression_1, expression_2);
  return eqCondition;
}

void *create_ne_condition(void *expression_1, void *expression_2) {
  NeCondition *neCondition = new NeCondition(expression_1, expression_2);
  return neCondition;
}

void *create_lt_condition(void *expression_1, void *expression_2) {
  LtCondition *ltCondition = new LtCondition(expression_1, expression_2);
  return ltCondition;
}

void *create_le_condition(void *expression_1, void *expression_2) {
  LeCondition *leCondition = new LeCondition(expression_1, expression_2);
  return leCondition;
}

void *create_gt_condition(void *expression_1, void *expression_2) {
  GtCondition *gtCondition = new GtCondition(expression_1, expression_2);
  return gtCondition;
}

void *create_ge_condition(void *expression_1, void *expression_2) {
  GeCondition *geCondition = new GeCondition(expression_1, expression_2);
  return geCondition;
}

void *create_add_expression(void *expression, void *term) {
  Expression *addExpression = new AddExpression(expression, term);
  return addExpression;
}

void *create_sub_expression(void *expression, void *term) {
  Expression *subExpression = new SubExpression(expression, term);
  return subExpression;
}

void *create_negate_expression(void *term) {
  Expression *negateExpression = new NegateExpression(term);
  return negateExpression;
}

void *create_mul_expression(void *term, void *factor) {
  Expression *mulExpression = new MulExpression(term, factor);
  return mulExpression;
}

void *create_div_expression(void *term, void *factor) {
  Expression *divExpression = new DivExpression(term, factor);
  return divExpression;
}

void *create_mod_expression(void *term, void *factor) {
  Expression *modExpression = new ModExpression(term, factor);
  return modExpression;
}

void *create_id_expression(void *ID) {
  Expression *idExpression = new IdExpression(ID);
  return idExpression;
}

void *create_array_expression(void *ID, void *expression) {
  Expression *arrayExpression = new ArrayExpression(ID, expression);
  return arrayExpression;
}

void *create_num_expression(void *NUM) {
  Expression *numExpression = new NumExpression(NUM);
  return numExpression;
}

void *create_call_expression(void *ID, void *argList) {
  Expression *callExpression = new CallExpression(ID, argList);
  return callExpression;
}

void *create_arg_list(void *expression) {
  ArgList *argList = new ArgList(expression);
  return argList;
}

void *append_arg_list(void *argList, void *expression) {
  ArgList *argListPtr = (ArgList *)argList;
  argListPtr->append((Expression *)expression);
  return argListPtr;
}

string GlobalBlock::generate_llvm() {
  string llvm_code = "";
  llvm_code += "; ModuleID = 'mypl'\n";
  llvm_code += "declare i32 @printf(i8*, ...)\n";
  llvm_code += "declare i32 @scanf(i8*, ...)\n";
  llvm_code += "@readwrite.str = constant [3 x i8] c\"%d\\00\"\n";
  llvm_code += "@writeln.str = constant [4 x i8] c\"%d\\0A\\00\"\n\n";
  llvm_code += "declare void @llvm.memcpy.p0.p0.i64(ptr, ptr, i64, i1)\n\n";

  string main_code = "";
  ParserNode *globalConstDecl = (GlobalConstDecl *)this->globalConstDecl;
  if (globalConstDecl != NULL) {
    main_code += globalConstDecl->generate_llvm();
  }
  ParserNode *globalVarDecl = (GlobalVarDecl *)this->globalVarDecl;
  if (globalVarDecl != NULL) {
    main_code += globalVarDecl->generate_llvm();
  }
  ParserNode *funcDecl = (FuncDecl *)this->funcDecl;
  if (funcDecl != NULL) {
    main_code += funcDecl->generate_llvm();
  }
  ParserNode *procDecl = (ProcDecl *)this->procDecl;
  if (procDecl != NULL) {
    main_code += procDecl->generate_llvm();
  }

  for (size_t i = 0; i < ::literals.size(); i++) {
    llvm_code += ::literals[i];
  }

  llvm_code += main_code;

  llvm_code += "define i32 @main() {\n";
  create_label(); // create the initial label
  Statement *statement = (Statement *)this->statement;
  llvm_code += statement->generate_llvm();
  llvm_code += "   ret i32 0\n";
  llvm_code += "}\n";

  return llvm_code;
}

string LocalBlock::generate_llvm() {
  string llvm_code = "";
  ParserNode *localVarDecl = this->localVarDecl;
  if (localVarDecl != NULL) {
    llvm_code += localVarDecl->generate_llvm();
  }
  ParserNode *localConstDecl = this->localConstDecl;
  if (localConstDecl != NULL) {
    llvm_code += localConstDecl->generate_llvm();
  }
  Statement *statement = (Statement *)this->statement;
  llvm_code += statement->generate_llvm();
  return llvm_code;
}

string GlobalConstDecl::generate_llvm() {
  ParserNode *globalConstNode = this->globalConstAssignments;
  return globalConstNode->generate_llvm();
}

string GlobalConstAssignmentList::generate_llvm() {
  string llvm_code = "";
  vector<GlobalConstNode *> globalConstAssignments =
      this->globalConstAssignments;
  for (size_t i = 0; i < globalConstAssignments.size(); i++) {
    llvm_code += globalConstAssignments[i]->generate_llvm();
  }
  return llvm_code;
}

string GlobalConstAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), "@" + this->ID->get_text());
  string llvm_code = "";
  llvm_code += get_variable(this->ID->get_text()) + " = constant i32 " +
               this->value->get_text() + "\n";
  return llvm_code;
}

string GlobalConstArrayAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), "@" + this->ID->get_text());
  set_array_size(this->ID->get_text(), to_string(this->values->size()));
  string llvm_code = "";
  llvm_code += get_variable(this->ID->get_text()) + " = constant [" +
               to_string(this->values->size()) + " x i32] ";
  llvm_code += this->values->generate_llvm();
  llvm_code += "\n";
  return llvm_code;
}

string NumList::generate_llvm() {
  string llvm_code = "[";
  vector<PrimitiveNode *> values = this->numList;
  for (size_t i = 0; i < values.size(); i++) {
    llvm_code += "i32 " + values[i]->get_text();
    if (i < values.size() - 1) {
      llvm_code += ", ";
    }
  }
  llvm_code += "]";
  return llvm_code;
}

string GlobalVarDecl::generate_llvm() {
  ParserNode *globalVarNode = this->globalVarAssignments;
  return globalVarNode->generate_llvm() + "\n";
}

string GlobalVarAssignmentList::generate_llvm() {
  string llvm_code = "";
  vector<GlobalVarNode *> globalVarAssignments = this->globalVarAssignments;
  for (size_t i = 0; i < globalVarAssignments.size(); i++) {
    llvm_code += globalVarAssignments[i]->generate_llvm();
  }
  return llvm_code;
}

string GlobalVarAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), "@" + this->ID->get_text());
  string llvm_code = "";
  llvm_code += get_variable(this->ID->get_text()) + " = global i32 0\n";
  return llvm_code;
}

string GlobalVarArrayAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), "@" + this->ID->get_text());
  set_array_size(this->ID->get_text(), this->size->get_text());
  string llvm_code = "";
  llvm_code += get_variable(this->ID->get_text()) + " = global [" +
               this->size->get_text() + " x i32] zeroinitializer\n";
  return llvm_code;
}

string LocalConstDecl::generate_llvm() {
  ParserNode *localConstNode = this->localConstAssignments;
  return localConstNode->generate_llvm();
}

string LocalConstAssignmentList::generate_llvm() {
  string llvm_code = "";
  vector<LocalConstNode *> localConstAssignments = this->localConstAssignments;
  for (size_t i = 0; i < localConstAssignments.size(); i++) {
    llvm_code += localConstAssignments[i]->generate_llvm();
  }
  return llvm_code;
}

string LocalConstAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), create_variable());
  string llvm_code = "";
  llvm_code += "   " + get_variable(this->ID->get_text()) + " = alloca i32\n";
  llvm_code += "   store i32 " + this->value->get_text() + ", i32* %" +
               this->ID->get_text() + "\n";
  return llvm_code;
}

string LocalConstArrayAssignment::generate_llvm() {
  string literal_name = "@__literal" + to_string(++literal_count);
  string literal_code = literal_name + " = constant [" +
                        to_string(this->values->size()) + " x i32] ";
  literal_code += this->values->generate_llvm() + "\n";
  ::literals.push_back(literal_code);

  set_variable(this->ID->get_text(), create_variable());
  set_array_size(this->ID->get_text(), to_string(this->values->size()));
  string llvm_code = "";
  llvm_code += "   " + get_variable(this->ID->get_text()) + " = alloca [" +
               to_string(this->values->size()) + " x i32]\n";
  llvm_code += "   call void @llvm.memcpy.p0.p0.i64(ptr " +
               get_variable(this->ID->get_text()) + ", ptr " + literal_name +
               ", i64 " + to_string(this->values->size() * 4) + ", i1 false)\n";
  return llvm_code;
}

string LocalVarDecl::generate_llvm() {
  ParserNode *localVarNode = this->localVarAssignments;
  return localVarNode->generate_llvm();
}

string LocalVarAssignmentList::generate_llvm() {
  string llvm_code = "";
  vector<LocalVarNode *> localVarAssignments = this->localVarAssignments;
  for (size_t i = 0; i < localVarAssignments.size(); i++) {
    llvm_code += localVarAssignments[i]->generate_llvm();
  }
  return llvm_code;
}

string LocalVarAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), create_variable());
  string llvm_code = "";
  llvm_code += "   " + get_variable(this->ID->get_text()) + " = alloca i32\n";
  return llvm_code;
}

string LocalVarArrayAssignment::generate_llvm() {
  set_variable(this->ID->get_text(), create_variable());
  set_array_size(this->ID->get_text(), this->size->get_text());
  string llvm_code = "";
  llvm_code += "   " + get_variable(this->ID->get_text()) + " = alloca [" +
               this->size->get_text() + " x i32]\n";
  return llvm_code;
}

string ProcDecl::generate_llvm() {
  string llvm_code = "";
  if (this->prev != NULL) {
    llvm_code += this->prev->generate_llvm();
  }
  llvm_code += "define void @" + this->ID->get_text() + "() {\n";
  create_label(); // create the initial label
  llvm_code += this->block->generate_llvm();
  llvm_code += "   ret void\n";
  llvm_code += "}\n\n";
  reset_variable_count();
  return llvm_code;
}

string FuncDecl::generate_llvm() {
  string llvm_code = "";
  if (this->prev != NULL) {
    llvm_code += this->prev->generate_llvm();
  }
  llvm_code += "define i32 @" + this->ID->get_text() + "(";
  llvm_code += this->paramList->generate_llvm();
  llvm_code += ") {\n";
  create_label(); // create the initial label
  llvm_code += this->paramList->allocate_parameters();
  llvm_code += this->block->generate_llvm();
  llvm_code += "   ret i32 0\n";
  llvm_code += "}\n\n";
  reset_variable_count();
  return llvm_code;
}

string ParamList::generate_llvm() {
  string llvm_code = "";
  vector<PrimitiveNode *> params = this->paramList;
  for (size_t i = 0; i < params.size(); i++) {
    set_variable(params[i]->get_text(), create_variable());
    llvm_code += "i32 " + get_variable(params[i]->get_text());
    if (i < params.size() - 1) {
      llvm_code += ", ";
    }
  }
  return llvm_code;
}

string ParamList::allocate_parameters() {
  string llvm_code = "";
  vector<PrimitiveNode *> params = this->paramList;
  for (size_t i = 0; i < params.size(); i++) {
    string variable = get_variable(params[i]->get_text());
    set_variable(params[i]->get_text(), create_variable());

    llvm_code +=
        "   " + get_variable(params[i]->get_text()) + " = alloca i32\n";
    llvm_code += "   store i32 " + variable + ", i32* " +
                 get_variable(params[i]->get_text()) + "\n";
  }
  return llvm_code;
}

string BlockStatement::generate_llvm() {
  return this->statementList->generate_llvm();
}

string StatementList::generate_llvm() {
  string llvm_code = "";
  vector<ParserNode *> statements = this->statementList;
  for (size_t i = 0; i < statements.size(); i++) {
    llvm_code += statements[i]->generate_llvm();
  }
  return llvm_code;
}

string StoreStatement::generate_llvm() {
  string llvm_code = "";
  Expression *expression = this->expression;
  llvm_code += expression->generate_llvm();
  llvm_code += "   store i32 " + expression->get_variable() + ", i32* " +
               get_variable(this->ID->get_text()) + "\n";
  return llvm_code;
}

string StoreArrayStatement::generate_llvm() {
  string llvm_code = "";
  Expression *index_expression = this->index_expression;
  Expression *expression = this->expression;
  llvm_code += index_expression->generate_llvm();
  llvm_code += expression->generate_llvm();

  string temp = create_variable();
  llvm_code += temp + " = getelementptr inbounds [" +
               get_array_size(this->ID->get_text()) + " x i32], [" +
               get_array_size(this->ID->get_text()) + " x i32]* " +
               get_variable(this->ID->get_text()) + ", i32 0, i32 " +
               index_expression->get_variable() + "\n";
  llvm_code +=
      "   store i32 " + expression->get_variable() + ", i32* " + temp + "\n";
  return llvm_code;
}

string CallStatement::generate_llvm() {
  string llvm_code = "";
  llvm_code += "   call void @" + this->ID->get_text() + "()\n";
  return llvm_code;
}

string IfStatement::generate_llvm() {
  string llvm_code = "";

  Condition *condition = this->condition;
  Statement *then_statement = this->then_statement;
  Statement *else_statement = this->else_statement;

  string condition_code = condition->generate_llvm();
  string then_label = create_label();
  string then_code = then_statement->generate_llvm();
  string else_label = create_label();

  llvm_code += condition_code;
  llvm_code += "   br i1 " + condition->get_variable() + ", label %" +
               then_label + ", label %" + else_label + "\n";
  llvm_code += then_label + ":\n";
  llvm_code += then_code;

  if (else_statement != NULL) {
    string else_code = else_statement->generate_llvm();
    string end_label = create_label();

    llvm_code += "   br label %" + end_label + "\n";
    llvm_code += else_label + ":\n";
    llvm_code += else_code;
    llvm_code += "   br label %" + end_label + "\n";
    llvm_code += end_label + ":\n";

  } else {
    llvm_code += "   br label %" + else_label + "\n";
    llvm_code += else_label + ":\n";
  }

  return llvm_code;
}

string WhileStatement::generate_llvm() {
  string llvm_code = "";

  loop_stack.push(this);

  Condition *condition = this->condition;
  Statement *statement = this->statement;

  string condition_label = create_label();
  string condition_code = condition->generate_llvm();
  string loop_label = create_label();
  string statement_code = statement->generate_llvm();
  string end_label = create_label();

  if (has_break()) {
    for (size_t i = 0; i < this->break_statements.size(); i++) {
      statement_code.replace(statement_code.find(this->break_statements[i]),
                             this->break_statements[i].length(), end_label);
    }
  }

  loop_stack.pop();

  llvm_code += "   br label %" + condition_label + "\n";
  llvm_code += condition_label + ":\n";
  llvm_code += condition_code;
  llvm_code += "   br i1 " + condition->get_variable() + ", label %" +
               loop_label + ", label %" + end_label + "\n";
  llvm_code += loop_label + ":\n";
  llvm_code += statement_code;
  llvm_code += "   br label %" + condition_label + "\n";
  llvm_code += end_label + ":\n";

  return llvm_code;
}

string ForStatement::generate_llvm() {
  string llvm_code = "";

  loop_stack.push(this);

  Statement *store_statement =
      new StoreStatement(this->ID, this->start_expression);
  Condition *condition =
      new LtCondition(new IdExpression(this->ID), this->end_expression);
  Statement *increment_statement = new StoreStatement(
      this->ID, new AddExpression(new IdExpression(this->ID),
                                  new NumExpression(new NumNode("1"))));

  Statement *statement = this->statement;

  string store_code = store_statement->generate_llvm();
  string condition_label = create_label();
  string condition_code = condition->generate_llvm();
  string loop_label = create_label();
  string loop_code = statement->generate_llvm();
  string increment_code = increment_statement->generate_llvm();
  string end_label = create_label();

  if (has_break()) {
    for (size_t i = 0; i < this->break_statements.size(); i++) {
      loop_code.replace(loop_code.find(this->break_statements[i]),
                        this->break_statements[i].length(), end_label);
    }
  }

  loop_stack.pop();

  llvm_code += store_code;
  llvm_code += "   br label %" + condition_label + "\n";
  llvm_code += condition_label + ":\n";
  llvm_code += condition_code;
  llvm_code += "   br i1 " + condition->get_variable() + ", label %" +
               loop_label + ", label %" + end_label + "\n";
  llvm_code += loop_label + ":\n";
  llvm_code += loop_code;
  llvm_code += increment_code;
  llvm_code += "   br label %" + condition_label + "\n";
  llvm_code += end_label + ":\n";

  return llvm_code;
}

string ReadStatement::generate_llvm() {
  string llvm_code = "";
  llvm_code += "   " + create_variable() +
               " = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x "
               "i8], [3 x i8]* @readwrite.str, i32 0, i32 0), i32* " +
               get_variable(this->ID->get_text()) + ")\n";
  return llvm_code;
}

string WriteStatement::generate_llvm() {
  string llvm_code = "";
  string variable = create_variable();
  llvm_code += "   " + variable + " = load i32, i32* " +
               get_variable(this->ID->get_text()) + "\n";
  llvm_code +=
      "   " + create_variable() +
      " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x "
      "i8], [3 x i8]* @readwrite.str, i32 0, i32 0), i32 " +
      variable + ")\n";
  return llvm_code;
}

string WritelnStatement::generate_llvm() {
  string llvm_code = "";
  string variable = create_variable();
  llvm_code += "   " + variable + " = load i32, i32* " +
               get_variable(this->ID->get_text()) + "\n";
  llvm_code +=
      "   " + create_variable() +
      " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x "
      "i8], [4 x i8]* @writeln.str, i32 0, i32 0), i32 " +
      variable + ")\n";
  return llvm_code;
}

string BreakStatement::generate_llvm() {
  string variable = create_break_var();
  if (loop_stack.empty()) {
    throw runtime_error("break statement not within a loop");
  }
  loop_stack.top()->add_break(variable);
  string llvm_code = "";
  llvm_code += "   br label %" + variable + "\n";
  create_label(); // create the basic block after the break statement
  return llvm_code;
}

string ReturnStatement::generate_llvm() {
  string llvm_code = "";
  Expression *expression = this->expression;
  llvm_code += expression->generate_llvm();
  llvm_code += "   ret i32 " + expression->get_variable() + "\n";
  create_label(); // create the basic block after the return statement
  return llvm_code;
}

string OddCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression = this->expression;
  llvm_code += expression->generate_llvm();
  string temp = create_variable();
  this->set_variable(create_variable());
  llvm_code +=
      "   " + temp + " = srem i32 " + expression->get_variable() + ", 2\n";
  llvm_code +=
      "   " + this->get_variable() + " = icmp eq i32 " + temp + ", 1\n";
  return llvm_code;
}

string EqCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression_1 = this->expression_1;
  Expression *expression_2 = this->expression_2;
  llvm_code += expression_1->generate_llvm();
  llvm_code += expression_2->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = icmp eq i32 " +
               expression_1->get_variable() + ", " +
               expression_2->get_variable() + "\n";
  return llvm_code;
}

string NeCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression_1 = this->expression_1;
  Expression *expression_2 = this->expression_2;
  llvm_code += expression_1->generate_llvm();
  llvm_code += expression_2->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = icmp ne i32 " +
               expression_1->get_variable() + ", " +
               expression_2->get_variable() + "\n";
  return llvm_code;
}

string LtCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression_1 = this->expression_1;
  Expression *expression_2 = this->expression_2;
  llvm_code += expression_1->generate_llvm();
  llvm_code += expression_2->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = icmp slt i32 " +
               expression_1->get_variable() + ", " +
               expression_2->get_variable() + "\n";
  return llvm_code;
}

string LeCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression_1 = this->expression_1;
  Expression *expression_2 = this->expression_2;
  llvm_code += expression_1->generate_llvm();
  llvm_code += expression_2->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = icmp sle i32 " +
               expression_1->get_variable() + ", " +
               expression_2->get_variable() + "\n";
  return llvm_code;
}

string GtCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression_1 = this->expression_1;
  Expression *expression_2 = this->expression_2;
  llvm_code += expression_1->generate_llvm();
  llvm_code += expression_2->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = icmp sgt i32 " +
               expression_1->get_variable() + ", " +
               expression_2->get_variable() + "\n";
  return llvm_code;
}

string GeCondition::generate_llvm() {
  string llvm_code = "";
  Expression *expression_1 = this->expression_1;
  Expression *expression_2 = this->expression_2;
  llvm_code += expression_1->generate_llvm();
  llvm_code += expression_2->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = icmp sge i32 " +
               expression_1->get_variable() + ", " +
               expression_2->get_variable() + "\n";
  return llvm_code;
}

string AddExpression::generate_llvm() {
  string llvm_code = "";
  Expression *expression = this->expression;
  Expression *term = this->term;
  llvm_code += expression->generate_llvm();
  llvm_code += term->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = add i32 " +
               expression->get_variable() + ", " + term->get_variable() + "\n";
  return llvm_code;
}

string SubExpression::generate_llvm() {
  string llvm_code = "";
  Expression *expression = this->expression;
  Expression *term = this->term;
  llvm_code += expression->generate_llvm();
  llvm_code += term->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = sub i32 " +
               expression->get_variable() + ", " + term->get_variable() + "\n";
  return llvm_code;
}

string NegateExpression::generate_llvm() {
  string llvm_code = "";
  Expression *term = this->term;
  llvm_code += term->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = sub i32 0, " +
               term->get_variable() + "\n";
  return llvm_code;
}

string MulExpression::generate_llvm() {
  string llvm_code = "";
  Expression *term = this->term;
  Expression *factor = this->factor;
  llvm_code += term->generate_llvm();
  llvm_code += factor->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = mul i32 " +
               term->get_variable() + ", " + factor->get_variable() + "\n";
  return llvm_code;
}

string DivExpression::generate_llvm() {
  string llvm_code = "";
  Expression *term = this->term;
  Expression *factor = this->factor;
  llvm_code += term->generate_llvm();
  llvm_code += factor->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = sdiv i32 " +
               term->get_variable() + ", " + factor->get_variable() + "\n";
  return llvm_code;
}

string ModExpression::generate_llvm() {
  string llvm_code = "";
  Expression *term = this->term;
  Expression *factor = this->factor;
  llvm_code += term->generate_llvm();
  llvm_code += factor->generate_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = srem i32 " +
               term->get_variable() + ", " + factor->get_variable() + "\n";
  return llvm_code;
}

string IdExpression::generate_llvm() {
  this->set_variable(create_variable());
  return "   " + this->get_variable() + " = load i32, i32* " +
         ::get_variable(this->ID->get_text()) + "\n";
}

string ArrayExpression::generate_llvm() {
  string llvm_code = "";
  Expression *expression = this->expression;
  llvm_code += expression->generate_llvm();
  string temp = create_variable();
  this->set_variable(create_variable());
  llvm_code += "   " + temp +
               " = getelementptr "
               "inbounds [" +
               get_array_size(this->ID->get_text()) + " x i32], [" +
               get_array_size(this->ID->get_text()) + " x i32]* " +
               ::get_variable(this->ID->get_text()) + ", i32 0, i32 " +
               expression->get_variable() + "\n";
  llvm_code +=
      "   " + this->get_variable() + " = load i32, i32* " + temp + "\n";
  return llvm_code;
}

string NumExpression::generate_llvm() {
  set_variable(this->NUM->get_text());
  return "";
}

string CallExpression::generate_llvm() {
  string llvm_code = "";
  ArgList *argList = this->argList;
  llvm_code += argList->generate_expressions_llvm();
  this->set_variable(create_variable());
  llvm_code += "   " + this->get_variable() + " = call i32 @" +
               this->ID->get_text() + "(";
  llvm_code += argList->generate_llvm();
  llvm_code += ")\n";
  return llvm_code;
}

string ArgList::generate_expressions_llvm() {
  string llvm_code = "";
  vector<Expression *> args = this->expressions;
  for (size_t i = 0; i < args.size(); i++) {
    llvm_code += args[i]->generate_llvm();
  }
  return llvm_code;
}

string ArgList::generate_llvm() {
  string llvm_code = "";
  vector<Expression *> args = this->expressions;
  for (size_t i = 0; i < args.size(); i++) {
    llvm_code += "i32 " + args[i]->get_variable();
    if (i < args.size() - 1) {
      llvm_code += ", ";
    }
  }
  return llvm_code;
}
