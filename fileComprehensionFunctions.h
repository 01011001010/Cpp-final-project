#include "Table.h"
#include "Order.h"

#include <string>
#include <vector>

#pragma once

class tableInfo {
    TableSize size;
    unsigned int numberOfTables;
    unsigned short numberOfChairs;

public:
    inline explicit tableInfo(const TableSize &type=DOUBLE, const unsigned int tables=0,
                              const unsigned short chairs=0)
            : size(type), numberOfTables(tables), numberOfChairs(chairs) {};

    inline TableSize getSize() const { return size; }
    inline unsigned int getNumberOfTables() const { return numberOfTables; }
    inline unsigned short getNumberOfChairs() const { return numberOfChairs; }
};

bool appendTableTypesFromFile(vector<tableInfo> &appendHere,
                              const string &configFileName="CoffeehouseParametersTables.txt");
// appends tableInfo from the file to the given vector
// if the file was successfully opened and the format was not corrupted,
// the return value is true
//
// if something went wrong, the return value is false
// as a note, if the file was successfully opened but the format was corrupted,
// the tables up to the corruption were nonetheless appended to the vector
bool appendItemsFromFile(vector<initialCountAndDetails> &appendHere,
                         const string &configFileName="CoffeehouseParametersItems.txt");
// appends initialCountAndDetails from the file to the given vector
// if the file was successfully opened and the format was not corrupted,
// the return value is true
//
// if something went wrong, the return value is false
// as a note, if the file was successfully opened but the format was corrupted,
// the items up to the corruption were nonetheless appended to the vector
