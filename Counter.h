#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <chrono>

#include "Order.h"
#include "Kitchen.h"
#include "fileComprehensionFunctions.h"

#include <gtest/gtest_prod.h>

#pragma once

using namespace std;
using namespace chrono;

class itemStockQueue {
    deque<milliseconds> stockQueue;

public:
    inline unsigned int InStock() const { return stockQueue.size(); };
    // returns the number of items in stock (physically and those promised by the kitchen alike)
    unsigned int currentlyInStock() const;
    // returns the number of items presently in stock
    milliseconds availability() const;
    // returns the time when the first in line has/will become available
    // as the items are in reality sorted by their availability, there
    // is no item in stock with availability smaller than the first's
    bool getItem(bool overrideAvailability=false);
    // removes the item from the queue if it is present at the time of this request
    // this can be, however, overridden
    // the return value states whether an item was successfully removed
    void addItem(milliseconds timeWhenAvailable);
    // adds an item to the end of the queue, but only if it's timestamp
    // is larger than that of the last added item
};

class todaysMenu {
    unsigned long cakeOffset;
    unsigned long priceOffset;
    map<ItemName, string> menuNames;
    map<ItemName, unsigned int> prices;
    set<ItemName> leftovers;
    set<ItemName> cakes;
    map<ItemName, milliseconds> drinks;

    static vector<ItemName> listMapKeys(const map<ItemName, milliseconds> *theMap) ;
    //creates a list of keys from the provided map

public:
    explicit todaysMenu(const vector<initialCountAndDetails> &initialStock, bool readLeftoversFromFile=true);
    // initializes the menu from the initialStock vector
    // if readLeftoversFromFile is true, the leftovers in the initialStock are
    // ignored and only those from the leftover file are added (if any)

    inline void removeLeftoverFromOffer(ItemName cake) { leftovers.erase(cake); };
    // if there is such a leftover, it's erased from the list of leftovers
    // if there is some left in stock, it can still be sold, it's price is still
    // in the prices map the only effect this has, is when someone asks what
    // leftovers are offered, the erased one will not be listed,
    // e.g. will show on the menu
    milliseconds getTimeToPrepare(ItemName drink) const;
    // returns the time it takes to prepare said drink
    // if not a drink invalidOrder is returned
    vector<ItemName> listAllOfType(ItemType type) const;
    // lists all items of said type
    inline unsigned int getPriceOffset() const { return priceOffset; };
    // returns the precalculated length needed to offset the price of the items
    // on the menu so that no item has name longer than the space provided
    inline unsigned int getCakeOffset() const { return cakeOffset; };
    // returns the precalculated length needed to offset the availability time
    // of the items on the menu so that no item has name longer than the space
    // provided
    inline unsigned int getPrice(const ItemName item) const { return onTheMenu(item) ? prices.at(item) : 0; };
    // if there is such item in the menu, it's price is returned, otherwise 0
    inline string getName(const ItemName item) const { return onTheMenu(item) ? menuNames.at(item) : ""; };
    // if there is such item in the menu, it's name is returned, otherwise an empty string
    inline bool onTheMenu(const ItemName item) const { return prices.find(item) != prices.end(); };
    // if there is such item in the menu, returns true, otherwise false
    bool typeMatchesItem(ItemName item, ItemType type) const;
    // determines if the item is of the specified type
    // if the item is not in the menu, returns false regardless of its type
};

class Counter {
    Kitchen kitchen;
    todaysMenu nowOffering;
    map<ItemName, itemStockQueue *> stock;
    milliseconds baristaFreeFrom;
    milliseconds waiterFreeFrom;
    milliseconds closingTime;
    bool winterTime;

    string timeFromMSSinceTheEpoch(const milliseconds &ms) const;
    // returns a time string in the format hh:mm:ss.msms based on the time from the epoch
    // if it's winter time, an hour is added for compensation

    FRIEND_TEST(Part3_TheBrains, SavingLeftoversAndReadingThemAutomatically);

public:
    Counter(const vector<initialCountAndDetails> &initialStock, milliseconds openedFor,
                     bool readLeftoversFromFile=true, bool winterTime=false);
    // notes the time of opening
    // creates the kitchen, full stock the menu according to specification
    // if readLeftoversFromFile is true, the leftovers in the initialStock are
    // ignored and only those from the leftover file are added (if any)
    ~Counter();
    // deletes each dynamic itemStockQueue in the stock

    inline milliseconds itemAvailability(ItemName item) const {
        return ((nowOffering.onTheMenu(item) &&
                (nowOffering.typeMatchesItem(item, CAKE) ||
                 nowOffering.typeMatchesItem(item, LEFTOVERCAKE)))) ? stock.at(item)->availability() : invalidOrder; };
    inline unsigned int itemPrice(ItemName item) const { return nowOffering.getPrice(item); };
    inline string itemName(ItemName item) const { return nowOffering.getName(item); };
    inline milliseconds getClosingTime() const { return closingTime; };

    milliseconds prepareCake(Order &order);
    // if the order came after the closing time, isn't sensible (there is
    // no such cake in the menu) or there is not enough in the stock,
    // it will be discarded (invalidOrder is returned)
    //
    // (unlike with drinks, the order is fulfilled even if the waiter
    // will start fulfilling it after the closing time, as long as the
    // order came before the closing time)
    //
    // otherwise, the time when the staff finishes preparing the order is
    // returned (the time is noted in the waiterFreeFrom)
    //
    // afterwards a refill is ordered from the kitchen
    // (the kitchen will decide if it's not too late for one
    // and if there is not enough in stock without a refill)
    milliseconds prepareDrink(Order &order);
    // if the order came after the closing time (or the barista is already busy
    // until the closing time), isn't sensible (there is no such drink in the
    // menu) or the number ordered is more than 20 (special orders above 20,
    // have to be prearranged and are not part of this system),
    // it will be discarded (invalidOrder is returned)
    //
    // otherwise, the time when the staff finishes preparing the order is
    // returned (the time is noted in the baristaFreeFrom)

    string createMenuString() const;
    // based on the offer and the stock a menu string is created
    void orderRefill(ItemName cake);
    // if it's not after the closing time, a refill is ordered from the kitchen
    // and the cakes that came back from the kitchen are added to the stock
    bool saveLeftovers(double discount) const;
    // moves all the cakes to the leftover file with the specified discount
    // (rounded down to 10 cents)
    // if there were a leftover file, it's overwritten
    // the return value states if there were any cakes successfully saved
};
