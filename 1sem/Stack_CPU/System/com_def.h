#include "arg_num.h"

typedef struct
{
    char* str;
    int arg_num;
} com_defs;

com_defs command_list [] = {{"HALT", ARG_NUM_HALT}, {"PUSH", ARG_NUM_PUSH}, {"PUSH", ARG_NUM_PUSH}, {"POP", ARG_NUM_POP},
                            {"MOV", ARG_NUM_MOV}, {"MOV", ARG_NUM_MOV}, {"ADD", ARG_NUM_ADD}, {"SUB", ARG_NUM_SUB},
                            {"MUL", ARG_NUM_MUL}, {"DIV", ARG_NUM_DIV}, {"SIN", ARG_NUM_SIN}, {"COS", ARG_NUM_COS},
                            {"TAN", ARG_NUM_TAN}, {"SQRT", ARG_NUM_SQRT}, {"EXP", ARG_NUM_EXP}, {"POW", ARG_NUM_POW},
                            {"DUMP", ARG_NUM_DUMP}, {"NOP", ARG_NUM_NOP}, {"JE", ARG_NUM_JE}, {"JNE", ARG_NUM_JNE},
                            {"JA", ARG_NUM_JA}, {"JB", ARG_NUM_JB}, {"JAE", ARG_NUM_JAE}, {"JBE", ARG_NUM_JAE},
                            {"JMP", ARG_NUM_JMP}, {"CALL", ARG_NUM_CALL}, {"RET", ARG_NUM_RET}, {"OUT", ARG_NUM_OUT},
                            {"IN", ARG_NUM_IN}};


