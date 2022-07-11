#include <iostream>
#include <sstream>
#include <iomanip>

#include "Table.h"

using namespace std;


Table::Table(const TableSize &type, const short chairs) : Table(type) {
    setNumberOfChairs(chairs);
}

void Table::setNumberOfChairs(const short chairs) {
    if (chairs > 0 && chairs <= tableType) {
        numberOfChairs = chairs;
    }
}

bool Table::seatGuests(const short numberOfGuests) {
    if (isAvailable() && numberOfGuests > 0 && getNumberOfChairs() >= numberOfGuests) {
        tableStatus = OCCUPIED;
        return true;
    }
    return false;
}

bool Table::bookForName(const string &nameToBookFor, const short numberOfGuests) {
    if (isAvailable() && numberOfGuests > 0 && getNumberOfChairs() >= numberOfGuests &&
        !nameToBookFor.empty()) {

        tableStatus = RESERVED;
        bookingName = nameToBookFor;
        return true;
    }
    return false;
}

bool Table::cancelBooking(const string &name) {
    if (isReserved() && bookingName == name) {
        tableStatus = AVAILABLE;
        bookingName.clear();
        return true;
    }
    return false;
}

bool Table::seatBookedGuests(const string &name, const short numberOfGuests) {
    if (isReserved() && bookingName == name && numberOfGuests > 0 &&
        getNumberOfChairs() >= numberOfGuests) {

        tableStatus = OCCUPIED;
        bookingName.clear();
        return true;
    }
    return false;
}

void Table::addToBill(const Order &order) {
    if (isOccupied()){
        bill.push_back(order);
    }
}

string Table::pay() {
    if (!isOccupied() || bill.empty()) {
        return "";
    }
    ostringstream receipt;
    unsigned int total = 0;
    receipt << "\nItem:";
    for (const Order &order : bill) {

        string toPad =  order.getName().substr(0,15);
        toPad.append(17-toPad.length(), ' ');

        receipt << '\n' << toPad << setw(2) << order.getNumber() << " *";
        receipt << setw(3) << order.getUnitPrice()/100 << "." ;
        receipt << setfill('0') << setw(2) << order.getUnitPrice()%100 << " Eur" << setfill(' ');

        unsigned int itemTotal = order.getUnitPrice()*order.getNumber();
        total += itemTotal;

        receipt << " | " << setw(4) << itemTotal/100 << "." ;
        receipt << setfill('0') << setw(2) << itemTotal%100 << " Eur" << setfill(' ');
    }
    receipt << '\n' << setfill('-') << setw(45) << "" << setfill(' ');
    receipt << '\n' << "Total" << setw(33) << total/100 << ".";
    receipt << setfill('0') << setw(2) << total%100 << " Eur" << setfill(' ');

    bill.clear();
    return receipt.str();
}

bool Table::theGuestsWantToLeave() {
    if (isOccupied() && bill.empty()){
        tableStatus = DIRTY;
        return true;
    }
    return false;
}

bool Table::clean() {
    if (isDirty()){
        tableStatus = AVAILABLE;
        return true;
    }
    return false;
}

void Table::showMenu() const {
    cout << currentMenu;
}

void Table::updateMenu(const string &menu) {
    currentMenu = menu;
}
