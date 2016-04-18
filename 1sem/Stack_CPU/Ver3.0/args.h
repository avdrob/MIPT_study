struct args
{
    char* str;
    int num;
};
typedef struct args args;

args arg_list [] = {{"ERROR", ARG_NUM_ERROR}, {"HALT", ARG_NUM_HALT}, {"ADD", ARG_NUM_ADD}, {"SUB", ARG_NUM_SUB},
                    {"MUL", ARG_NUM_MUL}, {"DIV", ARG_NUM_DIV}, {"SIN", ARG_NUM_SIN}, {"COS", ARG_NUM_COS},
                    {"TAN", ARG_NUM_TAN}, {"SQRT", ARG_NUM_SQRT}, {"EXP", ARG_NUM_EXP}, {"POW", ARG_NUM_POW},
                    {"DUMP", ARG_NUM_DUMP}, {"NOP", ARG_NUM_NOP}, {"PUSH", ARG_NUM_PUSH}, {"POP", ARG_NUM_POP},
                    {"MOV", ARG_NUM_MUL}};
