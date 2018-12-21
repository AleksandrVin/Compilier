#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../prop/settings.h"
#include "MacrosLib/macros.h"
#include "TreeLib/treeStruct.h"


Node * ParseToTree(char * buff, Programm * prog_in);

Node * GetG(); // :: = [GetFunc] +
Node * GetFunc(); //  :: = func GetId([var GetId][, var GetId] *{[GetOp; ] +}
Node * GetOp();  // :: = GetVar | GetIf | GetReturn | GetPrint | GetAct
Node * GetAct(char * name); //  :: = GetId = GetCall | GetId | GetE
Node * GetCall(char * name); // :: = GetId([GetId][, GetId] * )
char * GetId(); //  :: = [a - z][a - z0 - 9] *
Node * GetIf(); //  :: = if (GetCond) { [GetOp;] * }
Node * GetCond(); //  :: = GetId < | > | = GetId
void * GetVar(); // :: = var GetId // no node creation
Node * GetReturn(); //  :: = return(GetId)
Node * GetPrint(); // :: = print(GetId)
Node * GetIdVar();

Node * GetE(); // :: = GetT[[+-]GetT] ?
Node * GetT(); //  :: = GetP[[*/ ]GetP] ?
Node * GetP(); //  :: = (GetE) | GetN | GetId
Node * GetN(); // :: = [+-] ? [0 - 9] + [.] ? [0 - 9] *

void GetSpace();
bool GetBraket();

size_t FindVar(char * name);
size_t FindFunc(char * name);

Node * GetVarFromName(char * name);
