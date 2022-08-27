#include <iostream>
#include "QueryParser.h"
using namespace std;

int main() {
    QueryParser::parse("Constant c; Variable v; Select v;");
    return 0;
}