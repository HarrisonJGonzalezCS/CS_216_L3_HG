#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>

#define MAX_CREATURES 10 // Maximum number of creatures
const int MAX_NAME_LENGTH = 15;
const int MAX_TYPE_LENGTH = 15;

// Enum for menu options
enum MENU_OPTION { PRINT = 1, SORT, SEARCH, EXIT };

// Enum for sorting options
enum SORT_OPTION { NAME = 1, TYPE, BACK_TO_MAIN };

// Creature class
class Creature {
private:
    std::string name;
    std::string type;

public:
    // Constructor with default values
    Creature(std::string creatureName = "Unknown", std::string creatureType = "Unknown")
        : name(std::move(creatureName)), type(std::move(creatureType)) {}

    // Accessor methods
    std::string getName() const { return name; }
    std::string getType() const { return type; }

    // Convert string to lowercase
    static std::string toLower(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

    // Check if a query matches the name or type (case-insensitive, partial search)
    bool matches(const std::string& query) const {
        std::string qLower = toLower(query);
        return (toLower(name).find(qLower) != std::string::npos || toLower(type).find(qLower) != std::string::npos);
    }

    // Display creature info with proper alignment
    void display() const {
        std::cout << std::setfill(' ');
        std::cout << "| " << std::left << std::setw(MAX_NAME_LENGTH) << name
                  << "| " << std::setw(MAX_TYPE_LENGTH) << type << "|\n";
    }
};

// Army class to manage creatures
class Army {
private:
    Creature creatures[MAX_CREATURES];
    int count = 0;

public:
    // Load creatures from a file
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << "\n";
            return;
        }

        std::string name, type;
        while (file >> name >> type && count < MAX_CREATURES) {
            creatures[count++] = Creature(name, type);
        }
        file.close();
    }

    // Print all creatures in table format
    void printCreatures() const {
        std::cout << "\n" << std::setfill('-') << std::setw(35) << "-" << "\n";
        std::cout << std::setfill(' ');
        std::cout << "| " << std::left << std::setw(MAX_NAME_LENGTH) << "Name"
                  << "| " << std::setw(MAX_TYPE_LENGTH) << "Type" << "|\n";
        std::cout << std::setfill('-') << std::setw(35) << "-" << "\n";

        for (int i = 0; i < count; i++) {
            creatures[i].display();
        }
        std::cout << std::setfill('-') << std::setw(35) << "-" << "\n";
    }

    // Search creatures by partial name or type
    void searchCreature(const std::string& query) const {
        bool found = false;
        std::cout << "\nMatching Creatures:\n";
        std::cout << std::setfill('-') << std::setw(35) << "-" << "\n";

        for (int i = 0; i < count; i++) {
            if (creatures[i].matches(query)) {
                creatures[i].display();
                found = true;
            }
        }

        if (!found) {
            std::cout << "No matching creatures found.\n";
        }
        std::cout << std::setfill('-') << std::setw(35) << "-" << "\n";
    }

    // Sort creatures using bubble sort and pointers
    void sortCreatures(SORT_OPTION option) {
        Creature* pCreatures[MAX_CREATURES];

        for (int i = 0; i < count; i++) {
            pCreatures[i] = &creatures[i];
        }

        bool swapped;
        do {
            swapped = false;
            for (int i = 0; i < count - 1; i++) {
                bool toSwap = false;
                if (option == NAME) {
                    toSwap = pCreatures[i]->getName() > pCreatures[i + 1]->getName();
                } else if (option == TYPE) {
                    toSwap = pCreatures[i]->getType() > pCreatures[i + 1]->getType();
                }

                if (toSwap) {
                    std::swap(pCreatures[i], pCreatures[i + 1]);
                    swapped = true;
                }
            }
        } while (swapped);

        // Print sorted creatures
        std::cout << "\nSorted Creatures:\n";
        std::cout << std::setfill('-') << std::setw(35) << "-" << "\n";
        for (int i = 0; i < count; i++) {
            pCreatures[i]->display();
        }
        std::cout << std::setfill('-') << std::setw(35) << "-" << "\n";
    }
};

// Function to display the main menu
void displayMenu() {
    std::cout << "\nMenu:\n"
              << "1. Print Creatures\n"
              << "2. Sort Creatures\n"
              << "3. Search Creatures\n"
              << "4. Exit\n"
              << "Enter choice: ";
}

// Function to display the sort submenu
SORT_OPTION displaySortMenu() {
    int choice;
    std::cout << "\nSort Menu:\n"
              << "1. Sort by Name\n"
              << "2. Sort by Type\n"
              << "3. Go Back to Main Menu\n"
              << "Enter choice: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return BACK_TO_MAIN;
    }

    return static_cast<SORT_OPTION>(choice);
}

// Main function
int main() {
    Army army;
    army.loadFromFile("creatures.txt");

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a valid option.\n";
            choice = 0;
        }

        switch (choice) {
            case PRINT:
                army.printCreatures();
                break;
            case SORT: {
                SORT_OPTION sortChoice;
                do {
                    sortChoice = displaySortMenu();
                    switch (sortChoice) {
                        case NAME:
                        case TYPE:
                            army.sortCreatures(sortChoice);
                            break;
                        case BACK_TO_MAIN:
                            std::cout << "Returning to main menu...\n";
                            break;
                        default:
                            std::cout << "Invalid choice. Try again.\n";
                    }
                } while (sortChoice != BACK_TO_MAIN);
                break;
            }
            case SEARCH: {
                std::string query;
                std::cout << "Enter partial name or type to search: ";
                std::cin >> query;
                army.searchCreature(query);
                break;
            }
            case EXIT:
                std::cout << "Exiting program...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != EXIT);

    return 0;
}
/*
Menu:
1. Print Creatures
2. Sort Creatures
3. Search Creatures
4. Exit
Enter choice:1


-----------------------------------
| Name           | Type           |
-----------------------------------
| Dragon         | Flying         |
| Centaur        | Ground         |
| Parrot         | Flying         |
-----------------------------------

Menu:
1. Print Creatures
2. Sort Creatures
3. Search Creatures
4. Exit
Enter choice:2


Sort Menu:
1. Sort by Name
2. Sort by Type
3. Go Back to Main Menu
Enter choice:1


Sorted Creatures:
-----------------------------------
| Centaur        | Ground         |
| Dragon         | Flying         |
| Parrot         | Flying         |
-----------------------------------

Sort Menu:
1. Sort by Name
2. Sort by Type
3. Go Back to Main Menu
Enter choice:2


Sorted Creatures:
-----------------------------------
| Dragon         | Flying         |
| Parrot         | Flying         |
| Centaur        | Ground         |
-----------------------------------

Sort Menu:
1. Sort by Name
2. Sort by Type
3. Go Back to Main Menu
Enter choice:3

Returning to main menu...

Menu:
1. Print Creatures
2. Sort Creatures
3. Search Creatures
4. Exit
Enter choice:3

Enter partial name or type to search:Dragon


Matching Creatures:
-----------------------------------
| Dragon         | Flying         |
-----------------------------------

Menu:
1. Print Creatures
2. Sort Creatures
3. Search Creatures
4. Exit
Enter choice:4

Exiting program...

Process finished with exit code 0*/