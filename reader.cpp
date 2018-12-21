#include "Reader.h"

extern const char * s = nullptr;

extern Programm * prog = nullptr;

//double ParseAndSolveEquation(const char * input_string)
//{
//    s = input_string;
//    return GetG();
//}

double GetG()
{
    const char * start_pos = s;
    GetSpace();
    double val = GetE();
    GetSpace();
    if (*s != '\0') {
        LOG_ERR_MESSAGE("no ending of string like \\0");
        exit(-1);
    }
    if (s == start_pos) {
        LOG_ERR_MESSAGE("0 length of formula");
        exit(-1);
    }
    return val;
}

double GetN()
{
    double val = 0.0;
    GetSpace();
    if (*s < '0' || *s > '9') {
        char * name = GetId();
        size_t var_n = FindVar(name);
        if (var_n != (size_t)-1) {
            return prog->vars[var_n].value;
        }
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
    return val;
}

double GetE()
{
    GetSpace();
    double val = GetT();
    GetSpace();
    while (*s == '+' || *s == '-') {
        char operand = *s;
        s++;
        GetSpace();
        double val2 = GetT();
        GetSpace();
        if (operand == '+') {
            val += val2;
        }
        else {
            val -= val2;
        }
    }
    return val;
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

double GetT()
{
    GetSpace();
    double val = GetP();
    GetSpace();
    while (*s == '*' || *s == '/') {
        char operand = *s;
        s++;
        GetSpace();
        double val2 = GetP();
        GetSpace();
        if (operand == '*') {
            val *= val2;
        }
        else {
            val /= val2;
        }
    }
    return val;
}

double GetP()
{
    GetSpace();
    if (*s == '(') {
        s++;
        GetSpace();
        double val = GetE();
        GetSpace();
        assert(*s == ')');
        s++;
        GetSpace();
        return val;
    }
    else if (*s >= '0' && *s <= '9') {
        GetSpace();
        double val = GetN();
        GetSpace();
        return val;
    }
    else {
        GetSpace();
        char * name = GetId();
        if (*s == '(') {
            double val = 0.0;
            s++;
            GetSpace();
            if (strcmp(name, "cos") == 0) {
                val = cos(GetE());
            }
            else if (strcmp(name, "sin") == 0) {
                val = sin(GetE());
            }
            else {
                LOG_ERR("function name isn't registered", name);
                exit(-1);
            }
            GetSpace();
            if (*s != ')') {
                LOG_ERR_MESSAGE("no ) after function");
                exit(-1);
            }
            s++;
            GetSpace();
            return val;
        }
        return 0; /// this is broken part must return name of id
    }
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

Node * GetNG()
{
    Op * op = OpCtor('g');
    Node * node = TreeNodeCtor(op);
    Node * cur_node_1 = nullptr;
    cur_node_1 = node;
    while (*s != '\0') {
        puts("see no \\0 getting op");
        GetSpace();
        cur_node_1->left = GetNOp();
        if (cur_node_1->left == nullptr) {
            return node;
            break;
        }
        GetSpace();
        if (!GetBraket()) {
            LOG_ERR_MESSAGE("no ; after OP");
            exit(-1);
        }
        s++;
        GetSpace();
        if (cur_node_1 == nullptr) {
            LOG_ERR_MESSAGE("bad format of command");
        }
        Op * op_1 = OpCtor('o');
        Node * node_1 = TreeNodeCtor(op);
        cur_node_1->right = node_1;
        cur_node_1 = node_1;
    }
    GetSpace();
    return node;
}

size_t AddVarToFunc(Func * func , char * name)
{
    assert(func->stack_move < AMOUNT_OF_RAM_IN_FUNC);
    func->ram[func->stack_move].name = name;
    func->ram[func->stack_move].value = -2;
    func->stack_move++;
    return func->stack_move - 1;
}

Node * GetNA(char * name)
{
    Op * op = OpCtor('=');
    Node * node = TreeNodeCtor(op);
    puts("try to get A");
    size_t var_n = FindVar(name);
    printf("var_n in %zu\n", var_n);
    double value = 0.0;
    if (var_n != (size_t)-1) {
        GetSpace();
        if (*s == '=') {
            s++;
            GetSpace();
            value = GetN();
            GetSpace();
        }
        else {
            LOG_ERR_MESSAGE("bad format of =");
            exit(-1);
        }
        Op * op_1 = OpCtor('N');
        Node * node_1 = TreeNodeCtor(op_1);
        node_1->data->number = value;
        node->right = node_1;
        Op * op_2 = OpCtor('v');
        Node * node_2 = TreeNodeCtor(op_2);
        node_2->data->number = var_n;
        node->left = node_2;
    }
    return node;
}

Node * GetNOp()
{
    GetSpace();
    puts("getting op");
    char * name = GetId();
    if (name == "\0") {
        puts("no name gets");
        return nullptr;
    }
    printf("get name %s\n", name);
    Op * op = OpCtor('o');
    Node * node = TreeNodeCtor(op);
    if (strcmp(name, "var") == 0) {
        GetSpace();
        puts("this is var");
        node->left = GetVar();
        GetSpace();
    }
    else if (strcmp(name, "if") == 0) {
        GetSpace();
        puts("this is if");

        node->left = GetIf();
        GetSpace();
    }
    else if (strcmp(name, "func") == 0) {
        GetSpace();
        puts("this is func");

        node->left = GetFunc();
        GetSpace();
    }
    else if (strcmp(name, "return") == 0) {
        GetSpace();
        puts("this is return");

        node->left = GetReturn();
        GetSpace();
    }
    else if (node->left == nullptr) {
        puts("try to get A");
        Node * temp = GetNA(name);
        if (temp != nullptr) {
            GetSpace();
            puts("this is =");
            node->left = temp;
            GetSpace();
        }
        else {
            LOG_ERR("function name isn't registered", name);
            exit(-1);
        }
    }
    GetSpace();
    return node;
}

Node * GetNN()
{
    return nullptr;
}

Node * GetNE()
{
    GetSpace();
    Op * op = OpCtor('e');
    Node * node = TreeNodeCtor(op);
    node->left = GetNT();
    GetSpace();
    if (*s == '+' || *s == '-') {
        op->op = *s;
        s++;
        GetSpace();
        node->right = GetNT();
        GetSpace();
    }

    return node;
}

Node * GetNT()
{
    GetSpace();
    Op * op = OpCtor('t');
    Node * node = TreeNodeCtor(op);
    node->left = GetNP();
    GetSpace();
    if (*s == '*' || *s == '/') {
        op->op = *s;
        s++;
        GetSpace();
        node->right = GetNP();
        GetSpace();
    }
    return node;
}

Node * GetNP()
{
    GetSpace();
    Op * op = OpCtor('p');
    Node * node = TreeNodeCtor(op);
    if (*s == '(') {
        s++;
        GetSpace();
        node->left = GetNE();
        GetSpace();
        assert(*s == ')');
        s++;
        GetSpace();
        return node;
    }
    else if (*s >= '0' && *s <= '9') {
        GetSpace();
        op->op = 'N';
        op->number = GetN();
        GetSpace();
        return node;
    }
    else {
        GetSpace();
        char * name = GetId();
        if (*s == '(') {
            s++;
            GetSpace();
            if (strcmp(name, "cos") == 0) {
                op->op = 'C';
                node->left = GetNE();
            }
            else if (strcmp(name, "sin") == 0) {
                op->op = 'S';
                node->left = GetNE();
            }
            else {
                LOG_ERR("name isn't registered", name);
                exit(-1);
            }
            GetSpace();
            if (*s != ')') {
                LOG_ERR_MESSAGE("no ) after function");
                exit(-1);
            }
            s++;
            GetSpace();
            return node;
        }
        if (FindVar(name)) {
            op->op = 'N';
            op->number = prog->vars[FindVar(name)].value;
            GetSpace();
            return node;
        }
        LOG_ERR("name isn't registered", name);
        exit(-1);
        return 0; /// this is broken part must return name of id
    }
    return nullptr;
}

Node * GetVar()
{
    Op * op = OpCtor('v');
    Node * node = TreeNodeCtor(op);
    GetSpace();
    char * name = GetId();
    GetSpace();
    op->number = (int)AddVar(name);
    return node;
}

Node * GetIf()
{
    Op * op = OpCtor('i');
    Node * node = TreeNodeCtor(op);
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("bad if format");
        exit(-1);
    }
    s++;
    GetSpace();
    node->left = GetNCP();
    GetSpace();
    if (*s != ')') {
        LOG_ERR_MESSAGE("no end ) in if");
        exit(-1);
    }
    s++;
    GetSpace();
    if (*s != '{') {
        LOG_ERR_MESSAGE("no { in if");
        exit(-1);
    }
    s++;
    GetSpace();
    node->right = GetNG();
    GetSpace();
    if (*s != '}') {
        LOG_ERR_MESSAGE("no } in if");
        exit(-1);
    }
    s++;
    GetSpace();
    return node;
}

Node * GetNCP()
{
    Op * op = OpCtor('c');
    Node * node = TreeNodeCtor(op);
    GetSpace();
    node->left = GetNE();
    GetSpace();
    if (*s == '>') {
        op->op = '>';
    }
    else if (*s == '<') {
        op->op = '<';
    }
    else if (*s == '=') {
        op->op = '=';
    }
    else {
        LOG_ERR_MESSAGE("bad conparator format");
        exit(-1);
    }
    s++;
    GetSpace();
    node->right = GetNE();
    GetSpace();
    return node;
}

Node * GetFunc()
{
    Op * op = OpCtor('f');
    Node * tree = TreeNodeCtor(op);
    GetSpace();
    char * name = GetId();
    op->number = (int)AddFunc(name);
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("no ( in func");
        exit(-1);
    }
    s++;
    GetSpace();
    char * arg = GetId();
    if (strcmp(arg, "var")) {
        LOG_ERR_MESSAGE("bad arg in func");
        exit(-1);
    }
    GetSpace();
    char * arg_name = GetId();
    puts("adding arg to func");
    AddVarToFunc(&(prog->funcs[(size_t)op->number]), arg_name);
    GetSpace();
    if (*s != ')') {
        LOG_ERR_MESSAGE("bad func call no )");
        exit(-1);
    }
    s++;
    GetSpace();
    if (*s != '{') {
        LOG_ERR_MESSAGE("no { in func");
        exit(-1);
    }
    s++;
    GetSpace();
    Node * cur_node = tree->left;
    while (*s != '}') {
        cur_node = GetNOp();
    }
    tree->left = GetNG();
    GetSpace();
    if (*s != '}') {
        LOG_ERR_MESSAGE("no } in func");
        exit(-1);
    }
    s++;
    GetSpace();
    return tree;
}

void GetSpace()
{
    while (*s == ' ' || *s == '\n') {
        s++;
    }
}

Node * ParseToTree(char * buff, Programm * prog_in)
{
    puts("start parsing tree");
    s = buff;
    prog = prog_in;
    const char * start_pos = s;
    GetSpace();
    Op * op = OpCtor('s');
    Node * tree = TreeNodeCtor(op);
    tree->left = GetNG();
    GetSpace();
    if (tree->left->data->op != 'm') {
        LOG_ERR_MESSAGE("no main func");
        return nullptr;
    }
    if (*s != '\0') {
        LOG_ERR_MESSAGE("no ending of string like \\0");
        exit(-1);
    }
    if (s == start_pos) {
        LOG_ERR_MESSAGE("0 length of prog");
        exit(-1);
    }
    return tree;
}

size_t AddVar(char * name)
{
    puts("adding var");
    assert(prog->current_var + 1 < AMOUNT_OF_RAM);
    prog->vars[prog->current_var].name = name;
    prog->vars[prog->current_var].value = -2;
    prog->current_var++;
    return prog->current_var - 1;
}

size_t AddFunc(char * name)
{
    assert(prog->current_func + 1 < AMOUNT_OF_FUNCTIONS);
    prog->funcs[prog->current_func].name = name;
    prog->funcs[prog->current_func].stack_move = prog->current_stack_move;
    prog->current_func++;
    return prog->current_func - 1;
}

Node * GetReturn()
{
    Op * op = OpCtor('r');
    Node * tree = TreeNodeCtor(op);
    GetSpace();
    if (*s != '(') {
        LOG_ERR_MESSAGE("bad return format no (");
        exit(-1);
    }
    s++;
    GetSpace();
    tree->left = GetNE();
    GetSpace();
    if (*s != ')') {
        LOG_ERR_MESSAGE("no ) in return");
        exit(-1);
    }
    s++;
    GetSpace();
    return tree;
}

size_t FindVar(char * name)
{
    for (size_t i = 0; i < prog->current_var; i++) {
        if (strcmp(prog->vars[i].name, name) == 0) {
            return i;
        }
    }
    LOG_ERR_MESSAGE("no var found");
    return (size_t)-1;
}
