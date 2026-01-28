# Smart Inventory Management System

A comprehensive console-based inventory management system built with C++ using fundamental programming concepts.

## 🎯 Project Overview

This system helps small to medium-sized businesses manage their inventory efficiently with features like automated stock alerts, bulk discounts, transaction tracking, and comprehensive reporting.

## ✨ Key Features

- **Product Management**: Add, update, delete, and search products
- **Smart Alerts**: Automatic low stock warnings (≤5 units)
- **Bulk Discounts**: 10% automatic discount for purchases of 5+ items
- **Transaction History**: Complete sales records with timestamps
- **Analytics**: Inventory reports with revenue tracking
- **Category Management**: Organize products by categories
- **Supplier Management**: Track supplier information
- **Data Validation**: Comprehensive input validation and error handling

## 🛠️ Technical Details

- **Language**: C++ (C++11 or later)
- **Programming Paradigm**: Procedural/Structured Programming
- **Data Structures**: Arrays, Structures
- **Advanced Concepts**: Pointers, Call by Address, Call by Reference
- **Lines of Code**: 800+
- **Functions**: 40+

### Key Programming Concepts Demonstrated

This project showcases fundamental and intermediate C++ programming concepts:

**Basic Concepts:**

- Arrays for data storage
- Structures for custom data types
- Functions for modular code organization
- Loops (for, while, do-while) for iteration
- Conditionals (if-else, switch-case) for decision making
- String handling for text data

**Advanced Concepts:**

- **Pointers** for efficient data access and manipulation
- **Call by Address** for modifying multiple values in functions
- **Call by Reference** using pointers for efficient parameter passing
- Memory address manipulation
- Dereference operators for accessing pointer values

### Pointer Usage Examples

The system includes several functions that demonstrate pointer usage:

1. **validateProductData()** - Uses pointers to validate multiple product attributes efficiently
2. **calculateDiscount()** - Uses pointers to return both discount and total values
3. **updateInventoryStock()** - Uses pointer to Product structure for direct modification
4. **getProductStatistics()** - Uses pointers to return multiple statistical values

These pointer-based functions demonstrate:

- How to pass parameters by address
- How to modify multiple values through pointers
- How to work with pointers to structures
- The efficiency benefits of avoiding data copying

## 📋 Prerequisites

- C++ Compiler (GCC, Clang, or MSVC)
- Terminal/Console access
- Basic understanding of command-line operations

## 🚀 Compilation & Execution

### Windows (using g++)

```bash
g++ -std=c++11 main.cpp -o inventory.exe
inventory.exe
```

### Windows (using MSVC)

```bash
cl /EHsc main.cpp
main.exe
```

### Linux/Mac

```bash
g++ -std=c++11 main.cpp -o inventory
./inventory
```

## 📖 How to Use

### 1. Starting the Program

Run the compiled executable. You'll see the main menu with 15 options.

### 2. Sample Data

The system comes pre-loaded with sample data:

- **Categories**: Electronics, Clothing, Food
- **Products**: Laptop, Mouse, Keyboard, T-Shirt, Jeans
- **Suppliers**: TechSupply Co, Fashion World

### 3. Basic Workflow

#### Add a Category First

```
Select: 8 (Add Category)
Enter name: Books
Enter description: Books and magazines
```

#### Add a Product

```
Select: 1 (Add Product)
Enter ID: 10
Enter name: Programming Book
Enter category: Books
Enter quantity: 20
Enter price: 49.99
```

#### Make a Purchase

```
Select: 6 (Purchase Product)
Enter product name: Programming Book
Enter quantity: 6
→ Automatic 10% discount applied!
→ Invoice generated
→ Stock updated
```

#### Check Stock Alerts

```
Select: 15 (Check Low Stock Alerts)
→ Shows all products with quantity ≤ 5
```

#### View Reports

```
Select: 14 (Inventory Report)
→ Total products, inventory value, revenue, etc.
```

## 📊 Menu Options


### Category Management 

1. Add Category
2. View All Categories
3. Delete Category

### Product Management

4. Add Product
5. Update Product
6. Delete Product
7. Search Product
8. View All Products

### Sales & Transactions

9. Purchase Product (with auto-discount)
10. View Transaction History

### Supplier Management 

11. Add Supplier
12. View All Suppliers
13. Delete Supplier

### Reports & Analytics

14. Inventory Report
15. Check Low Stock Alerts

### Exit (0)

0. Exit Program

## 🧪 Testing Scenarios

### Test 1: Bulk Discount

1. Purchase 6 units of any product
2. Verify 10% discount is applied
3. Check invoice shows discount line

### Test 2: Low Stock Alert

1. Add product with quantity ≤ 5
2. View all products
3. Verify "LOW" status appears
4. Check alert message displays

### Test 3: Out of Stock

1. Purchase all units of a product
2. Try to purchase more
3. Verify "Insufficient stock" error

### Test 4: Duplicate Prevention

1. Try adding product with existing ID
2. Verify error message appears
3. Product not added

### Test 5: Category Validation

1. Try adding product with non-existent category
2. Verify error message
3. Product not added

## 📈 Sample Output

### Invoice Example

================================================================
                         INVOICE                            
================================================================
Transaction ID: 1
Date: 19/01/2026  Time: 14:30:45
----------------------------------------------------------------
Product                   Quantity       Unit Price  Amount
----------------------------------------------------------------
Laptop                    6         $899.99     $5399.94
----------------------------------------------------------------
                                    Subtotal: $5399.94
                              Discount (10%): -$539.99
[WARNING] Bulk discount applied!
                                       TOTAL: $4859.95
================================================================

### Low Stock Alert Example

```
[WARNING] LOW STOCK ALERT!
----------------------------------------------------------------
ID    Product             Quantity    Status
----------------------------------------------------------------
3     Keyboard            3           LOW STOCK
5     Jeans               2           LOW STOCK
----------------------------------------------------------------
```

## 🔧 Configuration

You can modify these constants in `main.cpp`:

```cpp
const int MAX_ITEMS = 100;              // Max products/categories/suppliers
const int MAX_TRANSACTIONS = 200;        // Max transaction records
const int MIN_STOCK_THRESHOLD = 5;       // Low stock alert threshold
const double BULK_DISCOUNT_THRESHOLD = 5; // Min quantity for discount
const double BULK_DISCOUNT_RATE = 0.10;  // Discount percentage (10%)
```

## 📁 Project Structure


├── Inventory_management_system.cpp           # Main source code
├── README.md                                 # This file
└── Group4 IVENTORY MANAGEMENT.doc            # Complete project documentation

## 🎓 Educational Value

This project demonstrates:

- ✅ Arrays and array manipulation
- ✅ Structures (custom data types)
- ✅ Functions and modular programming
- ✅ Loops (for, while, do-while)
- ✅ Conditionals (if-else, switch-case)
- ✅ String handling
- ✅ Input validation
- ✅ Algorithm implementation
- ✅ Problem-solving skills

## 🐛 Known Limitations

- Data is not persisted (lost on program exit)
- Product names must be entered exactly for purchases
- Fixed array sizes (not dynamic)
- Console-based interface only
- Single-user system (no concurrent access)

## 🔮 Future Enhancements

- [ ] File-based data persistence
- [ ] User authentication system
- [ ] Advanced search capabilities
- [ ] Export reports to PDF/Excel
- [ ] Graphical user interface (GUI)
- [ ] Database integration
- [ ] Multi-user support
- [ ] Barcode scanning support

## 📝 License

This project is created for educational purposes as part of a group assignment.
**Version**: 1.0
**Last Updated**: January 19, 2026
**Status**: Complete and Tested

