#pragma once

#include "../Reflexive.h"
#include "NextTTable.h"
#include "StmtToStmtRelationshipsTable.h"
#include "StmtToVarRelationshipsTable.h"

enum class Status { TRUE, FALSE, UNKNOWN };
enum class Position { LEFT, RIGHT };

class AffectsTable : public StmtToStmtRelationshipsTable, public Reflexive {
public:
    void initAffects(StorageView *storage);

    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef);

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  StorageView *storage);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 StorageView *storage);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   StorageView *storage);

    /*
     * Returns list of possible values that has a reflexive relationship.
     */
    std::vector<Value> solveBothReflexive(EntityName synonym,
                                          StorageView *storage);

    std::map<std::pair<int, int>, bool> eagerGetMatrix();

private:
    std::unordered_set<int> assignments;
    std::unordered_set<int> modifiableStatements;
    NextTable *next;
    ModifiesSTable *modifiesS;
    UsesSTable *usesS;
    int totalLines;
    std::map<std::pair<int, int>, Status> matrix;

    bool checkAffects(int left, int right);
    bool verifySingleWildcard(int stmt, Position stmtPos);
    bool verifyDoubleWildcards();
    int chooseStmt(int left, int right, Position pos);
    void solveSingleWildcard(std::unordered_set<Value> *intermediateResult,
                             Position stmtPos);
    void solveHelper(int stmt, std::unordered_set<Value> *intermediateResult,
                     Position stmtPos);

    /*
     * Updates the matrix based on all possible Affects from stmt.
     */
    void calculateAffects(int stmt);
    /*
     * Runs DFS through the CFG to find Affects(start, curr).
     */
    void calculateAffectsHelper(int start, int current,
                                std::string modifiedVariable,
                                std::map<int, int> visited);

    bool isAffects(int s2, std::string v);
    bool isModifiableStmt(int stmt);
    bool isModified(int stmt, std::string v);

    bool isAssignment(int stmt);
    bool areAssignments(int left, int right);
    bool isAssignmentEntity(EntityName entity);
};
