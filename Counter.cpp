#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "Counter.h"

unsigned int itemStockQueue::currentlyInStock() const {
    milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    unsigned int count = 0;
    for (int i = 0; i < stockQueue.size() && stockQueue.at(i) < currentTime ; ++i, ++count);
    return count;
}

milliseconds itemStockQueue::availability() const {
    if (stockQueue.empty()){
        return invalidOrder;
    }
    return stockQueue.front();
}

bool itemStockQueue::getItem(const bool overrideAvailability) {
    milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    if (!stockQueue.empty() && (stockQueue.at(0) < currentTime || overrideAvailability)){
        stockQueue.pop_front();
        return true;
    }
    return false;
}

void itemStockQueue::addItem(milliseconds timeWhenAvailable) {
    if (stockQueue.empty() || stockQueue.back() <= timeWhenAvailable){
        stockQueue.push_back(timeWhenAvailable);
    }
}

todaysMenu::todaysMenu(const vector<initialCountAndDetails> &initialStock, bool readLeftoversFromFile) {
    unsigned long longestCakeName = 0;
    unsigned long longestAnything = 0;
    for (const initialCountAndDetails &item : initialStock) {
        if (!item.getMenuName().empty() && (item.getType() != LEFTOVERCAKE || !readLeftoversFromFile)) {
            menuNames[item.getItemName()] = item.getMenuName();
            prices[item.getItemName()] = item.getPrice();
            if (menuNames[item.getItemName()].length() > longestAnything) {
                longestAnything = menuNames[item.getItemName()].length();
            }
            switch (item.getType()) {
                case LEFTOVERCAKE:
                    leftovers.insert(item.getItemName());
                    break;
                case CAKE:
                    if (item.getMenuName().length() > longestCakeName) {
                        longestCakeName = item.getMenuName().length();
                    }
                    cakes.insert(item.getItemName());
                    break;
                case DRINK:
                    drinks[item.getItemName()] = item.getTimeToMake();
                    break;
                default:
                    break;
            }
        }
    }
    if (readLeftoversFromFile){
        vector<initialCountAndDetails> leftoversFromFile;
        if (appendItemsFromFile(leftoversFromFile, "CoffeehouseLeftovers.txt")){
            for (const initialCountAndDetails &item : leftoversFromFile) {
                if (!item.getMenuName().empty()) {
                    menuNames[item.getItemName()] = item.getMenuName();
                    prices[item.getItemName()] = item.getPrice();
                    if (menuNames[item.getItemName()].length() > longestAnything) {
                        longestAnything = menuNames[item.getItemName()].length();
                    }
                    leftovers.insert(item.getItemName());
                }
            }
        }
    }

    longestCakeName += 10;
    longestAnything += 7;
    cakeOffset = longestCakeName > 30 ? longestCakeName : 30;
    priceOffset = longestAnything > 50 ? longestAnything : 50;

}

milliseconds todaysMenu::getTimeToPrepare(const ItemName drink) const {
     if (drinks.find(drink) != drinks.end()){
        return drinks.at(drink);
    }
    return invalidOrder;
}

vector<ItemName> todaysMenu::listAllOfType(const ItemType type) const {
    switch (type) {
        case LEFTOVERCAKE:
            return vector<ItemName> (leftovers.begin(), leftovers.end());
        case CAKE:
            return vector<ItemName> (cakes.begin(), cakes.end());
        case DRINK:
            return listMapKeys(&drinks);
        default:
            return {};
    }
}

vector<ItemName> todaysMenu::listMapKeys(const map<ItemName, milliseconds> *theMap) {
    vector<ItemName> returnVector;
    if (theMap != nullptr) {
        for (const auto pair: *theMap) {
            returnVector.push_back(pair.first);
        }
    }
    return returnVector;
}

bool todaysMenu::typeMatchesItem(const ItemName item, const ItemType type) const {
    switch (type) {
        case LEFTOVERCAKE:
            return leftovers.find(item) != leftovers.end();
        case CAKE:
            return cakes.find(item) != cakes.end();
        case DRINK:
            return drinks.find(item) != drinks.end();
        default:
            return false;
    }
}

Counter::Counter(const vector<initialCountAndDetails> &initialStock, const milliseconds openedFor,
                 const bool readLeftoversFromFile, const bool winterTime)
        : kitchen(initialStock), nowOffering(initialStock, readLeftoversFromFile), winterTime(winterTime),
          waiterFreeFrom(), baristaFreeFrom(), closingTime() {
    milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    waiterFreeFrom = currentTime;
    baristaFreeFrom = currentTime;
    closingTime = currentTime + openedFor;
    for (const initialCountAndDetails &item : initialStock) {
        if (item.getType() == CAKE || (item.getType() == LEFTOVERCAKE && !readLeftoversFromFile)){
            auto *itemQueue = new itemStockQueue();
            for (auto i = 0; i < item.getCount(); ++i) {
                itemQueue->addItem(currentTime);
            }
            stock[item.getItemName()] = itemQueue;
        }
    }
    if (readLeftoversFromFile){
        vector<initialCountAndDetails> leftovers;
        if (appendItemsFromFile(leftovers, "CoffeehouseLeftovers.txt")){
            for (const initialCountAndDetails &item : leftovers) {
                if (item.getType() == LEFTOVERCAKE){
                    auto *itemQueue = new itemStockQueue();
                    for (auto i = 0; i < item.getCount(); ++i) {
                        itemQueue->addItem(currentTime);
                    }
                    stock[item.getItemName()] = itemQueue;
                }
            }
        }
    }
}

Counter::~Counter() {
    for (auto &mapPair : stock) {
        delete mapPair.second;
    }
    stock.clear();
}

milliseconds Counter::prepareCake(Order &order) {
    const milliseconds timeToPrepareACake(1);

    milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    if (stock.find(order.getItem()) == stock.end() ||
        currentTime >= closingTime ||
        order.getNumber() > stock.at(order.getItem())->currentlyInStock()){
        return invalidOrder;
    }
    for (int i = 0; i < order.getNumber(); ++i) {
        stock.at(order.getItem())->getItem();
    }
    waiterFreeFrom = (currentTime > waiterFreeFrom ? currentTime : waiterFreeFrom) + order.getNumber() * timeToPrepareACake;
    if (nowOffering.typeMatchesItem(order.getItem(), LEFTOVERCAKE)){
        if (stock[order.getItem()]->InStock() == 0){
            nowOffering.removeLeftoverFromOffer(order.getItem());
        }
    }else{
        orderRefill(order.getItem());
    }
    return waiterFreeFrom;
}

milliseconds Counter::prepareDrink(Order &order) {
    milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    if (order.getNumber() > 20 ||
        currentTime >= closingTime ||
        baristaFreeFrom >= closingTime ||
        order.getType() != DRINK ||
        !nowOffering.typeMatchesItem(order.getItem(), DRINK)){
        return invalidOrder;
    }
    baristaFreeFrom = (currentTime > baristaFreeFrom ? currentTime : baristaFreeFrom)
                      + order.getNumber() * nowOffering.getTimeToPrepare(order.getItem());
    return baristaFreeFrom;
}

string Counter::createMenuString() const {
    ostringstream menuStream;
    string header = "Current offer:\n";
    menuStream << "\n" << setw((nowOffering.getPriceOffset()-header.length())/2) << "" << header;

    auto itemOffsetLength = 3;
    vector<pair<ItemType, string>> typePairs {{DRINK, "\nDrinks:"},
                                              {LEFTOVERCAKE, "\nYesterday's cakes with a discount"}};
    for (const auto &typePair: typePairs) {
        auto listOfType = nowOffering.listAllOfType(typePair.first);
        if (!listOfType.empty()){
            menuStream << typePair.second;
            for (const ItemName &item : listOfType){
                menuStream << "\n" << setw(itemOffsetLength) << "" << nowOffering.getName(item);
                menuStream << setw(nowOffering.getPriceOffset()-itemOffsetLength-nowOffering.getName(item).length()) << "";
                menuStream << setw(3) << nowOffering.getPrice(item)/100 << ".";
                menuStream << setfill('0') << setw(2) << nowOffering.getPrice(item)%100 << " Eur" << setfill(' ');
            }
            menuStream << "\n";
        }
    }

    auto listOfType = nowOffering.listAllOfType(CAKE);
    if (!listOfType.empty()){
        string title = "\nDesserts:";
        menuStream << title << setw(nowOffering.getCakeOffset()-title.length()-1) << "" << "freshly baked at:";
        bool noDisplayedCakes = true;
        for (const ItemName &item : listOfType){
            if (stock.at(item)->InStock() != 0){
                noDisplayedCakes = false;
                menuStream << "\n" << setw(itemOffsetLength) << "" << nowOffering.getName(item);
                menuStream << setw(nowOffering.getCakeOffset()-itemOffsetLength-nowOffering.getName(item).length()) << "";
                menuStream << setw(15) << timeFromMSSinceTheEpoch(stock.at(item)->availability());
                menuStream << setw(nowOffering.getPriceOffset()-nowOffering.getCakeOffset()-15) << "";
                menuStream << setw(3) << nowOffering.getPrice(item)/100 << "." ;
                menuStream << setfill('0') << setw(2) << nowOffering.getPrice(item)%100 << " Eur" << setfill(' ');
            }
        }
        if (noDisplayedCakes){
            menuStream << "\n" << setw(itemOffsetLength) << "" << "Sadly we are sold out for the day :(";
        }
        menuStream << "\n";
    }
    return menuStream.str();
}

void Counter::orderRefill(ItemName cake) {
    milliseconds currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    if (closingTime > currentTime &&
            stock.find(cake) != stock.end() &&
            nowOffering.typeMatchesItem(cake,CAKE)){
        auto bakedCake = kitchen.bakeACake(cake, closingTime-currentTime, stock[cake]->InStock());
        for (int i = 0; i < bakedCake.second; ++i) {
            stock.at(cake)->addItem(bakedCake.first);
        }
    }
}

bool Counter::saveLeftovers(const double discount) const {
    ostringstream toWrite;
    int count = 0;
    for (auto &item : stock){
        if (item.first/100 == 0 && item.second->InStock()){
            toWrite << item.first+100 << " " << item.second->InStock() << " ";
            toWrite<< int(nowOffering.getPrice(item.first)*(1-discount)/10)*10 << " 0 ";
            toWrite << nowOffering.getName(item.first) << '\n';
            count++;
            while (item.second->getItem(true));
        }
    }
    if (count == 0){
        return false;
    }
    ofstream outFile("CoffeehouseLeftovers.txt", ios::trunc);
    if (outFile.fail()){
        return false;
    }
    outFile << count << '\n';
    outFile << toWrite.str();
    outFile.close();
    return true;
}

string Counter::timeFromMSSinceTheEpoch(const milliseconds &ms) const {
    ostringstream timeStream;
    milliseconds justTime = ms%(24*60*60*1000);
    timeStream << setfill('0');
    timeStream << setw(2) << (justTime/(60*60*1000) + (winterTime ? milliseconds(1) : milliseconds(0))).count() << ':';
    timeStream << setw(2) << (justTime/(60*1000)%60).count() << ':';
    timeStream << setw(2) << (justTime/(1000)%60).count() << '.';
    timeStream << setw(3) << (justTime%1000).count();
    return timeStream.str();
}
