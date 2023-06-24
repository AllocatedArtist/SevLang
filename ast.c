#include "sev.h"

AST* new_node(AST ast) {
  AST* ptr = malloc(sizeof(AST)); 
  if (ptr == NULL) return NULL;
  *ptr = ast;
  return ptr;
}


void free_ast_node(AST* ast) {
  if (ast == NULL) return;
  switch (ast->type) {
    case AST_PROGRAM:
      free_ast_node(GET_AST_DATA(ast, AST_PROGRAM).body);
      break;
    case AST_VARIABLE:
      //TODO:
      break;
    case AST_VARIABLE_DECLARATION:
      //Ignore
      break;
    case AST_VARIABLE_ASSIGNMENT:
      break;
    case AST_NUMBER_LITERAL:
      break;
    case AST_CALL_EXPRESSION:
      //TODO:
      break;
    case AST_BINARY_OPERATION:
      break; 
    case AST_FUNCTION_DECLARATION:
      for (int i = 0; i < ast->data.AST_FUNCTION_DECLARATION.num_parameters; ++i) 
        free_ast_node(GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]);
      if (GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable != NULL) free_ast_node(GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable);
      if (GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).variable_assignment != NULL) free_ast_node(GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).variable_assignment);
      break;
    default:
      break;
  }  

  free(ast);
}

void push_ast_list(ast_list_t** list, AST* node) {
  ast_list_t* new_val = (ast_list_t*)malloc(sizeof(ast_list_t));
  if (new_val == NULL) {
    printf("Unable to allocate new AST value.");
    return;
  }
  new_val->current = node;
  new_val->next = NULL;
  
  if (*list == NULL) {
    *list = new_val;
    return;
  }
  ast_list_t* last = *list;
  while (last->next != NULL) last = last->next; 
  last->next = new_val;  
}

void free_ast_list(ast_list_t** list) {
  ast_list_t* current = *list;
  ast_list_t* temp = NULL;
  while (current != NULL) {
    temp = current->next; 
    free_ast_node(current->current); 
    free(current);
    current = temp;
  }

  *list = NULL;
}


void print_ast(AST* ast) {
  if (ast == NULL) return;
  switch (ast->type) {
    case AST_PROGRAM:
      print_ast(GET_AST_DATA(ast, AST_PROGRAM).body);
      break;
    case AST_VARIABLE:
      printf("%s", ast->data.AST_VARIABLE.variable_name);
      break;
    case AST_VARIABLE_DECLARATION:
      printf("%s %s", GET_AST_DATA(ast, AST_VARIABLE_DECLARATION).type, GET_AST_DATA(ast, AST_VARIABLE_DECLARATION).variable_name);
      break;
    case AST_ASSIGNMENT_R_VAL:
      printf("%s", GET_AST_DATA(ast, AST_ASSIGNMENT_R_VAL).assignment);
      break;
    case AST_VARIABLE_ASSIGNMENT:
      if (GET_AST_DATA(ast, AST_VARIABLE_ASSIGNMENT).variable != NULL)
        printf("%s = ", GET_AST_DATA(ast, AST_VARIABLE_ASSIGNMENT).variable);
      else
        printf(" = ");
      break;
    case AST_NUMBER_LITERAL:
      printf("%s", GET_AST_DATA(ast, AST_NUMBER_LITERAL).literal);
      break;
    case AST_CALL_EXPRESSION:
      break;
    case AST_BINARY_OPERATION:
      printf(" %s ", GET_AST_DATA(ast, AST_BINARY_OPERATION).op);
      break; 
    case AST_COMMENT:
      printf("%s", GET_AST_DATA(ast, AST_COMMENT).comment);
      break;
    case AST_FUNCTION_DECLARATION:
      printf("%s %s(", GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).type, GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).name);
      for (int i = 0; i < GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).num_parameters; ++i) {
        if (i + 1 != GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).num_parameters)
          printf("%s %s, ", GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.type, GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.variable_name);
        else if (i + 1 == GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).num_parameters)
          printf("%s %s", GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.type, GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.variable_name);
      }
      printf(")");
      if (GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable != NULL && GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable->type != AST_NUMBER_LITERAL) {
        const char* type = GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).type;
        AST* return_var = GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable;
        printf(" { %s %s;\n", type, GET_AST_DATA(return_var, AST_VARIABLE_DECLARATION).variable_name);
      } else {
        printf(" {\n");
      }
      break;
    case AST_END_LINE:
      if (GET_AST_DATA(ast, AST_END_LINE).value != NULL) {
        if (strncmp(GET_AST_DATA(ast, AST_END_LINE).value, "}", 2) == 0 && GET_AST_DATA(ast, AST_END_LINE).return_variable != NULL) {
          printf("return %s;\n", GET_AST_DATA(ast, AST_END_LINE).return_variable);
        }
        printf("%s\n", GET_AST_DATA(ast, AST_END_LINE).value);
      }
      break;
  }  
}

void print_ast_list(ast_list_t* list) {
  ast_list_t* current = list;
  while (current != NULL) {
    print_ast(current->current);  
    current = current->next;
  }
}

//I got lazy ok?? Making a programming lang is a lot harder than I anticipated lol
ast_list_t* produce_ast(token_list_t* tokens_list) {
  ast_list_t* list = NULL;
  token_list_t* current = tokens_list;

 
  while (current != NULL) {
    
    if (current->current.type == LetType) {
      if (current->next != NULL) {
        if (current->next->current.type == IdentifierType) {
          const char* variable_name = current->next->current.value;
          if (current->next->next != NULL) {
            if (current->next->next->current.type == ColonAssignmentType) {
              if (current->next->next->next != NULL) {
                if (current->next->next->next->current.type == TypeIdentifierType) {
                  const char* type = current->next->next->next->current.value; 
                  push_ast_list(&list, AST_NODE(AST_VARIABLE_DECLARATION, type, variable_name));
                  current = current->next->next->next;
                } else {
                  printf("Expected type identifier after colon!\n");
                  return NULL;
                }
              } else {
                printf("Expected type identifier after colon!\n");
                return NULL;
              }
            } else {
              printf("Expected colon assignment after variable name!\n");
              return NULL;
            }
          } else {
            printf("Expected colon after variable name!\n");
            return NULL;
          }
        } else {
          printf("Expected variable identifier type!\n");
          return NULL;
        }
      } else {
        printf("Expected variable identifier after let statement!\n");
        return NULL;
      }
    }

    if (current->current.type == FnType) {
      if (current->next != NULL) {
        if (current->next->current.type == IdentifierType) {
          const char* function_name = current->next->current.value;
          if (current->next->next != NULL) {
            if (current->next->next->current.type == OpenParenthesisType) {
             current = current->next->next; 
             if (current->next != NULL) {
               int num_names = 0;
               int num_types = 0;
               const char* parameter_names[MAX_PARAMETERS];
               const char* parameter_types[MAX_PARAMETERS];
               while (current->current.type != ClosedParenthesisType) {
                if (current->current.type == IdentifierType) {
                  parameter_names[num_names] = current->current.value;
                  num_names += 1;
                } else if (current->current.type == TypeIdentifierType) {
                  parameter_types[num_types] = current->current.value;
                  num_types += 1;
                }
                current = current->next;        
               }

              if (num_names == num_types) {
                if (current->next != NULL) {
                  if (current->next->current.type == AssignmentOperatorType) {
                    if (current->next->next != NULL) {
                      if (current->next->next->current.type == TypeIdentifierType) {
                        const char* function_type = current->next->next->current.value;
                        current = current->next->next;
                        if (current->next != NULL) {
                          if (current->next->current.type == ColonAssignmentType) {
                            if (current->next->next != NULL) {
                              if (current->next->next->current.type == OpenBracketType) {
                                //TODO: Handle actual type here
                                AST* result_type = NULL;
                                AST* assignment = NULL;
                                current = current->next->next;
                                while (current->current.type != ClosedBracketType) {
                                  if (current->current.type == IdentifierType) {
                                    result_type = AST_NODE(AST_VARIABLE_DECLARATION, function_type, current->current.value);
                                    current_function_return.return_variable = current->current.value;
                                    current_function_return.is_set = true; 
                                  } else if (current->current.type == NumberType) {
                                    result_type = AST_NODE(AST_NUMBER_LITERAL, current->current.value);
                                    current_function_return.return_variable = current->current.value;
                                    current_function_return.is_set = true; 
                                  }
                                  current = current->next;
                                  if (current == NULL) {
                                    printf("Expected more after bracket!\n"); 
                                    return NULL;
                                  }
                                }
                                if (current != NULL) {
                                  if (current->current.type == ClosedBracketType) {
                                    AST* ast = AST_NODE(
                                      AST_FUNCTION_DECLARATION, 
                                      num_names,
                                      function_type,
                                      function_name, 
                                      NULL,
                                    );
                                    ast->data.AST_FUNCTION_DECLARATION.return_variable = result_type;
                                    
                                    for (int i = 0; i < num_names; ++i) {
                                      ast->data.AST_FUNCTION_DECLARATION.parameters[i] = AST_NODE(AST_VARIABLE_DECLARATION, parameter_types[i], parameter_names[i]);
                                    }
                                    push_ast_list(&list, ast);                                             
                                  } else {
                                    printf("Expected '>'!\n");
                                    return NULL;
                                  }
                                } else {
                                  printf("Expected '>'!\n");
                                  return NULL;
                                }
                              } else {
                                printf("Expected '<'!\n");
                                return NULL;
                              }
                            } else {
                              printf("Expected more after colon assignment!\n");
                              return NULL;
                            }
                          } else {
                            printf("Expected colon after type identifier function declaration!\n");
                            return NULL;
                          }
                        } else {
                          printf("Expected colon after type identifier function declaration\n");
                          return NULL;
                        }
                      } else {
                        printf("Expected type identifier for function declaration!\n");
                        return NULL;
                      }
                    } else {
                      printf("Expected type identifier after assignment operator!\n");
                      return NULL;
                    }
                  } else {
                    printf("Expected assignment operator after parenthesis!\n");
                    return NULL;
                  }
                } else {
                  printf("Expected assignment operator after parenthesis!\n");
                  return NULL;
                }
              } else {
                printf("Uneven number of types and identifiers in parameters!\n");
                return NULL;
              }
             } else {
               printf("Expected more after parenthesis!\n");
               return NULL;
             }
            } else {
              printf("Expected parenthesis after function name!\n");
              return NULL;
            }
          } else {
            printf("Expected parenthesis after function name!\n");
            return NULL;
          }
        } else {
          printf("Expected function name after fn statement!\n");
          return NULL;
        }
      } else {
        printf("Expected function name after fn statement!\n");
        return NULL;
      }
    }

    if (current->current.type == IdentifierType) {
      const char* var_name = current->current.value;
      if (current->next != NULL) {
        if (current->next->current.type == AssignmentOperatorType) {
          current = current->next;
          push_ast_list(&list, AST_NODE(AST_VARIABLE_ASSIGNMENT, var_name));
          if (current->next == NULL) {
            printf("Expected something after assignment operator!\n");
            return NULL;
          } 
          current = current->next;
          while (current->current.type != SemiColonType) {
            if (current->current.type == OpenParenthesisType) 
              push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, "("));
            else if (current->current.type == ClosedParenthesisType)
              push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, ")"));
            else if (current->current.type == BinaryOperationType)
              push_ast_list(&list, AST_NODE(AST_BINARY_OPERATION, current->current.value));
            else 
              push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, current->current.value));
            current = current->next; 
            if (current == NULL) {
              printf("Expected more after assignment!\n");
              return NULL;
            }
          }    
        }       
      } else {
        printf("Expected assignment after identifier.\n");
      }
    } else if (current->current.type == CallType) {
      current = current->next;
      while (current->current.type != SemiColonType) {
            if (current->current.type == OpenParenthesisType) 
              push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, "("));
            else if (current->current.type == ClosedParenthesisType)
              push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, ")"));
            else if (current->current.type == BinaryOperationType)
              push_ast_list(&list, AST_NODE(AST_BINARY_OPERATION, current->current.value));
            else 
              push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, current->current.value));
            current = current->next; 
            if (current == NULL) {
              printf("Expected more after assignment!\n");
              return NULL;
            }
          }
    }

    if (current->current.type == EndType) {
      if (current_function_return.is_set == true) {
        current_function_return.is_set = false;
        push_ast_list(&list, AST_NODE(AST_END_LINE, "}", current_function_return.return_variable));
        current_function_return.return_variable = NULL;
      } else {
        push_ast_list(&list, AST_NODE(AST_END_LINE, "}"));
      }
    }
 
    if (current->current.type == AssignmentOperatorType) {
      push_ast_list(&list, AST_NODE(AST_VARIABLE_ASSIGNMENT));
      if (current->next == NULL) {
        printf("Expected something after assignment operator!\n");
        return NULL;
      } 
      current = current->next;
      while (current->current.type != SemiColonType) {
        if (current->current.type == OpenParenthesisType) 
          push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, "("));
        else if (current->current.type == ClosedParenthesisType)
          push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, ")"));
        else if (current->current.type == BinaryOperationType)
          push_ast_list(&list, AST_NODE(AST_BINARY_OPERATION, current->current.value));
        else 
          push_ast_list(&list, AST_NODE(AST_ASSIGNMENT_R_VAL, current->current.value));
        current = current->next; 
        if (current == NULL) {
          printf("Expected more after assignment!\n");
          return NULL;
        }
      }
    }

    if (current->current.type == CommentType) {
      push_ast_list(&list, AST_NODE(AST_COMMENT, "/*"));
      current = current->next;
      while (current != NULL && current->current.type != CommentType) {
        push_ast_list(&list, AST_NODE(AST_COMMENT, current->current.value));
        current = current->next;
      }
      push_ast_list(&list, AST_NODE(AST_COMMENT, "*/"));
      push_ast_list(&list, AST_NODE(AST_END_LINE, ""));
    }

    if (current->current.type == SemiColonType) {
      push_ast_list(&list, AST_NODE(AST_END_LINE, ";"));
    }

    current = current->next;
  }
  
  return list;
}

void write_ast(AST* ast, FILE* stream) {
  if (ast == NULL) return;
  switch (ast->type) {
    case AST_PROGRAM:
      break;
    case AST_VARIABLE:
      fprintf(stream, "%s", ast->data.AST_VARIABLE.variable_name);
      break;
    case AST_VARIABLE_DECLARATION:
      fprintf(stream, "%s %s", GET_AST_DATA(ast, AST_VARIABLE_DECLARATION).type, GET_AST_DATA(ast, AST_VARIABLE_DECLARATION).variable_name);
      break;
    case AST_ASSIGNMENT_R_VAL:
      fprintf(stream, "%s", GET_AST_DATA(ast, AST_ASSIGNMENT_R_VAL).assignment);
      break;
    case AST_VARIABLE_ASSIGNMENT:
      if (GET_AST_DATA(ast, AST_VARIABLE_ASSIGNMENT).variable != NULL)
        fprintf(stream, "%s = ", GET_AST_DATA(ast, AST_VARIABLE_ASSIGNMENT).variable);
      else
        fprintf(stream, " = ");
      break;
    case AST_NUMBER_LITERAL:
      fprintf(stream, "%s", GET_AST_DATA(ast, AST_NUMBER_LITERAL).literal);
      break;
    case AST_CALL_EXPRESSION:
      break;
    case AST_BINARY_OPERATION:
      fprintf(stream," %s ", GET_AST_DATA(ast, AST_BINARY_OPERATION).op);
      break; 
    case AST_FUNCTION_DECLARATION:
      fprintf(stream,"%s %s(", GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).type, GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).name);
      for (int i = 0; i < GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).num_parameters; ++i) {
        if (i + 1 != GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).num_parameters)
          fprintf(stream,"%s %s, ", GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.type, GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.variable_name);
        else if (i + 1 == GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).num_parameters)
          fprintf(stream,"%s %s", GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.type, GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).parameters[i]->data.AST_VARIABLE_DECLARATION.variable_name);
      }
      fprintf(stream,")");
      if (GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable != NULL && GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable->type != AST_NUMBER_LITERAL) {
        const char* type = GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).type;
        AST* return_var = GET_AST_DATA(ast, AST_FUNCTION_DECLARATION).return_variable;
        fprintf(stream," { %s %s;\n", type, GET_AST_DATA(return_var, AST_VARIABLE_DECLARATION).variable_name);
      } else {
        fprintf(stream," {\n");
      }
      break;
    case AST_COMMENT:
      fprintf(stream, "%s", GET_AST_DATA(ast, AST_COMMENT).comment);
      break;
    case AST_END_LINE:
      if (GET_AST_DATA(ast, AST_END_LINE).value != NULL) {
        if (strncmp(GET_AST_DATA(ast, AST_END_LINE).value, "}", 2) == 0 && GET_AST_DATA(ast, AST_END_LINE).return_variable != NULL) {
          fprintf(stream,"return %s;\n", GET_AST_DATA(ast, AST_END_LINE).return_variable);
        }
        fprintf(stream,"%s\n", GET_AST_DATA(ast, AST_END_LINE).value);
      }
      break;
  } 
}

void write_ast_to_file(FILE* stream, ast_list_t* list) {
 ast_list_t* current = list;
  while (current != NULL) {
    write_ast(current->current, stream);  
    current = current->next;
  } 
}
