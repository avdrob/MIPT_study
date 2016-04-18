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

    INVALID_INPUT;
}
