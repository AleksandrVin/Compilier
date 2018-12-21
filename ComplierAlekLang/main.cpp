#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../../../Libs/FileReaderLib/ReaderLib.h"

int main(size_t argc , char ** argv)
{
    if (argc <= 1) {
        puts("Privide file path to run\n");
        return 0;
    }
    if (argc != 2) {
        puts("Provide only one file to run\n");
            return 1;
    }
    argv++;
    char * file_buff = nullptr;
    size_t size_of_file = ReadFromFile(*argv, &file_buff);

    Tree prog_tree = nullptr;
    size_t amount_of_node = ParseToTree(file_buff);
    puts("enter to exit");
    getc();
    return 0;
}