#pragma once

#include "../Reflexive.h"
#include "NextTTable.h"
#include "StmtToStmtRelationshipsTable.h"
#include "StmtToVarRelationshipsTable.h"

enum class Status { TRUE, FALSE, UNKNOWN };

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
     * Returns list of possible (Value, Value) that a reflexive pair of synonyms
     * can be.
     */
    std::vector<Value> solveBothReflexive(EntityName synonym,
                                          StorageView *storage);

private:
    std::unordered_set<int> assignments;
    std::unordered_set<int> modifiableStatements;
    NextTable *next;
    NextTTable *nextT;
    ModifiesSTable *modifiesS;
    UsesSTable *usesS;
    int totalLines;
    std::map<std::pair<int, int>, Status> matrix;

    bool checkAffects(int left, int right);
    void calculateAffects(int left, int right);
    bool calculateAffectsHelper(int current, int goal,
                                std::vector<std::string> commonVariables,
                                std::map<int, int> visited);

    bool isAssignment(int stmt);
    bool areAssignments(int left, int right);
    bool isModifiableStmt(int stmt);
    bool isAssignmentEntity(EntityName entity);

    std::vector<std::string> getCommonVariables(int left, int right);

    std::vector<std::string>
    getRemainingVariables(std::vector<std::string> *variables, int stmt);
};
