#include "ClauseTable.h"

ClauseTable::ClauseTable(std::vector<Synonym> header) {
	this->header = header;
}

std::vector<int> ClauseTable::getIndices(std::vector<Synonym> common_headers) {
	std::vector<int> indices;
	for (int i = 0; i < common_headers.size(); i++) {
		for (int j = 0; j < header.size(); j++) {
			if (header[j].name == common_headers[i].name) {
				indices.push_back(j);
			}
		}
	}
	return indices;
}

void ClauseTable::insert(Tuple row) {
	this->rows.push_back(row);
}

size_t ClauseTable::size() {
	return this->rows.size();
}

std::vector<Value> ClauseTable::getValues(Synonym select) {
	int column = -1;
	for (int i = 0; i < this->header.size(); i++) {
		if (this->header[i].name == select.name) {
			column = i;
			break;
		}
	}
	if (column == -1) {
		return std::vector<Value>{};
	}
	else {
		std::vector<Value> values;
		for (int j = 0; j < this->rows.size(); j++) {
			values.push_back(this->rows[j].getValue(column));
		}
		return values;
	}
}

std::vector<Synonym> ClauseTable::getCommonHeaders(ClauseTable table1, ClauseTable table2) {
	std::vector<Synonym> header1 = table1.header;
	std::vector<Synonym> header2 = table2.header;
	std::vector<Synonym> header_common;
	for (int i = 0; i < header1.size(); i++) {
		for (int j = 0; j < header2.size(); j++) {
			if (header1[i].name == header2[j].name) {
				header_common.push_back(header1[i]);
			}
		}
	}
	return header_common;
}

ClauseTable ClauseTable::ConstructTable(ClauseTable table1, ClauseTable table2) {
	std::vector<Synonym> headers = ClauseTable::getCommonHeaders(table1, table2);
	std::vector<int> indices1 = table1.getIndices(headers);
	std::vector<int> indices2 = table2.getIndices(headers);

	for (int i = 0, j = 0; i < table1.header.size(); i++) {
		if (j < indices1.size() && i == indices1[j]) {
			j++;
		}
		else {
			headers.push_back(table1.header[i]);
		}
	}

	for (int i = 0, k = 0; i < table2.header.size(); i++) {
		if (k < indices2.size() && i == indices2[k]) {
			k++;
		}
		else {
			headers.push_back(table2.header[i]);
		}
	}
	return ClauseTable(headers);
}

ClauseTable ClauseTable::joinTables(ClauseTable table1, ClauseTable table2) {
	std::vector<Synonym> common_headers = ClauseTable::getCommonHeaders(table1, table2);
	/*
	if (common_headers.empty()) {
		ClauseTable result = ClauseTable::ConstructTable(table1, table2);
		for (int i = 0; i < table1.size(); i++) {
			for (int j = 0; j < table2.size(); j++) {
				result.insert(Tuple::combineSubTuples(std::vector<Tuple>{table1.rows[i], table2.rows[j]}));
			}
		}
		return result;
	}
	else {
	*/
	ClauseTable result = ClauseTable::ConstructTable(table1, table2);
	std::vector<int> table1Indices = table1.getIndices(common_headers);
	std::vector<int> table2Indices = table2.getIndices(common_headers);
	for (int i = 0; i < table1.size(); i++) {
		for (int j = 0; j < table2.size(); j++) {
			Tuple t1 = table1.rows[i];
			Tuple t2 = table2.rows[j];
			std::vector<Tuple> subTuple1 = t1.splitTuple(table1Indices);
			std::vector<Tuple> subTuple2 = t2.splitTuple(table2Indices);
			if (subTuple1[0].equal(subTuple2[0])) {
				result.insert(Tuple::combineSubTuples(std::vector<Tuple>{subTuple1[0], subTuple1[1], subTuple2[1]}));
			}				
		}
	}
	return result;
	//}
}
