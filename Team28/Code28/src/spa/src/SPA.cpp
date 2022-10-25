#include "SPA.h"

SPA::SPA() {
    this->pkb = PKB();
    this->qps = QPS(this->pkb.queryFacade);
    this->sp = SP(this->pkb.populateFacade);
}

void SPA::parse(std::string filename) { this->sp.parse(filename); }

void SPA::preprocess() {
    this->pkb.populateNext();
    this->pkb.populateNextT();
    this->pkb.initAffects();
}

void SPA::evaluateQuery(std::string query, std::list<std::string> &results) {
    this->qps.evaluate(query, results);
}

PKB SPA::getPKB() { return this->pkb; }
