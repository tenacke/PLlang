#include <string>
#include <vector>

using namespace std;

string create_variable();
string create_label();
string reset_variable_count();
string get_variable(string text);
void set_variable(string text, string variable);
string get_array_size(string text);
void set_array_size(string text, string size);
void *create_id(char *ID);
void *create_num(char *NUM);

class ParserNode {
public:
  virtual string generate_llvm() = 0;
};

class PrimitiveNode {
protected:
  string text;

public:
  string get_text() { return text; }
};

class IDNode : public PrimitiveNode {
public:
  IDNode(string ID) { this->text = ID; }
};

class NumNode : public PrimitiveNode {
public:
  NumNode(string NUM) { this->text = NUM; }
};

class NumList : public ParserNode {
private:
  vector<PrimitiveNode *> numList;

public:
  NumList(void *NUM) { this->numList.push_back((NumNode *)NUM); }
  void append(NumNode *NUM) { numList.push_back(NUM); }
  int size() { return numList.size(); }
  string get(int i) { return numList[i]->get_text(); }
  string generate_llvm();
};

class GlobalConstNode : public ParserNode {
public:
  virtual string generate_llvm() = 0;
};

class GlobalConstAssignmentList : public ParserNode {
private:
  vector<GlobalConstNode *> globalConstAssignments;

public:
  GlobalConstAssignmentList(void *globalConstAssign) {
    this->globalConstAssignments.push_back(
        (GlobalConstNode *)globalConstAssign);
  }
  void append(GlobalConstNode *globalConstAssign) {
    globalConstAssignments.push_back(globalConstAssign);
  }
  string generate_llvm();
};

class GlobalConstAssignment : public GlobalConstNode {
private:
  PrimitiveNode *ID;
  PrimitiveNode *value;

public:
  GlobalConstAssignment(void *ID, void *value) {
    this->ID = (IDNode *)ID;
    this->value = (NumNode *)value;
  }
  string generate_llvm();
};

class GlobalConstArrayAssignment : public GlobalConstNode {
private:
  PrimitiveNode *ID;
  NumList *values;

public:
  GlobalConstArrayAssignment(void *ID, void *values) {
    this->ID = (IDNode *)ID;
    this->values = (NumList *)values;
  }
  string generate_llvm();
};

class GlobalConstDecl : public ParserNode {
private:
  GlobalConstAssignmentList *globalConstAssignments;

public:
  GlobalConstDecl(void *globalConstAssList) {
    this->globalConstAssignments =
        (GlobalConstAssignmentList *)globalConstAssList;
  }
  string generate_llvm();
};

class GlobalVarNode : public ParserNode {
public:
  virtual string generate_llvm() = 0;
};

class GlobalVarAssignmentList : public ParserNode {
private:
  vector<GlobalVarNode *> globalVarAssignments;

public:
  GlobalVarAssignmentList(void *globalVarDecl) {
    this->globalVarAssignments.push_back((GlobalVarNode *)globalVarDecl);
  }
  void append(GlobalVarNode *globalVarDecl) {
    globalVarAssignments.push_back(globalVarDecl);
  }
  string generate_llvm();
};

class GlobalVarAssignment : public GlobalVarNode {
private:
  PrimitiveNode *ID;

public:
  GlobalVarAssignment(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class GlobalVarArrayAssignment : public GlobalVarNode {
private:
  PrimitiveNode *ID;
  PrimitiveNode *size;

public:
  GlobalVarArrayAssignment(void *ID, void *size) {
    this->ID = (IDNode *)ID;
    this->size = (NumNode *)size;
  }
  string generate_llvm();
};

class GlobalVarDecl : public ParserNode {
private:
  GlobalVarAssignmentList *globalVarAssignments;

public:
  GlobalVarDecl(void *globalVarDeclList) {
    this->globalVarAssignments = (GlobalVarAssignmentList *)globalVarDeclList;
  }
  string generate_llvm();
};

class LocalConstNode : public ParserNode {
public:
  virtual string generate_llvm() = 0;
};

class LocalConstAssignmentList : public ParserNode {
private:
  vector<LocalConstNode *> localConstAssignments;

public:
  LocalConstAssignmentList(void *constAssign) {
    this->localConstAssignments.push_back((LocalConstNode *)constAssign);
  }
  void append(LocalConstNode *constAssign) {
    localConstAssignments.push_back(constAssign);
  }
  string generate_llvm();
};

class LocalConstAssignment : public LocalConstNode {
private:
  PrimitiveNode *ID;
  PrimitiveNode *value;

public:
  LocalConstAssignment(void *ID, void *value) {
    this->ID = (IDNode *)ID;
    this->value = (NumNode *)value;
  }
  string generate_llvm();
};

class LocalConstArrayAssignment : public LocalConstNode {
private:
  PrimitiveNode *ID;
  NumList *values;

public:
  LocalConstArrayAssignment(void *ID, void *values) {
    this->ID = (IDNode *)ID;
    this->values = (NumList *)values;
  }
  string generate_llvm();
};

class LocalConstDecl : public ParserNode {
private:
  LocalConstAssignmentList *localConstAssignments;

public:
  LocalConstDecl(void *constAssList) {
    this->localConstAssignments = (LocalConstAssignmentList *)constAssList;
  }
  string generate_llvm();
};

class LocalVarNode : public ParserNode {
public:
  virtual string generate_llvm() = 0;
};

class LocalVarAssignmentList : public ParserNode {
private:
  vector<LocalVarNode *> localVarAssignments;

public:
  LocalVarAssignmentList(void *varDecl) {
    this->localVarAssignments.push_back((LocalVarNode *)varDecl);
  }
  void append(LocalVarNode *varDecl) { localVarAssignments.push_back(varDecl); }
  string generate_llvm();
};

class LocalVarAssignment : public LocalVarNode {
private:
  PrimitiveNode *ID;

public:
  LocalVarAssignment(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class LocalVarArrayAssignment : public LocalVarNode {
private:
  PrimitiveNode *ID;
  PrimitiveNode *size;

public:
  LocalVarArrayAssignment(void *ID, void *size) {
    this->ID = (IDNode *)ID;
    this->size = (NumNode *)size;
  }
  string generate_llvm();
};

class LocalVarDecl : public ParserNode {
private:
  LocalVarAssignmentList *localVarAssignments;

public:
  LocalVarDecl(void *varDeclList) {
    this->localVarAssignments = (LocalVarAssignmentList *)varDeclList;
  }
  string generate_llvm();
};

class Expression : public ParserNode {
private:
  string variable;

public:
  virtual string generate_llvm() = 0;
  string get_variable() { return variable; }
  void set_variable(string variable) { this->variable = variable; }
};

class AddExpression : public Expression {
private:
  Expression *expression;
  Expression *term;

public:
  AddExpression(void *expression, void *term) {
    this->expression = (Expression *)expression;
    this->term = (Expression *)term;
  }
  string generate_llvm();
};

class SubExpression : public Expression {
private:
  Expression *expression;
  Expression *term;

public:
  SubExpression(void *expression, void *term) {
    this->expression = (Expression *)expression;
    this->term = (Expression *)term;
  }
  string generate_llvm();
};

class NegateExpression : public Expression {
private:
  Expression *term;

public:
  NegateExpression(void *term) { this->term = (Expression *)term; }
  string generate_llvm();
};

class MulExpression : public Expression {
private:
  Expression *term;
  Expression *factor;

public:
  MulExpression(void *term, void *factor) {
    this->term = (Expression *)term;
    this->factor = (Expression *)factor;
  }
  string generate_llvm();
};

class DivExpression : public Expression {
private:
  Expression *term;
  Expression *factor;

public:
  DivExpression(void *term, void *factor) {
    this->term = (Expression *)term;
    this->factor = (Expression *)factor;
  }
  string generate_llvm();
};

class ModExpression : public Expression {
private:
  Expression *term;
  Expression *factor;

public:
  ModExpression(void *term, void *factor) {
    this->term = (Expression *)term;
    this->factor = (Expression *)factor;
  }
  string generate_llvm();
};

class IdExpression : public Expression {
private:
  PrimitiveNode *ID;

public:
  IdExpression(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class ArrayExpression : public Expression {
private:
  PrimitiveNode *ID;
  Expression *expression;

public:
  ArrayExpression(void *ID, void *expression) {
    this->ID = (IDNode *)ID;
    this->expression = (Expression *)expression;
  }
  string generate_llvm();
};

class NumExpression : public Expression {
private:
  PrimitiveNode *NUM;

public:
  NumExpression(void *NUM) { this->NUM = (NumNode *)NUM; }
  string generate_llvm();
};

class ArgList : public ParserNode {
private:
  vector<Expression *> expressions;

public:
  ArgList(void *expression) {
    this->expressions.push_back((Expression *)expression);
  }
  void append(Expression *expression) { expressions.push_back(expression); }
  string generate_llvm();
  string generate_expressions_llvm();
};

class CallExpression : public Expression {
private:
  PrimitiveNode *ID;
  ArgList *argList;

public:
  CallExpression(void *ID, void *argList) {
    this->ID = (IDNode *)ID;
    this->argList = (ArgList *)argList;
  }
  string generate_llvm();
};

class Condition : public ParserNode {
private:
  string variable;

public:
  virtual string generate_llvm() = 0;
  string get_variable() { return variable; }
  void set_variable(string variable) { this->variable = variable; }
};

class OddCondition : public Condition {
private:
  Expression *expression;

public:
  OddCondition(void *expression) {
    this->expression = (Expression *)expression;
  }
  string generate_llvm();
};

class EqCondition : public Condition { // expression_1 == expression_2
private:
  Expression *expression_1;
  Expression *expression_2;

public:
  EqCondition(void *expression_1, void *expression_2) {
    this->expression_1 = (Expression *)expression_1;
    this->expression_2 = (Expression *)expression_2;
  }
  string generate_llvm();
};

class NeCondition : public Condition { // expression_1 != expression_2
private:
  Expression *expression_1;
  Expression *expression_2;

public:
  NeCondition(void *expression_1, void *expression_2) {
    this->expression_1 = (Expression *)expression_1;
    this->expression_2 = (Expression *)expression_2;
  }
  string generate_llvm();
};

class LtCondition : public Condition {
private:
  Expression *expression_1;
  Expression *expression_2;

public:
  LtCondition(void *expression_1, void *expression_2) {
    this->expression_1 = (Expression *)expression_1;
    this->expression_2 = (Expression *)expression_2;
  }
  string generate_llvm();
};

class LeCondition : public Condition {
private:
  Expression *expression_1;
  Expression *expression_2;

public:
  LeCondition(void *expression_1, void *expression_2) {
    this->expression_1 = (Expression *)expression_1;
    this->expression_2 = (Expression *)expression_2;
  }
  string generate_llvm();
};

class GtCondition : public Condition {
private:
  Expression *expression_1;
  Expression *expression_2;

public:
  GtCondition(void *expression_1, void *expression_2) {
    this->expression_1 = (Expression *)expression_1;
    this->expression_2 = (Expression *)expression_2;
  }
  string generate_llvm();
};

class GeCondition : public Condition {
private:
  Expression *expression_1;
  Expression *expression_2;

public:
  GeCondition(void *expression_1, void *expression_2) {
    this->expression_1 = (Expression *)expression_1;
    this->expression_2 = (Expression *)expression_2;
  }
  string generate_llvm();
};

class Statement : public ParserNode {
public:
  virtual string generate_llvm() = 0;
};

class StatementList : public ParserNode {
private:
  vector<ParserNode *> statementList;

public:
  StatementList(void *statement) {
    this->statementList.push_back((Statement *)statement);
  }
  void append(Statement *statement) { statementList.push_back(statement); }
  string generate_llvm();
};

class StoreStatement : public Statement {
private:
  PrimitiveNode *ID;
  Expression *expression;

public:
  StoreStatement(void *ID, void *expression) {
    this->ID = (IDNode *)ID;
    this->expression = (Expression *)expression;
  }
  string generate_llvm();
};

class StoreArrayStatement : public Statement {
private:
  PrimitiveNode *ID;
  Expression *index_expression;
  Expression *expression;

public:
  StoreArrayStatement(void *ID, void *index_expression, void *expression) {
    this->ID = (IDNode *)ID;
    this->index_expression = (Expression *)index_expression;
    this->expression = (Expression *)expression;
  }
  string generate_llvm();
};

class CallStatement : public Statement {
private:
  PrimitiveNode *ID;

public:
  CallStatement(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class BlockStatement : public Statement {
private:
  ParserNode *statementList;

public:
  BlockStatement(void *block) { this->statementList = (StatementList *)block; }
  string generate_llvm();
};

class IfStatement
    : public Statement { // if (condition) then_statement else else_statement
private:
  Condition *condition;
  Statement *then_statement;
  Statement *else_statement;

public:
  IfStatement(void *condition, void *then_statement, void *else_statement) {
    this->condition = (Condition *)condition;
    this->then_statement = (Statement *)then_statement;
    this->else_statement = (Statement *)else_statement;
  }
  string generate_llvm();
};

class WhileStatement : public Statement {
private:
  Condition *condition;
  Statement *statement;

public:
  WhileStatement(void *condition, void *statement) {
    this->condition = (Condition *)condition;
    this->statement = (Statement *)statement;
  }
  string generate_llvm();
};

class ForStatement : public Statement {
private:
  PrimitiveNode *ID;
  Expression *start_expression;
  Expression *end_expression;
  Statement *statement;

public:
  ForStatement(void *ID, void *start_expression, void *end_expression,
               void *statement) {
    this->ID = (IDNode *)ID;
    this->start_expression = (Expression *)start_expression;
    this->end_expression = (Expression *)end_expression;
    this->statement = (Statement *)statement;
  }
  string generate_llvm();
};

class ReadStatement : public Statement {
private:
  PrimitiveNode *ID;

public:
  ReadStatement(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class WriteStatement : public Statement {
private:
  PrimitiveNode *ID;

public:
  WriteStatement(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class WritelnStatement : public Statement {
private:
  PrimitiveNode *ID;

public:
  WritelnStatement(void *ID) { this->ID = (IDNode *)ID; }
  string generate_llvm();
};

class BreakStatement : public Statement {
public:
  string generate_llvm();
};

class ReturnStatement : public Statement {
private:
  Expression *expression;

public:
  ReturnStatement(void *expression) {
    this->expression = (Expression *)expression;
  }
  string generate_llvm();
};

class LocalBlock : public ParserNode {
private:
  LocalConstDecl *localConstDecl;
  LocalVarDecl *localVarDecl;
  Statement *statement;

public:
  LocalBlock(void *localConstDecl, void *localVarDecl, void *statement) {
    this->localConstDecl = (LocalConstDecl *)localConstDecl;
    this->localVarDecl = (LocalVarDecl *)localVarDecl;
    this->statement = (Statement *)statement;
  }
  string generate_llvm();
};

class ProcDecl : public ParserNode {
private:
  ParserNode *prev;
  PrimitiveNode *ID;
  LocalBlock *block;

public:
  ProcDecl(void *prev, void *ID, void *block) {
    this->prev = (ProcDecl *)prev;
    this->ID = (IDNode *)ID;
    this->block = (LocalBlock *)block;
  }
  string generate_llvm();
};

class ParamList : public ParserNode {
private:
  vector<PrimitiveNode *> paramList;

public:
  ParamList(void *ID) { this->paramList.push_back((IDNode *)ID); }
  void append(IDNode *ID) { paramList.push_back(ID); }
  string generate_llvm();
  string allocate_parameters();
};

class FuncDecl : public ParserNode {
private:
  FuncDecl *prev;
  PrimitiveNode *ID;
  ParamList *paramList;
  LocalBlock *block;

public:
  FuncDecl(void *prev, void *ID, void *paramList, void *block) {
    this->prev = (FuncDecl *)prev;
    this->ID = (IDNode *)ID;
    this->paramList = (ParamList *)paramList;
    this->block = (LocalBlock *)block;
  }
  string generate_llvm();
};

class GlobalBlock : public ParserNode {
private:
  ParserNode *globalConstDecl;
  ParserNode *globalVarDecl;
  ParserNode *funcDecl;
  ParserNode *procDecl;
  ParserNode *statement;

public:
  GlobalBlock(void *_globalConstDecl, void *_globalVarDecl, void *_funcDecl,
              void *_procDecl, void *_statement) {
    this->globalConstDecl = (GlobalConstDecl *)_globalConstDecl;
    this->globalVarDecl = (GlobalVarDecl *)_globalVarDecl;
    this->funcDecl = (ParserNode *)_funcDecl;
    this->procDecl = (ParserNode *)_procDecl;
    this->statement = (ParserNode *)_statement;
  }
  string generate_llvm();
};