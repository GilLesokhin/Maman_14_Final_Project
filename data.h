#ifndef DATA_H
#define DATA_H

#include "assembler_util.h"
#include "const.h"
#include "pre_prossesor.h"

/*  after assembly colected all the needed info to build the .ob, .ent, .ext
    files this function will build those files while checking for adressing
    errors and some other errors

    @param *final_code  .ob file to write to
    @param *ent_file    .ent file to write to
    @param *ext_file    .ext file to write to
    @param *symb_table symbol table of all the symbols available
    @param *cmd_list a list of all the commands found

    @return TRUE if successful else return FALSE */
bool assembler_build_files(FILE *final_code, FILE *ent_file, FILE *ext_file,
                           map *symb_table, node *cmd_list);

/*  after the pre prossesor has done its job the .am file is given to
   assembler_collect_info to collect all the needed info from the file, check
   for errors and build a symbol table and command list all will be done while
   checking for errors in the code (some errors wont be found untill final
   assembly)

    @param *temp_code .am file from pre-procesor
    @param *reserved_table table of all the reserved words
    @param *symb_table symbol table of all the symbols available
    @param *cmd_list a list to save all the commands found in

    @return TRUE if successful else return FALSE */
bool assembler_collect_info(FILE *temp_code, map *reserved_table,
                            map *symb_table, node *cmd_list);

/* converts an int to two base 32 letters string
    @param  *str the string to right to
    @param  i the int to convert
    @return a string representation in base 32  */
char *base32_conv(char *str, int i);

/* converts an int to one base 32 letter string
    @param  *str the string to right to
    @param  i the int to convert
    @return a string representation in base 32  */
char *base32_conv1(char *str, int i);

/*  makes a string with base 32 representation of the command for .ob file

    @param  *dest_str the string to write to
    @param  *cmd the command to convert
    @param  op_ad_type_str unsigned int A/R/E of the two operands
    @return a string representation if the command in base 32 for .ob file */
char *get_base32_com_str(char *dest_str, com *cmd, unsigned int op_ad_type_str);

/*  makes a string with base 32 representation of the command's operands
    for .ob file and returning

    @param  *dest_str the string to write to
    @param  *source_op_str the first operand to convert (null for no operand)
    @param  *dest_op_str the seconed operand to convert (null for no operand)
    @param  *cmd the hole command
    @param  *symb_table symbol table of all the symbols available
    @param  *ext_file extenral file to write to when found ext refrence

    @return unsigned int representation of the operands in base 32 for .ob file
    and a string representation in base 32 of command operands at dest_str */
unsigned int get_base32_op_str(char *dest_str, char *source_op_str,
                               char *dest_op_str, com *cmd, map *symb_table,
                               FILE *ext_file);

/*  writes to ext_file all the refrences to external symbol

    @param  *ext_file extenral file to write to when found ext refrence
    @param  adress the adress of the refrence
    @param  *label string name of the symbol */
void save_to_ext_file(FILE *ext_file, int adress, char *label);

/*  adds an extern symbol to a table

    @param *symb_table symbol table
    @param *cmd the command
    @param *ent_file the file to write to

    @return TRUE extern was added
    @return FALSE if extern is in the list already or */
void save_entry_to_ent_file(map *symb_table, com *cmd, FILE *ent_file);

#endif