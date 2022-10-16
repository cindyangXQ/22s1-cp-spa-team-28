#include "ClauseTable.h"

// TOFIX: all loops that are commonly used can be abstracted
// TOFIX: joinTables() can be abstracted further, might be good to use more
// helper funcs
std::vector<int> ClauseTable::getIndices(std::vector<Reference> commonHeaders) {
    std::vector<int> indices;
    for (int i = 0; i < commonHeaders.size(); i++) {
        for (int j = 0; j < header.size(); j++) {
            if (header[j].syn.name == commonHeaders[i].syn.name) {
                indices.push_back(j);
            }
        }
    }
    return indices;
}

void ClauseTable::insert(Tuple row) { this->rows.push_back(row); }

size_t ClauseTable::size() { return this->rows.size(); }

std::vector<Value> ClauseTable::getValues(Reference select) {
    int column = UNINTIALIZED;
    for (int i = 0; i < this->header.size(); i++) {
        if (this->header[i].syn.name == select.syn.name) {
            column = i;
            break;
        }
    }
    if (column == UNINTIALIZED) {
        return std::vector<Value>{};
    }
    std::vector<Value> values;
    for (int j = 0; j < this->rows.size(); j++) {
        values.push_back(this->rows[j].getValue(column));
    }
    return values;
}

std::vector<Reference> ClauseTable::getCommonHeaders(ClauseTable table1,
                                                     ClauseTable table2) {
    std::vector<Reference> header1 = table1.getHeader();
    std::vector<Reference> header2 = table2.getHeader();
    std::vector<Reference> headerCommon;
    for (int i = 0; i < header1.size(); i++) {
        for (int j = 0; j < header2.size(); j++) {
            if (header1[i].syn.name == header2[j].syn.name) {
                headerCommon.push_back(header1[i]);
            }
        }
    }
    return headerCommon;
}

ClauseTable ClauseTable::ConstructTable(ClauseTable table1,
                                        ClauseTable table2) {
    std::vector<Reference> headers =
        ClauseTable::getCommonHeaders(table1, table2);
    std::vector<int> indices1 = table1.getIndices(headers);
    std::vector<int> indices2 = table2.getIndices(headers);

    for (int i = 0, j = 0; i < table1.getHeader().size(); i++) {
        if (j < indices1.size() && i == indices1[j]) {
            j++;
        } else {
            headers.push_back(table1.getHeader()[i]);
        }
    }

    for (int i = 0, k = 0; i < table2.getHeader().size(); i++) {
        if (k < indices2.size() && i == indices2[k]) {
            k++;
        } else {
            headers.push_back(table2.getHeader()[i]);
        }
    }
    return ClauseTable(headers);
}

ClauseTable ClauseTable::handleEmptyCommonHeadersJoin(ClauseTable table1,
                                                      ClauseTable table2) {
    ClauseTable result = ClauseTable::ConstructTable(table1, table2);
    for (int i = 0; i < table1.size(); i++) {
        for (int j = 0; j < table2.size(); j++) {
            result.insert(Tuple::combineSubTuples(
                std::vector<Tuple>{table1.getRows()[i], table2.getRows()[j]}));
        }
    }
    return result;
}

ClauseTable
ClauseTable::handleCommonHeadersJoin(ClauseTable table1, ClauseTable table2,
                                     std::vector<Reference> commonHeaders) {
    ClauseTable result = ClauseTable::ConstructTable(table1, table2);
    std::vector<int> table1Indices = table1.getIndices(commonHeaders);
    std::vector<int> table2Indices = table2.getIndices(commonHeaders);
    for (int i = 0; i < table1.size(); i++) {
        for (int j = 0; j < table2.size(); j++) {
            Tuple t1 = table1.getRows()[i];
            Tuple t2 = table2.getRows()[j];
            std::vector<Tuple> subTuple1 = t1.splitTuple(table1Indices);
            std::vector<Tuple> subTuple2 = t2.splitTuple(table2Indices);
            if (subTuple1[0].equal(subTuple2[0])) {
                result.insert(Tuple::combineSubTuples(std::vector<Tuple>{
                    subTuple1[0], subTuple1[1], subTuple2[1]}));
            }
        }
    }
    return result;
}

ClauseTable ClauseTable::joinTables(ClauseTable table1, ClauseTable table2) {
    if (table1.getHeader().size() == 0) {
        return table2;
    }
    if (table2.getHeader().size() == 0) {
        return table1;
    }
    std::vector<Reference> commonHeaders =
        ClauseTable::getCommonHeaders(table1, table2);

    if (commonHeaders.empty()) {
        return handleEmptyCommonHeadersJoin(table1, table2);
    } else {
        return handleCommonHeadersJoin(table1, table2, commonHeaders);
    }
}

std::vector<Reference> ClauseTable::getHeader() { return this->header; }
std::vector<Tuple> ClauseTable::getRows() { return this->rows; }
