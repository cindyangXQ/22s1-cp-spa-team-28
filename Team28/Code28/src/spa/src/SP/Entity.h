#include <vector>

using namespace std;

class Entity {};

class Program : public Entity {
	vector<Procedure> procList;

public:
	Program(vector<Procedure> procList);
};

class Procedure : public Entity {
	vector<Statment> stmtList;

public:
	Procedure(vector<Statment> stmtList);
};

class Statement : public Entity {};

class ReadStatement : public Statement {
	Variable var;

public:
	ReadStatement(Variable variable);
};

class PrintStatement : public Statement {
	Variable var;

public:
	PrintStatement(Variable variable);
};

class CallStatement : public Statement {
	Variable var;

public:
	CallStatement(Variable variable);
};

class AssignStatement : public Statement {
	Variable var;
	Expression expr;

public:
	AssignStatement(Variable variable, Expression expression);

};

class Variable : public Entity {
	string varName;

public:
	Variable(string varName);
};
