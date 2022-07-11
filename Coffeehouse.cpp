#include <numeric>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Coffeehouse.h"

Coffeehouse::Coffeehouse(milliseconds openedFor, vector<tableInfo> tableTypes,
                         const vector<initialCountAndDetails> &initialStockWithPrices,
                         bool readLeftoversFromFile, bool winterTime,
                         bool saveConfiguration)
        : counter(initialStockWithPrices, openedFor, readLeftoversFromFile, winterTime) {
    const auto numberOfTablesOfThisType = [](int addToThis, const tableInfo &tableInfoInst) {
        return addToThis + tableInfoInst.getNumberOfTables();
    };
    auto numberOfTables =  accumulate(tableTypes.begin(), tableTypes.end(), 0, numberOfTablesOfThisType);

    sort(tableTypes.begin(), tableTypes.end(), [](const tableInfo &a, const tableInfo &b) {
        return a.getNumberOfChairs() < b.getNumberOfChairs();
    });

    //table 0 is a virtual table for takeout, is always occupied
    auto *toAdd = new Table(true);
    tables.push_back(toAdd);

    if (tables.max_size() >= numberOfTables){
        tables.reserve(numberOfTables+1);
        for (const tableInfo &tableInfoInst : tableTypes) {
            if (tableInfoInst.getNumberOfTables() >= 0) {
                Table tryIfHasChairs(tableInfoInst.getSize(), tableInfoInst.getNumberOfChairs());
                if (tryIfHasChairs.getNumberOfChairs() != 0) {
                    for (int i = 0; i < tableInfoInst.getNumberOfTables(); ++i) {
                        toAdd = new Table(tableInfoInst.getSize(), tableInfoInst.getNumberOfChairs());
                        tables.push_back(toAdd);
                    }
                }
            }
        }
    }

    updateMenus();

    if (saveConfiguration){
        saveInitialConfigurationToFiles(openedFor, tableTypes, initialStockWithPrices, winterTime);
    }
}



Coffeehouse::~Coffeehouse() {
    while(!tables.empty()) {
        delete tables.back();
        tables.pop_back();
    }
}

bool Coffeehouse::placeOrder(const unsigned int tableNumber, const ItemName item, const unsigned int numberOf){
    if (tableNumber < 0 || tableNumber >= tables.size() || !(tables[tableNumber]->isOccupied())){
        cout << "\nThere is no such occupied table...";
        return false;
    }

    milliseconds availabilityBefore = counter.itemAvailability(item);
    Order order(item, numberOf, counter.itemPrice(item), counter.itemName(item));
    if (counter.prepareCake(order) == invalidOrder && counter.prepareDrink(order) == invalidOrder){
        cout << "\nNo such item can be ordered in this amount...";
        return false;
    }
    tables[tableNumber]->addToBill(order);
    if (availabilityBefore != counter.itemAvailability(item)){
        updateMenus();
    }
    cout << "\nThe order was successful.";
    return true;
}

bool Coffeehouse::seatGuests(const short numberOfGuests){
    auto i = 1;
    for (; i < tables.size() && !tables.at(i)->seatGuests(numberOfGuests); i++ );
    if (i == tables.size()){
        cout << "\nThere is not any free table...";
        return false;
    }
    cout << "\nThe guests were seated at the table " << i;
    return true;
}

bool Coffeehouse::bookATable(const string &nameToBookFor, const short numberOfGuests){
    if (nameToBookFor.empty()){
        cout << "\nPlease provide a name...";
        return false;
    }
    if (nameAlreadyHasABooking(nameToBookFor)){
        cout << "\nThere cannot be multiple bookings for one name...";
        return false;
    }
    auto i = 1;
    for (; i < tables.size() && !tables.at(i)->bookForName(nameToBookFor, numberOfGuests); i++ );
    if (i == tables.size()){
        cout << "\nThere isn't a free table for this number of guests...";
        return false;
    }
    cout << "\nTable " << i << " was successfully booked for the name " << nameToBookFor;
    return true;
}

bool Coffeehouse::seatBookedGuests(const string &name, const short numberOfGuests) {
    auto i = 1;
    for (; i < tables.size() && !tables.at(i)->seatBookedGuests(name, numberOfGuests); i++ );
    if (i == tables.size()){
        for (i = 1; i < tables.size() && !tables.at(i)->seatGuests(numberOfGuests); i++ );
        if (i == tables.size()) {
            cout << "\nThere is no booking for the provided name and there are no other free tables...";
            return false;
        }
        for (i = 1; i < tables.size() && !tables.at(i)->cancelBooking(name); i++ );
    }
    cout << "\nThe guests were seated at table " << i;
    return true;
}

void Coffeehouse::checkPlease(const unsigned int tableNumber) {
    if (tableNumber >= tables.size()){
        cout << "\nThere is no such table...";
        return;
    }
    string receipt = tables.at(tableNumber)->pay();
    if (receipt.empty()){
        cout << "\nThere is no open tab for that table...";
        return;
    }
    cout << "\nPrinting receipt for " << (tableNumber == 0 ? "takeout" : "table ");
    if (tableNumber != 0){
        cout << tableNumber;
    }
    receiptPrinter << receipt;
}

void Coffeehouse::finalPaymentAndDeparture(const unsigned int tableNumber){
    if (tableNumber >= tables.size()){
        cout << "\nThere is no such table...";
        return;
    }
    checkPlease(tableNumber);
    if (tableNumber == 0){
        return;
    }
    tables.at(tableNumber)->theGuestsWantToLeave();
    cout << "\nCleanup for table " << tableNumber;
    tables.at(tableNumber)->clean();
}

bool Coffeehouse::nameAlreadyHasABooking(const string &name) const {
    auto i = 1;
    for (; i < tables.size() && !tables.at(i)->isThisBookingName(name); i++ );
    if (i == tables.size()){
        return false;
    }
    return true;
}

int Coffeehouse::numberOfSeatsInTheHouse() const{
    const auto numberOfSeatsOfATable = [](int addToThis, const Table *table) {
        return addToThis + table->getNumberOfChairs();
    };
    return accumulate(tables.begin(), tables.end(), 0, numberOfSeatsOfATable);
}

int Coffeehouse::numberOfFreeTables() const{
    const auto addToCountIfFree = [](int addToThis, const Table *table) {
        return addToThis + (table->isAvailable() ? 1 : 0);
    };
    return accumulate(tables.begin(), tables.end(), 0, addToCountIfFree);
}

void Coffeehouse::updateMenus() {
    string menuString = counter.createMenuString();
    for (auto &table : tables) {
        table->updateMenu(menuString);
    }
}

void Coffeehouse::closeAndSaveLeftoversToFile(const double discount, const bool overrideTime) const {
    if (!overrideTime && counter.getClosingTime() >= duration_cast<milliseconds>(system_clock::now().time_since_epoch())){
        cout << "Sorry, the time to close has not yet come.";
        return;
    }
    if (abs(discount) >= 1){
        cout << "Discount cannot be larger than 1 (100%).";
        return;
    }
    if (!counter.saveLeftovers(abs(discount))){
        cout << "Saving failed, either couldn't create a file, or there were no leftovers.\n";
        cout << "Sorry for the inconvenience.";
    }

}

void Coffeehouse::saveInitialConfigurationToFiles(const milliseconds &openedFor, const vector<tableInfo> &tableTypes,
                                                 const vector<initialCountAndDetails> &initialStockWithPrices,
                                                 const bool &winterTime) {
    ofstream outFile("CoffeehouseParametersTables.txt", ios::trunc);
    if (outFile.fail()){
        cout << "Saving tables failed as a file couldn't be created.\n";
        cout << "Saving of the items was there fore not even attempted\nSorry for the inconvenience.";
        return;
    }
    outFile << tableTypes.size() << "\n";
    for (const tableInfo &table : tableTypes){
        outFile << table.getSize() << " " << table.getNumberOfTables() << " "<< table.getNumberOfChairs() << "\n";
    }
    outFile.close();
    outFile.open("CoffeehouseParametersItems.txt", ios::trunc);
    if (outFile.fail()){
        cout << "Tables were saved successfully.\n";
        cout << "Sadly, items were not as fortunate as a file for them couldn't be created.\n";
        cout << "Sorry for the inconvenience.";
        return;
    }
    outFile << initialStockWithPrices.size() << "\n";
    for (const initialCountAndDetails &item : initialStockWithPrices){
        outFile << item.getItemName() << " " << item.getCount() << " " << item.getPrice();
        outFile << " " << item.getTimeToMake().count() << " " << item.getMenuName() << "\n";
    }
}
