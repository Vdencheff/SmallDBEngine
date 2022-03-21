#pragma once
#include "QBDataBase.h"
#include <unordered_map>

typedef std::vector<QBRow> QBTable;
typedef std::unordered_map <std::string, std::vector<int>> LookUpTable;

class QBDBManager
{
public:

	QBDBManager() = default;
	QBDBManager(QBTable&);

	// print the whole database table
	void printTable();

	// print only one row of the database table
	// input params:
	//    row: the row number in the database table to be printed
	void printTableRow(uint16_t row);

	// print look up table
	// input params:
	//    lut: look up table to be printed
	void printLUT(LookUpTable& const lut);

	// fill the whole database table and related look up tables
	// input params:
	//    prefix: string to be used for data generation
	//    numRecords: how many new rows to be inserted into database
	void populateQBTable(const std::string& prefix, int numRecords);

	// deleting a row from the database. Look up tables are updated as well
	// input params:
	//    searchString: the unique id of the row to be deleted. Uniqie id is kept in column0
	void deleteElem(std::string& searchString);

	// performing search in the database
	// input params:
	//    searchColumn: database column to search
	//    searchString: what to search for in the column
	// return: vector containing all database rows that matches search criteria
	QBTable QBFindMatchingRecords(std::string searchColumn, std::string& searchString);

private:

	static constexpr int colNum = 4; // number of columns in the database

	// Update a look up table with new info or update the current info
	// input params:
	//    lut: look up table to be updated
	//    newKey: the key to be inserted or updated
	//    newValue: value to be inserted
	inline void setElemInLUT(LookUpTable& lut, std::string newKey, int newValue);

	// Update all look up tables with the data from the database
	inline void populateLUT();

	// delete element from look up table
	// input params:
	//    lutNo: look up table number
	//    pos: element position in the database
	inline void deleteElemInLUT(int lutNo, int pos);

	// Perform search in the database
	// input params:
	//    searchColumn: database column to search
	//    searchString: what to search for in the column
	// return: vector containing the positions of finded records in the database
	std::vector<int> PerformStrSearch(std::string searchColumn, std::string& searchString);

	QBTable mDb;
	LookUpTable mLUT[colNum];
	std::string col[colNum];
};
