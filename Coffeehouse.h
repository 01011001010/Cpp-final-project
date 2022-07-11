#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include "Order.h"
#include "Table.h"
#include "Counter.h"
#include "fileComprehensionFunctions.h"

#include <gtest/gtest_prod.h>

#pragma once

using namespace std;
using namespace chrono;


class Coffeehouse {
    vector<Table *> tables;
    Counter counter;
    ostringstream receiptPrinter;

    static void saveInitialConfigurationToFiles(const milliseconds &openedFor, const vector<tableInfo> &tableTypes,
                                         const vector<initialCountAndDetails> &initialStockWithPrices,
                                         const bool &winterTime);

    void updateMenus();
    //gets menu string from counter and passes it to each table

    FRIEND_TEST(Part4_OnceAgainAllAtOnce, HastagSaveTheTrees);
    FRIEND_TEST(Part4_OnceAgainAllAtOnce, ADayLikeAnyOther);

public:
    Coffeehouse(milliseconds openedFor, vector<tableInfo> tableTypes,
                const vector<initialCountAndDetails> &initialStockWithPrices,
                bool readLeftoversFromFile=true, bool winterTime=false,
                bool saveConfiguration=false);
    // creates the specified number of tables with specified number of chairs
    // (only tables that do have chairs are added to the vector of tables)
    // there is always a table with index 0 that is a virtual one for takeout and has no chairs
    //
    // the vector of tables is sorted according to the number of seats of each table,
    // so when looking for a free table, the first free with enough seats is the optimal one
    //
    // creates the counter according to specification
    //
    // sends menu string to each table
    //
    // if asked, saves the initial table and counter specifications to files

    ~Coffeehouse();
    // deletes all the tables as they are dynamically allocated

    bool placeOrder(unsigned int tableNumber, ItemName item, unsigned int numberOf);
    // order stands only if the table is occupied
    // if specified items cannot be ordered (the counter returned invalidOrder),
    // false is returned
    //
    // if specified items can be ordered (the counter did not return invalidOrder),
    // the order is added to the bill of the table and true is returned
    //
    // if as a result, an item just changed its availability, the menu is updated
    // and sent to each table

    bool seatGuests(short numberOfGuests);
    // if there is an empty table that can seat the given number of guests, seat them
    // return value is true it they were successfully seated
    bool bookATable(const string &nameToBookFor, short numberOfGuests);
    // similar to seatGuests, book a table if an appropriate one is available
    bool seatBookedGuests(const string &name, short numberOfGuests);
    // the name is self-explanatory
    // if there is no such booking or, the number of arrived guests does not fit the
    // reserved table, seat them anyway (if there is another free table for them)
    // if they were seated to a different table, the original reservation is canceled,
    // if not (and there really was a reservation), the reservation still stands
    bool nameAlreadyHasABooking(const string &name) const;
    // checks if the name has a booked table
    int numberOfSeatsInTheHouse() const;
    // returns the total number of chairs
    int numberOfFreeTables() const;
    // returns the number of free(available) tables

    void checkPlease(unsigned int tableNumber);
    // close the tab for the table (create a receipt and clear the bill)
    // the guests do not have to leave though
    void finalPaymentAndDeparture(unsigned int tableNumber);
    // if the table has an open tab, they pay
    // then the guests leave and the busboy is called to clean it (which he does)
    //
    // in case it's virtual takeout table (table 0), it is just calling checkPlease,
    // the table stays occupied, so order can be still placed

    void closeAndSaveLeftoversToFile(double discount=.3, bool overrideTime=false) const;
    // if it's after closing time(or this is overridden), asks the counter to save
    // leftovers to file
    // only fresh cakes are saved and do get a specified discount
};

