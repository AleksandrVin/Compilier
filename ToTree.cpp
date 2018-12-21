#include "ToTree.h"

extern char * s = nullptr;

extern Programm * prog = nullptr;

void GetSpace()
{
    while (*s == ' ' || *s == '\n') {
        s++;
    }
}

bool GetBraket()
{
    GetSpace();
    if (*s == ';') {
        s++;
        return true;
    }
    return false;
    GetSpace();
}

Node * GetN()
{
    double val = 0.0;
    GetSpace();
    if (*s < '0' || *s > '9') {
        LOG_ERR_MESSAGE("you must specify number here");
        exit(-1);
    }
    else {
        val = val * 10 + *s - '0';
        s++;
    }
    while (('0' <= *s && *s <= '9')) {
        if (*s < '0' || *s > '9') {
            LOG_ERR_MESSAGE("you must specify number here");
            exit(-1);
        }
        val = val * 10 + *s - '0';
        s++;
    }
    if (*s == '.') {
        s++;
        int presicion = 0;
        double val2 = 0.0;
        while (('0' <= *s && *s <= '9')) {
            if (*s < '0' || *s > '9') {
                LOG_ERR_MESSAGE("you must specify number here");
                exit(-1);
            }
            presicion++;
            val2 = val2 * 10 + *s - '0';
            s++;
        }
        val = val + val2 / (pow(10, presicion));
    }
    GetSpace();
    Op * op = OpCtor('N');
    op->number = val;
    Node * node = TreeNodeCtor(op);
    assert(node != nullptr);
    return node;
}

Node * GetP()
{
    GetSpace();
    if (*s == '(') {
        s++;
        GetSpace();
        Node * node = GetE();
        assert(node != nullptr);
        GetSpace();
        assert(*s == ')');
        s++;
        GetSpace();
        return node;
    }
    else if (*s >= '0' && *s <= '9') {
        GetSpace();
        Node * node = GetN();
        assert(node != nullptr);
        GetSpace();
        return node;
    }
    GetSpace();
    char * name = GetId();
    GetSpace();
    if (*s == '(') {
        Node * node = GetCall(name);
        assert(node != nullptr);
        GetSpace();
        return node;
    }
    return GetVarFromName(name);
}

Node * GetIdVar()
{
    char * name = GetId();
    assert(name != nullptr);
    size_t index = FindVar(name);
    if (index != (size_t)-1)
    {
        Op * op = OpCtor('v');
        op->index = index;
        Node * node = TreeNodeCtor(op);
        return node;
    }
    else
    {
        LOG_ERR("no var with name was found", name);
        return nullptr;
    }
}

size_t FindVar(char * name)
{
    for (size_t i = 0; i < prog->funcs[prog->current_func].offset; i++) {
        if (strcmp(prog->funcs[prog->current_func].ram[i].name, name) == 0) {
            return i;
        }
    }
    LOG_ERR_MESSAGE("no var found");
    return (size_t)-1;
}

size_t FindFunc(char * name)
{
    for (size_t i = 0; i < prog->current_func; i++) {
        if (strcmp(prog->funcs[i].name, name) == 0) {
            return i;
        }
    }
    LOG_ERR_MESSAGE("no func found");
    return (size_t)-1;
}


Node * GetT()
{
    GetSpace();
    Node * val = GetP();
    assert(val != nullptr);
    GetSpace();
    if (*s == '*' || *s == '/') {
        Op * op = OpCtor(*s);
        s++;
        GetSpace();
        Node * node = TreeNodeCtor(op);
        node->left = val;
        node->right = GetP();
        assert(node->right != nullptr);
        GetSpace();
        return node;
    }
    return val;
}

Node * GetE()
{
    GetSpace();
    Node * val = GetT();
    assert(val != nullptr);
    GetSpace();
    if (*s == '+' || *s == '-') {
        Op * op = OpCtor(*s);
        s++;
        GetSpace();
        Node * node = TreeNodeCtor(op);
        node->left = val;
        node->right = GetT();
        assert(node->right != nullptr);
        GetSpace();
        return node;
    }
    return val;
}

Node * GetPrint()
{
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("no ( in GetPrint");
        return nullptr;
    }
    s++;
    GetSpace();
    Node * id = GetIdVar();
    assert(id != nullptr);
    GetSpace();
    Op * op = OpCtor('p');
    Node * node = TreeNodeCtor(op);
    node->left = id;
    if (*s != ')') {
        LOG_ERR_MESSAGE("no ) in GetPrint");
    }
    s++;
    GetSpace();
    return node;
}

Node * GetReturn()
{
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("no ( in GetReturn");
        return nullptr;
    }
    s++;
    GetSpace();
    Node * id = GetIdVar();
    assert(id != nullptr);
    GetSpace();
    Op * op = OpCtor('r');
    Node * node = TreeNodeCtor(op);
    node->left = id;
    if (*s != ')') {
        LOG_ERR_MESSAGE("no ) in GetReturn");
    }
    s++;
    GetSpace();
    return node;
}

void * GetVar()
{
    GetSpace();
    char * name = GetId();
    assert(name != nullptr);
    puts("creating new var");
    size_t current_ram = prog->funcs[prog->current_func].offset;
    prog->funcs[prog->current_func].ram[current_ram].name = name;
    prog->funcs[prog->current_func].offset++;
    GetSpace();
    return 0;
}


Node * GetCond()
{
    puts("start reading a condition");
    GetSpace();
    Node * first = GetIdVar();
    assert(first != nullptr);
    GetSpace();
    Op * op = OpCtor('t'); // t - temp
    if (*s == '<') {
        op->op = '<';
    }
    else if (*s == '>') {
        op->op = '>';
    }
    else if (*s == '=') {
        op->op = '=';
    }
    else {
        LOG_ERR_MESSAGE("bad format of condition");
        return nullptr;
    }
    s++;
    GetSpace();
    Node * node = TreeNodeCtor(op);
    node->left = first;
    node->right = GetIdVar();
    assert(node->right != nullptr);
    GetSpace();
    return node;
}

Node * GetIf()
{
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("no ( in if");
        return nullptr;
    }
    s++;
    GetSpace();
    Op * op = OpCtor('i');
    Node * node = TreeNodeCtor(op);
    node->left = GetCond();
    assert(node->left != nullptr);
    GetSpace();
    if (*s != ')') {
        LOG_ERR_MESSAGE("no ) in if");
        return nullptr;
    }
    s++;
    GetSpace();
    if (*s != '{') {
        LOG_ERR_MESSAGE("no { in if");
        return nullptr;
    }
    s++;
    GetSpace();
    Node * node_op = GetOp();
    assert(node_op != nullptr);
    node->right = node_op;
    Node * cur_node = node_op;
    GetSpace();
    if (!GetBraket()) {
        LOG_ERR_MESSAGE("no ; in if");
        return nullptr;
    }
    while (*s != '}') {
        GetSpace();
        cur_node->right = GetOp();
        assert(cur_node != nullptr);
        GetSpace();
        if (!GetBraket()) {
            LOG_ERR_MESSAGE("no ; in if");
            return nullptr;
        }
        cur_node = cur_node->right;
        GetSpace();
    }
    if (*s != '}') {
        LOG_ERR_MESSAGE("no } in if");
        return nullptr;
    }
    *s = ';'; 
    GetSpace();
    return node;
}

Node * GetCall(char * name)
{
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("no ( in func call");
        return nullptr;
    }
    s++;
    GetSpace();
    Node * name_v = GetIdVar();
    Node * cur_name = name_v;
    assert(name_v != nullptr);
    GetSpace();
    while (*s == ',') {
        s++;
        GetSpace();
        cur_name->left = GetIdVar();
        assert(cur_name->left != nullptr);
        cur_name = cur_name->left;
        GetSpace();
    }
    if (*s != ')') {
        LOG_ERR_MESSAGE("no ) in func call");
        return nullptr;
    }
    s++;
    GetSpace();
    Op * op = OpCtor('c');
    op->index = FindFunc(name);
    Node * node = TreeNodeCtor(op);
    assert(node != nullptr);
    node->left = name_v;
    return node;
}

Node * GetAct(char * name)
{
    Node * node_var = GetVarFromName(name);
    assert(node_var != nullptr);
    GetSpace();
    if (*s != '=') {
        LOG_ERR_MESSAGE("no = in act");
        return nullptr;
    }
    s++;
    GetSpace();
    Op * op = OpCtor('=');
    Node * node = TreeNodeCtor(op);
    assert(node != nullptr);
    GetSpace();
    node->left = node_var;
    node->right = GetE();
    assert(node->right != nullptr);
    return node;
}

Node * GetVarFromName(char * name)
{
    Op * op = OpCtor('v');
    Node * node = TreeNodeCtor(op);
    op->index = FindVar(name);
    assert(node != nullptr);
    if (op->index == (size_t)-1) {
        LOG_ERR_MESSAGE("no var found for act");
        return nullptr;
    }
    return node;
}

Node * GetOp()
{
    char * name = GetId();
    assert(name != nullptr);
    Op * op = OpCtor('o');
    Node * node = TreeNodeCtor(op);
    assert(node != nullptr);
    GetSpace();
    if (strcmp(name, "var") == 0) {
        GetVar();
        return node;
    }
    if (strcmp(name, "if") == 0) {
        node->left = GetIf();
        assert(node->left != nullptr);
        return node;
    }
    if (strcmp(name, "return") == 0) {
        node->left = GetReturn();
        assert(node->left != nullptr);
        return node;
    }
    if (strcmp(name, "print") == 0) {
        node->left = GetPrint();
        assert(node->left != nullptr);
        return node;
    }
    node->left = GetAct(name);
    if (node->left == nullptr) {
        LOG_ERR_MESSAGE("can't read act in op");
        return nullptr;
    }
    return node;
}

Node * GetFunc()
{
    GetSpace();
    char * func = GetId();
    if (strcmp(func, "func") != 0) {
        LOG_ERR_MESSAGE("incorrect func init world func");
        return 0;
    }
    char * name = GetId();
    assert(name != nullptr);
    printf("getting function %s\n", name);
    prog->funcs[prog->current_func].name = name;
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("N ( in func decl");
        return nullptr;
    }
    s++;
    GetSpace();
    char * name_var_1 = GetId();
    if (strcmp(name_var_1, "var") != 0) {
        LOG_ERR_MESSAGE("incorrect fuct vars init");
        return 0;
    }
    GetVar();
    GetSpace();
    while (*s == ',') {
        s++;
        GetSpace();
        char * name_var_2 = GetId();
        if (strcmp(name_var_2, "var") != 0) {
            LOG_ERR_MESSAGE("incorrect fuct vars init");
            return 0;
        }
        GetSpace();
        GetVar();
        GetSpace();
    }
    if (*s != ')') {
        LOG_ERR_MESSAGE("no ) in the func decl");
        return nullptr;
    }
    s++;
    GetSpace();
    if (*s != '{') {
        LOG_ERR_MESSAGE("no { in the func decl");
        return nullptr;
    }
    s++;
    GetSpace();
    Op * op = OpCtor('f');
    Node * node = TreeNodeCtor(op);
    assert(node != nullptr);
    node->left = GetOp();
    Node * cur_node = node->left;
    GetSpace();
    if (!GetBraket()) {
        LOG_ERR_MESSAGE("no ; in func decl");
        return nullptr;
    }
    GetSpace();
    while (*s != '}') {
        cur_node->right = GetOp();
        assert(cur_node->right != nullptr);
        GetSpace();
        if (!GetBraket()) {
            LOG_ERR_MESSAGE("no ; in func decl");
            return nullptr;
        }
        GetSpace();
        cur_node = cur_node->right;
    }
    s++;
    GetSpace();
    prog->current_func++;
    return node;
}

Node * GetG()
{
    Op * op = OpCtor('g');
    Node * node = TreeNodeCtor(op);
    node->left = GetFunc();
    Node * cur_node = node->left;
    assert(node->left != nullptr);
    while (*s != '\0') {
        cur_node->right = GetFunc();
        assert(cur_node->right != nullptr);
        cur_node = cur_node->right;
    }
    return node;
}

Node * ParseToTree(char * buff, Programm * prog_in) {
    puts("start parsing tree");
    s = buff;
    prog = prog_in;
    return GetG();
}

char * GetId()
{
    GetSpace();
    char * Id = (char*)calloc(MAX_NAME_LENGTH, sizeof(char));
    char * currentIdPosition = Id;
    if ((*s >= 'A' && *s <= 'Z') ||
        (*s >= 'a' && *s <= 'z') ||
        (*s == '_') ||
        (*s == '$'))
    {
        *currentIdPosition = *s;
        currentIdPosition++;
        s++;
    }
    else {
        *Id = '\0';
        return Id;
    }
    while ((*s >= 'A' && *s <= 'Z') ||
        (*s >= 'a' && *s <= 'z') ||
        (*s >= '0' && *s <= '9') ||
        (*s == '_') ||
        (*s == '$'))
    {
        if (currentIdPosition - Id < MAX_NAME_LENGTH) {
            *currentIdPosition = *s;
            currentIdPosition++;
            s++;
        }
        else {
            *currentIdPosition = '\0';
            LOG_ERR_NUM("you can't use so long name here, max name lenght is", MAX_NAME_LENGTH);
            exit(-1);
        }
    }
    *currentIdPosition = '\0';
    GetSpace();
    return Id;
}