#include <string>
#include <deque>

#include "Order.h"

#pragma once

using namespace std;

enum TableStatus {
    AVAILABLE, RESERVED, OCCUPIED, DIRTY
};
enum TableSize {
    DOUBLE = 2, QUADRUPLE = 4, SEXTUPLE = 6, OCTUPLE = 8
};

class Table {
    TableStatus tableStatus = AVAILABLE;
    string bookingName;
    TableSize tableType;
    short numberOfChairs = 0;
    deque<Order> bill;
    string currentMenu;

    void setNumberOfChairs(short chairs);
    //if that many chairs do not fit the table, nothing changes
public:
    inline explicit Table(const bool virtualTakeoutTable=false)
        : tableType(DOUBLE), tableStatus(virtualTakeoutTable ? OCCUPIED : AVAILABLE) {};
    inline explicit Table(const TableSize &type) : tableType(type) {};
    Table(const TableSize &type, short chairs);

    inline short getNumberOfChairs() const { return numberOfChairs; };
    inline bool isAvailable() const { return tableStatus == AVAILABLE; };
    inline bool isReserved() const { return tableStatus == RESERVED; };
    inline bool isOccupied() const { return tableStatus == OCCUPIED; };
    inline bool isDirty() const { return tableStatus == DIRTY; };
    inline bool isThisBookingName(const string &name) const { return name == bookingName && !name.empty(); }

    bool seatGuests(short numberOfGuests);
    // seats said guests if they fit and the table is available
    // the return value states whether the guests were successfully seated
    bool bookForName(const string &nameToBookFor, short numberOfGuests);
    // if the given name is not empty, the guests fit and the table is available,
    // it's booked for the given name
    // the return value states whether the reservation was successful
    bool cancelBooking(const string &name);
    // if the table was booked for the name, the reservation is removed
    bool seatBookedGuests(const string &name, short numberOfGuests);
    // if the reservation name is a match and the number of guests fit the table,
    // they are seated
    // the return value states whether the guests were successfully seated

    void addToBill(const Order &order);
    // if the table is occupied, the items from the order are added to the bill
    string pay();
    // if the table is occupied and there are items in the bill, a receipt is
    // created and the bill is cleared
    // the return value is a receipt string, or an empty string if there was
    // nothing in the bill
    bool theGuestsWantToLeave();
    // if the table is occupied and the bill has been paid (or the guests did not
    // order anything), the table is set to dirty state and true is returned
    // otherwise the return value is false
    bool clean();
    // if the table is cleaned, it's cleaned
    // the return value states whether the table was cleaned

    void showMenu() const;
    // displays current menu
    void updateMenu(const string &menu);
    // replaces the menu string
};
