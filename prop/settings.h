#pragma once

#define MAX_NAME_LENGTH 20

constexpr auto AMOUNT_OF_RAM = 1000;

constexpr auto AMOUNT_OF_RAM_IN_FUNC = 10;

constexpr auto AMOUNT_OF_FUNCTIONS = 10;

struct Var {
    char * name = nullptr;
};

struct Func {
    Var ram[AMOUNT_OF_RAM_IN_FUNC];
    char * name = nullptr;
    size_t offset = 0;
};

struct Programm {
    Func funcs[AMOUNT_OF_FUNCTIONS];
    size_t current_func = 0;
};

