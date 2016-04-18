//{=======================================================================
//! @file DISASM.c
//! @date 13:11:2013 18:36
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This program translates stack CPU ver4 commands into asm program.
//}=======================================================================

#include "../Stack/stckcount.h"
#include "../System/com_def.h"
#include "../System/constants.h"
#include "../System/commands.h"
#include "../System/registers.h"
#include <stdio.h>

//{==========================================================================
//! Function def_reg - define register.
//!
//! @param      reg_code    Regisrter code.
//!
//! @return     Register string equivalent.
//}==========================================================================
char* def_reg_str (elem_t reg_code);

//{==========================================================================
//! Function is_using_reg - check out if che command uses registers.
//!
//! @param      cmd     Command code.
//!
//! @return     Number of registers the command uses.
//}==========================================================================
int is_using_reg (int cmd);

//{==========================================================================
//! Function write_down - write disassembled program text into outpu file.
//!
//! @param      reg_code    Regisrter code.
//!
//! @return     Register string equivalent.
//}==========================================================================
int write_down (FILE** out_file, elem_t* mem, int* labels, int num_com, int num_label);

//{==========================================================================
//! Function is_jumper - check out if che command is jumper.
//!
//! @param      cmd     Command code.
//!
//! @return     1 if it is;
//!             0 if not.
//}==========================================================================
int is_jumper (int cmd);

//{==========================================================================
//! Function real_to_int    This magic function receives a real number
//!                         and gives it back just like it's integer.
//!
//! @param      val         A real number.
//!
//! @return     Integer value.
//}===========================================================================
int real_to_int (double val);

//{==========================================================================
//! Function set_labels - find out labels and adresses they send to.
//!
//! @param      *labels     Array of adresses labels send to.
//! @param      *mem        Memory array.
//! @param      num_com     Number of used memory blocks.
//!
//! @return     Number of labels.
//}==========================================================================
int set_labels (int* labels, elem_t* mem, int num_com);

//{==========================================================================
//! Function load_program - fills in an array with CPU instructions read
//!                         from executable file.
//!
//! @param      *mem            Memory array.
//! @param      **exec_file     Pointer to the pointer to the executable
//!                             file structure.
//!
//! @return     Nothing to be returned.
//}==========================================================================
int load_program (elem_t* mem, FILE** exec_file);

//{==========================================================================
//! Function find labels - search for labels pointing at the current
//!                        memory adress.
//!
//! @param      *labels     Array of adresses labels point to.
//! @param      cnt         Current memory adress.
//! @param      *buf_val    Number of label will be written here.
//! @param      num_label   Number of labels.
//!
//! @return     1 if such a label was found;
//!             0 if it wasn't.
//}==========================================================================
int find_label (int* labels, int cnt, int* buf_val, int num_label);

int main ()
{
    FILE* exec_file = fopen (EXECUTION_FILE, "r");
    FILE* out_file = fopen (OUTPUT_FILE, "w");
    elem_t mem [MEM_SIZE] = {};
    int labels [LABEL_NUM] = {};
    int num_com = 0, num_label = 0;

    num_com = load_program (mem, &exec_file);
    num_label = set_labels (labels, mem, num_com);
    write_down (&out_file, mem, labels, num_com, num_label);

    return 0;
}


int load_program (elem_t* mem, FILE** exec_file)
{
    unsigned cnt = 0;
    elem_t buf_val = 0;

    for (cnt = 0; cnt <= MEM_SIZE - 1; cnt++)
        mem [cnt] = -1;
    cnt = 0;

    while (1)
    {
        if (feof (*exec_file))
            break;

        else
        {
            fscanf (*exec_file, "%lg", &buf_val);
            mem [cnt++] = buf_val;
        }
    }

    mem [cnt- 1] = -1;
    fclose (*exec_file);

    return cnt - 2;
}

int set_labels (int* labels, elem_t* mem, int num_com)
{
    int cnt = 0, num_label = 0;

    for (cnt = 0; cnt <= LABEL_NUM; cnt++)
        labels [cnt] = -1;

    for (cnt = 0; cnt <= num_com; cnt++)
        if (is_jumper (mem [cnt]))
            labels [++num_label] = mem [cnt + 1];

    return num_label;
}

int real_to_int (double val)
{
    return val;
}

int is_jumper (int cmd)
{
    return ((cmd == CMD_JMP) || (cmd == CMD_JE) || (cmd == CMD_JNE) || (cmd == CMD_CALL) ||
            (cmd == CMD_JA) || (cmd == CMD_JB) || (cmd == CMD_JAE) || (cmd == CMD_JBE));
}


int write_down (FILE** out_file, elem_t* mem, int* labels, int num_com, int num_label)
{
    int cnt = 0, used_regs = 0, arg_cnt = 0, pointer = 0, buf_val = 0, label_cnt = 1;

    for (cnt = 0; cnt <= num_com; cnt++)
    {
        if (find_label (labels, cnt, &buf_val, num_label))
            fprintf (*out_file, ":%d\n", buf_val);

        pointer = mem [cnt];
        fprintf (*out_file, "%s ", (command_list [pointer]).str);
        used_regs = is_using_reg (pointer);

        if (is_jumper (pointer))
        {
            fprintf (*out_file, ":%d\n", label_cnt++);
            cnt++;
            continue;
        }

        if (used_regs)
            for (arg_cnt = 1; arg_cnt <= used_regs; arg_cnt++)
                fprintf (*out_file, "%s ", def_reg_str (mem [++cnt]));

        for (arg_cnt = 1; arg_cnt <= (command_list [pointer]).arg_num - used_regs - 1; arg_cnt++)
            fprintf (*out_file, "%g ", mem [++cnt]);

        fprintf (*out_file, "\n");
    }
}

int  is_using_reg (int cmd)
{
    switch (cmd)
    {
        case CMD_PUSH_REG:
        case CMD_POP_REG:
        case CMD_MOV_NUM:
        case CMD_IN_REG:
            return 1;
            break;

        case CMD_MOV_REG:
            return 2;
            break;

        default:
            return 0;
            break;
    }
}

char* def_reg_str (elem_t reg_code)
{
    char* reg = (char*) calloc (2, sizeof (char));

    switch (real_to_int(reg_code))
    {
        case REG_AX:
            reg = "AX";
            break;

        case REG_BX:
            reg = "BX";
            break;

        case REG_CX:
            reg = "CX";
            break;

        case REG_DX:
            reg = "DX";
            break;
    }

    return reg;
}

int find_label (int* labels, int cnt, int* buf_val, int num_label)
{
    int seeker = 0;

    for (seeker = 1; seeker <= num_label; seeker++)
        if (labels [seeker] == cnt)
        {
            *buf_val = seeker;
            return 1;
        }

    return 0;
}
