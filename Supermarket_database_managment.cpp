#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// a structure to represent the expiration date
struct Date {
    int year;
    int month;
    int day;
};

class Item {
protected:
    int ID;
    string name;
    float price;
    int quantity;
    fstream database;

public:
    // Static method to open the file
    static fstream openFile() {
        fstream file("supermarket.txt", ios::in | ios::out | ios::app);
        if (!file.is_open()) {
            cout << "Error opening the file!" << endl;
        }
        return file;
    }

    // Static method to close the file
    static void closeFile(fstream& file) {
        file.close();
    }

    Item(int id = 0, const string& itemName = "", float itemPrice = 0.0, int itemQuantity = 0)
        : ID(id), name(itemName), price(itemPrice), quantity(itemQuantity) {
        // Open the file in constructor
        database = openFile();
    }

    // Copy constructor
    Item(const Item& other)
        : ID(other.ID), name(other.name), price(other.price), quantity(other.quantity) {
        database = openFile();
    }

    // Copy assignment operator
    Item& operator=(const Item& other) {
        if (this != &other) {
            ID = other.ID;
            name = other.name;
            price = other.price;
            quantity = other.quantity;

            // Close the existing file and open a new one
            closeFile(database);
            database = openFile();
        }
        return *this;
    }

    int getID() const { return ID; }
    const string& getName() const { return name; }
    float getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    virtual void additem() {
        fstream database = openFile();

        if (!database.is_open()) {
            cout << "Error opening the file!" << endl;
        } else {
            cout << "Enter ID: ";
            cin >> ID;

            cout << "Enter name: ";
            cin >> name;

            cout << "Enter quantity: ";
            cin >> quantity;

            cout << "Enter price: ";
            cin >> price;

            database << ID << " " << name << " " << quantity << " " << price << endl;
        }
        closeFile(database);
    }

    void removeitem() {
        cout << "Enter the name of the item to remove: ";
        string itemName;
        cin >> itemName;

        // Open the file in read mode
        fstream database = openFile();
        ofstream tempFile("temp.txt", ios::out);

        if (!database.is_open() || !tempFile.is_open()) {
            cout << "Error opening files!" << endl;
            return;
        }

        bool itemFound = false;

        while (database >> ID >> name >> quantity >> price) {
            if (name == itemName) {
                cout << "Item found and removed: " << name << endl;
                itemFound = true;
            } else {
                tempFile << ID << " " << name << " " << quantity << " " << price << endl;
            }
        }

        database.close();
        tempFile.close();

        remove("supermarket.txt");
        rename("temp.txt", "supermarket.txt");

        if (!itemFound) {
            cout << "Item not found in the database." << endl;
        }
    }

    virtual void editdatabase() {
        cout << "Enter the name of the item to edit: ";
        string itemName;
        cin >> itemName;

        // Open the file in read mode
        fstream database = openFile();
        ofstream tempFile("temp.txt", ios::out);

        if (!database.is_open() || !tempFile.is_open()) {
            cout << "Error opening files!" << endl;
            return;
        }

        bool itemFound = false;

        while (database >> ID >> name >> quantity >> price) {
            if (name == itemName) {
                cout << "Item found. Editing options:" << endl;
                cout << "1. Edit name" << endl;
                cout << "2. Edit quantity" << endl;
                cout << "3. Edit price" << endl;
                cout << "Enter your choice: ";

                int choice;
                cin >> choice;

                switch (choice) {
                    case 1:
                        cout << "Enter the new name: ";
                        cin >> name;
                        break;
                    case 2:
                        cout << "Enter the new quantity: ";
                        cin >> quantity;
                        break;
                    case 3:
                        cout << "Enter the new price: ";
                        cin >> price;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }

                cout << "Item edited successfully." << endl;
                itemFound = true;
            }

            tempFile << ID << " " << name << " " << quantity << " " << price << endl;
        }

        database.close();
        tempFile.close();

        remove("supermarket.txt");
        rename("temp.txt", "supermarket.txt");

        if (!itemFound) {
            cout << "Item not found in the database." << endl;
        }
    }

    void displayitem() {
        fstream database = openFile();
        if (database.is_open()) {
            string line;
            while (getline(database, line)) {
                cout << line << endl;
            }
            database.close();
        }
    }

    void addtocart() {
        vector<Item> shoppingCart;

        while (true) {
            cout << "Enter the name of the item to add to the cart (type 'done' to finish): ";
            string itemName;
            cin >> itemName;

            if (itemName == "done") {
                break;
            }

            // Open the file in read mode
            fstream database = openFile();

            if (!database.is_open()) {
                cout << "Error opening the file!" << endl;
                return;
            }

            bool itemFound = false;

            while (database >> ID >> name >> quantity >> price) {
                if (name == itemName) {
                    Item newItem;
                    newItem.ID = ID;
                    newItem.name = name;
                    newItem.quantity = 1;
                    newItem.price = price;

                    shoppingCart.push_back(newItem);

                    cout << "Item added to the cart successfully." << endl;
                    itemFound = true;
                }
            }

            database.close();

            if (!itemFound) {
                cout << "Item not found in the database." << endl;
            }
        }

        cout << "Items in the shopping cart:" << endl;
        for (const auto& item : shoppingCart) {
            cout << "ID: " << item.getID() << " Name: " << item.getName() << " Quantity: " << item.getQuantity() << " Price: " << item.getPrice() << endl;
        }

        // Call billcalculate after collecting all items in the shopping cart
        billcalculate(shoppingCart);
    }

    void billcalculate(const vector<Item>& shoppingCart) {
        float totalCost = 0.0;
        cout << "Bill Details:" << endl;
        for (const auto& item : shoppingCart) {
            float itemCost = item.getPrice();
            cout << "ID: " << item.getID() << " Name: " << item.getName() << " Price: " << item.getPrice() << " Total Cost: " << itemCost << endl;
            totalCost += itemCost;
        }
        cout << "Total Cost: " << totalCost << endl;
    }

    ~Item() {
        // Close the file in the destructor
        closeFile(database);
    }
};

class Clothes : public Item {
private:
    string size;
    int discount;

public:
    Clothes(int id = 0, const string& itemName = "", float itemPrice = 0.0, int itemQuantity = 0, const string& itemSize = "", int itemDiscount = 0)
        : Item(id, itemName, itemPrice, itemQuantity), size(itemSize), discount(itemDiscount) {}

    void additem() override {
        Item::additem();
        cout << "Enter sizes: ";
        cin >> size;
        cout << "Enter discount: ";
        cin >> discount;
        fstream database = openFile();
        if (!database.is_open()) {
            cout << "Error opening the file!" << endl;
        } else {
            database << " Size: " << size << " Discount: " << discount << "%" << endl;
            closeFile(database);
            cout << "Clothes item added to the database successfully." << endl;
        }
    }

    void editdatabase() override {
        Item::editdatabase();

        cout << "Enter the new size: ";
        cin >> size;
        cout << "Enter the new discount: ";
        cin >> discount;
    }

    void displayitem() {
        Item::displayitem();
        cout << " Size: " << size << " Discount: " << discount << "%" << endl;
    }
};

class Food : public Item {
protected:
    Date expiryDate; // expiration date

public:
    Food(int id = 0, const string& itemName = "", float itemPrice = 0.0, int itemQuantity = 0, int year = 0, int month = 0, int day = 0)
        : Item(id, itemName, itemPrice, itemQuantity), expiryDate({ year, month, day }) {}

    void additem() override {
        Item::additem();
        cout << "Enter expiry date (YYYY MM DD): ";
        cin >> expiryDate.year >> expiryDate.month >> expiryDate.day;
        fstream database = openFile();
        if (!database.is_open()) {
            cout << "Error opening the file!" << endl;
        } else {
            database << " Expiry Date: " << expiryDate.year << " " << expiryDate.month << " " << expiryDate.day << endl;
            closeFile(database);
            cout << "Food item added to the database successfully." << endl;
        }
    }

    void editdatabase() override {
        Item::editdatabase();

        cout << "Enter the new expiration date (YYYY MM DD): ";
        cin >> expiryDate.year >> expiryDate.month >> expiryDate.day;
    }

    void displayitem() {
        Item::displayitem();
        cout << " Expiry Date: " << expiryDate.year << " " << expiryDate.month << " " << expiryDate.day << endl;
    }
};

int main() {
    Item itemObj;
    Clothes clothesObj;
    Food foodObj;
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "              Welcome to the Menu         \n";
        cout << "=========================================\n";
        cout << "1. Add Item\n";
        cout << "2. Display Items\n";
        cout << "3. Add to Cart\n";
        cout << "4. Remove Item\n";
        cout << "5. Edit Database\n";
        cout << "6. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nAdding items to the database:\n";
                cout << "1. Clothes\n";
                cout << "2. Food\n";
                cout << "Enter your choice: ";
                int itemType;
                cin >> itemType;

                switch (itemType) {
                    case 1:
                        clothesObj.additem();
                        break;
                    case 2:
                        foodObj.additem();
                        break;
                    default:
                        cout << "Invalid choice!\n";
                        break;
                }

                break;
            case 2:
                cout << "\nDisplaying items from the database:\n";
                itemObj.displayitem();
                break;
            case 3:
                cout << "\nAdding items to the cart:\n";
                itemObj.addtocart();
                break;
            case 4:
                cout << "\nRemoving items from the database:\n";
                itemObj.removeitem();
                break;
            case 5:
                cout << "\nEditing items in the database:\n";
                itemObj.editdatabase();
                break;
            case 6:
                cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }

    } while (choice != 6);

    return 0;
}
