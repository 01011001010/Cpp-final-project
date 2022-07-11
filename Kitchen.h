#include <vector>
#include <map>
#include <chrono>

#include "Order.h"

#include <gtest/gtest_prod.h>

#pragma once

using namespace std;
using namespace chrono;

class Recipe {
    milliseconds timeToBake;
    unsigned int batchSize;

public:
    inline explicit Recipe(const initialCountAndDetails &initialStockForItem)
            : batchSize(initialStockForItem.getCount()),
              timeToBake(initialStockForItem.getTimeToMake()) {};

    inline milliseconds getTimeToBake() const { return timeToBake; }
    inline unsigned int getBatchSize() const { return batchSize; }
};

class Kitchen {
    map<ItemName, Recipe> recipes;

    FRIEND_TEST(Part2_NoBakeryIsCompleteWithoutABaker, WeBakeOnlyCakes);

public:
    explicit Kitchen(const vector<initialCountAndDetails> &initialStock);
    // saves recipes for cakes specified in the initial stock vector

    pair<const milliseconds, const unsigned int> bakeACake(ItemName cake, milliseconds timeTillClosing,
                                                           unsigned int currentStock) const;
    // if the current stock is less than one third of a batch size, the cakes are baked
    // the return pair consist of the availability time and the count of baked cakes
    //
    // if what was asked for is not a cake, is not on today's menu or the time till
    // closing is less than 1.5 times the time to it takes to bake the cake
    // the return pair consist of the invalidOrder time (0) and the count of baked cakes,
    // which is 0
};
