#pragma once

#include "../Cache.h"
#include "../Reflexive.h"
#include "StmtToStmtRelationshipsTable.h"

enum class Position { LEFT, RIGHT };

class AffectsBaseTable : public StmtToStmtRelationshipsTable,
                         public Reflexive,
                         public Cache {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    virtual bool validate(Reference leftRef, Reference rightRef) = 0;

    /*
     * Returns list of possible values that the right synonym can be.
     */
    virtual std::vector<Value> solveRight(Reference leftRef,
                                          EntityName rightSynonym,
                                          StorageView *storage) = 0;

    /*
     * Returns list of possible values that the left synonym can be.
     */
    virtual std::vector<Value> solveLeft(Reference rightRef,
                                         EntityName leftSynonym,
                                         StorageView *storage) = 0;

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    virtual std::vector<std::pair<Value, Value>>
    solveBoth(EntityName leftSynonym, EntityName rightSynonym,
              StorageView *storage) = 0;

    /*
     * Returns list of possible (Value, Value) that a reflexive pair of synonyms
     * can be.
     */
    virtual std::vector<Value> solveBothReflexive(EntityName synonym,
                                                  StorageView *storage) = 0;

    int getTableSize();

protected:
    std::unordered_set<int> assignments;

    bool isAssignment(int stmt);
    bool areAssignments(int left, int right);
    bool isAssignmentEntity(EntityName entity);
    int chooseStmt(int left, int right, Position pos);

    template <class object>
    bool validateHelper(Reference *leftRef, Reference *rightRef, object *table,
                        bool (object::*verifyDoubleWildcards)(),
                        bool (object::*verifySingleWildcard)(int, Position),
                        bool (object::*checkRs)(int, int)) {
        if (leftRef->isWildcard() && rightRef->isWildcard()) {
            return (table->*(verifyDoubleWildcards))();
        }
        if (leftRef->isWildcard()) {
            int right = convertToType<int>(rightRef->getValueString());
            return (table->*(verifySingleWildcard))(right, Position::RIGHT);
        }
        if (rightRef->isWildcard()) {
            int left = convertToType<int>(leftRef->getValueString());
            return (table->*(verifySingleWildcard))(left, Position::LEFT);
        }
        int left = convertToType<int>(leftRef->getValueString());
        int right = convertToType<int>(rightRef->getValueString());
        if (!areAssignments(left, right)) {
            return false;
        }
        return (table->*(checkRs))(left, right);
    };

    template <class object>
    std::vector<Value> solveRightAffectsHelper(
        Reference *leftRef, object *table,
        void (object::*solveSingleWildcard)(std::unordered_set<Value> *,
                                            Position),
        void (object::*solveHelper)(int, std::unordered_set<Value> *,
                                    Position)) {

        std::unordered_set<Value> intermediateResult;
        if (leftRef->isWildcard()) {
            (table->*(solveSingleWildcard))(&intermediateResult,
                                            Position::RIGHT);
        } else {
            int left = convertToType<int>(leftRef->getValueString());
            if (!isAssignment(left)) {
                return std::vector<Value>();
            }
            (table->*(solveHelper))(left, &intermediateResult, Position::LEFT);
        }
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        return result;
    };

    template <class object>
    std::vector<Value> solveLeftAffectsHelper(
        Reference *rightRef, object *table,
        void (object::*solveSingleWildcard)(std::unordered_set<Value> *,
                                            Position),
        void (object::*solveHelper)(int, std::unordered_set<Value> *,
                                    Position)) {

        std::unordered_set<Value> intermediateResult;
        if (rightRef->isWildcard()) {
            (table->*(solveSingleWildcard))(&intermediateResult,
                                            Position::LEFT);
        } else {
            int right = convertToType<int>(rightRef->getValueString());
            if (!isAssignment(right)) {
                return std::vector<Value>();
            }
            (table->*(solveHelper))(right, &intermediateResult,
                                    Position::RIGHT);
        }
        std::vector<Value> result = std::vector<Value>(
            intermediateResult.begin(), intermediateResult.end());
        return result;
    };

    template <class object>
    std::vector<std::pair<Value, Value>>
    solveBothAffectsHelper(object *table, bool (object::*checkRs)(int, int)) {

        std::vector<std::pair<Value, Value>> result;
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                if ((table->*(checkRs))(left, right)) {
                    Value leftValue =
                        Value(ValueType::STMT_NUM, toString(left));
                    Value rightValue =
                        Value(ValueType::STMT_NUM, toString(right));
                    result.push_back(std::make_pair(leftValue, rightValue));
                }
            }
        }
        return result;
    };

    template <class object>
    std::vector<Value>
    solveBothReflexAffectsHelper(object *table,
                                 bool (object::*checkRs)(int, int)) {

        std::vector<Value> result;
        for (int stmt : this->assignments) {
            if ((table->*(checkRs))(stmt, stmt)) {
                Value stmtValue = Value(ValueType::STMT_NUM, toString(stmt));
                result.push_back(stmtValue);
            }
        }
        return result;
    };
};
