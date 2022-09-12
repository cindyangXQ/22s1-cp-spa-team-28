#include "ClauseTable.h"

ClauseTable::ClauseTable(std::vector<Synonym> header) {
	this->header = header;
}

std::vector<int> ClauseTable::getIndices(std::vector<Synonym> common_headers) {
	std::vector<int> indices;
	int i, j;
	for (i = 0; i < common_headers.size(); i++) {
		for (j = 0; j < header.size(); j++) {
			if (header[j].name == common_headers[i].name) {
				indices.push_back(j);
			}
		}
	}
	_ASSERT(indices.size() == common_headers.size());
	return indices;
}

void ClauseTable::insert(Tuple row) {
	_ASSERT(row.size() == this->header.size());
	this->rows.push_back(row);
}

size_t ClauseTable::size() {
	return this->rows.size();
}

std::vector<Synonym> ClauseTable::getCommonHeaders(ClauseTable table1, ClauseTable table2) {
	std::vector<Synonym> header1 = table1.header;
	std::vector<Synonym> header2 = table2.header;
	std::vector<Synonym> header_common;
	int i, j;
	for (i = 0; i < header1.size(); i++) {
		for (j = 0; j < header2.size(); j++) {
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
	int i, j;
	for (i = 0, j = 0; i < table1.header.size(); i++) {
		if (j < indices1.size() && i == indices1[j]) {
			j++;
		}
		else {
			headers.push_back(table1.header[i]);
		}
	}
	for (i = 0, j = 0; i < table2.header.size(); i++) {
		if (j < indices2.size() && i == indices2[j]) {
			j++;
		}
		else {
			headers.push_back(table2.header[i]);
		}
	}
	return ClauseTable(headers);
}
/*
ClauseTable ClauseTable::joinTables(ClauseTable table1, ClauseTable table2) {
	return 
}
*/