#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../prop/settings.h"
#include "MacrosLib/macros.h"
#include "TreeLib/treeStruct.h"

bool SaveMainFunc(FILE * file, Node * tree, Programm * prog);

Node * FindMainFunc(Node * tree, Programm * prog);

bool SaveOtherFuncs(FILE  * file, Node * tree, Programm * prog);

size_t FindFuncInFuncs(Programm * prog, const char * name);
