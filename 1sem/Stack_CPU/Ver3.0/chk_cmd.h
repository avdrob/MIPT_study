{
#define CHK_CMD(name)   {                                                                                                  \
                            if (!(strcmp (buf_str1, #name)))                                                               \
                            {                                                                                              \
                                mem [(*pointer)++] = CMD_##name;                                                           \
                                                                                                                           \
                                if (feof (*in_file))                                                                       \
                                    break;                                                                                 \
                                continue;                                                                                  \
                            }                                                                                              \
                        }

    CHK_CMD (ERROR);
    CHK_CMD (HALT);
    CHK_CMD (ADD);
    CHK_CMD (SUB);
    CHK_CMD (MUL);
    CHK_CMD (DIV);
    CHK_CMD (SIN);
    CHK_CMD (COS);
    CHK_CMD (TAN);
    CHK_CMD (SQRT);
    CHK_CMD (EXP);
    CHK_CMD (POW);
    CHK_CMD (DUMP);
    CHK_CMD (NOP);

    INVALID_INPUT;
}
