#pragma once
#include <string>
#include <vector>

class QBRow
{
public:
    QBRow(int c0, std::string c1, int c2, std::string c3);

    int column0; // unique element number
    std::string column1;
    long column2;
    std::string column3;
};