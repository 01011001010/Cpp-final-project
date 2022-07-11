#include "Kitchen.h"

Kitchen::Kitchen(const vector<initialCountAndDetails> &initialStock) {
    for (const initialCountAndDetails &item : initialStock) {
        if (item.getType() == CAKE){
            recipes.insert({item.getItemName(), Recipe(item)});
        }
    }
}

pair<const milliseconds, const unsigned int> Kitchen::bakeACake(const ItemName cake, const milliseconds timeTillClosing,
                                                                const unsigned int currentStock) const {
    if (recipes.find(cake) != recipes.end() &&
        (recipes.at(cake).getTimeToBake() * 1.5) <= timeTillClosing &&
        (recipes.at(cake).getBatchSize() * 0.3) >= currentStock){
        milliseconds timeWhenReady = duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                                     + recipes.at(cake).getTimeToBake();
        return pair<const milliseconds, const unsigned int> {timeWhenReady, recipes.at(cake).getBatchSize()};
    }
    return pair<const milliseconds, const unsigned int> {invalidOrder, 0};
}
