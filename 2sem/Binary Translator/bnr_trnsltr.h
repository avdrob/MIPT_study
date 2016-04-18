#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>

struct Code_Block
{
        uint32_t *code_array_;
        Code_Block *prev_block_, *next_block_;
        uint32_t block_num_, code_size_, code_offset_, src_instr_num_begin_, src_instr_num_end_;
        Code_Block (Code_Block *prev, unsigned char *old_prog);
        Code_Block (Code_Block *prev);
        bool is_jump_;
        void dump ();
        bool ok ();
        void write_out (FILE *out_file);
        ~Code_Block () {printf ("I'll destroy you\n");};
};

struct Sect_Txt
{
        Code_Block *head_block_, *tail_block_;
        uint32_t blocks_counter_, code_size_;
        Sect_Txt (unsigned char *old_prog, uint32_t src_instr_num);
        void set_jumps ();
        void write_out (FILE* out_file);
        void dump ();
        Sect_Txt ();
};

#define STACK_ELEM_NUM 48
struct Sect_Data
{
        uint32_t *data_stack_;
        uint32_t stack_size_;
        Sect_Data ();
        void write_out (FILE* out_file);
};

struct Sect_Shstrtab
{
    char * sect_names_ [3];
    uint32_t sects_counter_, tab_size_;
    Sect_Shstrtab ();
    void write_out (FILE* out_file);
};

#define LNG_EI_IDENT 4
#define DFLT_ENTR_POINT 0x08048000

const uint32_t sign [LNG_EI_IDENT] = {0x464c457f, 0x00010101, 0x00000000, 0x00000000};

struct Program
{
    Elf32_Ehdr file_header_;
    Elf32_Phdr phdr_load_text_, phdr_load_data_;
    Sect_Txt exec_code_;
    Sect_Data data_stack_;
    Sect_Shstrtab sects_table_;
    Elf32_Shdr shdr_sect_entry_, shdr_sect_text_, shdr_sect_data_, shdr_sect_shstrtab_;

    Program (unsigned char *old_prog, uint32_t src_instr_num);
    void write_ehdr (Elf32_Ehdr ehdr, FILE *out_file);
    void write_phdr (Elf32_Phdr phdr, FILE *out_file);
    void write_shdr (Elf32_Shdr shdr, FILE *out_file);
    void write_out (FILE *out_file);
};

struct Instr_Set
{
    const uint32_t *code_array_;
    uint32_t code_size_;
    uint32_t arr_indx;
};

#define SOME_NUM 0x4e554d42 /* "NUMB" */
#define SOME_REG 0xff
#define NAC 0x90909090  /*Not a command*/

const uint32_t ENTR_FRAME_INDX = 29;                     //Commands number
const uint32_t LEAVE_FRAME_INDX = ENTR_FRAME_INDX + 1;  //dependent

const uint32_t _HALT [] = {0x000001b8, 0xcddb3100, 0x90909080};     /*push   %ebp
                                                                      mov    %esp,%ebp
                                                                      mov    $0x80490a8,%edi
                                                                      xor    %esi,%esi
                                                                    */

const uint32_t _PUSH_NUM [] = {0x07c7f701, SOME_NUM, 0xc683f729, 0x90909004};       /*add    %esi,%edi
                                                                                      movl   $SOME_NUM,(%edi)
                                                                                      sub    %esi,%edi
                                                                                      add    $0x4,%esi
                                                                                    */

const uint32_t _PUSH_REG [] = {0xff89f701, 0xc683f729, 0x90909004};                 /*add    %esi,%edi
                                                                                      movl   $SOME_REG,(%edi)
                                                                                      sub    %esi,%edi
                                                                                      add    $0x4,%esi
                                                                                    */

const uint32_t _POP_REG [] = {0x0104ee83, 0x29ff8bf7, 0x909090f7};      /*sub    $0x4,%esi
                                                                          add    %esi,%edi
                                                                          mov    (%edi),%SOME_REG
                                                                          sub    %esi,%edi
                                                                          nop
                                                                          nop
                                                                          nop
                                                                        */

const uint32_t _MOV_NUM [] = {0x4d554eb7, 0x90909042};      /*mov $SOME_NUM, %SOME_REG
                                                              nop
                                                              nop
                                                              nop*/

const uint32_t _MOV_REG [] = {0x9090b789};          /*mov %SOME_REG, %SOME_REG
                                                      nop
                                                      nop*/

const uint32_t _ADD [] = {0x7208fe83, 0xee835315, 0x8bf70104, 0x04ef831f, 0x1f891f03, 0x2904c783, 0x90905bf7};  /*cmp   $0x8,%esi
                                                                                                                  jb    n_engh
                                                                                                                  push   %ebx
                                                                                                                  sub    $0x4,%esi
                                                                                                                  add    %esi,%edi
                                                                                                                  mov    (%edi),%ebx
                                                                                                                  sub    $0x4,%edi
                                                                                                                  add    (%edi),%ebx
                                                                                                                  mov    %ebx,(%edi)
                                                                                                                  add    $0x4,%edi
                                                                                                                  sub    %esi,%edi
                                                                                                                  pop    %ebx
                                                                                                                  n_engh:
                                                                                                                  nop
                                                                                                                  nop*/

const uint32_t _SUB [] = {0x7208fe83, 0xee835318, 0x8bf70108, 0x04c7831f, 0xef831f2b, 0x291f8904, 0x04c683f7, 0x9090905b};
                                                                                                                /*cmp    $0x8,%esi
                                                                                                                  jb     n_engh
                                                                                                                  push   %ebx
                                                                                                                  sub    $0x8,%esi
                                                                                                                  add    %esi,%edi
                                                                                                                  mov    (%edi),%ebx
                                                                                                                  add    $0x4,%edi
                                                                                                                  sub    (%edi),%ebx
                                                                                                                  sub    $0x4,%edi
                                                                                                                  mov    %ebx,(%edi)
                                                                                                                  sub    %esi,%edi
                                                                                                                  add    $0x4,%esi
                                                                                                                  pop    %ebx
                                                                                                                  n_engh:
                                                                                                                  nop
                                                                                                                  nop*/

const uint32_t _MUL [] = {NAC};
const uint32_t _DIV [] = {NAC};
const uint32_t _SIN [] = {NAC};
const uint32_t _COS [] = {NAC};
const uint32_t _TAN [] = {NAC};
const uint32_t _SQRT [] = {NAC};
const uint32_t _EXP [] = {NAC};
const uint32_t _POW [] = {NAC};
const uint32_t _DUMP [] = {NAC};

const uint32_t _NOP [] = {0x90909090};  /*nop
                                          nop
                                          nop
                                          nop*/

const uint32_t _JE [] = {0x90900074};   /*je .
                                          nop
                                          nop*/

const uint32_t _JNE [] = {0x90900075};  /*jne .
                                          nop
                                          nop*/

const uint32_t _JA [] = {0x90900077};   /*ja .
                                          nop
                                          nop*/

const uint32_t _JB [] = {0x90900072};   /*jb .
                                          nop
                                          nop*/

const uint32_t _JAE [] = {0x90900073};  /*jae .
                                          nop
                                          nop*/

const uint32_t _JBE [] = {0x90900076};  /*jbe .
                                          nop
                                          nop*/

const uint32_t _JMP [] = {0x909000eb};  /*jmp .
                                          nop
                                          nop*/

const uint32_t _CALL [] = {0xe8909090, 0x00000000};   /*nop
                                                        nop
                                                        nop
                                                        call .*/

const uint32_t _RET [] = {0x909090c3};                /*ret
                                                        nop
                                                        nop
                                                        nop*/

const uint32_t _OUT [] = {0x7204fe83, 0x04ee834e, 0x5350f701, 0x57565251, 0xf631078b, 0x00000abb, 0xf7d23100, 0x01ec83f3,
                          0x80241488, 0x46302404, 0xed75c085, 0xe189e389, 0x04b89090, 0xbb000000, 0x00000001, 0x000001ba,
                          0x8380cd00, 0x854e01c4, 0x5fe375f6, 0x5b595a5e, 0x90f72958};


                          /*0x46241488, 0xf175c085, 0x00b9e389, 0x02000000, 0x04b8240c, 0xbb000000, 0x00000001, 0x000001ba,
                          0x8380cd00, 0x854e01c4, 0x5fdf75f6, 0x5b595a5e, 0x90f72958};*/

const uint32_t _IN_REG [] = {0x52515350, 0x000003b8, 0x0001bb00, 0xec830000, 0xbae1890b, 0x0000000a, 0x0abb80cd, 0x3a000000,
                             0x0c74241c, 0x30242c80, 0x0402e3f6, 0xefe24424, 0x595acc89, 0x9090585b};

const uint32_t _ENTR_FRAME [] = {0xbfe58955, SOME_NUM, 0x9090f631};   /*pushl %ebp
                                                                        movl %esp, %ebp
                                                                        movl $e_entry, %edi
                                                                        xorl %esi, %esi
                                                                        nop
                                                                        nop*/

const uint32_t _LEAVE_FRAME [] = {0x909090c9};          /*leave
                                                          nop
                                                          nop
                                                          nop*/

struct Old_Prog_Info
{
    unsigned char *prog_data_;
    uint32_t prog_size_;
};

Old_Prog_Info load_old_prog (FILE *in_file);
