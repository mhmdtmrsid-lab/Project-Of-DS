📄 README – Supermarket Management System
🛒 Supermarket Management System (C++ Project)
This project is a console‑based application that manages products, customers, shopping carts, and provides an undo system.
It uses advanced data structures such as Hash Table, BST, Queue, Linked List, and Stack.

## 🚀 How to Run the Application
1. Requirements
C++ compiler (G++ / MinGW / MSVC)

C++11 or later

Any IDE such as:

Code::Blocks

Visual Studio

VS Code

CLion

## ▶️ How to Use the Application
When you run the program, you will see a main menu with multiple choices.

Below is a full guide on how to use each part of the application.

📌 Main Menu Features
1️⃣ Product Management
You can:

✔ Add a Product
Enter: name, barcode, price, quantity, expiry date

Product is added into:

Hash Table

BST sorted by price

BST sorted by expiry date

✔ Edit a Product
Search by barcode

Update:

Name

Price

Quantity

Expiry

✔ Delete a Product
Enter barcode

Product is removed from all structures

✔ Display Products
Options:

Show products sorted by price

Show products sorted by expiry date

2️⃣ Customer Management
✔ Add Customer to Queue
Enter customer ID

Customer joins the waiting line

✔ Serve Customer
The first customer in the queue is served and removed

3️⃣ Shopping Cart
Every customer has their own cart.
You can:

✔ Add Item to Cart
Enter barcode

Enter quantity

If the product exists, it’s added to the customer's cart

✔ Edit Item Quantity
Change the amount of a previously added product

✔ Remove Item
Delete product from cart

✔ Clear Cart
Removes all cart items

✔ View Cart
Shows the list of products

Shows total price

4️⃣ Undo System
Every action you make can be undone:

Undo add product

Undo edit product

Undo delete product

Undo cart operations

Undo works using a stack to restore previous states.

## 🧪 Example Workflow
1. Add a product
1 → Add Product  
Enter Name: Milk  
Enter Price: 30  
Enter Barcode: 111  
Enter Quantity: 50  
Enter Expiry Date: 01/01/2026
2. Add customer
4 → Add Customer  
Enter ID: 1001
3. Add product to cart
7 → Add item to cart  
Enter Customer ID: 1001  
Enter Barcode: 111  
Enter Quantity: 2
4. View cart
10 → View cart for customer  
Enter ID: 1001
## 🔥 Key Features
Fast product lookup using Hash Table

Multi-criteria sorting using BST

Full shopping cart system

Customer queue simulation

Undo for all operations

Automatic restructuring when products change

## 📎 GitHub Link
https://github.com/mhmdtmrsid-lab/Project-Of-DS.git
