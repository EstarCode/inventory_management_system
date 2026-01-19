#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;


const int MAX_ITEMS = 100;
const int MAX_TRANSACTIONS = 200;
const int MIN_STOCK_THRESHOLD = 5;
const double BULK_DISCOUNT_THRESHOLD = 5;
const double BULK_DISCOUNT_RATE = 0.10; // 10% discount

// DATA STRUCTURES
struct Product {
    int id;
    string name;
    string category;
    int quantity;
    double price;
    bool active;
};

struct Category {
    string name;
    string description;
    bool active;
};

struct Supplier {
    string name;
    string contact;
    bool active;
};

struct Transaction {
    int transactionId;
    string productName;
    int productId;
    int quantity;
    double unitPrice;
    double totalPrice;
    double discount;
    string date;
    string time;
};

// GLOBAL ARRAYS
Product products[MAX_ITEMS];
Category categories[MAX_ITEMS];
Supplier suppliers[MAX_ITEMS];
Transaction transactions[MAX_TRANSACTIONS];

int productCount = 0;
int categoryCount = 0;
int supplierCount = 0;
int transactionCount = 0;
int nextTransactionId = 1;

// Functions DECLARATIONS
bool isDuplicateProductId(int id);
bool categoryExists(string name);
bool validateProductData(int* id, int* quantity, double* price, string* category);
void calculateDiscount(int quantity, double subtotal, double* discount, double* total);
void updateInventoryStock(Product* product, int quantitySold);
void getProductStatistics(int* totalProducts, int* lowStock, int* outOfStock, double* totalValue);

// ============================================================
// UTILITY FUNCTIONS
// ============================================================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    clearInputBuffer();
    cin.get();
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    return string(buffer);
}

string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}

void printTableHeader(string title) {
    cout << "\n";
    cout << "================================================================\n";
    cout << "  " << title << "\n";
    cout << "================================================================\n";
}

void printSuccess(string message) {
    clearScreen();
    cout << "\n[SUCCESS] " << message << "\n";
}

void printError(string message) {
    clearScreen();
    cout << "\n[ERROR] " << message << "\n";
}

void printWarning(string message) {
    cout << "\n[WARNING] " << message << "\n";
}

// ============================================================
// VALIDATION FUNCTIONS
// ============================================================
bool isValidId(int id) {
    return id > 0;
}

bool isValidQuantity(int quantity) {
    return quantity >= 0;
}

bool isValidPrice(double price) {
    return price >= 0.0;
}

int findProductById(int id) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id && products[i].active) {
            return i;
        }
    }
    return -1;
}

int findProductByName(string name) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].name == name && products[i].active) {
            return i;
        }
    }
    return -1;
}

int findCategoryByName(string name) {
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].name == name && categories[i].active) {
            return i;
        }
    }
    return -1;
}

int findSupplierByName(string name) {
    for (int i = 0; i < supplierCount; i++) {
        if (suppliers[i].name == name && suppliers[i].active) {
            return i;
        }
    }
    return -1;
}

bool isDuplicateProductId(int id) {
    return findProductById(id) != -1;
}

bool categoryExists(string name) {
    return findCategoryByName(name) != -1;
}

// Validate product data using pointers (call by address)
bool validateProductData(int* id, int* quantity, double* price, string* category) {
    if (*id <= 0) {
        printError("Invalid ID! Must be positive.");
        return false;
    }

    if (isDuplicateProductId(*id)) {
        printError("Product ID already exists!");
        return false;
    }

    if (*quantity < 0) {
        printError("Invalid quantity! Must be non-negative.");
        return false;
    }

    if (*price < 0.0) {
        printError("Invalid price! Must be non-negative.");
        return false;
    }

    if (!categoryExists(*category)) {
        printError("Category does not exist! Please create it first.");
        return false;
    }

    return true;
}

// ============================================================
// LOW STOCK ALERT
// ============================================================
void checkLowStock() {
    bool hasLowStock = false;

    for (int i = 0; i < productCount; i++) {
        if (products[i].active && products[i].quantity <= MIN_STOCK_THRESHOLD) {
            if (!hasLowStock) {
                printWarning("LOW STOCK ALERT!");
                cout << "----------------------------------------------------------------\n";
                cout << left << setw(6) << "ID" << setw(20) << "Product"
                     << setw(12) << "Quantity" << "Status\n";
                cout << "----------------------------------------------------------------\n";
                hasLowStock = true;
            }

            cout << left << setw(6) << products[i].id
                 << setw(20) << products[i].name
                 << setw(12) << products[i].quantity;

            if (products[i].quantity == 0) {
                cout << "OUT OF STOCK\n";
            } else {
                cout << "LOW STOCK\n";
            }
        }
    }

    if (hasLowStock) {
        cout << "----------------------------------------------------------------\n";
    }
}

// ============================================================
// PRODUCT MANAGEMENT
// ============================================================
void displayProductsTable() {
    if (productCount == 0) {
        printError("No products available!");
        return;
    }

    clearScreen();
    printTableHeader("ALL PRODUCTS");

    cout << left << setw(6) << "ID" << setw(20) << "Name"
         << setw(15) << "Category" << setw(10) << "Quantity"
         << setw(12) << "Price" << "Status\n";
    cout << "----------------------------------------------------------------\n";

    for (int i = 0; i < productCount; i++) {
        if (products[i].active) {
            cout << left << setw(6) << products[i].id
                 << setw(20) << products[i].name
                 << setw(15) << products[i].category
                 << setw(10) << products[i].quantity
                 << "$" << setw(11) << fixed << setprecision(2) << products[i].price;

            if (products[i].quantity == 0) {
                cout << "OUT\n";
            } else if (products[i].quantity <= MIN_STOCK_THRESHOLD) {
                cout << "LOW\n";
            } else {
                cout << "OK\n";
            }
        }
    }
    cout << "----------------------------------------------------------------\n";
    checkLowStock();
}

void addProduct() {
    if (productCount >= MAX_ITEMS) {
        printError("Product limit reached!");
        return;
    }

    clearScreen();
    printTableHeader("ADD NEW PRODUCT");

    Product newProduct;

    cout << "Enter Product ID: ";
    if (!(cin >> newProduct.id)) {
        clearInputBuffer();
        printError("Invalid input! ID must be a number.");
        return;
    }

    clearInputBuffer();
    cout << "Enter Product Name: ";
    getline(cin, newProduct.name);

    if (newProduct.name.empty()) {
        printError("Product name cannot be empty!");
        return;
    }

    cout << "Enter Category: ";
    getline(cin, newProduct.category);

    if (newProduct.category.empty()) {
        printError("Category cannot be empty!");
        return;
    }

    cout << "Enter Quantity: ";
    if (!(cin >> newProduct.quantity)) {
        clearInputBuffer();
        printError("Invalid input! Quantity must be a number.");
        return;
    }

    cout << "Enter Price: $";
    if (!(cin >> newProduct.price)) {
        clearInputBuffer();
        printError("Invalid input! Price must be a number.");
        return;
    }

    // Validate using pointer-based function (call by address)
    if (!validateProductData(&newProduct.id, &newProduct.quantity, &newProduct.price, &newProduct.category)) {
        return;
    }

    newProduct.active = true;
    products[productCount++] = newProduct;

    printSuccess("Product added successfully!");

    if (newProduct.quantity <= MIN_STOCK_THRESHOLD) {
        printWarning("This product has low stock!");
    }
}

void updateProduct() {
    clearScreen();
    printTableHeader("UPDATE PRODUCT");

    int id;
    cout << "Enter Product ID to update: ";
    if (!(cin >> id)) {
        clearInputBuffer();
        printError("Invalid input! ID must be a number.");
        return;
    }

    int index = findProductById(id);
    if (index == -1) {
        printError("Product not found!");
        return;
    }

    cout << "\nCurrent Details:\n";
    cout << "Name: " << products[index].name << "\n";
    cout << "Category: " << products[index].category << "\n";
    cout << "Quantity: " << products[index].quantity << "\n";
    cout << "Price: $" << fixed << setprecision(2) << products[index].price << "\n";

    clearInputBuffer();
    cout << "\nEnter New Name: ";
    getline(cin, products[index].name);

    if (products[index].name.empty()) {
        printError("Product name cannot be empty!");
        return;
    }

    cout << "Enter New Category: ";
    getline(cin, products[index].category);

    if (products[index].category.empty()) {
        printError("Category cannot be empty!");
        return;
    }

    if (!categoryExists(products[index].category)) {
        printError("Category does not exist!");
        return;
    }

    cout << "Enter New Quantity: ";
    if (!(cin >> products[index].quantity)) {
        clearInputBuffer();
        printError("Invalid input! Quantity must be a number.");
        return;
    }

    if (!isValidQuantity(products[index].quantity)) {
        printError("Invalid quantity!");
        return;
    }

    cout << "Enter New Price: $";
    if (!(cin >> products[index].price)) {
        clearInputBuffer();
        printError("Invalid input! Price must be a number.");
        return;
    }

    if (!isValidPrice(products[index].price)) {
        printError("Invalid price!");
        return;
    }

    printSuccess("Product updated successfully!");
}

void deleteProduct() {
    clearScreen();
    printTableHeader("DELETE PRODUCT");

    int id;
    cout << "Enter Product ID to delete: ";
    if (!(cin >> id)) {
        clearInputBuffer();
        printError("Invalid input! ID must be a number.");
        return;
    }

    int index = findProductById(id);
    if (index == -1) {
        printError("Product not found!");
        return;
    }

    products[index].active = false;
    printSuccess("Product deleted successfully!");
}

void searchProduct() {
    clearScreen();
    printTableHeader("SEARCH PRODUCT");

    int id;
    cout << "Enter Product ID: ";
    if (!(cin >> id)) {
        clearInputBuffer();
        printError("Invalid input! ID must be a number.");
        return;
    }

    int index = findProductById(id);
    if (index == -1) {
        printError("Product not found!");
        return;
    }

    clearScreen();
    cout << "\n================================================================\n";
    cout << "  PRODUCT DETAILS\n";
    cout << "================================================================\n";
    cout << "ID:           " << products[index].id << "\n";
    cout << "Name:         " << products[index].name << "\n";
    cout << "Category:     " << products[index].category << "\n";
    cout << "Quantity:     " << products[index].quantity << "\n";
    cout << "Price:        $" << fixed << setprecision(2) << products[index].price << "\n";
    cout << "Status:       ";

    if (products[index].quantity == 0) {
        cout << "OUT OF STOCK\n";
    } else if (products[index].quantity <= MIN_STOCK_THRESHOLD) {
        cout << "LOW STOCK\n";
    } else {
        cout << "IN STOCK\n";
    }
    cout << "================================================================\n";
}

// ============================================================
// CATEGORY MANAGEMENT
// ============================================================
void displayCategoriesTable() {
    if (categoryCount == 0) {
        printError("No categories available!");
        return;
    }

    clearScreen();
    printTableHeader("ALL CATEGORIES");

    cout << left << setw(20) << "Name" << "Description\n";
    cout << "----------------------------------------------------------------\n";

    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].active) {
            cout << left << setw(20) << categories[i].name
                 << categories[i].description << "\n";
        }
    }
    cout << "----------------------------------------------------------------\n";
}

void addCategory() {
    if (categoryCount >= MAX_ITEMS) {
        printError("Category limit reached!");
        return;
    }

    clearScreen();
    printTableHeader("ADD NEW CATEGORY");

    Category newCategory;

    clearInputBuffer();
    cout << "Enter Category Name: ";
    getline(cin, newCategory.name);

    if (newCategory.name.empty()) {
        printError("Category name cannot be empty!");
        return;
    }

    if (categoryExists(newCategory.name)) {
        printError("Category already exists!");
        return;
    }

    cout << "Enter Description: ";
    getline(cin, newCategory.description);

    newCategory.active = true;
    categories[categoryCount++] = newCategory;

    printSuccess("Category added successfully!");
}

void deleteCategory() {
    clearScreen();
    printTableHeader("DELETE CATEGORY");

    string name;
    clearInputBuffer();
    cout << "Enter Category Name: ";
    getline(cin, name);

    if (name.empty()) {
        printError("Category name cannot be empty!");
        return;
    }

    int index = findCategoryByName(name);
    if (index == -1) {
        printError("Category not found!");
        return;
    }

    // Check if any products use this category
    bool inUse = false;
    for (int i = 0; i < productCount; i++) {
        if (products[i].active && products[i].category == name) {
            inUse = true;
            break;
        }
    }

    if (inUse) {
        printError("Cannot delete! Category is in use by products.");
        return;
    }

    categories[index].active = false;
    printSuccess("Category deleted successfully!");
}

// ============================================================
// SUPPLIER MANAGEMENT
// ============================================================
void displaySuppliersTable() {
    if (supplierCount == 0) {
        printError("No suppliers available!");
        return;
    }

    clearScreen();
    printTableHeader("ALL SUPPLIERS");

    cout << left << setw(25) << "Name" << "Contact\n";
    cout << "----------------------------------------------------------------\n";

    for (int i = 0; i < supplierCount; i++) {
        if (suppliers[i].active) {
            cout << left << setw(25) << suppliers[i].name
                 << suppliers[i].contact << "\n";
        }
    }
    cout << "----------------------------------------------------------------\n";
}

void addSupplier() {
    if (supplierCount >= MAX_ITEMS) {
        printError("Supplier limit reached!");
        return;
    }

    clearScreen();
    printTableHeader("ADD NEW SUPPLIER");

    Supplier newSupplier;

    clearInputBuffer();
    cout << "Enter Supplier Name: ";
    getline(cin, newSupplier.name);

    if (newSupplier.name.empty()) {
        printError("Supplier name cannot be empty!");
        return;
    }

    if (findSupplierByName(newSupplier.name) != -1) {
        printError("Supplier already exists!");
        return;
    }

    cout << "Enter Contact Info: ";
    getline(cin, newSupplier.contact);

    newSupplier.active = true;
    suppliers[supplierCount++] = newSupplier;

    printSuccess("Supplier added successfully!");
}

void deleteSupplier() {
    clearScreen();
    printTableHeader("DELETE SUPPLIER");

    string name;
    clearInputBuffer();
    cout << "Enter Supplier Name: ";
    getline(cin, name);

    if (name.empty()) {
        printError("Supplier name cannot be empty!");
        return;
    }

    int index = findSupplierByName(name);
    if (index == -1) {
        printError("Supplier not found!");
        return;
    }

    suppliers[index].active = false;
    printSuccess("Supplier deleted successfully!");
}

// ============================================================
// TRANSACTION & PURCHASE MANAGEMENT
// ============================================================
void recordTransaction(int productId, string productName, int quantity,
                       double unitPrice, double discount, double total) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printWarning("Transaction history limit reached!");
        return;
    }

    Transaction trans;
    trans.transactionId = nextTransactionId++;
    trans.productId = productId;
    trans.productName = productName;
    trans.quantity = quantity;
    trans.unitPrice = unitPrice;
    trans.discount = discount;
    trans.totalPrice = total;
    trans.date = getCurrentDate();
    trans.time = getCurrentTime();

    transactions[transactionCount++] = trans;
}

// Calculate discount using call by reference (pointers)
void calculateDiscount(int quantity, double subtotal, double* discount, double* total) {
    *discount = 0.0;

    // Apply bulk discount if quantity meets threshold
    if (quantity >= BULK_DISCOUNT_THRESHOLD) {
        *discount = subtotal * BULK_DISCOUNT_RATE;
    }

    *total = subtotal - *discount;
}

// Update inventory using pointer (call by address)
void updateInventoryStock(Product* product, int quantitySold) {
    if (product == NULL) {
        printError("Invalid product pointer!");
        return;
    }

    if (product->quantity < quantitySold) {
        printError("Insufficient stock!");
        return;
    }

    product->quantity -= quantitySold;
}

// Get product statistics using pointers (call by address)
void getProductStatistics(int* totalProducts, int* lowStock, int* outOfStock, double* totalValue) {
    *totalProducts = 0;
    *lowStock = 0;
    *outOfStock = 0;
    *totalValue = 0.0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].active) {
            (*totalProducts)++;
            *totalValue += products[i].price * products[i].quantity;

            if (products[i].quantity == 0) {
                (*outOfStock)++;
            } else if (products[i].quantity <= MIN_STOCK_THRESHOLD) {
                (*lowStock)++;
            }
        }
    }
}

void purchaseProduct() {
    clearScreen();
    printTableHeader("PURCHASE PRODUCT");

    string name;
    clearInputBuffer();
    cout << "Enter Product Name: ";
    getline(cin, name);

    if (name.empty()) {
        printError("Product name cannot be empty!");
        return;
    }

    int index = findProductByName(name);
    if (index == -1) {
        printError("Product not found!");
        return;
    }

    int quantity;
    cout << "Enter Quantity to Purchase: ";
    if (!(cin >> quantity)) {
        clearInputBuffer();
        printError("Invalid input! Quantity must be a number.");
        return;
    }

    if (quantity <= 0) {
        printError("Invalid quantity! Must be positive.");
        return;
    }

    if (products[index].quantity < quantity) {
        printError("Insufficient stock!");
        cout << "Available: " << products[index].quantity << "\n";
        cout << "Requested: " << quantity << "\n";
        return;
    }

    // Calculate pricing using pointer-based function
    double unitPrice = products[index].price;
    double subtotal = unitPrice * quantity;
    double discount = 0.0;
    double total = 0.0;

    // Call by address - discount and total are calculated via pointers
    calculateDiscount(quantity, subtotal, &discount, &total);

    // Update inventory using pointer
    updateInventoryStock(&products[index], quantity);

    // Record transaction
    recordTransaction(products[index].id, products[index].name,
                     quantity, unitPrice, discount, total);

    // Display invoice
    clearScreen();
    cout << "\n================================================================\n";
    cout << "                         INVOICE                                \n";
    cout << "================================================================\n";
    cout << "Transaction ID: " << (nextTransactionId - 1) << "\n";
    cout << "Date: " << getCurrentDate() << "  Time: " << getCurrentTime() << "\n";
    cout << "----------------------------------------------------------------\n";
    cout << left << setw(25) << "Product" << setw(10) << "Qty"
         << setw(12) << "Unit Price" << "Amount\n";
    cout << "----------------------------------------------------------------\n";
    cout << left << setw(25) << products[index].name
         << setw(10) << quantity
         << "$" << setw(11) << fixed << setprecision(2) << unitPrice
         << "$" << fixed << setprecision(2) << subtotal << "\n";
    cout << "----------------------------------------------------------------\n";
    cout << right << setw(47) << "Subtotal: $" << fixed << setprecision(2) << subtotal << "\n";

    if (discount > 0) {
        cout << right << setw(47) << "Discount (10%): -$" << fixed << setprecision(2) << discount << "\n";
        printWarning("Bulk discount applied!");
    }

    cout << right << setw(47) << "TOTAL: $" << fixed << setprecision(2) << total << "\n";
    cout << "================================================================\n";

    if (products[index].quantity <= MIN_STOCK_THRESHOLD) {
        printWarning("Low stock alert for this product!");
    }
}

void viewTransactionHistory() {
    if (transactionCount == 0) {
        printError("No transactions recorded!");
        return;
    }

    clearScreen();
    printTableHeader("TRANSACTION HISTORY");

    cout << left << setw(8) << "Trans#" << setw(20) << "Product"
         << setw(6) << "Qty" << setw(12) << "Unit Price"
         << setw(12) << "Discount" << setw(12) << "Total"
         << setw(12) << "Date" << "Time\n";
    cout << "--------------------------------------------------------------------------------\n";

    double totalRevenue = 0.0;

    for (int i = transactionCount - 1; i >= 0; i--) {
        cout << left << setw(8) << transactions[i].transactionId
             << setw(20) << transactions[i].productName
             << setw(6) << transactions[i].quantity
             << "$" << setw(11) << fixed << setprecision(2) << transactions[i].unitPrice
             << "$" << setw(11) << fixed << setprecision(2) << transactions[i].discount
             << "$" << setw(11) << fixed << setprecision(2) << transactions[i].totalPrice
             << setw(12) << transactions[i].date
             << transactions[i].time << "\n";

        totalRevenue += transactions[i].totalPrice;
    }

    cout << "--------------------------------------------------------------------------------\n";
    cout << right << setw(70) << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << "\n";
    cout << "================================================================================\n";
}

// ============================================================
// REPORTS & ANALYTICS
// ============================================================
void generateInventoryReport() {
    clearScreen();
    printTableHeader("INVENTORY REPORT");

    int totalProducts = 0;
    int lowStockProducts = 0;
    int outOfStockProducts = 0;
    double totalInventoryValue = 0.0;

    // Use pointer-based function to get statistics
    getProductStatistics(&totalProducts, &lowStockProducts, &outOfStockProducts, &totalInventoryValue);

    cout << "Total Products:        " << totalProducts << "\n";
    cout << "Low Stock Products:    " << lowStockProducts << "\n";
    cout << "Out of Stock Products: " << outOfStockProducts << "\n";
    cout << "Total Inventory Value: $" << fixed << setprecision(2) << totalInventoryValue << "\n";
    cout << "Total Categories:      " << categoryCount << "\n";
    cout << "Total Suppliers:       " << supplierCount << "\n";
    cout << "Total Transactions:    " << transactionCount << "\n";
    cout << "================================================================\n";
}

// ============================================================
// SAMPLE DATA
// ============================================================
void loadSampleData() {
    // Add sample categories
    categories[categoryCount++] = {"Electronics", "Electronic devices and accessories", true};
    categories[categoryCount++] = {"Clothing", "Apparel and fashion items", true};
    categories[categoryCount++] = {"Food", "Food and beverages", true};

    // Add sample products
    products[productCount++] = {1, "Laptop", "Electronics", 15, 899.99, true};
    products[productCount++] = {2, "Mouse", "Electronics", 50, 19.99, true};
    products[productCount++] = {3, "Keyboard", "Electronics", 3, 49.99, true};
    products[productCount++] = {4, "T-Shirt", "Clothing", 100, 15.99, true};
    products[productCount++] = {5, "Jeans", "Clothing", 2, 39.99, true};

    // Add sample suppliers
    suppliers[supplierCount++] = {"TechSupply Co", "tech@supply.com", true};
    suppliers[supplierCount++] = {"Fashion World", "contact@fashion.com", true};
}

// ============================================================
// MAIN MENU
// ============================================================
void displayMainMenu() {
    cout << "\n";
    cout << "================================================================\n";
    cout << "           SMART INVENTORY MANAGEMENT SYSTEM                    \n";
    cout << "================================================================\n";
    cout << "  CATEGORY MANAGEMENT\n";
    cout << "    1. Add Category\n";
    cout << "    2. View All Categories\n";
    cout << "    3. Delete Category\n";
    cout << "----------------------------------------------------------------\n";
    cout << "  PRODUCT MANAGEMENT\n";
    cout << "    4. Add Product\n";
    cout << "    5. Update Product\n";
    cout << "    6. Delete Product\n";
    cout << "    7. Search Product\n";
    cout << "    8. View All Products\n";
    cout << "----------------------------------------------------------------\n";
    cout << "  SALES & TRANSACTIONS\n";
    cout << "    9. Purchase Product\n";
    cout << "    10. View Transaction History\n";
    cout << "----------------------------------------------------------------\n";
    cout << "  SUPPLIER MANAGEMENT\n";
    cout << "   11. Add Supplier\n";
    cout << "   12. View All Suppliers\n";
    cout << "   13. Delete Supplier\n";
    cout << "----------------------------------------------------------------\n";
    cout << "  REPORTS & ANALYTICS\n";
    cout << "   14. Inventory Report\n";
    cout << "   15. Check Low Stock Alerts\n";
    cout << "----------------------------------------------------------------\n";
    cout << "    0. Exit\n";
    cout << "================================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    loadSampleData();

    int choice;

    do {
        displayMainMenu();

        if (!(cin >> choice)) {
            clearInputBuffer();
            printError("Invalid input! Please enter a number.");
            pauseScreen();
            clearScreen();
            continue;
        }

        switch (choice) {
            case 1:
                addCategory();
                pauseScreen();
                clearScreen();
                break;
            case 2:
                displayCategoriesTable();
                pauseScreen();
                clearScreen();
                break;

            case 3:
                deleteCategory();
                pauseScreen();
                clearScreen();
                break;

            case 4:
                addProduct();
                pauseScreen();
                clearScreen();
                break;
            case 5:
                updateProduct();
                pauseScreen();
                clearScreen();
                break;
            case 6:
                deleteProduct();
                pauseScreen();
                clearScreen();
                break;
            case 7:
                searchProduct();
                pauseScreen();
                clearScreen();
                break;
            case 8:
                displayProductsTable();
                pauseScreen();
                clearScreen();
                break;
            case 9:
                purchaseProduct();
                pauseScreen();
                clearScreen();
                break;
            case 10:
                viewTransactionHistory();
                pauseScreen();
                clearScreen();
                break;
            case 11:
                addSupplier();
                pauseScreen();
                clearScreen();
                break;
            case 12:
                displaySuppliersTable();
                pauseScreen();
                clearScreen();
                break;
            case 13:
                deleteSupplier();
                pauseScreen();
                clearScreen();
                break;
            case 14:
                generateInventoryReport();
                pauseScreen();
                clearScreen();
                break;
            case 15:
                clearScreen();
                checkLowStock();
                pauseScreen();
                clearScreen();
                break;
            case 0:
                clearScreen();
                cout << "\n================================================================\n";
                cout << "  Thank you for using Smart Inventory Management System!\n";
                cout << "================================================================\n\n";
                break;
            default:
                printError("Invalid choice! Please try again.");
                pauseScreen();
                clearScreen();
                break;
        }

    } while (choice != 0);

    return 0;
}
