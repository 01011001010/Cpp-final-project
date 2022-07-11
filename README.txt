// Programovanie (3)
// Semestrálny projekt
// Katarína Osvaldová, 2DAV

This project creates a software backbone for a café.

The main class that acts as a representation of the whole
café and is the main interface for the staff is Coffeehouse.
It is initialised with the specs for the café, the
information about tables and the menu. It is able to read
leftovers from a special file if desired and save the
configuration specs for future use. Such files can be used
to initialise the Coffeehouse with the help of the
fileComprehensionFunctions functions.

The Coffeehouse class controls tables of the café and
controls the seating arrangement. Each table holds the
information about the tab of the table and is able to
create a receipt for the table. Each table also holds the
current menu (with information about the availability of
fresh desserts) and is able to display it on command.
The Coffeehouse class acts as a messenger between the
counter of the café and the tables, and between the waiter
and the counter.

The main feature of the Coffeehouse class is, of course,
the order system. An order can be either for a takeout
(logged under the virtual table number 0) or a specific
table. The Coffeehouse class relays the order to the
counter.
The Counter class is the brain of the café. It coordinates
the staff and the stock. It also holds the power to order
cakes from the kitchen. It holds the information about the
day's offer and is able to create menu with information
about the availability of offered desserts.