#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../prop/settings.h"
#include "FileReaderLib/ReaderLib.h"
#include "MacrosLib/macros.h"
#include "TreeLib/treeStruct.h"

#include "ToTree.h"
#include "Saver.h"
#define DOT_FILE "../../data/dot.dot"
#define PNG_FILE "../../data/png.png"
#define file_name "C:\\Users\\Aleksandr\\OneDrive\\Data\\Information\\university\\subjcts\\cs\\Compiler\\data\\prog.aleklang"

int main(size_t argc, char ** argv)
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
    size_t size_of_file = 0;
    size_of_file = ReadFromFile(file_name, &file_buff);

    Programm prog;
    Node * prog_tree = nullptr;
    prog_tree = ParseToTree(file_buff,&prog);
    system("dir");

    Node * tree_main = FindMainFunc(prog_tree,&prog);
    TreeDumpDot(prog_tree, DOT_FILE, PNG_FILE, 0);


    OpenFileError("../data/out.softasm","w",-1);
    SaveMainFunc(file, prog_tree , &prog);
    SaveOtherFuncs(file, prog_tree , &prog);
    fclose(file);

    puts("enter to exit");
    getc(stdin);
    return 0;
}