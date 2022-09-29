#pragma once

#include <vector>

#include "../Storage/Storage.h"

/*
 * A Facade class for SP->PKB interactions.
 */
class PopulateFacade {
public:
    /*
     * Explicit constructor for a PopulateFacade.
     */
    explicit PopulateFacade(Storage *storage);

    /*
     * Stores the given statements in a StatementsTable.
     */
    void storeStatements(std::vector<Statement *> *statements);

    /*
     * Stores the given assignments in an AssignmenstTable.
     */
    void storeAssignments(std::vector<Assignment *> *assignments);

    /*
     * Stores the given variables in a VariablesTable.
     */
    void storeVariables(std::vector<Variable *> *variables);

    /*
     * Stores the given constants in a ConstantsTable.
     */
    void storeConstants(std::vector<Constant *> *constants);

    /*
     * Stores the given procedures in a ProceduresTable.
     */
    void storeProcedures(std::vector<Procedure *> *procedures);

    // TODO relook storage into RelationshipsTables, do we need so many repeats?

    /*
     * Stores the given follows relations in a FollowsTable.
     */
    void storeFollows(std::vector<Relationship<int, int> *> *follows);

    /*
     * Stores the given followsT relations in a FollowsTTable.
     */
    void storeFollowsT(std::vector<Relationship<int, int> *> *followsT);

    /*
     * Stores the given parent relations in a ParentTable.
     */
    void storeParent(std::vector<Relationship<int, int> *> *parents);

    /*
     * Stores the given parentT relations in a ParentTTable.
     */
    void storeParentT(std::vector<Relationship<int, int> *> *parentsT);

    /*
     * Stores the given modifiesS relations in a ModifiesSTable.
     */
    void
    storeModifiesS(std::vector<Relationship<int, std::string> *> *modifiesS);

    /*
     * Stores the given modifiesP relations in a ModifiesPTable.
     */
    void storeModifiesP(
        std::vector<Relationship<std::string, std::string> *> *modifiesP);

    /*
     * Stores the given usesS relations in a UsesSTable.
     */
    void storeUsesS(std::vector<Relationship<int, std::string> *> *usesS);

    /*
     * Stores the given usesP relations in a UsesPTable.
     */
    void
    storeUsesP(std::vector<Relationship<std::string, std::string> *> *usesP);

    /*
    * Stores the given Calls relations in a CallsTable.
    */
    void
    storeCalls(std::vector<Relationship<std::string, std::string> *> *calls);

    /*
    * Stores the given CallsT relations in a CallsTTable.
    */
    void
    storeCallsT(std::vector<Relationship<std::string, std::string> *> *callsT);

    /*
    * Stores the given branch ins in a BranchInTable.
    */
    void storeBranchIn(std::vector<Relationship<int, int> *> *branchIn);

    /*
     * Stores the given branch out in a BranchOutTable.
     */
    void storeBranchOut(std::vector<Relationship<int, int> *> *branchOut);

private:
    Storage *storage;
};
