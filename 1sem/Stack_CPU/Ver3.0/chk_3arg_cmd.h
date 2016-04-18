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
