// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include <thread>
#include "gtest/gtest.h"

using namespace std;
using namespace this_thread;

#include "Coffeehouse.h"

using namespace ::testing;

// part1
TEST(Part1_OfTablesAndChairs, WhereTheHellAreTheChairs) {
	Table table1;
    ASSERT_EQ(0, table1.getNumberOfChairs());

    Table table2(OCTUPLE);
    ASSERT_EQ(0, table2.getNumberOfChairs());

    Table table3(DOUBLE,4);
    ASSERT_EQ(0, table3.getNumberOfChairs());

    Table table4(OCTUPLE,-5);
    ASSERT_EQ(0, table4.getNumberOfChairs());
}

TEST(Part1_OfTablesAndChairs, OhThereTheyAre) {
    Table table1(DOUBLE,1);
    ASSERT_EQ(1, table1.getNumberOfChairs());

    Table table2(OCTUPLE, 1);
    ASSERT_EQ(1, table2.getNumberOfChairs());

    Table table3(DOUBLE,1);
    ASSERT_EQ(1, table3.getNumberOfChairs());

    Table table4(OCTUPLE,8);
    ASSERT_EQ(8, table4.getNumberOfChairs());
}
TEST(Part1_OfTablesAndChairs, VirtualOccupiedTable) {
    Table table(true);
    ASSERT_EQ(0, table.getNumberOfChairs());
    ASSERT_TRUE(table.isOccupied());
}
TEST(Part1_OfTablesAndChairs, MenuDisplayStatusNominal) {
    Table table;
    string testMenuString = "Just\nTesting";
    table.updateMenu(testMenuString);
    internal::CaptureStdout();
    table.showMenu();
    ASSERT_EQ(testMenuString, internal::GetCapturedStdout());
}
TEST(Part1_OfTablesAndChairs, TheyAreNotJustIllusions) {
    Table table(QUADRUPLE, 4);
    ASSERT_TRUE(table.seatGuests(4));
}
TEST(Part1_OfTablesAndChairs, WeAreJustIllusions) {
    Table table(QUADRUPLE, 4);
    ASSERT_FALSE(table.seatGuests(-4));
}
TEST(Part1_OfTablesAndChairs, AWholeContubernium) {
    Table table1(QUADRUPLE, 4);
    ASSERT_FALSE(table1.seatGuests(8));
    Table table2(OCTUPLE,8);
    ASSERT_TRUE(table2.seatGuests(8));
}
TEST(Part1_OfTablesAndChairs, ABook) {
    Table table(QUADRUPLE, 4);
    string cpt = "Jean-Luc Picard";
    ASSERT_TRUE(table.bookForName(cpt, 4));
    ASSERT_TRUE(table.isThisBookingName(cpt));
}
TEST(Part1_OfTablesAndChairs, ATome) {
    Table table(QUADRUPLE, 4);
    string cpt = "Jean-Luc Picard";
    ASSERT_FALSE(table.bookForName(cpt, 1014));
}
TEST(Part1_OfTablesAndChairs, Who) {
    Table table(QUADRUPLE, 4);
    string noName = "";
    ASSERT_FALSE(table.bookForName(noName, 3));
}
TEST(Part1_OfTablesAndChairs, QWho) {
    Table table(QUADRUPLE, 4);
    string q = "Q";
    ASSERT_FALSE(table.bookForName(q, -1014));
}
TEST(Part1_OfTablesAndChairs, SoIndecisive) {
    Table table(SEXTUPLE, 6);
    ASSERT_TRUE(table.isAvailable());
    string q = "Q";
    ASSERT_TRUE(table.bookForName(q, 5));
    ASSERT_TRUE(table.isReserved());
    string queue = "Queue";
    ASSERT_FALSE(table.cancelBooking(queue));
    ASSERT_TRUE(table.cancelBooking(q));
    ASSERT_FALSE(table.cancelBooking(q));
    ASSERT_TRUE(table.isAvailable());
}
TEST(Part1_OfTablesAndChairs, ItsReallyHim) {
    Table table(SEXTUPLE, 6);
    ASSERT_TRUE(table.isAvailable());
    string q = "Q";
    ASSERT_TRUE(table.bookForName(q, 5));
    ASSERT_TRUE(table.isReserved());
    ASSERT_TRUE(table.seatBookedGuests(q,5));
    ASSERT_TRUE(table.isOccupied());
}
TEST(Part1_OfTablesAndChairs, TheLawsOfTheUniverse) {
    Table table(SEXTUPLE, 6);
    ASSERT_TRUE(table.isAvailable());
    string q = "Q";
    ASSERT_TRUE(table.bookForName(q, 5));
    ASSERT_FALSE(table.seatBookedGuests(q, -5));
    ASSERT_FALSE(table.seatBookedGuests(q, 15206));
    ASSERT_TRUE(table.isReserved());
}
TEST(Part1_OfTablesAndChairs, TenForward) {
    Table table(QUADRUPLE, 4);
    string lt = "Data";
    ASSERT_FALSE(table.theGuestsWantToLeave());
    ASSERT_TRUE(table.bookForName(lt, 2));
    ASSERT_TRUE(table.seatBookedGuests(lt, 2));
    ASSERT_TRUE(table.isOccupied());
    Order whiteRussian(SODA,2,430,"White Russian");
    table.addToBill(whiteRussian);
    ASSERT_FALSE(table.theGuestsWantToLeave());
    ASSERT_TRUE(table.isOccupied());
    ASSERT_FALSE(table.clean());
    ASSERT_EQ("\nItem:\nWhite Russian     2 *  4.30 Eur |    8.60 Eur\n---------------------------------------------\nTotal                                8.60 Eur",table.pay());
    ASSERT_TRUE(table.theGuestsWantToLeave());
    ASSERT_TRUE(table.isDirty());
    ASSERT_TRUE(table.clean());
    ASSERT_TRUE(table.isAvailable());
}
// part2
TEST(Part2_NoBakeryIsCompleteWithoutABaker, EngageTheAutopilot) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(DOUBLE,4,2));
    tables.push_back(tableInfo(QUADRUPLE,6,3));
    tables.push_back(tableInfo(SEXTUPLE,2,4));
    tables.push_back(tableInfo(SEXTUPLE,2,6));
    tables.push_back(tableInfo(OCTUPLE,6,7));

    vector<tableInfo> tablesFile;
    ASSERT_TRUE(appendTableTypesFromFile(tablesFile,"Tables1.txt"));
    ASSERT_EQ(tablesFile.size(), tables.size());
    for (auto i = 0; i < tables.size(); i++){
        ASSERT_EQ(tablesFile.at(i).getSize(), tables.at(i).getSize());
        ASSERT_EQ(tablesFile.at(i).getNumberOfTables(), tables.at(i).getNumberOfTables());
        ASSERT_EQ(tablesFile.at(i).getNumberOfChairs(), tables.at(i).getNumberOfChairs());
    }

    vector<initialCountAndDetails> stock;
    stock.push_back(initialCountAndDetails(RVCUPCAKE, 12, "Red Velvet Cupcakes", 190, 70));
    stock.push_back(initialCountAndDetails(VANCUPCAKE, 12, "Vanilla Cupcakes", 160, 55));
    stock.push_back(initialCountAndDetails(LVANCUPCAKE, 3, "Vanilla Cupcakes", 110, 1));
    stock.push_back(initialCountAndDetails(MONTBLANC, 6, "MontBlanc", 650, 120));
    stock.push_back(initialCountAndDetails(ESPRESSO, 0, "Espresso", 200, 1));
    stock.push_back(initialCountAndDetails(LUNGO, 0, "Lungo", 220, 1));
    stock.push_back(initialCountAndDetails(CAPPUCCINO, 0, "Cappuccino", 290, 2));
    stock.push_back(initialCountAndDetails(SODA, 0, "Various Soft Drinks", 150, 1));

    vector<initialCountAndDetails> stockFile;
    ASSERT_TRUE(appendItemsFromFile(stockFile, "Items1.txt"));
    ASSERT_EQ(stockFile.size(), stock.size());
    for (auto i = 0; i < stock.size(); i++){
        ASSERT_EQ(stockFile.at(i).getType(), stock.at(i).getType());
        ASSERT_EQ(stockFile.at(i).getItemName(), stock.at(i).getItemName());
        ASSERT_EQ(stockFile.at(i).getMenuName(), stock.at(i).getMenuName());
        ASSERT_EQ(stockFile.at(i).getCount(), stock.at(i).getCount());
        ASSERT_EQ(stockFile.at(i).getPrice(), stock.at(i).getPrice());
        ASSERT_EQ(stockFile.at(i).getTimeToMake(), stock.at(i).getTimeToMake());
    }
}
TEST(Part2_NoBakeryIsCompleteWithoutABaker, ThereAreRecipeCardsSoWrite) {
    initialCountAndDetails nanasBreadPudding(PARFAIT,18,"Spotted Dick",200,40);
    Recipe nanasSecretBreadPudding(nanasBreadPudding);
    ASSERT_EQ(milliseconds(40), nanasSecretBreadPudding.getTimeToBake());
    ASSERT_EQ(18, nanasSecretBreadPudding.getBatchSize());
}
TEST(Part2_NoBakeryIsCompleteWithoutABaker, WeBakeOnlyCakes) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    ASSERT_EQ(8, stock.size());
    Kitchen lowerDecks(stock);
    ASSERT_EQ(3, lowerDecks.recipes.size());
}
TEST(Part2_NoBakeryIsCompleteWithoutABaker, ItsTooLateForACake) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    ASSERT_EQ(8, stock.size());
    Kitchen lowerDecks(stock);
    pair<const milliseconds, const unsigned int> noCakeForYou {invalidOrder, 0};
    ASSERT_EQ(noCakeForYou, lowerDecks.bakeACake(MONTBLANC,milliseconds(20),0));
}
TEST(Part2_NoBakeryIsCompleteWithoutABaker, YouGluton) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    ASSERT_EQ(8, stock.size());
    Kitchen lowerDecks(stock);
    pair<const milliseconds, const unsigned int> noCakeForYou {invalidOrder, 0};
    ASSERT_EQ(noCakeForYou, lowerDecks.bakeACake(MONTBLANC,milliseconds(2550),50));
}
TEST(Part2_NoBakeryIsCompleteWithoutABaker, FinallyCake) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    ASSERT_EQ(8, stock.size());
    Kitchen lowerDecks(stock);
    auto cake = lowerDecks.bakeACake(MONTBLANC,milliseconds(2550),0);
    ASSERT_NE(invalidOrder, cake.first);
    ASSERT_EQ(stock.at(3).getCount(), cake.second);
}
// part3
TEST(Part3_TheBrains, HippocampusIsFunctioningWithinNormalParameters) {
    itemStockQueue itemQueue;
    ASSERT_EQ(0, itemQueue.InStock());
    itemQueue.addItem(milliseconds(50));
    itemQueue.addItem(milliseconds(5530));
    ASSERT_EQ(2, itemQueue.currentlyInStock());
    auto futTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch())+milliseconds(100);
    itemQueue.addItem(futTime);
    ASSERT_EQ(2, itemQueue.currentlyInStock());
    ASSERT_EQ(3, itemQueue.InStock());
    sleep_for(milliseconds(120));
    ASSERT_EQ(3, itemQueue.currentlyInStock());
    ASSERT_EQ(milliseconds(50), itemQueue.availability());
    itemQueue.addItem(milliseconds(5530));
    ASSERT_EQ(3, itemQueue.InStock());
    itemQueue.addItem(duration_cast<milliseconds>(system_clock::now().time_since_epoch())+milliseconds(1000*60*30));
    ASSERT_EQ(4, itemQueue.InStock());
    ASSERT_EQ(3, itemQueue.currentlyInStock());
    ASSERT_TRUE(itemQueue.getItem());
    ASSERT_EQ(milliseconds(5530), itemQueue.availability());
    ASSERT_EQ(3, itemQueue.InStock());
    ASSERT_EQ(2, itemQueue.currentlyInStock());
    ASSERT_TRUE(itemQueue.getItem());
    ASSERT_TRUE(itemQueue.getItem());
    ASSERT_FALSE(itemQueue.getItem());
    ASSERT_TRUE(itemQueue.getItem(true));
}
TEST(Part3_TheBrains, TemporalLobeIsFunctioningWithinNormalParameters) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    todaysMenu offer(stock, false);

    ASSERT_EQ(3, offer.listAllOfType(CAKE).size());
    ASSERT_EQ(1, offer.listAllOfType(LEFTOVERCAKE).size());
    ASSERT_EQ(4, offer.listAllOfType(DRINK).size());
    ASSERT_TRUE(offer.typeMatchesItem(SODA, DRINK));
    ASSERT_FALSE(offer.typeMatchesItem(LRVCUPCAKE, LEFTOVERCAKE));
    ASSERT_FALSE(offer.typeMatchesItem(CROISSANTS, CAKE));
    ASSERT_TRUE(offer.typeMatchesItem(MONTBLANC, CAKE));
    ASSERT_FALSE(offer.onTheMenu(CROISSANTS));
    ASSERT_TRUE(offer.onTheMenu(LUNGO));
    ASSERT_EQ(offer.getName(LVANCUPCAKE), offer.getName(VANCUPCAKE));
    ASSERT_TRUE(offer.getName(MACARONS).empty());
    ASSERT_EQ(160, offer.getPrice(VANCUPCAKE));
    ASSERT_EQ(0, offer.getPrice(VICTORIASPONGE));
    ASSERT_EQ(offer.getTimeToPrepare(ESPRESSO), offer.getTimeToPrepare(LUNGO));
    ASSERT_EQ(milliseconds(0), offer.getTimeToPrepare(VICTORIASPONGE));
    offer.removeLeftoverFromOffer(VANCUPCAKE);
    ASSERT_EQ(1, offer.listAllOfType(LEFTOVERCAKE).size());
    offer.removeLeftoverFromOffer(LVANCUPCAKE);
    ASSERT_EQ(0, offer.listAllOfType(LEFTOVERCAKE).size());
    ASSERT_TRUE(offer.onTheMenu(LVANCUPCAKE));
}
TEST(Part3_TheBrains, SavingLeftoversAndReadingThemAutomatically) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Counter counter(stock, milliseconds(100000), false, true);
    sleep_for(nanoseconds(100));
    Order cupcake(VANCUPCAKE,1,160,"Vanilla Cupcakes");
    ASSERT_NE(invalidOrder, counter.prepareCake(cupcake));
    ASSERT_TRUE(counter.saveLeftovers(.3));
    ASSERT_EQ(invalidOrder, counter.prepareCake(cupcake));
    Order oldCupcake(LVANCUPCAKE,1,110,"Vanilla Cupcakes");
    ASSERT_NE(invalidOrder, counter.prepareCake(oldCupcake));

    Counter counter2(stock, milliseconds(1000), true, true);
    sleep_for(nanoseconds(100));
    ASSERT_NE(counter2.stock.find(LVANCUPCAKE),counter2.stock.end());
    ASSERT_EQ(11, counter2.stock.at(LVANCUPCAKE)->InStock());
}
TEST(Part3_TheBrains, SorryTheBakerHasRonaTodayItsJustDrinksAndLeftovers) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Counter yesterdayAllMyTroublesSeemedSoFarAway(stock, milliseconds(1));
    sleep_for(nanoseconds(100));
    ASSERT_TRUE(yesterdayAllMyTroublesSeemedSoFarAway.saveLeftovers(.3));

    vector<initialCountAndDetails> stock2;
    ASSERT_TRUE(appendItemsFromFile(stock2, "Items2.txt"));
    Counter thingsWeSaidToday(stock2, milliseconds(1000), true, true);
    sleep_for(nanoseconds(100));
    string menu1 = "\n                 Current offer:\n\nDrinks:\n   Espresso                                         2.00 Eur\n   Lungo                                            2.20 Eur\n   Cappuccino                                       2.90 Eur\n   Homemade Lemonade                                2.50 Eur\n   Various Soft Drinks                              1.50 Eur\n\nYesterday's cakes with a discount\n   MontBlanc                                        4.50 Eur\n   Red Velvet Cupcakes                              1.30 Eur\n   Vanilla Cupcakes                                 1.10 Eur\n";
    ASSERT_EQ(menu1, thingsWeSaidToday.createMenuString());
    Order mntblnc(LMONTBLANC,6,450,"MontBlanc");
    ASSERT_NE(invalidOrder, thingsWeSaidToday.prepareCake(mntblnc));
    ASSERT_EQ(invalidOrder, thingsWeSaidToday.prepareCake(mntblnc));
    string menu2 = "\n                 Current offer:\n\nDrinks:\n   Espresso                                         2.00 Eur\n   Lungo                                            2.20 Eur\n   Cappuccino                                       2.90 Eur\n   Homemade Lemonade                                2.50 Eur\n   Various Soft Drinks                              1.50 Eur\n\nYesterday's cakes with a discount\n   Red Velvet Cupcakes                              1.30 Eur\n   Vanilla Cupcakes                                 1.10 Eur\n";
    ASSERT_EQ(menu2, thingsWeSaidToday.createMenuString());
}
TEST(Part3_TheBrains, SorryWeHaveClosedForTheDay) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Counter IAmSpeed(stock, milliseconds(30),false,true);
    sleep_for(nanoseconds (100));
    Order cap(CAPPUCCINO,6,290,"Cappuccino");
    ASSERT_NE(invalidOrder, IAmSpeed.prepareDrink(cap));
    Order rvc(RVCUPCAKE,6,190,"Red Velvet Cupcakes");
    Order vc(VANCUPCAKE,6,160,"Vanilla Cupcakes");
    Order mntblnc(MONTBLANC,6,650,"MontBlanc");
    ASSERT_NE(invalidOrder, IAmSpeed.prepareCake(vc));
    ASSERT_NE(invalidOrder, IAmSpeed.prepareCake(rvc));
    ASSERT_NE(invalidOrder, IAmSpeed.prepareCake(vc));
    ASSERT_NE(invalidOrder, IAmSpeed.prepareCake(rvc));
    ASSERT_NE(invalidOrder, IAmSpeed.prepareCake(mntblnc));
    sleep_for(milliseconds (20));
    ASSERT_EQ(invalidOrder, IAmSpeed.prepareDrink(cap));
}
TEST(Part3_TheBrains, ComeBackInSixty) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Counter counter(stock, milliseconds(1000),false,true);
    sleep_for(nanoseconds (100));
    string menu = counter.createMenuString();
    Order vc(VANCUPCAKE,12,160,"Vanilla Cupcakes");
    ASSERT_NE(invalidOrder, counter.prepareCake(vc));
    ASSERT_EQ(invalidOrder, counter.prepareCake(vc));
    ASSERT_NE(menu, counter.createMenuString());
    sleep_for(milliseconds (60));
    ASSERT_NE(invalidOrder, counter.prepareCake(vc));
}
TEST(Part3_TheBrains, ThreeAMBrain) {
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Counter counter(stock, milliseconds(1000),false,true);
    sleep_for(nanoseconds (100));
    Order vc(VANCUPCAKE,1,160,"Vanilla Cupcakes");
    Order cap(CAPPUCCINO,1,290,"Cappuccino");
    ASSERT_EQ(invalidOrder, counter.prepareDrink(vc));
    ASSERT_EQ(invalidOrder, counter.prepareCake(cap));
}
// TODO part4
TEST(Part4_OnceAgainAllAtOnce, AntiChairs) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,-10));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(0, BeansAndBees.numberOfSeatsInTheHouse());

}
TEST(Part4_OnceAgainAllAtOnce, IHateChairsIDontWantAny) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,0));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(10));
    ASSERT_EQ(0, BeansAndBees.numberOfSeatsInTheHouse());

}
TEST(Part4_OnceAgainAllAtOnce, TooManyChairs) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,10));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(0, BeansAndBees.numberOfSeatsInTheHouse());

}
TEST(Part4_OnceAgainAllAtOnce, GoodManyChairs) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(00));
    ASSERT_EQ(160, BeansAndBees.numberOfSeatsInTheHouse());

}
TEST(Part4_OnceAgainAllAtOnce, GoodAndBadManyChairs) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    tables.push_back(tableInfo(OCTUPLE,20,-8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(160, BeansAndBees.numberOfSeatsInTheHouse());

}
TEST(Part4_OnceAgainAllAtOnce, GoodAndBadManyTables) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    tables.push_back(tableInfo(OCTUPLE,20,-8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(160, BeansAndBees.numberOfSeatsInTheHouse());
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
}
TEST(Part4_OnceAgainAllAtOnce, BadPeopleCannotFitHere) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_FALSE(BeansAndBees.seatGuests(666));
    ASSERT_FALSE(BeansAndBees.seatGuests(-6));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
}
TEST(Part4_OnceAgainAllAtOnce, WhyIsThereJustOneChair) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,1));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
    ASSERT_FALSE(BeansAndBees.seatGuests(2));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
}
TEST(Part4_OnceAgainAllAtOnce, IReallyNeededToSitDownForThis) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.seatGuests(7));
    ASSERT_EQ(19, BeansAndBees.numberOfFreeTables());
}
TEST(Part4_OnceAgainAllAtOnce, WeAreNotWastingSpace) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,5,8));
    tables.push_back(tableInfo(SEXTUPLE,3,6));
    tables.push_back(tableInfo(QUADRUPLE,2,4));
    tables.push_back(tableInfo(DOUBLE,2,2));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    for (int i = 0; i < 2; ++i) {
        ASSERT_TRUE(BeansAndBees.seatGuests(2));
    }
    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(BeansAndBees.seatGuests(6));
    }
    for (int i = 0; i < 2; ++i) {
        ASSERT_TRUE(BeansAndBees.seatGuests(4));
    }
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(BeansAndBees.seatGuests(8));
    }
    ASSERT_FALSE(BeansAndBees.seatGuests(1));
}
TEST(Part4_OnceAgainAllAtOnce, PeopleMustReallyLikeYourEstablishment) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    string brano = "Braňo Mojsej";
    ASSERT_TRUE(BeansAndBees.bookATable(brano, 8));
}
TEST(Part4_OnceAgainAllAtOnce, HeWhoShallNotBeNamed) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    string HeWhoShallNotBeNamed = "";
    ASSERT_FALSE(BeansAndBees.bookATable(HeWhoShallNotBeNamed, 1));
}
TEST(Part4_OnceAgainAllAtOnce, PrivateCompanyAndMore) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE, 20, 8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    string brano = "Braňo Mojsej";
    for (int i = 0; i < 20; ++i) {
        ASSERT_FALSE(BeansAndBees.nameAlreadyHasABooking(brano + to_string(i)));
        ASSERT_TRUE(BeansAndBees.bookATable(brano + to_string(i), 8));
    }
    ASSERT_TRUE(BeansAndBees.nameAlreadyHasABooking(brano + "10"));
    ASSERT_FALSE(BeansAndBees.bookATable(brano, 8));
    ASSERT_EQ(0, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.seatBookedGuests(brano + "10", 8));
}
TEST(Part4_OnceAgainAllAtOnce, DoYouKnowWhoIAm) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE, 20, 8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    string theCenterOfTheUniverse = "Karen";
    ASSERT_FALSE(BeansAndBees.nameAlreadyHasABooking(theCenterOfTheUniverse));
    for (int i = 0; i < 20; ++i) {
        ASSERT_TRUE(BeansAndBees.seatGuests(2));
    }
    ASSERT_FALSE(BeansAndBees.seatBookedGuests(theCenterOfTheUniverse, 1));
}
TEST(Part4_OnceAgainAllAtOnce, DamnIForgotToBookTheTable) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE, 20, 8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock);
    sleep_for(nanoseconds(100));
    string Neville = "Neville Longbottom";
    ASSERT_FALSE(BeansAndBees.nameAlreadyHasABooking(Neville));
    ASSERT_TRUE(BeansAndBees.seatBookedGuests(Neville, 4));
}
TEST(Part4_OnceAgainAllAtOnce, FullAutoCauseManualIsLame) {
    vector<tableInfo> tTypes;
    ASSERT_TRUE(appendTableTypesFromFile(tTypes, "Tables1.txt"));
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Coffeehouse BeansAndBees(milliseconds(1000), tTypes, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(88, BeansAndBees.numberOfSeatsInTheHouse());
}
TEST(Part4_OnceAgainAllAtOnce, WhatDoYouMeanYouHaveNothing) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.seatGuests(7));
    ASSERT_EQ(19, BeansAndBees.numberOfFreeTables());
    ASSERT_FALSE(BeansAndBees.placeOrder(1,SODA,7));
}
TEST(Part4_OnceAgainAllAtOnce, ProstPleaseYouMustHaveThat) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    stock.push_back(initialCountAndDetails(SODA, 0, "Prost",180, 1));
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.seatGuests(7));
    ASSERT_EQ(19, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.placeOrder(1,SODA,7));
}
TEST(Part4_OnceAgainAllAtOnce, MoneyMoneyMoney) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    stock.push_back(initialCountAndDetails(SODA, 0, "Prost",180, 1));
    stock.push_back(initialCountAndDetails(RVCUPCAKE, 12, "Red Velvet Cupcakes", 190, 70));
    Coffeehouse BeansAndBees(milliseconds(100000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.seatGuests(7));
    ASSERT_EQ(19, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.placeOrder(1,SODA,7));
    ASSERT_TRUE(BeansAndBees.placeOrder(1,RVCUPCAKE,10));
    ASSERT_TRUE(BeansAndBees.placeOrder(1,RVCUPCAKE,1));
    ASSERT_TRUE(BeansAndBees.placeOrder(1,RVCUPCAKE,1));
    ASSERT_FALSE(BeansAndBees.placeOrder(1,RVCUPCAKE,1));
    BeansAndBees.finalPaymentAndDeparture(1);
}
TEST(Part4_OnceAgainAllAtOnce, HastagSaveTheTrees) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    stock.push_back(initialCountAndDetails(RVCUPCAKE, 12, "Red Velvet Cupcakes", 190, 70));
    Coffeehouse BeansAndBees(milliseconds(100000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_TRUE(BeansAndBees.seatGuests(7));
    ASSERT_TRUE(BeansAndBees.placeOrder(1,RVCUPCAKE,1));
    BeansAndBees.checkPlease(1);
    auto wastedPaper = BeansAndBees.receiptPrinter.str().size();
    auto wastedPaper2 = wastedPaper;
    for (auto i = 0; i < 11; ++i){
        ASSERT_TRUE(BeansAndBees.placeOrder(1,RVCUPCAKE,1));
        BeansAndBees.checkPlease(1);
        wastedPaper2 = BeansAndBees.receiptPrinter.str().size();
        ASSERT_LT(wastedPaper, wastedPaper2);
        wastedPaper = wastedPaper2;
    }
}
TEST(Part4_OnceAgainAllAtOnce, AreYouHEreBusboy) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(OCTUPLE,20,8));
    vector<initialCountAndDetails> stock;
    stock.push_back(initialCountAndDetails(SODA, 0, "Prost",180, 1));
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
    BeansAndBees.seatGuests(7);
    ASSERT_EQ(19, BeansAndBees.numberOfFreeTables());
    ASSERT_TRUE(BeansAndBees.placeOrder(1,SODA,7));
    BeansAndBees.finalPaymentAndDeparture(1);
    ASSERT_EQ(20, BeansAndBees.numberOfFreeTables());
}
TEST(Part4_OnceAgainAllAtOnce, OkienkovyPredaj) {
    vector<tableInfo> tables;
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    ASSERT_TRUE(BeansAndBees.placeOrder(0,CAPPUCCINO,1));
    BeansAndBees.finalPaymentAndDeparture(0);
    ASSERT_TRUE(BeansAndBees.placeOrder(0,ESPRESSO,1));
    ASSERT_TRUE(BeansAndBees.placeOrder(0,MONTBLANC,1));
    BeansAndBees.finalPaymentAndDeparture(0);
}
TEST(Part4_OnceAgainAllAtOnce, ADayLikeAnyOther) {
    vector<tableInfo> tables;
    ASSERT_TRUE(appendTableTypesFromFile(tables,"Tables1.txt"));
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    string menu = BeansAndBees.counter.createMenuString();
    ASSERT_TRUE(BeansAndBees.placeOrder(0,CAPPUCCINO,1));
    BeansAndBees.finalPaymentAndDeparture(0);
    ASSERT_TRUE(BeansAndBees.placeOrder(0,ESPRESSO,1));
    ASSERT_TRUE(BeansAndBees.placeOrder(0,MONTBLANC,1));
    BeansAndBees.finalPaymentAndDeparture(0);
    BeansAndBees.seatGuests(1);
    ASSERT_TRUE(BeansAndBees.placeOrder(0,VANCUPCAKE,12));
    BeansAndBees.finalPaymentAndDeparture(0);
    BeansAndBees.seatGuests(1);
    ASSERT_TRUE(BeansAndBees.placeOrder(1,CAPPUCCINO,1));
    ASSERT_FALSE(BeansAndBees.placeOrder(1,VANCUPCAKE,1));
    sleep_for(milliseconds (60));
    ASSERT_TRUE(BeansAndBees.placeOrder(1,VANCUPCAKE,1));
    auto usedPaper = BeansAndBees.receiptPrinter.str().size();
    BeansAndBees.finalPaymentAndDeparture(1);
    ASSERT_LT(usedPaper, BeansAndBees.receiptPrinter.str().size());
    internal::CaptureStdout();
    BeansAndBees.tables.at(1)->showMenu();
    ASSERT_NE(menu, internal::GetCapturedStdout());
}
TEST(Part4_OnceAgainAllAtOnce, TheStaffReallyWantsToGetHome) {
    vector<tableInfo> tables;
    ASSERT_TRUE(appendTableTypesFromFile(tables,"Tables1.txt"));
    vector<initialCountAndDetails> stock;
    ASSERT_TRUE(appendItemsFromFile(stock, "Items1.txt"));
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true);
    sleep_for(nanoseconds(100));
    BeansAndBees.closeAndSaveLeftoversToFile(.3, true);
    ASSERT_FALSE(BeansAndBees.placeOrder(0,MONTBLANC,1));
    vector<initialCountAndDetails> stock2;
    ASSERT_TRUE(appendItemsFromFile(stock2, "Items2.txt"));
    Coffeehouse BeansAndBees2(milliseconds(1000), tables, stock2, true, true);
    sleep_for(nanoseconds(100));
    ASSERT_FALSE(BeansAndBees2.placeOrder(0,MONTBLANC,1));
    ASSERT_TRUE(BeansAndBees2.placeOrder(0,LMONTBLANC,1));
}
TEST(Part4_OnceAgainAllAtOnce, WorkSmarterNotFaster) {
    vector<tableInfo> tables;
    tables.push_back(tableInfo(DOUBLE,4,2));
    tables.push_back(tableInfo(QUADRUPLE,6,3));
    tables.push_back(tableInfo(SEXTUPLE,2,4));
    tables.push_back(tableInfo(SEXTUPLE,2,6));
    tables.push_back(tableInfo(OCTUPLE,6,7));
    vector<initialCountAndDetails> stock;
    stock.push_back(initialCountAndDetails(RVCUPCAKE, 12, "Red Velvet Cupcakes", 190, 70));
    stock.push_back(initialCountAndDetails(VANCUPCAKE, 12, "Vanilla Cupcakes", 160, 55));
    stock.push_back(initialCountAndDetails(MONTBLANC, 6, "MontBlanc", 650, 120));
    stock.push_back(initialCountAndDetails(ESPRESSO, 0, "Espresso", 200, 1));
    stock.push_back(initialCountAndDetails(LUNGO, 0, "Lungo", 220, 1));
    stock.push_back(initialCountAndDetails(CAPPUCCINO, 0, "Cappuccino", 290, 2));
    stock.push_back(initialCountAndDetails(SODA, 0, "Various Soft Drinks", 150, 1));
    Coffeehouse BeansAndBees(milliseconds(1000), tables, stock, false, true, true);
    BeansAndBees.closeAndSaveLeftoversToFile(.2, true);
    vector<tableInfo> tables2;
    ASSERT_TRUE(appendTableTypesFromFile(tables2));
    vector<initialCountAndDetails> stock2;
    ASSERT_TRUE(appendItemsFromFile(stock2));
    Coffeehouse BeansAndBees2(milliseconds(1000), tables2, stock2, true, true, true);
    ASSERT_EQ(BeansAndBees2.numberOfSeatsInTheHouse(), BeansAndBees.numberOfSeatsInTheHouse());
    ASSERT_TRUE(BeansAndBees2.placeOrder(0,VANCUPCAKE,12));
    ASSERT_TRUE(BeansAndBees2.placeOrder(0,LVANCUPCAKE,12));
    ASSERT_TRUE(BeansAndBees2.placeOrder(0,CAPPUCCINO,2));
}
