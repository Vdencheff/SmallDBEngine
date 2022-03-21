#include "QBDBManager.h"
#include <iostream>
#include <map>
QBDBManager::QBDBManager(QBTable& db)
    :mDb{db}
{
}

void QBDBManager::printTable()
{
    for (QBRow it : mDb)
    {
        std::cout << "c0= " << it.column0 << std::endl;
        std::cout << "c1= " << it.column1 << std::endl;
        std::cout << "c2= " << it.column2 << std::endl;
        std::cout << "c3= " << it.column3 << std::endl;
        std::cout << std::endl;
    }
}

void QBDBManager::printTableRow(uint16_t row)
{
    if (mDb.size() >= row)
    {
        std::cout << "c0= " << mDb[row].column0 << std::endl;
        std::cout << "c1= " << mDb[row].column1 << std::endl;
        std::cout << "c2= " << mDb[row].column2 << std::endl;
        std::cout << "c3= " << mDb[row].column3 << std::endl;
        std::cout << std::endl;
    }
}

void QBDBManager::printLUT(LookUpTable& const lut)
{
    std::unordered_map<std::string, std::vector<int>>::iterator it;
    for (it = lut.begin(); it != lut.end(); ++it)
    {
        std::vector<int>::iterator it1;
        for (it1 = it->second.begin(); it1 != it->second.end(); ++it1)
        {
            std::cout << it->first << " ==> " << *it1 << std::endl;
        }
    }
}

inline void QBDBManager::setElemInLUT(LookUpTable& lut, std::string newKey, int newValue)
{
    std::unordered_map<std::string, std::vector<int>>::iterator it = lut.find(newKey);
    if (it != lut.end()) // such element exists, add the supplied value
    {
        // add the supplied value only if it is not present in the vector
        std::vector<int>::iterator iter;
        iter = std::find(it->second.begin(), it->second.end(), newValue);
        if (iter == it->second.end())
        {
            it->second.push_back(newValue);
        }
    }
    else // such element does not exist insert the supplied key and value
    {
        lut.insert(std::pair<std::string, std::vector<int>>(newKey, {newValue}));

    }
}

void QBDBManager::populateQBTable(const std::string& prefix, int numRecords)
{
    // updata database
    mDb.reserve(numRecords);
    for (int i = 0; i < numRecords; ++i)
    {
        QBRow rec = { i, prefix + std::to_string(i), i % 100, std::to_string(i) + prefix };
        mDb.emplace_back(rec);
    }
    // update look up tables
    populateLUT();
}

inline void QBDBManager::populateLUT()
{
    std::vector<QBRow>::iterator it;
    int index;
    for (it = mDb.begin(); it != mDb.end(); ++it)
    {
        index = std::distance(mDb.begin(), it);
        setElemInLUT(mLUT[0], std::to_string(it->column0), index);// send iter->column0 (int value) and its index for processing
        setElemInLUT(mLUT[1], it->column1, index);// send iter->column1 (string value) and its index for processing
        setElemInLUT(mLUT[2], std::to_string(it->column2), index);// send iter->column2 (int value) and its index for processing
        setElemInLUT(mLUT[3], it->column3, index);// send iter->column3 (string value) and its index for processing
    }
}

QBTable QBDBManager::QBFindMatchingRecords(std::string searchColumn, std::string& searchString)
{
    QBTable res;
    // find the matched items database position
    std::vector<int> tempRes = PerformStrSearch(searchColumn, searchString);
    // take the matched items positions and extract all the whole rows from the database into new vector
    for (int it : tempRes)
    {
        res.emplace_back(mDb[it]);
    }
    return res;
}

std::vector<int> QBDBManager::PerformStrSearch(std::string searchColumn, std::string& searchString)
{
    std::map<std::string, int> strColToInt{{"column0",0}, {"column1",1}, {"column2",2}, {"column3",3}};
    std::map<std::string, int>::iterator col = strColToInt.find(searchColumn);
    if (col != strColToInt.end())
    {
        // find the searched string in the corresponding look up table
        LookUpTable::iterator it = mLUT[col->second].find(searchString);
        if (it != mLUT[col->second].end())
        {
            return it->second; // return all the position the search item resides in the database
        }
    }
    else
    {
        std::cout << "Wrong column name supplied!" << std::endl;
    }

    std::vector<int> retRes{}; // return empty vector if search does not succeed
    return retRes;
}

void QBDBManager::deleteElem(std::string& searchString)
{
    std::vector<int> retVal;
    // Find all the positions where the search item resides in the database
    // the returned vector must contain only one entry, because searchString contains unique id
    retVal = PerformStrSearch( "column0", searchString);
    if (retVal.size() != 0)
    {
        std::cout << "Element proposed for deletion found!" << std::endl;

        // gather info needed in deletion process
        int pos = *retVal.begin(); // element position in the database
        // Extract all the fields that the searched item contains.
        // Each field will be searched in the corresponding look up table,
        // where the position of this searched element will be removed.
        // If the position of this searched element is the only value in the vector, 
        // the whole element is removed from the look up table.
        col[0] = std::to_string(mDb[pos].column0); // the string representation of the column0 value of the element to be deleted
        col[1] = mDb[pos].column1;                 // the string representation of the column1 value of the element to be deleted
        col[2] = std::to_string(mDb[pos].column2); // the string representation of the column2 value of the element to be deleted
        col[3] = mDb[pos].column3;                 // the string representation of the column3 value of the element to be deleted

        // erase from the database
        mDb.erase(mDb.begin() + pos);
        std::cout << "Database element that is to be deleted:" << std::endl;
        for (int i = 0; i < colNum; ++i)
        {
            std::cout << "column["<<i<<"] = " << col[i] << std::endl;
        }

        // erase from LUTs
        for (int i = 0; i < colNum; ++i)
        {
            deleteElemInLUT(i, pos);
        }
    }
    else
    {
        std::cout << "Element proposed for deletion not found" << std::endl;
    }
}

inline void QBDBManager::deleteElemInLUT(int lutNo, int pos)
{
    LookUpTable::iterator searchRes;
    // find in the corresponding look up table the corresponding database column field
    searchRes = mLUT[lutNo].find(col[lutNo]);
    if (searchRes != mLUT[lutNo].end())
    {
        // erase from the vector only the value corresponding to the deleted element position in the database
        std::vector<int>::iterator iter;
        iter = std::find(searchRes->second.begin(), searchRes->second.end(), pos);
        if (iter != searchRes->second.end())
        {
            std::cout << "Searched element found in LUT "<< lutNo <<" ! Erasing it." << std::endl;
            searchRes->second.erase(iter); // erase the element position from the vector
            // if the serached string is uniqie, then the whole entry must be removed, because after the row deletion it will not exist in the database
            if (searchRes->second.size() == 0)
            {
                mLUT[lutNo].erase(searchRes);
            }
        }
    }
    else
    {
        std::cout << "Element proposed for deletion not found in LUT0!" << std::endl;
    }
}