void set_error();
void *create_id(char *ID);
void *create_num(char *NUM);
void create_program(void *globalBlock);
void *create_global_block(void *globalConstDecl, void *globalVarDecl,
                          void *funcDecl, void *procDecl, void *statement);
void *create_simple_block(void *localConstDecl, void *localVarDecl,
                          void *statement);
void *resolve_global_const_assignment(void *globalConstAssList);
void *add_global_const_assignment(void *globalConstAssign);
void *append_global_const_assignment(void *globalConstAssList,
                                     void *globalConstAssign);
void *create_global_const_assignment(void *ID, void *NUM);
void *create_global_const_array_assignment(void *ID, void *numList);
void *resolve_local_const_assignment(void *constAssList);
void *add_local_const_assignment(void *constAssign);
void *append_local_const_assignment(void *constAssList, void *constAssign);
void *create_local_const_assignment(void *ID, void *NUM);
void *create_local_const_array_assignment(void *ID, void *numList);
void *add_num_list(void *NUM);
void *append_num_list(void *NUM, void *numList);
void *resolve_global_var_assignment(void *globalVarDeclList);
void *add_global_var_assignment(void *globalVariableDec);
void *append_global_var_assignment(void *globalVarDeclList,
                                   void *globalVariableDec);
void *create_global_var_assignment(void *ID);
void *create_global_var_array_assignment(void *ID, void *NUM);
void *resolve_local_var_assignment(void *varDeclList);
void *add_local_var_assignment(void *variableDec);
void *append_local_var_assignment(void *varDeclList, void *variableDec);
void *create_local_var_assignment(void *ID);
void *create_local_var_array_assignment(void *ID, void *NUM);
void *append_proc_decl(void *procDecl, void *ID, void *block);
void *append_func_decl(void *funcDecl, void *ID, void *paramList, void *block);
void *create_param_list(void *ID);
void *append_param_list(void *paramList, void *ID);
void *create_store_statement(void *ID, void *expression);
void *create_store_array_statement(void *ID, void *index_expression,
                                   void *expression);
void *create_call_statement(void *ID);
void *resolve_statement_list(void *statementList);
void *create_if_statement(void *condition, void *then_statement,
                          void *else_statement);
void *create_while_statement(void *condition, void *statement);
void *create_for_statement(void *ID, void *start_expression,
                           void *end_expression, void *statement);
void *create_read_statement(void *ID);
void *create_write_statement(void *ID);
void *create_writeln_statement(void *ID);
void *create_break_statement();
void *create_return_statement(void *expression);
void *create_statement_list(void *statement);
void *append_statement_list(void *statementList, void *statement);
void *create_odd_condition(void *expression);
void *create_eq_condition(void *expression_1, void *expression_2);
void *create_ne_condition(void *expression_1, void *expression_2);
void *create_lt_condition(void *expression_1, void *expression_2);
void *create_le_condition(void *expression_1, void *expression_2);
void *create_gt_condition(void *expression_1, void *expression_2);
void *create_ge_condition(void *expression_1, void *expression_2);
void *create_add_expression(void *expression, void *term);
void *create_sub_expression(void *expression, void *term);
void *create_negate_expression(void *term);
void *create_mul_expression(void *term, void *factor);
void *create_div_expression(void *term, void *factor);
void *create_mod_expression(void *term, void *factor);
void *create_id_expression(void *ID);
void *create_array_expression(void *ID, void *expression);
void *create_num_expression(void *NUM);
void *create_call_expression(void *ID, void *argList);
void *create_arg_list(void *expression);
void *append_arg_list(void *argList, void *expression);
