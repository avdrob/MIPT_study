#include "bnr_trnsltr.h"
#include "../Stack_CPU/System/commands.h"
#include "../Stack_CPU/System/registers.h"

#define DEF_COM(com)    {_##com, sizeof (_##com) - 1, CMD_##com}

/*const Instr_Set x86_interps [] = {{_HALT, 11}, {_PUSH_NUM, 15}, {_PUSH_REG, 11}, {_POP_REG, 11}, {_MOV_NUM, 7}, {_MOV_REG, 3},
    {_ADD, 27}, {_SUB, 31}, {_MUL,3}, {_DIV, 3}, {_SIN, 3}, {_COS, 3}, {_TAN, 3}, {_SQRT, 3}, {_EXP, 3}, {_POW, 3}, {_DUMP, 3},
    {_NOP, 3}, {_JE, 3}, {_JNE, 3}, {_JA, 3}, {_JB, 3}, {_JAE, 3}, {_JBE, 3}, {_JMP, 3}, {_CALL, 7}, {_RET, 3}, {_OUT, 3},
    {_IN_REG, 3}, {_ENTR_FRAME, 11}, {_LEAVE_FRAME, 3}};*/

const Instr_Set x86_interps [] = {DEF_COM (HALT), DEF_COM (PUSH_NUM), DEF_COM (PUSH_REG), DEF_COM (POP_REG), DEF_COM (MOV_NUM),
    DEF_COM (MOV_REG), DEF_COM (ADD), DEF_COM (SUB), DEF_COM (MUL), DEF_COM (DIV), DEF_COM (SIN), DEF_COM (COS), DEF_COM (TAN),
    DEF_COM (SQRT), DEF_COM (EXP), DEF_COM (POW), DEF_COM (DUMP), DEF_COM (NOP), DEF_COM (JE), DEF_COM (JNE), DEF_COM (JA),
    DEF_COM (JB), DEF_COM (JAE), DEF_COM (JBE), DEF_COM (JMP), DEF_COM (CALL), DEF_COM (RET), DEF_COM (OUT), DEF_COM (IN_REG),
    {_ENTR_FRAME, 11, ENTR_FRAME_INDX}, {_LEAVE_FRAME, 3, LEAVE_FRAME_INDX}};

#define COM_ORDER_CHECK     {                                                           \
                                uint32_t blocks_num = LEAVE_FRAME_INDX;                 \
                                for (uint32_t cnt = 0; cnt <= blocks_num; cnt++)        \
                                {                                                       \
                                    assert (cnt == (x86_interps [cnt]).arr_indx);       \
                                }                                                       \
                            }                                                           \

Code_Block :: Code_Block (Code_Block *prev, unsigned char *old_prog)
{
    assert (prev);

    prev_block_ = prev;
    next_block_ = NULL;
    Instr_Set cur_instr;

    block_num_ = prev->block_num_ + 1;
    prev->next_block_ = this;
    code_offset_ = prev->code_offset_ + prev->code_size_ + 1;
    src_instr_num_begin_ = prev->src_instr_num_end_ + 1;
    cur_instr = x86_interps [old_prog [src_instr_num_begin_]];
    is_jump_ = false;

    code_array_ = (uint32_t*) calloc (cur_instr.code_size_, sizeof (char));
    code_size_ = cur_instr.code_size_;

    src_instr_num_end_ = src_instr_num_begin_;

    if (old_prog [src_instr_num_begin_] == CMD_PUSH_NUM)
    {
        code_array_ [0] = cur_instr.code_array_  [0];
        code_array_ [2] = cur_instr.code_array_  [2];
        code_array_ [3] = cur_instr.code_array_  [3];

        code_array_ [1] = old_prog [src_instr_num_begin_ + 1];

        src_instr_num_end_++;

        return;
    }

    for (uint32_t cnt = 0; cnt <= code_size_; cnt++)
    {
        ((unsigned char*) code_array_) [cnt] = ((unsigned char*) cur_instr.code_array_) [cnt];
    }

    if (old_prog [src_instr_num_begin_] == CMD_PUSH_REG)
    {
        ((unsigned char*) code_array_) [3] += (old_prog [src_instr_num_begin_ + 1]) * 8;
        src_instr_num_end_++;

        return;
    }

    if (old_prog [src_instr_num_begin_] == CMD_POP_REG)
    {
        ((unsigned char*) code_array_) [6] += (old_prog [src_instr_num_begin_ + 1]) * 8;
         src_instr_num_end_++;

        return;
    }

    if (old_prog [src_instr_num_begin_] == CMD_MOV_NUM)
    {
        ((unsigned char*) code_array_) [0] += old_prog [src_instr_num_begin_ + 1];

        uint32_t buf = old_prog [src_instr_num_begin_ + 2];

        ((unsigned char*) code_array_) [1] = buf & 0xff;
        ((unsigned char*) code_array_) [2] = (buf >> 8) & 0xff;
        ((unsigned char*) code_array_) [3] = (buf >> 16) & 0xff;
        ((unsigned char*) code_array_) [4] = (buf >> 24) & 0xff;

        src_instr_num_end_ += 2;
        return;
    }

    if (old_prog [src_instr_num_begin_] == CMD_MOV_REG)
    {
        ((unsigned char*) code_array_) [1] += old_prog [src_instr_num_begin_ + 1] + old_prog [src_instr_num_begin_ + 2] * 8;

        src_instr_num_end_ += 2;
        return;
    }

    unsigned char buf1 = ((unsigned char*) code_array_) [0];
    unsigned char buf2 = ((unsigned char*) code_array_) [3];
    if ((0x72 <= buf1 && buf1 <= 0x77) || (buf2 == 0xe8) || (buf1 == 0xeb))
    {
        is_jump_ = true;
        ((unsigned char*) code_array_) [1] = ((unsigned char) old_prog [src_instr_num_begin_ + 1]);
        src_instr_num_end_ +=1;

        return;
    }
}

Code_Block :: Code_Block (Code_Block *prev)
{
    Instr_Set cur_instr;

    if (!prev)
    {
        block_num_ = code_offset_ = 0;
        cur_instr = x86_interps [ENTR_FRAME_INDX];
    }
    else
    {
        block_num_ = prev->block_num_ + 1;
        prev->next_block_ = this;
        code_offset_ = prev->code_offset_ + prev->code_size_ + 1;
        cur_instr = x86_interps [LEAVE_FRAME_INDX];
    }
    prev_block_ = prev;
    next_block_ = NULL;
    src_instr_num_begin_ = 0xffffffff; // + 1 == 0

    code_array_ = (uint32_t*) calloc (cur_instr.code_size_, sizeof (char));
    code_size_ = cur_instr.code_size_;

    for (uint32_t cnt = 0; cnt <= code_size_; cnt++)
    {
        ((unsigned char*) code_array_) [cnt] = ((unsigned char*) cur_instr.code_array_) [cnt];
    }
    src_instr_num_end_ = src_instr_num_begin_ ;
}

bool Code_Block :: ok ()
{
    if (!code_array_)
        return false;

    if (prev_block_ && (block_num_ == 0 || (block_num_ != prev_block_->block_num_ + 1)))
        return false;

    if ((prev_block_ && (prev_block_->next_block_ != this)) ||
        (next_block_ && (next_block_->prev_block_ != this)))
            return false;

    if (prev_block_ && (code_offset_ != prev_block_->code_offset_ + prev_block_->code_size_ + 1))
        return false;

    return true;
}

void Code_Block :: dump ()
{
    printf ("##############################################################\n");
    printf ("Code block 0x%x is ", (uint32_t) this);
    if (ok ())
        printf ("ok\n");
    else
        printf ("BAD!\n");
    printf ("##############################################################\n");
    printf ("Block number = %d\n", block_num_);
    printf ("Previous block: 0x%x\n", (uint32_t) prev_block_);
    printf ("Next block: 0x%x\n", (uint32_t) next_block_);
    printf ("Code offset: 0x%x\n", code_offset_);
    printf ("Code size: 0x%x\n", code_size_);
    printf ("Source instr numbers: from %d to %d\n", src_instr_num_begin_, src_instr_num_end_);
    printf ("##############################################################\n");

    int cnt_longs = 0, cnt_bytes = 0;
    for (; cnt_longs < code_size_; cnt_longs += 4)
    {
        for (cnt_bytes = 0; cnt_bytes <= 3; cnt_bytes++)
        {
            //assert ((cnt_longs + cnt_bytes) > code_size_);
            if (((unsigned char*) code_array_) [cnt_longs + cnt_bytes] >= 0x10)
                printf ("%x", ((unsigned char*) code_array_) [cnt_longs + cnt_bytes]);
            else
                printf ("%x%x", 0, ((unsigned char*) code_array_) [cnt_longs + cnt_bytes]);
        }
        putchar (' ');
    }
    putchar (0xa);
    printf ("##############################################################\n");
    printf ("##############################################################\n\n");

    return;
}

void Code_Block :: write_out (FILE* out_file)
{
    assert (ok ());
    fwrite (code_array_, 1, code_size_ + 1, out_file);
}

Sect_Txt :: Sect_Txt (unsigned char *old_prog, uint32_t src_instr_num)
{
    /*Code_Block entr_frame (0x0);
    head_block_ = &entr_frame;*/
    Code_Block *entr_frame = new Code_Block (0x0);
    head_block_ = entr_frame;
    Code_Block *prev = head_block_;

    blocks_counter_ = 0;
    code_size_ = entr_frame->code_size_;

    while (1)
    {
        /*Code_Block *blocks_space = (Code_Block*) calloc (1, sizeof (Code_Block));
        Code_Block init_block (prev, old_prog);
        *blocks_space = init_block;
        prev->next_block_ = blocks_space;*/

        Code_Block *blocks_space = new Code_Block (prev, old_prog);
        prev->next_block_ = blocks_space;
        //blocks_space->dump ();
        assert (blocks_space->block_num_ <= src_instr_num);
        assert (blocks_space->ok ());
        blocks_counter_++;
        code_size_ = code_size_ + (*blocks_space).code_size_ + 1;
        tail_block_ = prev = blocks_space;

        if (blocks_space->src_instr_num_end_ == src_instr_num)
            break;
    }
    Code_Block *leav_frame = new Code_Block (tail_block_);
    tail_block_ = leav_frame;
    blocks_counter_++;
    code_size_ += leav_frame->code_size_ + 1;

    set_jumps ();
    COM_ORDER_CHECK;
}

void Sect_Txt :: set_jumps ()
{
    Code_Block *cur_block = head_block_->next_block_;
    for (uint32_t cnt = 1; cnt <= blocks_counter_ - 1; cnt++)
    {
        if (cur_block->is_jump_)
        {
            uint32_t dest_num = ((unsigned char*) cur_block->code_array_) [1];
            Code_Block *dest_block = head_block_->next_block_;

            while (1)
            {
                if (dest_block->src_instr_num_begin_ == dest_num)
                    break;

                dest_block = dest_block->next_block_;
            }

            unsigned char *cur_array = (unsigned char*) (cur_block->code_array_);
            if ((cur_array [3]) == 0xe8)
            {
                cur_array [1] = 0x90;
                ((uint32_t*) cur_array) [1] = dest_block->code_offset_ - cur_block->code_offset_ - 8;
            }
            else
                cur_array [1] = (unsigned char) (dest_block->code_offset_ - cur_block->code_offset_ - 2);
        }

        cur_block = cur_block->next_block_;
    }

    return;
}

    /*Code_Block *cur_block = head_block_->next_block_;
    while (1)
    {
        if (cur_block == tail_block_)
            break;

        if (cur_block->is_jump_)
        {
            Code_Block *search_block = head_block_->next_block_, *dest_block = NULL;
            uint32_t dest_num = ((unsigned char*) cur_block->code_array_) [1] + 1;
            printf ("%d\n", dest_num);
            cur_block->dump ();
            for (uint32_t cnt = 1; cnt < blocks_counter_ ; cnt++)
            {
                if (cnt == dest_num)
                {

                    dest_block = search_block;
                    break;
                }

                search_block = search_block->next_block_;
            }
            unsigned char *cur_array = (unsigned char*) (cur_block->code_array_);

            if ((cur_array [3]) == 0xe8)
            {
                cur_array [1] = 0x90;
                ((uint32_t*) cur_array) [1] = dest_block->code_offset_ - cur_block->code_offset_;
            }
            else
            {
                cur_array [1] = (unsigned char) (dest_block->code_offset_ - cur_block->code_offset_);
                printf ("%x\n", cur_array [3]);
            }

        }
    }

    return;*/

void Sect_Txt :: dump ()
{
    printf ("##############################################################\n");
    printf ("Exex code consists of %u code blocks\n", blocks_counter_);
    printf ("Code size = %u bytes\n", code_size_);
    printf ("##############################################################\n");
    printf ("##############################################################\n\n");
    printf ("HEAD BLOCK:\n");
    head_block_->dump ();
    printf ("TAIL BLOCK:\n");
    tail_block_->dump ();
    printf ("##############################################################\n");
    printf ("##############################################################\n\n");
}

void Sect_Txt :: write_out (FILE* out_file)
{
    uint32_t cnt = 0;
    Code_Block *cur_block = head_block_;
    for (; cnt <= blocks_counter_ ; cnt++)
    {
        assert ((0 <= cnt) && (cnt <= blocks_counter_));
        cur_block->write_out (out_file);
        cur_block = cur_block->next_block_;
    }
}

Sect_Data :: Sect_Data ():
    stack_size_ (STACK_ELEM_NUM)
{
    data_stack_ = (uint32_t*) calloc (STACK_ELEM_NUM, sizeof (uint32_t));
}

void Sect_Data :: write_out (FILE* out_file)
{
    fwrite (data_stack_, 1, stack_size_, out_file);
}

Sect_Shstrtab :: Sect_Shstrtab ()
{
    sect_names_ [0] = "\0.shstrtab";
    sect_names_ [1] = "\0.text";
    sect_names_ [2] =  "\0.data\0\0";

    sects_counter_ = 2;
    tab_size_ = 24;
}

void Sect_Shstrtab :: write_out (FILE* out_file)
{
    fwrite (sect_names_ [0], 1, 10, out_file);
    fwrite (sect_names_ [1], 1, 6, out_file);
    fwrite (sect_names_ [2], 1, 8, out_file);
}

Program :: Program (unsigned char *old_prog, uint32_t src_instr_num):
    exec_code_ (old_prog, src_instr_num),
    sects_table_ (),
    data_stack_ ()
{
    memcpy (file_header_.e_ident, (unsigned char*) sign, EI_NIDENT);
    file_header_.e_type = ET_EXEC;
    file_header_.e_machine = EM_386;
    file_header_.e_version = EV_CURRENT;
    file_header_.e_entry = DFLT_ENTR_POINT + sizeof (Elf32_Ehdr) + 2 * sizeof (Elf32_Phdr);
    file_header_.e_phoff = sizeof (Elf32_Ehdr);
    file_header_.e_shoff = sizeof (Elf32_Ehdr) + 2 * sizeof (Elf32_Phdr) +
                            (exec_code_.code_size_  + 1) + data_stack_.stack_size_ + sects_table_.tab_size_;
    file_header_.e_flags = 0x00000000;
    file_header_.e_ehsize = sizeof (Elf32_Ehdr);
    file_header_.e_phentsize = sizeof (Elf32_Phdr);
    file_header_.e_phnum = 2;
    file_header_.e_shentsize = sizeof (Elf32_Shdr);
    file_header_.e_shnum = 4;
    file_header_.e_shstrndx = 3;

    phdr_load_text_.p_type = phdr_load_data_.p_type = PT_LOAD;
    phdr_load_text_.p_offset = 0x00000000;  phdr_load_data_.p_offset = file_header_.e_entry + exec_code_.code_size_ + 1 - DFLT_ENTR_POINT;
    phdr_load_data_.p_vaddr = phdr_load_data_.p_paddr = DFLT_ENTR_POINT + phdr_load_data_.p_offset + 0x00001000;
    phdr_load_text_.p_vaddr = phdr_load_text_.p_paddr = DFLT_ENTR_POINT;
    phdr_load_data_.p_filesz = phdr_load_data_.p_memsz = data_stack_.stack_size_;
    phdr_load_text_.p_filesz = phdr_load_text_.p_memsz = file_header_.e_entry - DFLT_ENTR_POINT + exec_code_.code_size_;
    phdr_load_data_.p_flags = PF_W + PF_R;  phdr_load_text_.p_flags = PF_X + PF_R;
    phdr_load_data_.p_align = phdr_load_text_.p_align = 0x00001000;

    shdr_sect_entry_.sh_name = shdr_sect_entry_.sh_type = shdr_sect_entry_.sh_flags = shdr_sect_entry_.sh_addr =
        shdr_sect_entry_.sh_offset = shdr_sect_entry_.sh_size = shdr_sect_entry_.sh_link = shdr_sect_entry_.sh_info =
        shdr_sect_entry_.sh_addralign = shdr_sect_entry_.sh_entsize = 0x00000000;

    shdr_sect_text_.sh_name = 0x0000000b;
    shdr_sect_text_.sh_type = SHT_PROGBITS;
    shdr_sect_text_.sh_flags = SHF_ALLOC + SHF_EXECINSTR;
    shdr_sect_text_.sh_addr = file_header_.e_entry;
    shdr_sect_text_.sh_offset = shdr_sect_text_.sh_addr - DFLT_ENTR_POINT;
    shdr_sect_text_.sh_size = exec_code_.code_size_ + 1;
    shdr_sect_text_.sh_link = shdr_sect_text_.sh_info = shdr_sect_text_.sh_entsize = 0x00000000;
    shdr_sect_text_.sh_addralign = 0x00000004;

    shdr_sect_data_.sh_name = 0x00000011;
    shdr_sect_data_.sh_type = SHT_PROGBITS;
    shdr_sect_data_.sh_flags = SHF_WRITE + SHF_ALLOC;
    shdr_sect_data_.sh_addr = phdr_load_data_.p_vaddr;
    shdr_sect_data_.sh_offset = phdr_load_data_.p_offset;
    shdr_sect_data_.sh_size = data_stack_.stack_size_;
    shdr_sect_data_.sh_link = shdr_sect_data_.sh_info = shdr_sect_data_.sh_entsize = 0x00000000;
    shdr_sect_data_.sh_addralign = 0x00000004;

    shdr_sect_shstrtab_.sh_name = 0x00000001;
    shdr_sect_shstrtab_.sh_type = SHT_STRTAB;
    shdr_sect_shstrtab_.sh_flags = shdr_sect_shstrtab_.sh_addr = shdr_sect_shstrtab_.sh_link =
        shdr_sect_shstrtab_.sh_info = shdr_sect_shstrtab_.sh_entsize = 0x00000000;
    shdr_sect_shstrtab_.sh_offset = shdr_sect_data_.sh_offset + shdr_sect_data_.sh_size;
    shdr_sect_shstrtab_.sh_size = sects_table_.tab_size_;
    shdr_sect_shstrtab_.sh_addralign = 0x00000001;

    exec_code_.head_block_->code_array_ [1] = phdr_load_data_.p_paddr;
}


void Program :: write_ehdr (Elf32_Ehdr ehdr, FILE* out_file)
{
    fwrite (sign, 4, 4, out_file);
    fwrite (&(ehdr.e_type), 2, 1, out_file);
    fwrite (&(ehdr.e_machine), 2, 1, out_file);
    fwrite (&(ehdr.e_version), 4, 1, out_file);
    fwrite (&(ehdr.e_entry), 4, 1, out_file);
    fwrite (&(ehdr.e_phoff), 4, 1, out_file);
    fwrite (&(ehdr.e_shoff), 4, 1, out_file);
    fwrite (&(ehdr.e_flags), 4, 1, out_file);
    fwrite (&(ehdr.e_ehsize), 2, 1, out_file);
    fwrite (&(ehdr.e_phentsize), 2, 1, out_file);
    fwrite (&(ehdr.e_phnum), 2, 1, out_file);
    fwrite (&(ehdr.e_shentsize), 2, 1, out_file);
    fwrite (&(ehdr.e_shnum), 2, 1, out_file);
    fwrite (&(ehdr.e_shstrndx), 2, 1, out_file);
}

void Program :: write_phdr (Elf32_Phdr phdr, FILE *out_file)
{
    fwrite (&(phdr.p_type), 4, 1, out_file);
    fwrite (&(phdr.p_offset), 4, 1, out_file);
    fwrite (&(phdr.p_vaddr), 4, 1, out_file);
    fwrite (&(phdr.p_paddr), 4, 1, out_file);
    fwrite (&(phdr.p_filesz), 4, 1, out_file);
    fwrite (&(phdr.p_memsz), 4, 1, out_file);
    fwrite (&(phdr.p_flags), 4, 1, out_file);
    fwrite (&(phdr.p_align), 4, 1, out_file);
}

void Program :: write_shdr (Elf32_Shdr shdr, FILE *out_file)
{
    fwrite (&(shdr.sh_name), 4, 1, out_file);
    fwrite (&(shdr.sh_type), 4, 1, out_file);
    fwrite (&(shdr.sh_flags), 4, 1, out_file);
    fwrite (&(shdr.sh_addr), 4, 1, out_file);
    fwrite (&(shdr.sh_offset), 4, 1, out_file);
    fwrite (&(shdr.sh_size), 4, 1, out_file);
    fwrite (&(shdr.sh_link), 4, 1, out_file);
    fwrite (&(shdr.sh_info), 4, 1, out_file);
    fwrite (&(shdr.sh_addralign), 4, 1, out_file);
    fwrite (&(shdr.sh_entsize), 4, 1, out_file);
}

void Program:: write_out (FILE *out_file)
{
    write_ehdr (file_header_, out_file);
    write_phdr (phdr_load_text_, out_file);
    write_phdr (phdr_load_data_, out_file);
    exec_code_.write_out (out_file);
    data_stack_.write_out (out_file);
    sects_table_.write_out (out_file);
    write_shdr (shdr_sect_entry_, out_file);
    write_shdr (shdr_sect_text_, out_file);
    write_shdr (shdr_sect_data_, out_file);
    write_shdr (shdr_sect_shstrtab_, out_file);
}

Old_Prog_Info load_old_prog (FILE *in_file)
{
    unsigned char *old_prog = (unsigned char*) calloc (1, sizeof (unsigned char));
    uint32_t cur_size = 0, max_size = 1;

    while (1)
    {
        if (feof (in_file))
            break;

        uint32_t buf = 0;
        fscanf (in_file, "%d", &buf);
        old_prog [cur_size] = (unsigned char) buf;

        if (feof (in_file))
            break;

        cur_size++;

        if (cur_size == max_size)
            old_prog = (unsigned char*) realloc (old_prog, max_size * 2);
    }
    cur_size--;

    Old_Prog_Info inf;
    inf.prog_data_ = old_prog;
    inf.prog_size_ = cur_size;

    return inf;
}
