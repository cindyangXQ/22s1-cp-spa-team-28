#include "ClauseTable.h"

std::vector<int> ClauseTable::getIndices(std::vector<Reference> commonHeaders) {
    std::vector<int> indices;
    for (int i = 0; i < commonHeaders.size(); i++) {
        for (int j = 0; j < header.size(); j++) {
            if (header[j].getSynonymName() ==
                commonHeaders[i].getSynonymName()) {
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
        if (this->header[i].getSynonymName() == select.getSynonymName()) {
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
    std::vector<Reference> headerCommon;
    for (int i = 0; i < table1.header.size(); i++) {
        for (int j = 0; j < table2.header.size(); j++) {
            if (table1.header[i].getSynonymName() ==
                table2.header[j].getSynonymName()) {
                headerCommon.push_back(table1.header[i]);
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

    for (int i = 0; i < table1.header.size(); i++) {
        if (std::find(indices1.begin(), indices1.end(), i) == indices1.end()) {
            headers.push_back(table1.header[i]);
        }
    }

    for (int i = 0; i < table2.header.size(); i++) {
        if (std::find(indices2.begin(), indices2.end(), i) == indices2.end()) {
            headers.push_back(table2.header[i]);
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
                std::vector<Tuple>{table1.rows[i], table2.rows[j]}));
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

    std::unordered_map<std::string, std::unordered_set<int>> tuple_to_rows;
    for (int i = 0; i < table1.size(); i++) {
        Tuple t1 = table1.rows[i];
        std::vector<Tuple> subTuple1 = t1.splitTuple(table1Indices);
        std::string key = subTuple1[0].to_string();
        if (tuple_to_rows.find(key) == tuple_to_rows.end()) {
            tuple_to_rows.insert({key, {i}});
        } else {
            tuple_to_rows.at(key).insert(i);
        }
    }
    for (int j = 0; j < table2.size(); j++) {
        Tuple t2 = table2.rows[j];
        std::vector<Tuple> subTuple2 = t2.splitTuple(table2Indices);
        std::string key = subTuple2[0].to_string();
        if (tuple_to_rows.find(key) != tuple_to_rows.end()) {
            std::unordered_set<int> table1_rows = tuple_to_rows.at(key);
            for (auto iter = table1_rows.begin(); iter != table1_rows.end();
                 iter++) {
                Tuple t1 = table1.rows[(*iter)];
                std::vector<Tuple> subTuple1 = t1.splitTuple(table1Indices);
                result.insert(Tuple::combineSubTuples(std::vector<Tuple>{
                    subTuple1[0], subTuple1[1], subTuple2[1]}));
            }
        }
    }
    return result;
}

ClauseTable ClauseTable::joinTables(ClauseTable table1, ClauseTable table2) {
    if (table1.header.size() == 0) {
        return table2;
    }
    if (table2.header.size() == 0) {
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
