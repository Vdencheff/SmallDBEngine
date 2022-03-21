/*
searched item- element of the structure of the data base: column1, column1 etc.

To increase search procedure, the searched items must be processed.
Some examples for such processing:
1. Add hasesh to each search element
2. Sort search elements
3. Create tree out of search elements

The return result is a vector of data structures. Searched item is one element of this data structure
Due to that fact the above methods are more or less appropriate
1. Adding hashes will not work if they are added as additional fieled in the structure. Hashes must be sorted to be able to profit from them.
And we cannot sort by all elements of the table at the same time, unless we create separate structure for each element of the data structure.
Unordered map uses hashes and sorts them. This is a good choise for increase search speed, but it has increased memory consumption and create database time.
The unordered map must be external for the data base structure. There will be one unordered map for each search item.
The key part of the unordered map is the element of the data structure, the value part is vector of numbers showing at which position in the database vector
the searched element is found. Then by just accessing by index the results from the database vector are extracted.
This is the approach in the proposed solution.

2. Sorting each element is not appropriate, because multiple copies of the database must be present - one copy for each element of the structure

3. Create tree is a good approach. The tree will substitute the unordered map proposed above. The nodes are the keys.
Each node that corresponds to valid search item will have an vector element,
that points to where in the database vector the search item is found. The thee has advantages to be less memory greedy than unordered map,
because no hashes are needed, and not need to store each search item completely (imagine creating a dictionary).
The search time is expected to be fast, because the search time depends only on the number of letters of the search string,
and on how many places each reached node points to.


If large enough data base table is required, exception could be thrown. Check all memory reservation opertions.
*/



#include <iostream>
#include <unordered_map>
#include <string>
#include <iterator> 
#include <time.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <assert.h>
#include "QBDBManager.h"

using namespace std::chrono;
void printRes(QBTable searchResults)
{
    std::cout << "Search results:" << std::endl;
    for (QBRow it : searchResults)
    {
        std::cout << "c0= " << it.column0 << std::endl;
        std::cout << "c1= " << it.column1 << std::endl;
        std::cout << "c2= " << it.column2 << std::endl;
        std::cout << "c3= " << it.column3 << std::endl;
        std::cout << std::endl;
    }
    std::cout << "End search results!\n" << std::endl;
}

int main()
{
    QBTable res;
    std::string poplulateStr = "testdata";

    // test 1
    std::cout << "!!!!!!!!!! TEST 1 !!!!!!!!!!" << std::endl;
    QBDBManager dbManager;
    std::string searchStr = "2";
    dbManager.populateQBTable(poplulateStr, 3);
    std::cout << "Search in column0 for " << searchStr << std::endl;
    auto startTimer = steady_clock::now();
    res = dbManager.QBFindMatchingRecords("column0", searchStr);
    std::cout << "profiler: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
    printRes(res);
    assert(res.size() == 1);
    std::cout << "Print table before deletion!" << std::endl;
    dbManager.printTable();
    std::cout << "Deletion follows!" << std::endl;
    dbManager.deleteElem(searchStr);
    std::cout << "Print table after deletion!" << std::endl;
    dbManager.printTable();
    std::cout << "Search in column0 for " << searchStr << std::endl;
    res = dbManager.QBFindMatchingRecords("column0", searchStr);
    printRes(res);
    assert(res.size() == 0);

    // test 2
    std::cout << "!!!!!!!!!! TEST 2 !!!!!!!!!!" << std::endl;
    QBDBManager dbManager1;
    dbManager1.populateQBTable(poplulateStr, 300);
    startTimer = steady_clock::now();
    res = dbManager1.QBFindMatchingRecords("column2", searchStr);
    std::cout << "profiler: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
    printRes(res);
    assert(res.size() == 3);

    // test 3
    std::cout << "!!!!!!!!!! TEST 3 !!!!!!!!!!" << std::endl;
    QBDBManager dbManager2;
    dbManager2.populateQBTable(poplulateStr, 5000);
    std::string searchStr3 = "testdata2";
    startTimer = steady_clock::now();
    res = dbManager2.QBFindMatchingRecords("column1", searchStr3);
    std::cout << "profiler: " << double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den << std::endl;
    printRes(res);
    assert(res.size() == 1);
}
