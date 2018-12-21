#include "Saver.h"

bool SaveMainFunc(FILE * file, Node * tree, Programm * prog)
{
    Node * func = FindMainFunc(tree, prog); /// here 
    if (func == nullptr) {
        LOG_ERR_MESSAGE("no func in prog");
        return false;
    }
    puts("init");
    Node * cur_op = func->left;
    if (cur_op == nullptr) {
        LOG_ERR_MESSAGE("no op in func main");
        return false;
    }
    assert(func->left != nullptr);
    size_t func_index = FindFuncInFuncs(prog, "main");

    return false;
}

bool SaveFunc(FILE * file, Node * tree, Programm * prog, size_t index);

Node * FindMainFunc(Node * tree, Programm * prog)
{
    Node * current = tree->left;
    if (current == nullptr) {
        LOG_ERR_MESSAGE("no func in prog");
        return nullptr;
    }
    for (size_t i = 0; i < prog->current_func; i++) {
        if (strcmp(prog->funcs[i].name, "main") == 0) {
            return current;
        }
        current = current->right;
        assert(current != nullptr);
    }
    return nullptr;
}

size_t FindFuncInFuncs(Programm * prog, const char * name) {
    for (size_t i = 0; i < prog->current_func; i++)
    {
        if (strcmp(name, prog->funcs[i].name) == 0)
        {
            return i;
        }
    }
    return (size_t)-1;
}