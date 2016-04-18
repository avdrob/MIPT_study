//{=========================================================================================================================
//! @def        CHK_3ARG_CMD(name) - compares a string with processor instruction consists of three key words. If there's
//!                                  such a command, puts its code and its arguments codes into memory.
//!
//! @param      name     -      a string should be checked.
//! @param      buf_val1 -      first argument code.
//! @param      buf_val2 -      second argument code.
//}=========================================================================================================================
{
#define CHK_3ARG_CMD(name, val1, val2)      {                                                                              \
                                                if (!(strcmp (buf_str1, #name)))                                           \
                                                {                                                                          \
                                                    mem [(*pointer)++] = CMD_##name;                                       \
                                                    mem [(*pointer)++] = val1;                                             \
                                                    mem [(*pointer)++] = val2;                                             \
                                                                                                                           \
                                                    if (feof (*in_file))                                                   \
                                                        break;                                                             \
                                                    continue;                                                              \
                                                }                                                                          \
                                            }

    CHK_3ARG_CMD (MOV_NUM, buf_val1, buf_val2);
    CHK_3ARG_CMD (MOV_REG, buf_val1, buf_val2);

    INVALID_INPUT;
}
