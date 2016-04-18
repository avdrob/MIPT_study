#include <stdio.h>
#include "bnr_trnsltr.h"

int main ()
{
    FILE *in_file = fopen ("executable.pc", "r");
    FILE *out_file = fopen ("bnr.out", "w");
    Old_Prog_Info inf = load_old_prog (in_file);
    Program prog (inf.prog_data_, inf.prog_size_);
    prog.write_out (out_file);

    fclose (in_file);
    fclose (out_file);

    return 0;
}
