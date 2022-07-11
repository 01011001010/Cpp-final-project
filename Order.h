#include <string>
#include <chrono>
#include <utility>

#pragma once

using namespace std;
using namespace chrono;

const milliseconds invalidOrder(0); //the return value if unable to fulfill an order

enum ItemName {
    //WARNING! when adding items, keep the first and the last items of any line as they are and only add between them
    // also, for any added cake, add a leftover version to the second line and keep the order of the first line
    // consistent with the order of the second line
    REDVELVET,      VICTORIASPONGE,  MACARONS,  MUFFINS,  CROISSANTS,  MONTBLANC,  PARFAIT,  RVCUPCAKE,  VANCUPCAKE,
    LREDVELVET=100, LVICTORIASPONGE, LMACARONS, LMUFFINS, LCROISSANTS, LMONTBLANC, LPARFAIT, LRVCUPCAKE, LVANCUPCAKE,

    ESPRESSO=200, LUNGO, CAPPUCCINO, LEMONADE, SODA
};
enum ItemType {
    CAKE, LEFTOVERCAKE, DRINK
};

class initialCountAndDetails {
    string menuName;
    ItemName item;
    unsigned int count; // disregarded for drinks
    unsigned int price;
    milliseconds timeToBakeOrBrew;

public:
    inline explicit initialCountAndDetails(const ItemName item, const unsigned int count=8,
                                           string menuName="", const unsigned int price=280,
                                           const unsigned int timeToMake=10)
            : menuName(std::move(menuName)), count(count == 0 ? 8 : count), price(price),
              item(item), timeToBakeOrBrew(milliseconds(timeToMake)) {};

    inline ItemType getType() const { return ItemType(item/100); }
    inline ItemName getItemName() const { return item; };
    inline string getMenuName() const { return menuName; }
    inline unsigned int getCount() const { return count; }
    inline unsigned int getPrice() const { return price; }
    inline milliseconds getTimeToMake() const { return timeToBakeOrBrew; }
};

class Order {
    const ItemName item;
    const unsigned int numberOf;
    const unsigned int unitPrice;
    const string name;

public:
    inline Order(ItemName item, unsigned int numberOf, unsigned int unitPrice, string name)
            : item(item), numberOf(numberOf), unitPrice(unitPrice), name(move(name)) {};
    inline ItemType getType() const { return ItemType(item/100); };
    inline ItemName getItem() const { return item; };
    inline unsigned int getNumber() const { return numberOf; }
    inline unsigned int getUnitPrice() const { return unitPrice; }
    inline string getName() const { return name; }
};
