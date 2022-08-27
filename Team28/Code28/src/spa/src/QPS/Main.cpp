#include <iostream>
#include "QueryParser.h"
using namespace std;

int main() {
    QueryParser::parse("Assign a; Constant c; Variable v; Select v such that Modifies(1, v) pattern a(v, _)");
    return 0;
}