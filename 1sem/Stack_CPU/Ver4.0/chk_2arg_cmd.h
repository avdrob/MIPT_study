//{=========================================================================================================================
//! @def        CHK_2ARG_CMD(name) - compares a string with processor instruction consists of two key words. If there's
//!                                  such a command, puts its code and its argument code into memory.
//!
//! @param      name     -      a string should be checked.
//! @param      buf_val1 -      argument code.
//}=========================================================================================================================
{
#define CHK_2ARG_CMD(name,val)     {                                                                                       \
                                        if (!(strcmp (buf_str1, #name)))                                                   \
                                        {                                                                                  \
                                            mem [(*pointer)++] = CMD_##name;                                               \
                                            mem [(*pointer)++] = val;                                                      \
                                                                                                                           \
                                            if (feof (*in_file))                                                           \
                                                break;                                                                     \
                                            continue;                                                                      \
                                        }                                                                                  \
                                    }

    CHK_2ARG_CMD (PUSH_NUM, buf_val1);
    CHK_2ARG_CMD (PUSH_REG, buf_val1);
    CHK_2ARG_CMD (POP_REG, buf_val1);
    CHK_2ARG_CMD (JMP, buf_val1);
    CHK_2ARG_CMD (JE, buf_val1);
    CHK_2ARG_CMD (JNE, buf_val1);
    CHK_2ARG_CMD (JA, buf_val1);
    CHK_2ARG_CMD (JB, buf_val1);
    CHK_2ARG_CMD (JAE, buf_val1);
    CHK_2ARG_CMD (JBE, buf_val1);
    CHK_2ARG_CMD (IN_REG, buf_val1);
    CHK_2ARG_CMD (CALL, buf_val1);

    INVALID_INPUT;
}
