#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../prop/settings.h"
#include "MacrosLib/macros.h"
#include "TreeLib/treeStruct.h"

//double ParseAndSolveEquation(const char * input_string);

double GetG();

double GetN();

double GetE();

double GetT();

double GetP();

char * GetId();

Node * GetNG();

size_t AddVarToFunc(Func * func,char * name);

Node * GetNA(char * name);

Node * GetNOp();

Node * GetNE();

Node * GetNT();

Node * GetNP();

Node * GetVar();

Node * GetIf();

Node * GetNCP();

Node * GetFunc();

void GetSpace(); // and end of string

Node * ParseToTree(char * buff, Programm * prog_in);

size_t AddVar(char * name);

size_t AddFunc(char * name);

Node * GetReturn();

size_t FindVar(char * name);