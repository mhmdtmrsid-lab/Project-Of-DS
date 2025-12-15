#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;

// ============ Forward Declarations ============
class product;
class product_manger;

// ============ Helper Functions ============
void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool validateQuantity(int qty)
{
    return qty >= 0;
}

bool validatePrice(float price)
{
    return price > 0;
}

bool validateBarcode(int barcode)
{
    return barcode > 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
// ============ Product Class ============
///////////////////////////////////////////////////////////////////////////////////////////
class product
{
private :
    int barcode ;
    float price ;
    string name ;
    int quantity ;
    string expiryDate;
///////////////////////////////////////////////////
public :
    product (string n = "" , float p = 0 , int b = 0 , int q = 0 , string e = "")
    {
        name = n;
        price = p;
        barcode = b;
        quantity = q;
        expiryDate = e;
    }
///////////////////////////////////////////////////
    product(const product& other)
    {
        name = other.name;
        price = other.price;
        barcode = other.barcode;
        quantity = other.quantity;
        expiryDate = other.expiryDate;
    }
///////////////////////////////////////////////////
    product &operator =(const product& other)
    {
        if (this != &other)
        {
            name = other.name;
            price = other.price;
            barcode = other.barcode;
            quantity = other.quantity;
            expiryDate = other.expiryDate;
        }
        return *this;
    }
///////////////////////////////////////////////////
/////////////        getter        ////////////////
///////////////////////////////////////////////////
    string getName() const
    {
        return name;
    }

    float getPrice() const
    {
        return price;
    }

    int getBarcode() const
    {
        return barcode;
    }

    int getQuantity() const
    {
        return quantity;
    }

    string getExpiryDate() const
    {
        return expiryDate;
    }
///////////////////////////////////////////////////
/////////////        setter        ////////////////
///////////////////////////////////////////////////
    void setName(string n)
    {
        name = n;
    }

    void setPrice(float p)
    {
        price = p;
    }

    void setBarcode(int b)
    {
        barcode = b;
    }

    void setQuantity(int q)
    {
        quantity = q;
    }

    void setExpiryDate(string e)
    {
        expiryDate = e;
    }
///////////////////////////////////////////////////
    void display () const
    {
        cout << "Name        : "  << name << endl;
        cout << "Price       : $" << fixed << setprecision(2) << price << endl;
        cout << "Barcode     : "  << barcode << endl;
        cout << "Quantity    : "  << quantity << endl;
        cout << "Expiry Date : "  << expiryDate << endl;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////
// ============ Action Struct ============
///////////////////////////////////////////////////////////////////////////////////////////
struct Action
{
    string type;
    product item;
    int oldQty;
    int newQty;
    int customerID;
    string oldName;
    float oldPrice;
    string oldExpiry;
    vector<product> cartItems;
///////////////////////////////////////////////////
    Action(string t = "", product p = product(), int oldQ = 0, int newQ = 0,
           int cID = -1, string oName = "", float oPrice = 0, string oExpiry = "",
           vector<product> items = {})
    {
        type = t;
        item = p;
        oldQty = oldQ;
        newQty = newQ;
        oldName = oName;
        customerID = cID;
        oldPrice = oPrice;
        cartItems = items;
        oldExpiry = oExpiry;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////
// ============ Stack Node ============
///////////////////////////////////////////////////////////////////////////////////////////
class StackNode
{
public :
    Action action;
    StackNode* next;

    StackNode(Action a)
    {
        action = a;
        next = nullptr;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////
// ============ Action Stack ============
///////////////////////////////////////////////////////////////////////////////////////////
class ActionStack
{
private :
    StackNode* top;

public :

    ActionStack() { top = nullptr; }

    ~ActionStack() 
    {
        while (!isEmpty()) 
        {
            pop();
        }
    }
///////////////////////////////////////////////////
    bool isEmpty() const { return top == nullptr; }

    void push(Action a)
    {
        StackNode* newNode = new StackNode(a);
        newNode->next = top;
        top = newNode;
    }
///////////////////////////////////////////////////
    Action pop()
    {
        if (isEmpty())
        {
            cout << "No actions to undo!\n";
            return Action();
        }

        StackNode* del = top;
        Action a = del->action;
        top = top->next;
        delete del;
        return a;
    }
///////////////////////////////////////////////////
    Action peek() const
    {
        if (isEmpty()) 
        {
            return Action();
        }
        
        else 
        {
            return top->action;
        }
    }
};
///////////////////////////////////////////////////////////////////////////////////////////
// ============ BST Classes ============
///////////////////////////////////////////////////////////////////////////////////////////
class ProductBST
{
private :

    struct BSTNode
    {
        product item;
        BSTNode* left;
        BSTNode* right;

        BSTNode(product p)
        {
            item = p;
            left = nullptr;
            right = nullptr;
        }
    };

    BSTNode* root;
///////////////////////////////////////////////////
    BSTNode* findNodeByBarcode(BSTNode* node, int barcode)
    {
        if (!node) return nullptr;

        if (barcode == node->item.getBarcode())
        {
            return node;
        }

        BSTNode* leftResult = findNodeByBarcode(node->left, barcode);
        if (leftResult) return leftResult;

        return findNodeByBarcode(node->right, barcode);
    }
///////////////////////////////////////////////////
    BSTNode* insertByPrice(BSTNode* node , product p)
    {
        if (!node) {
            return new BSTNode(p);
        }
        else {
            if (p.getPrice() < node->item.getPrice())
                node->left = insertByPrice(node->left, p);
            else
                node->right = insertByPrice(node->right, p);

            return node;
        }
    }
///////////////////////////////////////////////////
    BSTNode* removeNodeByBarcode(BSTNode* node, int barcode, bool& found, product& removedProduct)
    {
        if (!node) 
        {
            found = false;
            return nullptr;
        }
///////////////////////////////////////////////////
        if (barcode == node->item.getBarcode())
        {
            found = true;
            removedProduct = node->item;

            if (!node->left)
            {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            
            else if (!node->right)
            {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            
            else
            {
                BSTNode* temp = minValueNode(node->right);
                product successorCopy = temp->item; // copy successor
                node->item = successorCopy;
                node->right = removeNodeByBarcode(node->right, successorCopy.getBarcode(), found, removedProduct);
                return node;
            }
        }
///////////////////////////////////////////////////
        node->left = removeNodeByBarcode(node->left, barcode, found, removedProduct);
        if (!found) 
        {
            node->right = removeNodeByBarcode(node->right, barcode, found, removedProduct);
        }
        return node;
    }
///////////////////////////////////////////////////
    BSTNode* minValueNode(BSTNode* node)
    {
        BSTNode* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }
///////////////////////////////////////////////////
    void inorderDisplay(BSTNode* node) const
    {
        if (!node) return;
        inorderDisplay(node->left);
        node->item.display();
        cout << "-------------------\n";
        inorderDisplay(node->right);
    }
///////////////////////////////////////////////////
    void deleteTree(BSTNode* node)
    {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public :

    ProductBST() { root = nullptr; }

    ~ProductBST()
    {
        deleteTree(root);
    }
///////////////////////////////////////////////////
    void insertProductByPrice(product p)
    {
        root = insertByPrice(root, p);
    }
///////////////////////////////////////////////////
    bool removeProduct(int barcode)
    {
        bool found = false;
        product dummy;
        root = removeNodeByBarcode(root, barcode, found, dummy);
        return found;
    }
///////////////////////////////////////////////////
    void updateProduct(product updatedProduct)
    {
        removeProduct(updatedProduct.getBarcode());
        insertProductByPrice(updatedProduct);
    }
///////////////////////////////////////////////////
    product* findProduct(int barcode)
    {
        BSTNode* node = findNodeByBarcode(root, barcode);
        if (node)
        {
            return &node->item;
        }
        else
        {
            return nullptr;
        }
    }
///////////////////////////////////////////////////
    void displayInOrder() const
    {
        if (!root)
        {
            cout << "No products in BST.\n";
            return;
        }
        else {
            cout << "\n=== Products Sorted by Price (Lowest to Highest) ===\n";
            inorderDisplay(root);
        }
    }
};
///////////////////////////////////////////////////////////////////////////////////////////
class ProductBSTByExpiry
{
private :
    struct BSTNode
    {
        product item;
        BSTNode* left;
        BSTNode* right;

        BSTNode(product p)
        {
            item = p;
            left = nullptr;
            right = nullptr;
        }
    };

    BSTNode* root;
///////////////////////////////////////////////////

    BSTNode* findNodeByBarcode(BSTNode* node, int barcode)
    {
        if (!node) return nullptr;

        if (barcode == node->item.getBarcode())
        {
            return node;
        }

        BSTNode* leftResult = findNodeByBarcode(node->left, barcode);
        if (leftResult) return leftResult;

        return findNodeByBarcode(node->right, barcode);
    }
///////////////////////////////////////////////////
    BSTNode* insertByExpiry(BSTNode* node, product p)
    {
        if (!node)
        {
            return new BSTNode(p);
        }
        else
        {
            if (p.getExpiryDate() < node->item.getExpiryDate())
                node->left = insertByExpiry(node->left, p);
            else
                node->right = insertByExpiry(node->right, p);

            return node;
        }
    }
///////////////////////////////////////////////////
    BSTNode* removeNodeByBarcode(BSTNode* node, int barcode, bool& found, product& removedProduct)
    {
        if (!node) 
        {
            found = false;
            return nullptr;
        }
///////////////////////////////////////////////////
        if (barcode == node->item.getBarcode())
        {
            found = true;
            removedProduct = node->item;

            if (!node->left)
            {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            
            else if (!node->right)
            {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            
            else
            {
                BSTNode* temp = minValueNode(node->right);
                product successorCopy = temp->item;
                node->item = successorCopy;
                node->right = removeNodeByBarcode(node->right, successorCopy.getBarcode(), found, removedProduct);
                return node;
            }
        }
///////////////////////////////////////////////////
        node->left = removeNodeByBarcode(node->left, barcode, found, removedProduct);
        if (!found) 
        {
            node->right = removeNodeByBarcode(node->right, barcode, found, removedProduct);
        }
        return node;
    }
///////////////////////////////////////////////////
    BSTNode* minValueNode(BSTNode* node)
    {
        BSTNode* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }
///////////////////////////////////////////////////
    void inorderDisplay(BSTNode* node) const
    {
        if (!node) return;
        inorderDisplay(node->left);
        node->item.display();
        cout << "-------------------\n";
        inorderDisplay(node->right);
    }
///////////////////////////////////////////////////
    void deleteTree(BSTNode* node)
    {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
///////////////////////////////////////////////////
public :
    ProductBSTByExpiry()
    {
        root = nullptr;
    }
///////////////////////////////////////////////////
    ~ProductBSTByExpiry()
    {
        deleteTree(root);
    }
///////////////////////////////////////////////////
    void insertProduct(product p)
    {
        root = insertByExpiry(root, p);
    }
///////////////////////////////////////////////////
    bool removeProduct(int barcode)
    {
        bool found = false;
        product dummy;
        root = removeNodeByBarcode(root, barcode, found, dummy);
        return found;
    }
///////////////////////////////////////////////////
    void updateProduct(product updatedProduct)
    {
        removeProduct(updatedProduct.getBarcode());
        insertProduct(updatedProduct);
    }
///////////////////////////////////////////////////
    product* findProduct(int barcode)
    {
        BSTNode* node = findNodeByBarcode(root, barcode);
        if (node)
        {
            return &node->item;
        }
        else
        {
            return nullptr;
        }
    }
///////////////////////////////////////////////////
    void displayInOrder() const
    {
        if (!root)
        {
            cout << "No products in BST.\n";
            return;
        }
        else {
            cout << "\n=== Products Sorted by Expiry Date (Earliest to Latest) ===\n";
            inorderDisplay(root);
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////////////
// ============ Cart Node ============
///////////////////////////////////////////////////////////////////////////////////////////
class cart_node
{
private :
    product item;          
    int quantity ;
    cart_node* next ;
    cart_node* prev ;
///////////////////////////////////////////////////
public :
    cart_node(product i = product() , int q = 0)
        : item(i), quantity(q), next(nullptr), prev(nullptr){}

    product* getItem() { return &item; }
    const product* getItem() const { return &item; }
    cart_node* getNext() const { return next; }
    cart_node* getPrev() const { return prev; }
    int getQuantity() const { return quantity; }

///////////////////////////////////////////////////   
    
    void setNext(cart_node* n) { next = n; }
    void setPrev(cart_node* p) { prev = p; }
    void setQuantity(int q) { quantity = q; }
};
///////////////////////////////////////////////////////////////////////////////////////////
// ============ Linked List ============
///////////////////////////////////////////////////////////////////////////////////////////
class linked_list
{
public :

    cart_node* head ;
    cart_node* tail ;

    linked_list()
    {
        head = nullptr ;
        tail = nullptr ;
    }

    ~linked_list()
    {
        clear_cart();
    }

    bool isempty () const { return (head == nullptr); }
///////////////////////////////////////////////////   

    void add_to_cart (product* item , int q)
    {
        if (!item || q <= 0) return;

        cart_node* temp = head;
        
        while (temp != nullptr)
        {
            if (temp->getItem()->getBarcode() == item->getBarcode())
            {
                temp->setQuantity(temp->getQuantity() + q);
                return;
            }
            temp = temp->getNext();
        }

        cart_node* newNode = new cart_node(*item , q);
///////////////////////////////////////////////////   
        if (isempty())
        {
            head = newNode ;
            tail = newNode ;
            return ;
        }

        else
        {
            tail->setNext(newNode);
            newNode->setPrev(tail);
            tail = newNode;
        }
    }
///////////////////////////////////////////////////   
    void remove_from_cart (int barcode)
    {
        if (isempty())
        {
            cout << "Cart is empty.\n";
            return ;
        }

        cart_node* current = head;
        
        while (current != nullptr && current->getItem()->getBarcode() != barcode)
        {
            current = current->getNext();
        }
///////////////////////////////////////////////////   
        if (current == nullptr)
        {
            cout << "Item not found in cart.\n";
            return;
        }

        if (current == head)
        {
            head = current->getNext();

            if (head != nullptr)
                head->setPrev(nullptr);
            else
                tail = nullptr;

            delete current;
            cout << "Item removed from cart.\n";
            return;
        }
///////////////////////////////////////////////////           
        else if (current == tail)
        {
            tail = current->getPrev();

            if (tail != nullptr)
                tail->setNext(nullptr);
            else
                head = nullptr;

            delete current;
            cout << "Item removed from cart.\n";
            return;
        }
///////////////////////////////////////////////////           
        else
        {
            cart_node* prevNode = current->getPrev();
            cart_node* nextNode = current->getNext();
            if (prevNode) prevNode->setNext(nextNode);
            if (nextNode) nextNode->setPrev(prevNode);
            delete current;
            cout << "Item removed from cart.\n";
        }
    }
///////////////////////////////////////////////////   
    void display_cart () const
    {
        if (isempty())
        {
            cout << "Cart is empty.\n";
            return;
        }

        cart_node* current = head;
///////////////////////////////////////////////////           
        cout << "\n=== Cart Contents ===\n";
        while (current != nullptr)
        {
            const product* item = current->getItem();
            cout << "• " << item->getName()
                 << " | Price: $" << fixed << setprecision(2) << item->getPrice()
                 << " | Barcode: " << item->getBarcode()
                 << " | Qty: " << current->getQuantity()
                 << " | Total: $" << fixed << setprecision(2) << (item->getPrice() * current->getQuantity()) << endl;
            current = current->getNext();
        }
        cout << "=====================\n";
    }
///////////////////////////////////////////////////   
    void update_quantity (int barcode , int newQuantity)
    {
        if (isempty())
        {
            cout << "Cart is empty.\n";
            return;
        }
        cart_node* current = head;
        
        while (current != nullptr)
        {
            if (current->getItem()->getBarcode() == barcode)
            {
                if (newQuantity <= 0) 
                {
                    remove_from_cart(barcode);
                } 
                
                else 
                {
                    current->setQuantity(newQuantity);
                    cout << "Quantity updated.\n";
                }
                return;
            }
            current = current->getNext();
        }
        cout << "Item not found in cart.\n";
    }
///////////////////////////////////////////////////   
    void clear_cart ()
    {
        cart_node* current = head;

        while (current != nullptr)
        {
            cart_node* nextNode = current->getNext();
            delete current;
            current = nextNode;
        }

        head = nullptr;
        tail = nullptr;
        cout << "Cart cleared successfully.\n";
    }
///////////////////////////////////////////////////   
    float total_price () const
    {
        if (isempty())
        {
            return 0.0f;
        }

        else
        {
            float sum = 0.0f;
            cart_node* current = head;
            while (current != nullptr)
            {
                const product* item = current->getItem();
                sum += item->getPrice() * current->getQuantity();
                current = current->getNext();
            }
            return sum;
        }
    }
///////////////////////////////////////////////////   
    int getItemQuantity(int barcode) const
    {
        cart_node* current = head;
        while (current != nullptr)
        {
            if (current->getItem()->getBarcode() == barcode)
            {
                return current->getQuantity();
            }
            current = current->getNext();
        }
        return 0;
    }
///////////////////////////////////////////////////   
    bool hasItem(int barcode) const
    {
        cart_node* current = head;
        while (current != nullptr)
        {
            if (current->getItem()->getBarcode() == barcode)
            {
                return true;
            }
            current = current->getNext();
        }
        return false;
    }
///////////////////////////////////////////////////   
    vector<product> getCartItems() const
    {
        vector<product> items;
        cart_node* current = head;

        while (current != nullptr)
        {
            items.push_back(*(current->getItem()));
            current = current->getNext();
        }
        return items;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////
// ============ Customer ============
///////////////////////////////////////////////////////////////////////////////////////////
class Customer
{
private :
    int id ;
    linked_list* cart;

public :
    Customer(int i)
    {
        id = i;
        cart = new linked_list();
    }

    ~Customer()
    {
        if (cart)
        {
            cart->clear_cart();
            delete cart;
        }
    }
///////////////////////////////////////////////////   
    int getID() const { return id; }
///////////////////////////////////////////////////   
    void addToCart(product* item , int quantity)
    {
        cart->add_to_cart(item, quantity);
    }
///////////////////////////////////////////////////   
    void removeFromCart(int barcode)
    {
        cart->remove_from_cart(barcode);
    }
///////////////////////////////////////////////////   
    void displayCart() const
    {
        cart->display_cart();
    }
///////////////////////////////////////////////////   
    void updateCartQuantity(int barcode , int newQty)
    {
        cart->update_quantity(barcode, newQty);
    }
///////////////////////////////////////////////////   
    void clearCart()
    {
        cart->clear_cart();
    }
///////////////////////////////////////////////////   
    float getTotalPrice() const
    {
        return cart->total_price();
    }
///////////////////////////////////////////////////   
    bool hasItem(int barcode) const
    {
        return cart->hasItem(barcode);
    }
///////////////////////////////////////////////////   
    int getCartItemQuantity(int barcode) const
    {
        return cart->getItemQuantity(barcode);
    }
///////////////////////////////////////////////////   
    linked_list* getCart() { return cart; }
    const linked_list* getCart() const { return cart; }
///////////////////////////////////////////////////   
    vector<product> getCartItems() const
    {
        return cart->getCartItems();
    }
};

///////////////////////////////////////////////////////////////////////////////////////////
// ============ Customer Queue ============
///////////////////////////////////////////////////////////////////////////////////////////
class CustomerQueue
{
private:
    Customer** queueArray;
    int frontIndex;
    int rearIndex;
    int capacity;
    int size;

    void resize()
    {
        int newCapacity = capacity * 2;
        Customer** newArray = new Customer*[newCapacity];

        for (int i = 0; i < size; i++)
        {
            newArray[i] = queueArray[(frontIndex + i) % capacity];
        }

        for (int i = size; i < newCapacity; ++i) newArray[i] = nullptr;
///////////////////////////////////////////////////   
        delete[] queueArray;
        queueArray = newArray;
        capacity = newCapacity;
        frontIndex = 0;
        rearIndex = size;

        cout << "[Queue Resized] New capacity: " << capacity << endl;
    }

public :
    CustomerQueue()
    {
        capacity = 10;
        queueArray = new Customer*[capacity];
        for (int i = 0; i < capacity; i++)
        {
            queueArray[i] = nullptr;
        }
        frontIndex = 0;
        rearIndex = 0;
        size = 0;
    }
///////////////////////////////////////////////////   
    ~CustomerQueue()
    {
        for (int i = 0; i < capacity; i++)
        {
            if (queueArray[i])
            {
                delete queueArray[i];
                queueArray[i] = nullptr;
            }
        }
        delete[] queueArray;
    }
///////////////////////////////////////////////////   
    bool isEmpty() const { return size == 0; }
    bool isFull() const { return size == capacity; }

    void enqueue(Customer* c)
    {
        if (isFull())
        {
            resize();
        }

        queueArray[rearIndex] = c;
        rearIndex = (rearIndex + 1) % capacity;
        size++;
    }
///////////////////////////////////////////////////   
    Customer* dequeue()
    {
        if (isEmpty())
        {
            return nullptr;
        }

        else
        {
            Customer* cust = queueArray[frontIndex];
            queueArray[frontIndex] = nullptr;
            frontIndex = (frontIndex + 1) % capacity;
            size--;
            return cust;
        }
    }
///////////////////////////////////////////////////   
    Customer* getFront() const
    {
        if (isEmpty())
        {
            return nullptr;
        }

        else
        {
            return queueArray[frontIndex];
        }
    }
///////////////////////////////////////////////////   
    void displayQueue() const
    {
        if (isEmpty())
        {
            cout << "Queue is empty.\n";
            return;
        }
///////////////////////////////////////////////////   
        cout << "\n=== Customers in Queue (Front to Rear) ===\n";
        for (int i = 0; i < size; i++)
        {
            int index = (frontIndex + i) % capacity;
            if (queueArray[index])
            {
                cout << "[" << (i+1) << "] Customer ID: " << queueArray[index]->getID()
                     << " | Items in cart: ";
                const linked_list* cart = queueArray[index]->getCart();
                if (cart->isempty()) {
                    cout << "Empty";
                } else {
                    cout << "Has items";
                }
                cout << endl;
            }
        }
        cout << "Total: " << size << " customer(s) in queue\n";
    }
///////////////////////////////////////////////////   
    Customer* findCustomerByID(int id) const
    {
        for (int i = 0; i < size; i++)
        {
            int index = (frontIndex + i) % capacity;
            if (queueArray[index] && queueArray[index]->getID() == id)
            {
                return queueArray[index];
            }
        }
        return nullptr;
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
};

///////////////////////////////////////////////////////////////////////////////////////////
// ============ Product Manager ============
///////////////////////////////////////////////////////////////////////////////////////////
class product_manger
{
private :
    vector<vector<product>> table ;
    int TABLE_SIZE ;
    int elements ;
    ProductBST* bst;
    ProductBSTByExpiry* bstExpiry;
    ActionStack* actionHistory;
///////////////////////////////////////////////////   
    int hash_Function(int barcode)
    {
        if (TABLE_SIZE == 0) return 0;
        return barcode % TABLE_SIZE ;
    }
///////////////////////////////////////////////////   
    void rehash()
    {
        int oldSize = TABLE_SIZE ;
        TABLE_SIZE *= 2 ;

        vector<vector<product>> newTable(TABLE_SIZE);

        for (int i = 0 ; i < oldSize ; i++)
        {
            for (auto & prod : table[i])
            {
                int newIndex = prod.getBarcode() % TABLE_SIZE;
                newTable[newIndex].push_back(prod);
            }
        }

        table = newTable;
        cout << "\n[Rehash Completed] New table size = " << TABLE_SIZE << endl;
    }
///////////////////////////////////////////////////   
    bool barcodeExists(int barcode)
    {
        int index = hash_Function(barcode);

        for (auto & prod : table[index])
        {
            if (prod.getBarcode() == barcode)
            {
                return true;
            }
        }
        return false;
    }
///////////////////////////////////////////////////   
    void recordAction(string type , product p , int oldQ , int newQ ,
                      int customerID = -1, string oldName = "",
                      float oldPrice = 0, string oldExpiry = "",
                      vector<product> cartItems = {})
    {
        Action a(type, p, oldQ, newQ, customerID, oldName, oldPrice, oldExpiry, cartItems);
        actionHistory->push(a);
    }
///////////////////////////////////////////////////   

    bool addProductNoRecord(product p)
    {
        if (barcodeExists(p.getBarcode()))
        {
            return false;
        }

        float load_Factor = (float)elements / TABLE_SIZE;
        if (load_Factor >= 0.75) rehash();

        int index = hash_Function(p.getBarcode());
        table[index].push_back(p);
        elements++;

        bst->insertProductByPrice(p);
        bstExpiry->insertProduct(p);

        return true;
    }
///////////////////////////////////////////////////   
    bool removeProductNoRecord(int barcode)
    {
        int index = hash_Function(barcode);
        for (auto it = table[index].begin() ; it != table[index].end() ; ++it)
        {
            if (it->getBarcode() == barcode)
            {
                bst->removeProduct(barcode);
                bstExpiry->removeProduct(barcode);

                table[index].erase(it);
                elements--;
                return true;
            }
        }
        return false;
    }
///////////////////////////////////////////////////   
public :
    product_manger()
    {
        elements = 0;
        TABLE_SIZE = 10;
        table.resize(TABLE_SIZE);
        bst = new ProductBST();
        bstExpiry = new ProductBSTByExpiry();
        actionHistory = new ActionStack();

        addDefaultProducts();
    }
///////////////////////////////////////////////////   
    ~product_manger()
    {
        delete bst;
        delete bstExpiry;
        delete actionHistory;
    }
///////////////////////////////////////////////////   
    void addDefaultProducts()
    {
        // منتجات سوبر ماركت حقيقية
        add_Product("Milk"   , 50.0f , 101 , 130  , "2025-12-31" , false);
        add_Product("Bread"  , 10.0f , 102 , 100  , "2025-12-19" , false);
        add_Product("Eggs"   , 20.0f , 103 , 150  , "2026-01-01" , false);
        add_Product("Rice"   , 30.0f , 104 , 180  , "2026-06-30" , false);
        add_Product("Sugar"  , 40.0f , 105 , 170  , "2026-03-15" , false);

        cout << "\n══════════════════════════════════════════\n";
        cout << "  5 Default Products Added Successfully!\n";
        cout << "══════════════════════════════════════════\n";
    }

    ActionStack* getActionHistory() { return actionHistory; }
    ProductBST* getBST() { return bst; }
    ProductBSTByExpiry* getBSTExpiry() { return bstExpiry; }
///////////////////////////////////////////////////   

    void input_Products()
    {
        int n;
        cout << "Enter number of products to add : ";
        cin  >> n;
        if (n <= 0) return;

        for (int i = 0 ; i < n ; i++)
        {
            cout << "\n══════════════════════════════════════════\n";
            cout << "  Product " << i + 1 << " Details\n";
            cout << "══════════════════════════════════════════\n";

            clearInput();
            string name;
            cout << "Name : ";
            getline(cin , name);

            float price;
            do {
                cout << "Price : $";
                cin  >>  price;
                if (!validatePrice(price)) {
                    cout << "Price must be positive!\n";
                }
            } while (!validatePrice(price));
///////////////////////////////////////////////////   
            int barcode;
            do {
                cout << "Barcode : ";
                cin  >>  barcode;

                if (!validateBarcode(barcode))
                {
                    cout << "Barcode must be positive!\n";
                }

                else if (barcodeExists(barcode))
                {
                    cout << "Barcode already exists! Enter unique barcode.\n";
                }
            } while (!validateBarcode(barcode) || barcodeExists(barcode));
///////////////////////////////////////////////////   
            int quantity;
            do {
                cout << "Quantity: ";
                cin  >>  quantity;

                if (!validateQuantity(quantity))
                {
                    cout << "Quantity cannot be negative!\n";
                }
            } while (!validateQuantity(quantity));
///////////////////////////////////////////////////   
            clearInput();
            string expiryDate;
            cout << "Expiry Date (YYYY-MM-DD): ";
            getline(cin , expiryDate);

            add_Product(name , price , barcode , quantity , expiryDate);
            cout << "Product '" << name << "' added successfully!\n";
        }
    }
///////////////////////////////////////////////////   
    void add_Product(string name, float price, int barcode, int quantity, string expiry, bool record = true)
    {
        if (barcodeExists(barcode))
        {
            cout << "Error : Barcode already exists!\n";
            return;
        }

        float load_Factor = (float)elements / TABLE_SIZE;
        if (load_Factor >= 0.75) rehash();
///////////////////////////////////////////////////   
        product p(name, price, barcode, quantity, expiry);
        int index = hash_Function(barcode);
        table[index].push_back(p);
        elements++;

        cout << "✓ " << name << " added successfully.\n";

        bst->insertProductByPrice(p);
        bstExpiry->insertProduct(p);

        if (record)
        {
            recordAction("add", p, 0, quantity);
        }
    }
///////////////////////////////////////////////////   
    void update_Product(int barcode, string newName, float newPrice, int newQuantity, string newExpiry, bool record = true)
    {
        if (elements == 0)
        {
            cout << "\nNo products available to update.\n";
            return;
        }
///////////////////////////////////////////////////   
        int index = hash_Function(barcode);
        for (auto & prod : table[index])
        {
            if (prod.getBarcode() == barcode)
            {
                string oldName = prod.getName();
                float oldPrice = prod.getPrice();
                int oldQty = prod.getQuantity();
                string oldExpiry = prod.getExpiryDate();

                if (!newName.empty()) prod.setName(newName);
                prod.setPrice(newPrice);
                prod.setQuantity(newQuantity);
                
                if (!newExpiry.empty()) prod.setExpiryDate(newExpiry);

                bst->updateProduct(prod);
                bstExpiry->updateProduct(prod);

                cout << "\n✓ Product updated successfully.\n";

                if (record)
                {
                    recordAction("edit", prod, oldQty, newQuantity, -1,
                                oldName, oldPrice, oldExpiry);
                }
                return;
            }
        }
        cout << "\n✗ Product not found.\n";
    }
///////////////////////////////////////////////////   
    void delete_Product(int barcode , bool record = true)
    {
        if (elements == 0)
        {
            cout << "\nNo products available to delete.\n";
            return;
        }

        int index = hash_Function(barcode);
        for (auto it = table[index].begin() ; it != table[index].end() ; ++it)
        {
            if (it->getBarcode() == barcode)
            {
                product removed = *it;

                bst->removeProduct(barcode);
                bstExpiry->removeProduct(barcode);

                table[index].erase(it);
                elements--;

                cout << "\n✓ Product deleted successfully.\n";

                if (record) {
                    recordAction("delete", removed, removed.getQuantity(), 0);
                }
                return;
            }
        }
        cout << "\n✗ Product not found.\n";
    }
///////////////////////////////////////////////////   
    void interactive_Product(const string & operation)
    {
        if (elements == 0)
        {
            cout << "\nNo products available to " << operation << ".\n";
            return;
        }

        int barcode;
        cout << "Enter barcode to " << operation << ": ";
        cin  >> barcode;

        if (operation == "update")
        {
            product* prod = findProduct(barcode);
            if (!prod)
            {
                cout << "Product with barcode " << barcode << " not found.\n";
                return;
            }

            cout << "\nCurrent Details:\n";
            prod->display();
            cout << "\nEnter new details:\n";

            string newName, newExpiry;
            float newPrice;
            int newQuantity;

            clearInput();
            cout << "New name [" << prod->getName() << "] : ";
            getline(cin, newName);
///////////////////////////////////////////////////   
            do {
                cout << "New price [" << prod->getPrice() << "] : $";
                cin  >> newPrice;

                if (!validatePrice(newPrice)) {
                    cout << "Price must be positive!\n";
                }
            } while (!validatePrice(newPrice));

            do {
                cout << "New quantity [" << prod->getQuantity() << "] : ";
                cin  >> newQuantity;

                if (!validateQuantity(newQuantity))
                {
                    cout << "Quantity cannot be negative!\n";
                }
            } while (!validateQuantity(newQuantity));

            clearInput();
            cout << "New expiry date [" << prod->getExpiryDate() << "] : ";
            getline(cin, newExpiry);

            update_Product(barcode, newName, newPrice, newQuantity, newExpiry);
        }
///////////////////////////////////////////////////   
        else if (operation == "delete")
        {
            product* prod = findProduct(barcode);
            if (!prod)
            {
                cout << "Product with barcode " << barcode << " not found.\n";
                return;
            }

            cout << "\nAre you sure you want to delete this product?\n";
            prod->display();
            cout << "\nEnter 'YES' to confirm: ";
            string confirm;
            clearInput();
            getline(cin, confirm);
///////////////////////////////////////////////////   
            if (confirm == "YES" || confirm == "yes")
            {
                delete_Product(barcode);
            }
            
            else
            {
                cout << "Deletion cancelled.\n";
            }
        }

        else 
        {
            cout << "Invalid operation!\n";
        }
    }
///////////////////////////////////////////////////   
    void display_Products()
    {
        if (elements == 0)
        {
            cout << "\nNo products available to display.\n";
            return;
        }

        cout << "\n══════════════════════════════════════════\n";
        cout << "           ALL PRODUCTS IN STORE\n";
        cout << "══════════════════════════════════════════\n";

        int totalProducts = 0;
        for (int i = 0 ; i < TABLE_SIZE ; i++)
        {
            for (auto & prod : table[i])
            {
                prod.display();
                cout << "-------------------\n";
                totalProducts++;
            }
        }
        cout << "Total Products: " << totalProducts << endl;
    }
///////////////////////////////////////////////////   
    product* findProduct(int barcode)
    {
        int index = hash_Function(barcode);

        for(auto & p : table[index])
        {
            if(p.getBarcode() == barcode)
                return &p;
        }
        return nullptr;
    }
///////////////////////////////////////////////////   
    void reduceQuantity(int barcode , int qty)
    {
        product* p = findProduct(barcode);
        if(p != nullptr)
        {
            int newQty = p->getQuantity() - qty;
            if (newQty < 0) newQty = 0;
            p->setQuantity(newQty);

            bst->updateProduct(*p);
            bstExpiry->updateProduct(*p);
        }
    }
///////////////////////////////////////////////////       
void increaseQuantity(int barcode, int qty)
    {
        product* p = findProduct(barcode);

        if(p != nullptr)
        {
            p->setQuantity(p->getQuantity() + qty);

            bst->updateProduct(*p);
            bstExpiry->updateProduct(*p);
        }
    }
///////////////////////////////////////////////////   
    void addToCartWithUndo(int barcode, int quantity, int customerID, CustomerQueue& customerQueue)
    {
        product* p = findProduct(barcode);

        if (p && p->getQuantity() >= quantity && quantity > 0)
        {
            reduceQuantity(barcode, quantity);
            recordAction("cart_add", *p, 0, quantity, customerID);
        }
    }
///////////////////////////////////////////////////   
    void removeFromCartWithUndo(int barcode, int oldQty, int customerID) 
    {
        product* p = findProduct(barcode);

        if (p)
        {
            increaseQuantity(barcode, oldQty);
            recordAction("cart_remove", *p, oldQty, 0, customerID);
        }
    }
///////////////////////////////////////////////////   
    void updateCartQuantityWithUndo(int barcode, int oldQty, int newQty, int customerID)
    {
        product* p = findProduct(barcode);

        if (p)
        {
            int diff = newQty - oldQty;

            if (diff > 0)
            {
                reduceQuantity(barcode, diff);
            }

            else
            {
                increaseQuantity(barcode, -diff);
            }
            recordAction("cart_edit", *p, oldQty, newQty, customerID);
        }
    }
///////////////////////////////////////////////////   
    void addToGuestCartWithUndo(int barcode, int quantity, linked_list& guestCart)
    {
        product* p = findProduct(barcode);

        if (p && p->getQuantity() >= quantity && quantity > 0)
        {
            reduceQuantity(barcode, quantity);
            recordAction("guest_cart_add", *p, 0, quantity, -2);
        }
    }
///////////////////////////////////////////////////   
    void removeFromGuestCartWithUndo(int barcode, int oldQty)
    {
        product* p = findProduct(barcode);
        if (p)
        {
            increaseQuantity(barcode, oldQty);
            recordAction("guest_cart_remove", *p, oldQty, 0, -2);
        }
    }
///////////////////////////////////////////////////   
    void updateGuestCartQuantityWithUndo(int barcode, int oldQty, int newQty)
    {
        product* p = findProduct(barcode);
        if (p)
        {
            int diff = newQty - oldQty;
            if (diff > 0)
            {
                reduceQuantity(barcode, diff);
            }

            else
            {
                increaseQuantity(barcode, -diff);
            }
            recordAction("guest_cart_edit", *p, oldQty, newQty, -2);
        }
    }
///////////////////////////////////////////////////   
    void clearCartWithUndo(Customer* customer, int customerID)
    {
        if (!customer) return;

        vector<product> cartItems = customer->getCartItems();

        recordAction("cart_clear", product(), 0, 0, customerID, "", 0, "", cartItems);

        customer->clearCart();
    }
///////////////////////////////////////////////////   
    void clearGuestCartWithUndo(linked_list& guestCart)
    {
        vector<product> cartItems = guestCart.getCartItems();

        recordAction("guest_cart_clear", product(), 0, 0, -2, "", 0, "", cartItems);

        guestCart.clear_cart();
    }
///////////////////////////////////////////////////   
    void undoLastAction(CustomerQueue& customerQueue, linked_list& guestCart)
    {
        if (actionHistory->isEmpty())
        {
            cout << "No actions to undo!\n";
            return;
        }

        Action a = actionHistory->peek(); // look first
        cout << "\n=== Undo Last Action ===\n";
        cout << "Action Type: " << a.type << endl;
///////////////////////////////////////////////////   
        if (a.type == "add")
        {
            cout << "Undoing: Add product '" << a.item.getName() << "'\n";
        }
        else if (a.type == "delete")
        {
            cout << "Undoing: Delete product '" << a.item.getName() << "'\n";
        }
        else if (a.type == "edit")
        {
            cout << "Undoing: Edit product '" << a.item.getName() << "'\n";
        }
        else if (a.type.find("cart") != string::npos)
        {
            cout << "Undoing: Cart operation\n";
        }
///////////////////////////////////////////////////   
        cout << "Enter 'YES' to confirm undo: ";
        string confirm;
        clearInput();
        getline(cin, confirm);

        if (confirm != "YES" && confirm != "yes")
        {
            cout << "Undo cancelled.\n";
            return;
        }

        a = actionHistory->pop(); // remove after confirmation
///////////////////////////////////////////////////   
        if (a.type == "add")
        {
            if (removeProductNoRecord(a.item.getBarcode())) {
                cout << "✓ Undo Add -> Product removed.\n";
            } else {
                cout << "✗ Error: Product not found for undo add.\n";
            }
        }

        else if (a.type == "delete")
        {
            if (addProductNoRecord(a.item)) {
                cout << "✓ Undo Delete -> Product restored.\n";
            } else {
                cout << "✗ Error: Could not restore product for undo delete.\n";
            }
        }
///////////////////////////////////////////////////   
        else if (a.type == "edit")
        {
            product* p = findProduct(a.item.getBarcode());
            if (p != nullptr)
            {
                p->setName(a.oldName);
                p->setPrice(a.oldPrice);
                p->setQuantity(a.oldQty);
                p->setExpiryDate(a.oldExpiry);

                bst->updateProduct(*p);
                bstExpiry->updateProduct(*p);

                cout << "✓ Undo Edit -> Product restored completely.\n";
            } 
            
            else 
            {
                cout << "✗ Error: Product not found for undo edit.\n";
            }
        }
///////////////////////////////////////////////////           
        else if (a.type == "cart_add")
        {
            Customer* c = customerQueue.findCustomerByID(a.customerID);
            if (c != nullptr)
            {
                c->removeFromCart(a.item.getBarcode());
                increaseQuantity(a.item.getBarcode(), a.newQty);
                cout << "✓ Undo Cart Add -> removed from customer " << a.customerID << "\n";
            } 
            
            else 
            {
                cout << "✗ Error: Customer not found for undo cart add.\n";
            }
        }
        
        else if (a.type == "guest_cart_add")
        {
            guestCart.remove_from_cart(a.item.getBarcode());
            increaseQuantity(a.item.getBarcode(), a.newQty);
            cout << "✓ Undo Guest Cart Add -> item removed\n";
        }
///////////////////////////////////////////////////   
        else if (a.type == "cart_remove")
        {
            Customer* c = customerQueue.findCustomerByID(a.customerID);
            if (c != nullptr)
            {
                product* prod = findProduct(a.item.getBarcode());
                if (prod)
                {
                    c->addToCart(prod, a.oldQty);
                    reduceQuantity(a.item.getBarcode(), a.oldQty);
                    cout << "✓ Undo Cart Remove -> restored to customer " << a.customerID << "\n";
                } else {
                    cout << "✗ Error: Product not found for undo cart remove.\n";
                }
            } 
            
            else 
            {
                cout << "✗ Error: Customer not found for undo cart remove.\n";
            }
        }
///////////////////////////////////////////////////           
        else if (a.type == "guest_cart_remove")
        {
            product* prod = findProduct(a.item.getBarcode());
            if (prod)
            {
                guestCart.add_to_cart(prod, a.oldQty);
                reduceQuantity(a.item.getBarcode(), a.oldQty);
                cout << "✓ Undo Guest Cart Remove -> item restored\n";
            } else {
                cout << "✗ Error: Product not found for undo guest cart remove.\n";
            }
        }
///////////////////////////////////////////////////           
        else if (a.type == "cart_edit")
        {
            Customer* c = customerQueue.findCustomerByID(a.customerID);
            if (c != nullptr)
            {
                int diff = a.newQty - a.oldQty;
                c->updateCartQuantity(a.item.getBarcode(), a.oldQty);

                if (diff > 0)
                {
                    increaseQuantity(a.item.getBarcode(), diff);
                }

                else
                {
                    reduceQuantity(a.item.getBarcode(), -diff);
                }
                cout << "✓ Undo Cart Edit -> restored quantity for customer " << a.customerID << "\n";
            } 
            
            else 
            {
                cout << "✗ Error: Customer not found for undo cart edit.\n";
            }
        }
///////////////////////////////////////////////////           
        else if (a.type == "guest_cart_edit")
        {
            int diff = a.newQty - a.oldQty;
            guestCart.update_quantity(a.item.getBarcode(), a.oldQty);

            if (diff > 0)
            {
                increaseQuantity(a.item.getBarcode(), diff);
            }

            else
            {
                reduceQuantity(a.item.getBarcode(), -diff);
            }
            cout << "✓ Undo Guest Cart Edit -> quantity restored\n";
        }
///////////////////////////////////////////////////   
        else if (a.type == "cart_clear")
        {
            Customer* c = customerQueue.findCustomerByID(a.customerID);
            if (c != nullptr && !a.cartItems.empty())
            {
                for (auto& item : a.cartItems)
                {
                    product* prod = findProduct(item.getBarcode());
                    if (prod)
                    {
                        c->addToCart(prod, item.getQuantity());
                        reduceQuantity(item.getBarcode(), item.getQuantity());
                    }
                }
                cout << "✓ Undo Clear Cart -> entire cart restored for customer " << a.customerID << "\n";
            } 
            
            else if (c == nullptr) 
            {
                cout << "✗ Error: Customer not found for undo cart clear.\n";
            }
        }
///////////////////////////////////////////////////           
        else if (a.type == "guest_cart_clear")
        {
            if (!a.cartItems.empty())
            {
                for (auto& item : a.cartItems)
                {
                    product* prod = findProduct(item.getBarcode());
                    if (prod)
                    {
                        guestCart.add_to_cart(prod, item.getQuantity());
                        reduceQuantity(item.getBarcode(), item.getQuantity());
                    }
                }
                cout << "✓ Undo Guest Cart Clear -> entire cart restored\n";
            }
        }

        else
        {
            cout << "Unknown action type: " << a.type << "\n";
  }
}
};
///////////////////////////////////////////////////////////////////////////////////////////
// ============ Display Functions ============
///////////////////////////////////////////////////////////////////////////////////////////

void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
///////////////////////////////////////////////////   
void displayMainMenu()
{
    clearScreen();
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║      SUPERMARKET MANAGEMENT SYSTEM         ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "[1] 📦  Product Management\n";
    cout << "[2] 🛍  Customer Cart Management\n";
    cout << "[3] 👥  Customer Queue Management\n";
    cout << "[4] 👤  Guest Cart Management\n";
    cout << "[5] ❌  Exit\n";
    cout << "══════════════════════════════════════════════\n";
    cout << "Enter your choice [1-5] : ";
}
///////////////////////////////////////////////////   
void displayProductMenu()
{
    clearScreen();
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║             PRODUCT MANAGEMENT             ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "[1] ➕  Add Products\n";
    cout << "[2] 📋  Display All Products\n";
    cout << "[3] ✏  Update a Product\n";
    cout << "[4] 🗑  Delete a Product\n";
    cout << "[5] 💰  Display Products Sorted by Price\n";
    cout << "[6] 📅  Display Products Sorted by Expiry Date\n";
    cout << "[7] ↩  Undo Last Action\n";
    cout << "[8] 🔙  Back to Main Menu\n";
    cout << "══════════════════════════════════════════════\n";
    cout << "Enter your choice [1-8] : ";
}
///////////////////////////////////////////////////   
void displayCustomerCartMenu()
{
    clearScreen();
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║            CUSTOMER CART MANAGEMENT        ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "[1] ➕  Add Product to Customer Cart\n";
    cout << "[2] 👁  Display Customer Cart by ID\n";
    cout << "[3] ✏  Update Cart Quantity\n";
    cout << "[4] 🗑  Remove from Customer Cart\n";
    cout << "[5] 🧹  Clear Customer Cart\n";
    cout << "[6] 💰  Total Customer Cart Price\n";
    cout << "[7] ↩  Undo Last Action\n";
    cout << "[8] 🔙  Back to Main Menu\n";
    cout << "══════════════════════════════════════════════\n";
    cout << "Enter your choice [1-8] : ";
}
///////////////////////////////////////////////////   
void displayCustomerQueueMenu()
{
    clearScreen();
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║            CUSTOMER QUEUE MANAGEMENT       ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "[1] ➕  Add Customer to Queue\n";
    cout << "[2] ⏭  Serve Next Customer\n";
    cout << "[3] 👁  Display Customers in Queue\n";
    cout << "[4] ↩  Undo Last Action\n";
    cout << "[5] 🔙  Back to Main Menu\n";
    cout << "══════════════════════════════════════════════\n";
    cout << "Enter your choice [1-5] : ";
}
///////////////////////////////////////////////////   
void displayGuestCartMenu()
{
    clearScreen();
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║             GUEST CART MANAGEMENT          ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "[1] 👁  Display Guest Cart\n";
    cout << "[2] ➕  Add Product to Guest Cart\n";
    cout << "[3] 🗑  Remove from Guest Cart\n";
    cout << "[4] ✏  Update Guest Cart Quantity\n";
    cout << "[5] 🧹  Clear Guest Cart\n";
    cout << "[6] ↩  Undo Last Action\n";
    cout << "[7] 🔙  Back to Main Menu\n";
    cout << "══════════════════════════════════════════════\n";
    cout << "Enter your choice [1-7] : ";
}

///////////////////////////////////////////////////////////////////////////////////////////
// ============ Menu Handler Functions ============
///////////////////////////////////////////////////////////////////////////////////////////

void productManagement(product_manger& manager, CustomerQueue& customerQueue, linked_list& guestCart)
{
    int choice;

    do {
        displayProductMenu();
        cin >> choice;

        switch(choice)
        {
            case 1 :
                manager.input_Products();
                break;

            case 2 :
                manager.display_Products();
                break;

            case 3 :
                manager.interactive_Product("update");
                break;

            case 4 :
                manager.interactive_Product("delete");
                break;

            case 5 :
                manager.getBST()->displayInOrder();
                break;

            case 6 :
                manager.getBSTExpiry()->displayInOrder();
                break;

            case 7 :
                manager.undoLastAction(customerQueue, guestCart);
                break;

            case 8 :
                return;

            default :
                cout << "Invalid choice! Please enter 1-8.\n";
        }

        if (choice != 8)
        {
            cout << "\nPress Enter to continue...";
            clearInput();
            cin.get();
        }
    } while (true);
}
///////////////////////////////////////////////////   
void customerCartManagement(product_manger& manager, CustomerQueue& customerQueue, linked_list& guestCart)
{
    int choice;

    do {
        displayCustomerCartMenu();
        cin >> choice;

        switch(choice)
        {
            case 1 :
            {
                int barcode, quantity, customerID;
                cout << "Enter customer ID : ";
                cin  >> customerID;

                cout << "Enter barcode to add to cart : ";
                cin  >> barcode;

                cout << "Enter the quantity : ";
                cin  >> quantity;

                product* p = manager.findProduct(barcode);

                if(p == nullptr)
                {
                    cout << "✗ Product not found in store.\n";
                }

                else if(quantity <= 0)
                {
                    cout << "✗ Quantity must be positive!\n";
                }

                else if(p->getQuantity() < quantity)
                {
                    cout << "✗ Not enough quantity available. Available : " << p->getQuantity() << endl;
                }

                else
                {
                    Customer* cust = customerQueue.findCustomerByID(customerID);
                    if(cust == nullptr)
                    {
                        Customer* newCustomer = new Customer(customerID);
                        customerQueue.enqueue(newCustomer);
                        cust = newCustomer;
                        cout << "✓ Customer " << customerID << " created and enqueued.\n";
                    }

                    cust->addToCart(p, quantity);
                    manager.addToCartWithUndo(barcode, quantity, customerID, customerQueue);
                    cout << "✓ Product '" << p->getName() << "' added to customer " << customerID << "'s cart.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 2 :
            {
                int cid;
                cout << "Enter customer ID to display cart : ";
                cin  >> cid;
                Customer* c = customerQueue.findCustomerByID(cid);
                if(c == nullptr)
                {
                    cout << "✗ Customer not found in queue.\n";
                }

                else
                {
                    cout << "\n=== Customer " << cid << "'s Cart ===\n";
                    c->displayCart();
                    cout << "Total: $" << fixed << setprecision(2) << c->getTotalPrice() << endl;
                }
                break;
            }
///////////////////////////////////////////////////   
            case 3:
            {
                int cid, barcode, newQty;
                cout << "Enter customer ID : ";
                cin  >> cid;
                cout << "Enter barcode to update quantity : ";
                cin  >> barcode;
                cout << "Enter new quantity : ";
                cin  >> newQty;

                Customer* c = customerQueue.findCustomerByID(cid);
                if(c == nullptr)
                {
                    cout << "✗ Customer not found.\n";
                    break;
                }

                int oldQty = c->getCartItemQuantity(barcode);
                if(oldQty == 0)
                {
                    cout << "✗ Item not found in cart.\n";
                    break;
                }

                product* prod = manager.findProduct(barcode);
                if(prod == nullptr)
                {
                    cout << "✗ Product not found in store.\n";
                    break;
                }

                if(newQty != oldQty)
                {
                    c->updateCartQuantity(barcode, newQty);
                    manager.updateCartQuantityWithUndo(barcode, oldQty, newQty, cid);
                    cout << "✓ Quantity updated from " << oldQty << " to " << newQty << ".\n";
                }

                else
                {
                    cout << "Quantity unchanged.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 4 :
            {
                int cid, barcode;
                cout << "Enter customer ID: ";
                cin  >> cid;
                cout << "Enter barcode to remove from cart : ";
                cin  >> barcode;

                Customer* c = customerQueue.findCustomerByID(cid);
                if(c == nullptr)
                {
                    cout << "✗ Customer not found.\n";
                    break;
                }

                int oldQty = c->getCartItemQuantity(barcode);
                if(oldQty == 0)
                {
                    cout << "✗ Item not found in cart.\n";
                    break;
                }

                c->removeFromCart(barcode);
                manager.removeFromCartWithUndo(barcode, oldQty, cid);
                cout << "✓ Item removed from cart. " << oldQty << " units returned to stock.\n";
                break;
            }
///////////////////////////////////////////////////   
            case 5 :
            {
                int cid;
                cout << "Enter customer ID to clear cart : ";
                cin  >> cid;
                Customer* c = customerQueue.findCustomerByID(cid);

                if(c == nullptr)
                {
                    cout << "✗ Customer not found.\n";
                    break;
                }

                manager.clearCartWithUndo(c, cid);
                cout << "✓ Cart cleared (undo available).\n";
                break;
            }
///////////////////////////////////////////////////   
            case 6 :
            {
                int cid;
                cout << "Enter customer ID to calculate total : ";
                cin  >> cid;
                Customer* c = customerQueue.findCustomerByID(cid);

                if(c == nullptr)
                {
                    cout << "✗ Customer not found.\n";
                    break;
                }
                cout << "\n=== Customer " << cid << " Cart Total ===\n";
                cout << "Total Cart Price : $" << fixed << setprecision(2) << c->getTotalPrice() << endl;
                break;
            }
///////////////////////////////////////////////////   
            case 7 :
                manager.undoLastAction(customerQueue, guestCart);
                break;
///////////////////////////////////////////////////   
            case 8 :
                return;
///////////////////////////////////////////////////   
            default :
                cout << "Invalid choice! Please enter 1-8.\n";
        }
///////////////////////////////////////////////////   
        if (choice != 8)
        {
            cout << "\nPress Enter to continue...";
            clearInput();
            cin.get();
        }
    } while (true);
}
///////////////////////////////////////////////////   
void customerQueueManagement(CustomerQueue& customerQueue, product_manger& manager, linked_list& guestCart)
{
    int choice;

    do {
        displayCustomerQueueMenu();
        cin >> choice;

        switch(choice)
        {
///////////////////////////////////////////////////   
            case 1 : 
            {
                int customerID;
                cout << "Enter customer ID : ";
                cin  >> customerID;

                Customer* existing = customerQueue.findCustomerByID(customerID);
                if(existing != nullptr)
                {
                    cout << "Customer " << customerID << " already in queue.\n";
                }

                else
                {
                    Customer* newCustomer = new Customer(customerID);
                    customerQueue.enqueue(newCustomer);
                    cout << "✓ Customer " << customerID << " added to queue.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 2 :
            {
                Customer* nextCustomer = customerQueue.dequeue();
                if(nextCustomer == nullptr)
                {
                    cout << "No customers in queue.\n";
                }

                else
                {
                    cout << "\n══════════════════════════════════════════\n";
                    cout << "  Processing Customer ID : " << nextCustomer->getID() << "\n";
                    cout << "══════════════════════════════════════════\n";

                    if (nextCustomer->getTotalPrice() > 0)
                    {
                        cout << "Items Purchased :\n";
                        nextCustomer->displayCart();
                        cout << "\nTotal Price: $" << fixed << setprecision(2) << nextCustomer->getTotalPrice() << endl;
                        cout << "Thank you for your purchase!\n";
                    }
                    else
                    {
                        cout << "Cart is empty. No purchase made.\n";
                    }

                    cout << "══════════════════════════════════════════\n";

                    nextCustomer->clearCart();
                    delete nextCustomer;
                    cout << "✓ Customer processed and removed from queue.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 3 :
            {
                cout << "\n=== Current Customers in Queue ===\n";
                customerQueue.displayQueue();
                cout << "Queue size : " << customerQueue.getSize() << "/" << customerQueue.getCapacity() << endl;
                break;
            }
///////////////////////////////////////////////////   
            case 4 :
                manager.undoLastAction(customerQueue, guestCart);
                break;
///////////////////////////////////////////////////   
            case 5 :
                return;
///////////////////////////////////////////////////   
            default :
                cout << "Invalid choice! Please enter 1-5.\n";
        }
///////////////////////////////////////////////////   
        if (choice != 5)
        {
            cout << "\nPress Enter to continue...";
            clearInput();
            cin.get();
        }
    } while (true);
}
///////////////////////////////////////////////////   
void guestCartManagement(product_manger& manager, linked_list& guestCart, CustomerQueue& customerQueue)
{
    int choice;

    do {
        displayGuestCartMenu();
        cin  >> choice;

        switch(choice)
        {
            case 1 :
            {
                cout << "\n=== Guest Cart Contents ===\n";
                guestCart.display_cart();
                cout << "Total: $" << fixed << setprecision(2) << guestCart.total_price() << endl;
                break;
            }
///////////////////////////////////////////////////   
            case 2 :
            {
                int barcode, quantity;
                cout << "Enter barcode to add to guest cart : ";
                cin  >> barcode;
                cout << "Enter quantity : ";
                cin  >> quantity;

                product* p = manager.findProduct(barcode);
                if(p == nullptr)
                {
                    cout << "✗ Product not found in store.\n";
                }

                else if(quantity <= 0)
                {
                    cout << "✗ Quantity must be positive!\n";
                }

                else if(p->getQuantity() < quantity)
                {
                    cout << "✗ Not enough stock. Available : " << p->getQuantity() << endl;
                }

                else
                {
                    guestCart.add_to_cart(p, quantity);
                    manager.addToGuestCartWithUndo(barcode, quantity, guestCart);
                    cout << "✓ Added " << quantity << " x " << p->getName() << " to guest cart.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 3 :
            {
                int barcode;
                cout << "Enter barcode to remove from guest cart : ";
                cin >> barcode;

                int oldQty = guestCart.getItemQuantity(barcode);
                if (oldQty == 0)
                {
                    cout << "✗ Item not found in guest cart.\n";
                }
                else
                {
                    guestCart.remove_from_cart(barcode);
                    manager.removeFromGuestCartWithUndo(barcode, oldQty);
                    cout << "✓ Item removed from guest cart. " << oldQty << " units returned to stock.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 4 :
            {
                int barcode, newQty;
                cout << "Enter barcode to update quantity : ";
                cin >> barcode;
                cout << "Enter new quantity : ";
                cin >> newQty;

                int oldQty = guestCart.getItemQuantity(barcode);
                
                if (oldQty == 0)
                {
                    cout << "✗ Item not found in guest cart.\n";
                }
                
                else
                {
                    guestCart.update_quantity(barcode, newQty);
                    manager.updateGuestCartQuantityWithUndo(barcode, oldQty, newQty);
                    cout << "✓ Quantity updated from " << oldQty << " to " << newQty << ".\n";
                }
                break;
            }
///////////////////////////////////////////////////     
            case 5 :
            {
                cout << "\n=== Clearing Guest Cart ===\n";
                if(guestCart.isempty())
                {
                    cout << "Guest cart is already empty.\n";
                }

                else
                {
                    manager.clearGuestCartWithUndo(guestCart);
                    cout << "✓ Guest cart cleared and items returned to stock.\n";
                }
                break;
            }
///////////////////////////////////////////////////   
            case 6 :
                manager.undoLastAction(customerQueue, guestCart);
                break;

            case 7 :
                return;

            default :
                cout << "Invalid choice! Please enter 1-7.\n";
        }

        if (choice != 7)
        {
            cout << "\nPress Enter to continue...";
            clearInput();
            cin.get();
        }
    } while (true);
}

///////////////////////////////////////////////////////////////////////////////////////////
// ============ Main Function ============
///////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    product_manger manager;
    linked_list guestCart;
    CustomerQueue customerQueue;

    int mainChoice;

    do {
        displayMainMenu();
        cin  >> mainChoice;

        switch (mainChoice)
        {
            case 1 :
                productManagement(manager, customerQueue, guestCart);
                break;

            case 2 :
                customerCartManagement(manager, customerQueue, guestCart);
                break;

            case 3 :
                customerQueueManagement(customerQueue, manager, guestCart);
                break;

            case 4 :
                guestCartManagement(manager, guestCart, customerQueue);
                break;

            case 5 :
                cout << "\n";
                cout << "══════════════════════════════════════════\n";
                cout << "  Thank you for using Supermarket System!\n";
                cout << "           Goodbye! 👋\n";
                cout << "══════════════════════════════════════════\n";


                while(!customerQueue.isEmpty())
                {
                    Customer* cust = customerQueue.dequeue();
                    if(cust)
                    {
                        cust->clearCart();
                        delete cust;
                    }
                }

                if(!guestCart.isempty())
                {
                    guestCart.clear_cart();
                }

                return 0;

            default :
                cout << "Invalid choice. Please enter 1-5.\n";
                clearInput();
                cin.get();
        }

    } while (true);

return 0;
}
