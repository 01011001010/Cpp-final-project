#include <fstream>

#include "fileComprehensionFunctions.h"

bool appendTableTypesFromFile(vector<tableInfo> &appendHere, const string &configFileName){
    ifstream inFile(configFileName.c_str());
    if (!inFile) {
        return false;
    }
    unsigned int nOfLines;
    if (!(inFile >> nOfLines)){
        inFile.close();
        return false;
    }
    int size;
    int count;
    short chairs;
    for (int i = 0; i < nOfLines; ++i) {
        if (!(inFile >> size >> count >> chairs)){
            inFile.close();
            return false;
        }
        if (size%2 != 0 || size < DOUBLE || size > OCTUPLE){
            inFile.close();
            return false;
        }
        appendHere.push_back(tableInfo(static_cast<TableSize>(size), count, chairs));
    }
    inFile.close();
    return true;
}

bool appendItemsFromFile(vector<initialCountAndDetails> &appendHere, const string &configFileName){
    ifstream inFile(configFileName.c_str());
    if (!inFile) {
        return false;
    }
    unsigned int nOfLines;
    if (!(inFile >> nOfLines)){
        inFile.close();
        return false;
    }
    int item;
    unsigned int batchSize;
    unsigned int price;
    unsigned int timeToPrepare;
    string menuName;
    for (int i = 0; i < nOfLines; ++i) {
        if (!(inFile >> item >> batchSize >> price >> timeToPrepare) ||
            !getline(inFile, menuName) ||
            menuName.empty() ||
            menuName.front() != ' ' ||
            menuName == " "
           ) {
            inFile.close();
            return false;
        }
        if (!((item >= REDVELVET && item <= VANCUPCAKE) ||
              (item >= REDVELVET+100 && item <= VANCUPCAKE+100) ||
              (item >= ESPRESSO && item <= SODA))
           ) {
            inFile.close();
            return false;
        }
        appendHere.push_back(initialCountAndDetails(static_cast<ItemName>(item), batchSize,
                                                    menuName.substr(1), price, timeToPrepare));
    }
    inFile.close();
    return true;
}