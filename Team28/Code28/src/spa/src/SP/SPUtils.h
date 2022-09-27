#include "EntityNode.h"
#include "Tokenizer.h"
#include "ExprParser.h"

class SPUtils {
public:
    static void follows(const std::vector<StatementNode *> &stmtList,
                        std::vector<Relationship<int, int> *> &result);
    static void followsT(const std::vector<StatementNode *> &stmtList,
                         std::vector<Relationship<int, int> *> &result);
    static void parent(StatementNode *parentNode,
                       std::vector<Relationship<int, int> *> &result);
    static std::vector<int> * parentT(StatementNode *parentNode, std::vector<Relationship<int, int> *> &result);

    static bool compareStmtList(const std::vector<StatementNode *> &list1,
                                const std::vector<StatementNode *> &list2);
    static bool compareProcList(std::vector<ProcedureNode *> &list1,
                                std::vector<ProcedureNode *> &list2);

    static std::string convertExpression(std::string input);
};
