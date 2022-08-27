#pragma once
#include <iostream>
#include "DesignEntity.h"
using namespace std;

class Synonym
{
public:
	DesignEntity entity;
	string name;
    Synonym();
    Synonym(string s);
	Synonym(DesignEntity entity, string name);
};