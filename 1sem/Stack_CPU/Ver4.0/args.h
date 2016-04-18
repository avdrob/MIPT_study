struct args
{
    char* str;
    int num;
};
typedef struct args args;

args arg_list [] = {{"ERROR", ARG_NUM_ERROR}, {"HALT", ARG_NUM_HALT}, {"ADD", ARG_NUM_ADD}, {"SUB", ARG_NUM_SUB},
                    {"MUL", ARG_NUM_MUL}, {"DIV", ARG_NUM_DIV}, {"SIN", ARG_NUM_SIN}, {"COS", ARG_NUM_COS},
                    {"TAN", ARG_NUM_TAN}, {"SQRT", ARG_NUM_SQRT}, {"EXP", ARG_NUM_EXP}, {"POW", ARG_NUM_POW},
                    {"DUMP", ARG_NUM_DUMP}, {"NOP", ARG_NUM_NOP}, {"OUT", ARG_NUM_OUT}, {"JMP", ARG_NUM_JMP},
                    {"JE", ARG_NUM_JE}, {"JNE", ARG_NUM_JNE}, {"JA", ARG_NUM_JA}, {"JB", ARG_NUM_JB},
                    {"JAE", ARG_NUM_JAE}, {"JBE", ARG_NUM_JBE}, {"NOP", ARG_NUM_NOP}, {"CALL", ARG_NUM_CALL},
                    {"RET", ARG_NUM_RET}, {"PUSH", ARG_NUM_PUSH}, {"POP", ARG_NUM_POP}, {"MOV", ARG_NUM_MOV},
                    {"IN", ARG_NUM_IN}};

