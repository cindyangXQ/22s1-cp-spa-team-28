#include "Parser.h"
#include "../Tokenizer/Token.h"
#include "EntityNode.h"
#include "ExprParser.h"
#include <map>
#include <vector>

std::string REL_LIST[] = {"!", ">", "<", "==", "!=", ">=", "<=", "&&", "||"};

// Constructors
Parser::Parser(int offset, std::vector<Token *> tokens) {
    this->offset = offset;
    this->tokens = tokens;
}

ProcedureParser::ProcedureParser(int offset, std::vector<Token *> tokens,
                                 int startline)
    : Parser(offset, tokens) {
    this->startline = startline;
}

StatementParser::StatementParser(int offset, std::vector<Token *> tokens,
                                 int line)
    : Parser(offset, tokens) {
    this->line = line;
}

// Parse
ProgramNode *ProgramParser::parse() {
    std::vector<Token *> tokenList = this->tokens;

    std::vector<ProcedureNode *> procList;
    int line = 1;

    std::vector<std::string> procNames;

    std::map<std::string, std::vector<std::string>> procCallsMap;

    while (offset < tokenList.size()) {
        ProcedureParser *parser = new ProcedureParser(offset, tokenList, line);
        ProcedureNode *temp = parser->parse();

        // Check if procedures have unique names.
        for (size_t i = 0; i < procNames.size(); i++) {
            if (temp->getName() == procNames[i]) {
                throw "procedure of same name is not allowed";
            }
        }
        procNames.push_back(temp->getName());

        procList.push_back(temp);

        line = temp->getEndline() + 1;
        offset = parser->getOffset();
    }

    for (size_t i = 0; i < procList.size(); i++) {
        procCallsMap[procList[i]->getName()] = procList[i]->getAllCalls();
    }

    for (size_t i = 0; i < procNames.size(); i++) {
        std::vector<std::string> path;

        // Detect cyclic/recursive call
        checkCall(procNames[i], path, procCallsMap);

        // Detect undeclared procedure call
        for (size_t j = 0; j < procCallsMap[procNames[i]].size(); j++) {
            if (find(begin(procNames), end(procNames),
                     procCallsMap[procNames[i]][j]) == end(procNames)) {
                throw "calling undeclared procedure is not allowed";
            }
        }
    }

    return new ProgramNode(procList);
}

void ProgramParser::checkCall(
    std::string proc, std::vector<std::string> path,
    std::map<std::string, std::vector<std::string>> &callmap) {
    if (find(begin(path), end(path), proc) != end(path)) {
        throw "cyclic calling is not allowed";
    }

    path.push_back(proc);

    std::vector<std::string> calls = callmap[proc];
    for (size_t i = 0; i < calls.size(); i++) {
        checkCall(calls[i], path, callmap);
    }
}

ProcedureNode *ProcedureParser::parse() {
    int line = this->startline;
    std::vector<Token *> tokenList = this->tokens;

    std::vector<StatementNode *> stmtList;
    std::vector<std::string> allCalls;

    Token *firstToken = tokenList.at(offset++);
    Token *secondToken = tokenList.at(offset++);
    Token *thirdToken = tokenList.at(offset++);

    if (firstToken->isKeyword() && firstToken->equals("procedure") &&
        secondToken->isName() && thirdToken->equals("{")) {
        while (!tokenList.at(offset)->equals("}")) {
            StatementParser parser = StatementParser(offset, tokenList, line);
            StatementNode *temp = parser.parse();
            stmtList.push_back(temp);

            std::vector<std::string> stmCalls = temp->getAllCalls();
            for (size_t i = 0; i < stmCalls.size(); i++) {
                if (stmCalls[i] == secondToken->getValue()) {
                    throw "recursive call is not allowed";
                }
                allCalls.push_back(stmCalls[i]);
            }

            line = temp->getEndLine() + 1;

            offset = parser.getOffset();
            if (offset >= tokenList.size()) {
                throw "procedure wrong syntax";
            }
        }
    } else {
        throw "procedure wrong syntax";
    }
    if (stmtList.size() == 0) {
        throw "procedure wrong syntax";
    }
    offset++;

    ProcedureNode *result =
        new ProcedureNode(secondToken->getValue(), stmtList);
    result->setAllCalls(allCalls);

    return result;
}

StatementNode *StatementParser::parse() {
    std::vector<Token *> tokenList = this->tokens;
    StatementNode *result;

    Token *firstToken = tokenList.at(offset);

    if (firstToken->isKeyword()) {
        if (firstToken->equals("read")) {
            ReadStmParser parser = ReadStmParser(offset, tokenList, line);
            result = parser.parse();
            offset = parser.getOffset();
        } else if (firstToken->equals("print")) {
            PrintStmParser parser = PrintStmParser(offset, tokenList, line);
            result = parser.parse();
            offset = parser.getOffset();
        } else if (firstToken->equals("call")) {
            CallStmParser parser = CallStmParser(offset, tokenList, line);
            result = parser.parse();
            offset = parser.getOffset();
        } else if (firstToken->equals("while")) {
            WhileStmParser parser = WhileStmParser(offset, tokenList, line);
            result = parser.parse();
            offset = parser.getOffset();
        } else if (firstToken->equals("if")) {
            IfStmParser parser = IfStmParser(offset, tokenList, line);
            result = parser.parse();
            offset = parser.getOffset();
        } else {
            throw "statement wrong syntax";
        }
    } else {
        AssignStmParser parser = AssignStmParser(offset, tokenList, line);
        result = parser.parse();
        offset = parser.getOffset();
    } // TODO: Need to change later

    return result;
}

ReadStatementNode *ReadStmParser::parse() {
    std::vector<Token *> tokenList = this->tokens;

    Token *firstToken = tokenList.at(offset++);
    Token *secondToken = tokenList.at(offset++);
    Token *thirdToken = tokenList.at(offset++);

    if (firstToken->isKeyword() && firstToken->equals("read") &&
        secondToken->isName() && thirdToken->equals(";")) {
        return new ReadStatementNode(new VariableNode(secondToken->getValue()),
                                     line);
    } else {
        throw "read statement wrong syntax";
    }
}

PrintStatementNode *PrintStmParser::parse() {
    std::vector<Token *> tokenList = this->tokens;

    Token *firstToken = tokenList.at(offset++);
    Token *secondToken = tokenList.at(offset++);
    Token *thirdToken = tokenList.at(offset++);

    if (firstToken->isKeyword() && firstToken->equals("print") &&
        secondToken->isName() && thirdToken->equals(";")) {
        return new PrintStatementNode(new VariableNode(secondToken->getValue()),
                                      line);
    } else {
        throw "print statement wrong syntax";
    }
}

CallStatementNode *CallStmParser::parse() {
    std::vector<Token *> tokenList = this->tokens;

    Token *firstToken = tokenList.at(offset++);
    Token *secondToken = tokenList.at(offset++);
    Token *thirdToken = tokenList.at(offset++);

    if (firstToken->isKeyword() && firstToken->equals("call") &&
        secondToken->isName() && thirdToken->equals(";")) {
        return new CallStatementNode(new VariableNode(secondToken->getValue()),
                                     line);
    } else {
        throw "call statement wrong syntax";
    }
}

AssignStatementNode *AssignStmParser::parse() {
    std::vector<Token *> tokenList = this->tokens;

    Token *firstToken = tokenList.at(offset++);
    Token *secondToken = tokenList.at(offset++);

    if (firstToken->isName() && secondToken->equals("=")) {
        ExprParser parser = ExprParser(offset, tokenList, false);
        ExpressionNode *expr = parser.parse();
        offset = parser.getOffset();

        return new AssignStatementNode(new VariableNode(firstToken->getValue()),
                                       expr, line);
    } else {
        throw "assignment statement wrong syntax";
    }
}

WhileStatementNode *WhileStmParser::parse() {
    std::vector<std::string> allCalls;
    Token *firstToken = tokens.at(offset++);
    Token *secondToken = tokens.at(offset++);
    ExpressionNode *cond;
    int startline = this->line;

    if (secondToken->equals("(")) {
        CondParser parser = CondParser(offset, tokens);
        cond = parser.parse();
        if (std::find(std::begin(REL_LIST), std::end(REL_LIST),
                      cond->getToken()->getValue()) == std::end(REL_LIST)) {
            throw "invalid cond expression";
        }
        offset = parser.getOffset();
        if (tokens.at(offset)->equals(")")) {
            offset++;
        } else {
            throw "while statement wrong syntax";
        }
    } else {
        throw "while statement wrong syntax";
    }

    Token *curr = tokens.at(offset++);
    std::vector<StatementNode *> stmtList;

    if (curr->equals("{")) {
        line++;
        while (!tokens.at(offset)->equals("}")) {
            StatementParser parser = StatementParser(offset, tokens, line);
            StatementNode *temp = parser.parse();
            line = temp->getEndLine() + 1;
            stmtList.push_back(temp);
            offset = parser.getOffset();
            if (offset >= tokens.size()) {
                throw "while statement wrong syntax";
            }

            std::vector<std::string> stmCalls = temp->getAllCalls();
            for (size_t i = 0; i < stmCalls.size(); i++) {
                allCalls.push_back(stmCalls[i]);
            }
        }
        if (stmtList.size() == 0) {
            throw "while statement wrong syntax";
        }
        offset++;
        WhileStatementNode *result =
            new WhileStatementNode(stmtList, cond, startline);
        result->setAllCalls(allCalls);
        return result;
    } else {
        throw "while statement wrong syntax";
    }
}

IfStatementNode *IfStmParser::parse() {
    std::vector<std::string> allCalls;

    Token *firstToken = tokens.at(offset++);
    Token *secondToken = tokens.at(offset++);
    ExpressionNode *cond;
    int startline = this->line;

    if (secondToken->equals("(")) {
        CondParser parser = CondParser(offset, tokens);
        cond = parser.parse();
        if (std::find(std::begin(REL_LIST), std::end(REL_LIST),
                      cond->getToken()->getValue()) == std::end(REL_LIST)) {
            throw "invalid cond expression";
        }
        offset = parser.getOffset();
        if (!tokens.at(offset++)->equals(")") ||
            !tokens.at(offset++)->equals("then")) {
            throw "if statement wrong syntax";
        }
    } else {
        throw "if statement wrong syntax";
    }

    Token *curr = tokens.at(offset++);
    std::vector<StatementNode *> ifStmtList;
    std::vector<StatementNode *> elseStmtList;

    if (curr->equals("{")) {
        line++;
        while (!tokens.at(offset)->equals("}")) {
            StatementParser parser = StatementParser(offset, tokens, line);
            StatementNode *temp = parser.parse();
            line = temp->getEndLine() + 1;
            ifStmtList.push_back(temp);
            offset = parser.getOffset();
            if (offset >= tokens.size()) {
                throw "if statement wrong syntax";
            }
            std::vector<std::string> stmCalls = temp->getAllCalls();
            for (size_t i = 0; i < stmCalls.size(); i++) {
                allCalls.push_back(stmCalls[i]);
            }
        }
        offset++;
    } else {
        throw "while statement wrong syntax";
    }

    if (!tokens.at(offset++)->equals("else") ||
        !tokens.at(offset++)->equals("{")) {
        throw "if statement wrong syntax";
    }
    while (!tokens.at(offset)->equals("}")) {
        StatementParser parser = StatementParser(offset, tokens, line);
        StatementNode *temp = parser.parse();
        line = temp->getEndLine() + 1;
        elseStmtList.push_back(temp);
        offset = parser.getOffset();
        if (offset >= tokens.size()) {
            throw "while statement wrong syntax";
        }
        std::vector<std::string> stmCalls = temp->getAllCalls();
        for (size_t i = 0; i < stmCalls.size(); i++) {
            allCalls.push_back(stmCalls[i]);
        }
    }
    offset++;
    if (ifStmtList.size() == 0 || elseStmtList.size() == 0) {
        throw "if statement wrong syntax";
    }
    IfStatementNode *result =
        new IfStatementNode(ifStmtList, elseStmtList, cond, startline);
    result->setAllCalls(allCalls);
    return result;
}
