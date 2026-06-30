#include <iostream>
#include <string>
#if __has_include(<mysql/mysql.h>)
#include <mysql/mysql.h>
#elif __has_include(<mysql.h>)
#include <mysql.h>
#else
#error "mysql headers not found, please install MySQL client development headers and update includePath"
#endif
using namespace std;

//  DATABASE
MYSQL* conn;
void connectDB() {
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "", "warehouse_management_system", 3306, NULL, 0))
        cout << "DB Connection Failed: " << mysql_error(conn) << endl;
    else
        cout << "Database connected successfully!" << endl;
}
void saveProductToDB(int id, string name, string category, int qty, double price) {
    char query[500];
    sprintf(query,
        "INSERT INTO products VALUES (%d, '%s', '%s', %d, %.2f) "
        "ON DUPLICATE KEY UPDATE name='%s', category='%s', quantity=%d, price=%.2f",
        id, name.c_str(), category.c_str(), qty, price,
        name.c_str(), category.c_str(), qty, price);
    if (mysql_query(conn, query))
        cout << "DB Error (save product): " << mysql_error(conn) << endl;
}
void deleteProductFromDB(int id) {
    char query[200];
    sprintf(query, "DELETE FROM products WHERE id=%d", id);
    if (mysql_query(conn, query))
        cout << "DB Error (delete product): " << mysql_error(conn) << endl;
}
void saveOrderToDB(int pid, string pname, int qty, double cost, string type) {
    char query[500];
    sprintf(query,
        "INSERT INTO orders (product_id, product_name, qty, total_cost, type) "
        "VALUES (%d, '%s', %d, %.2f, '%s')",
        pid, pname.c_str(), qty, cost, type.c_str());
    if (mysql_query(conn, query))
        cout << "DB Error (save order): " << mysql_error(conn) << endl;
}

//  PRODUCT STRUCT
struct Product {
    int    id;
    string name;
    string category;
    int    quantity;
    double price;
};

//  BST functions 
struct TreeNode {
    Product  data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Product p) {
        data  = p;
        left  = right = nullptr;
    }
};

// Function to insert a node
TreeNode* insert(TreeNode* root, Product p) {
    if (root == nullptr) {
        return new TreeNode(p);
    }
    if (p.id < root->data.id) {
        root->left = insert(root->left, p);
    } 
    else if (p.id > root->data.id) {
        root->right = insert(root->right, p);
    } 
    else {
        cout << "Product ID " << p.id << " already exists." << endl;
    }
    return root;
}

// Function to find minimum value node in BST
TreeNode* minValueNode(TreeNode* node) {
    TreeNode* current = node;
    while (current != nullptr && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Function to delete a node
TreeNode* deleteNode(TreeNode* root, int id) {
    if (root == nullptr)
        return nullptr;
    if (id < root->data.id) {
        root->left = deleteNode(root->left, id);
    }
    else if (id > root->data.id) {
        root->right = deleteNode(root->right, id);
    }
    else {
        // Case 1: No child
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        // Case 2: One child (right)
        if (root->left == nullptr) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        }
        // Case 3: One child (left)
        if (root->right == nullptr) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }
        // Case 4: Two children
        TreeNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data.id);
    }
    return root;
}

// Function to search a node
TreeNode* search(TreeNode* root, int id) {
    if (root == nullptr || root->data.id == id)
        return root;

    if (id < root->data.id)
        return search(root->left, id);

    return search(root->right, id);
}

// Inorder traversal (Display)
void inorder(TreeNode* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << left;
        cout.width(8); 
        cout << root->data.id;
        cout.width(22);
        cout << root->data.name;
        cout.width(12);
        cout << root->data.category;
        cout.width(10);
        cout << root->data.quantity;
        cout.width(10);
        cout << root->data.price;
        cout << endl;
        inorder(root->right);
    }
}

void displayCatalog(TreeNode* root) {
    if (!root) { 
        cout << "Catalog is empty." << endl; 
        return; 
    }
    cout << "Warehouse Product Catalog" << endl;
    cout << left;
    cout.width(8);  
    cout << "ID";
    cout.width(22); 
    cout << "Name";
    cout.width(12); 
    cout << "Category";
    cout.width(10); 
    cout << "Qty";
    cout.width(10); 
    cout << "Price(Rs)" << endl;
    cout << string(62, '-') << endl;
    inorder(root);
}

//  Linked list functions
struct OrderNode {
    int        orderId;
    int        productId;
    string     productName;
    int        qty;
    double     totalCost;
    string     type;
    OrderNode* next;
};
int orderCounter = 1;
OrderNode* makeOrder(int pid, string pname, int qty, double cost, string type) {
    OrderNode* newNode   = new OrderNode();
    newNode->orderId     = orderCounter++;
    newNode->productId   = pid;
    newNode->productName = pname;
    newNode->qty         = qty;
    newNode->totalCost   = cost;
    newNode->type        = type;
    newNode->next        = NULL;
    return newNode;
}

// 1. Insert Node at front Position
OrderNode* Insert_Front(OrderNode* head, int pid, string pname, int qty, double cost, string type) {
    OrderNode* newNode = makeOrder(pid, pname, qty, cost, type);
    newNode->next = head;
    head = newNode;
    cout << "Inserted at front: Order #" << newNode->orderId << endl;
    saveOrderToDB(pid, pname, qty, cost, type);
    return head;
}

// 2. Insert node at last Position
OrderNode* Insert_End(OrderNode* head, int pid, string pname, int qty, double cost, string type) {
    OrderNode* newNode = makeOrder(pid, pname, qty, cost, type);
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
    } 
    else {
        OrderNode* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    cout << "Inserted at end: Order #" << newNode->orderId << endl;
    saveOrderToDB(pid, pname, qty, cost, type);
    return head;
}

// 3. Insert node at Specific position
OrderNode* Insert_Position(OrderNode* head, int pos, int pid, string pname, int qty, double cost, string type) {
    OrderNode* newNode = makeOrder(pid, pname, qty, cost, type);
    if (pos < 0) {
        cout << "Invalid position." << endl;
        return head;
    }
    // Case 1: insert at position 0 (front)
    if (pos == 0 || head == nullptr) {
        newNode->next = head;
        head = newNode;
        cout << "Inserted at position (0): Order #" << newNode->orderId << endl;
        saveOrderToDB(pid, pname, qty, cost, type);
        return head;
    }
    OrderNode* current = head;
    // Move to (pos-1)th node
    for (int i = 1; i < pos && current != nullptr; i++) {
        current = current->next;
    }
    // Insert the node
    newNode->next = current->next;
    current->next = newNode;
    cout << "Inserted at position " << pos << ": Order #" << newNode->orderId << endl;
    saveOrderToDB(pid, pname, qty, cost, type);
    return head;
}

// 4. Delete node at first position
OrderNode* Delete_Front(OrderNode* head) {
    if (head == NULL) {
        cout << "List is already empty." << endl;
        return NULL;
    }
    OrderNode* temp = head;
    head = head->next;
    cout << "Deleted from front: Order #" << temp->orderId << endl;
    delete temp;
    return head;
}

// 5. Delete node at last position
OrderNode* Delete_End(OrderNode* head) {
    if (head == NULL) {
        cout << "List is already empty." << endl;
        return NULL;
    }
    if (head->next == NULL) {
        cout << "Deleted from end: Order #" << head->orderId << endl;
        delete head;
        return NULL;
    }
    OrderNode* current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    cout << "Deleted from end: Order #" << current->next->orderId << endl;
    delete current->next;
    current->next = NULL;
    return head;
}

// 6. Delete node at Specific position
OrderNode* Delete_Position(OrderNode* head, int pos) {
    if (head == NULL) {
        cout << "List is already empty." << endl;
        return NULL;
    }
    if (pos < 0) {
        cout << "Invalid position." << endl;
        return head;
    }
    // Case 1: delete first node
    if (pos == 0) {
        OrderNode* temp = head;
        head = head->next;
        cout << "Deleted from position 0: Order #" << temp->orderId << endl;
        delete temp;
        return head;
    }
    OrderNode* current = head;
    OrderNode* previous = NULL;
    // Traverse to (pos)th node
    for (int i = 0; i < pos && current != NULL; i++) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        cout << "Position out of range." << endl;
        return head;
    }
    // Delete the node
    previous->next = current->next;
    cout << "Deleted from position " << pos << ": Order #" << current->orderId << endl;
    delete current;
    return head;
}

// 7. Display List
void Display(OrderNode* head) {
    if (head == NULL) {
        cout << "Linked list is empty." << endl;
        return;
    }
    cout << endl << "Order History" << endl;
    cout << left;
    cout.width(8);  
    cout << "Order#";
    cout.width(10); 
    cout << "Type";
    cout.width(8);  
    cout << "ProdID";
    cout.width(20); 
    cout << "Product";
    cout.width(8);  
    cout << "Qty";
    cout.width(10); 
    cout << "Cost(Rs)" << endl;
    cout << string(64, '-') << endl;
    OrderNode* current = head;
    while (current != NULL) {
        cout.width(8);  
        cout << current->orderId;
        cout.width(10); 
        cout << current->type;
        cout.width(8);  
        cout << current->productId;
        cout.width(20); 
        cout << current->productName;
        cout.width(8);  
        cout << current->qty;
        cout.width(10); 
        cout << current->totalCost << endl;
        current = current->next;
    }
}

//  SORTING & SEARCHING 
int collectProducts(TreeNode* root, Product arr[], int idx) {
    if (!root) 
    return idx;
    idx = collectProducts(root->left,  arr, idx);
    arr[idx++] = root->data;
    return collectProducts(root->right, arr, idx);
}
int countNodes(TreeNode* root) {
    if (!root) 
    return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}
void printProducts(Product arr[], int n) {
    cout << left;
    cout.width(8);  
    cout << "ID";
    cout.width(22); 
    cout << "Name";
    cout.width(12); 
    cout << "Category";
    cout.width(10); 
    cout << "Qty";
    cout.width(10); 
    cout << "Price(Rs)" << endl;
    cout << string(62, '-') << endl;
    for (int i = 0; i < n; i++) {
        cout.width(8); 
        cout << arr[i].id;
        cout.width(22); 
        cout << arr[i].name;
        cout.width(12); 
        cout << arr[i].category;
        cout.width(10); 
        cout << arr[i].quantity;
        cout.width(10); 
        cout << arr[i].price << endl;
    }
}
void bubbleSort(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (arr[j].quantity > arr[j+1].quantity)
                swap(arr[j], arr[j+1]);
    cout << "Sorted by Quantity (Bubble Sort):" << endl;
}
void selectionSort(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].price < arr[m].price) m = j;
        swap(arr[i], arr[m]);
    }
    cout << "Sorted by Price (Selection Sort):" << endl;
}
void insertionSort(Product arr[], int n) {
    for (int i = 1; i < n; i++) {
        Product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].name > key.name) { 
            arr[j+1] = arr[j]; j--; 
        }
        arr[j+1] = key;
    }
    cout << "Sorted by Name (Insertion Sort):" << endl;
}
int linearSearch(Product arr[], int n, const string& name) {
    for (int i = 0; i < n; i++)
    if (arr[i].name == name) 
    return i;
    return -1;
}
void sortById(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j].id > arr[j+1].id) 
            swap(arr[j], arr[j+1]);
}
int binarySearch(Product arr[], int n, int id) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid].id == id) 
        return mid;
        else if (arr[mid].id <  id) 
        lo = mid + 1;
        else 
        hi = mid - 1;
    }
    return -1;
}

void loadProductsFromDB(TreeNode*& root) {
    if (mysql_query(conn, "SELECT id, name, category, quantity, price FROM products")) {
        cout << "DB Error (load products): " << mysql_error(conn) << endl;
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW  row;
    while ((row = mysql_fetch_row(res))) {
        Product p;
        p.id       = atoi(row[0]);
        p.name     = row[1];
        p.category = row[2];
        p.quantity = atoi(row[3]);
        p.price    = atof(row[4]);
        root = insert(root, p);
    }
    mysql_free_result(res);
    cout << "Products loaded from database." << endl;
}

int main() {
    connectDB();
    TreeNode*  root = nullptr;
    OrderNode* head = nullptr;
    loadProductsFromDB(root);
    int choice;
    do {
        cout << "Warehouse Management System" << endl;
        cout << "  1.  Add Product (BST - Insert)" << endl;
        cout << "  2.  Remove Product (BST - Delete)" << endl;
        cout << "  3.  Search Product by ID (BST - Search)" << endl;
        cout << "  4.  Display All Products (BST - Display)" << endl;
        cout << "  5.  Insert Order at Front (Linked List - Insert Front)" << endl;
        cout << "  6.  Insert Order at End (Linked List - Insert Last)" << endl;
        cout << "  7.  Insert Order at Position (Linked List - Insert Specific Position)" << endl;
        cout << "  8.  Delete First Order (Linked List - Delete First)" << endl;
        cout << "  9.  Delete Last Order (Linked List - Delete Last)" << endl;
        cout << "  10. Delete Order at Position (Linked List - Delete Specific Position)" << endl;
        cout << "  11. View Order History (Linked List - Display)" << endl;
        cout << "  12. Search Order by Product ID (Linked List - Search)" << endl;
        cout << "  13. Sort Products by Quantity (Bubble Sort)" << endl;
        cout << "  14. Sort Products by Price (Selection Sort)" << endl;
        cout << "  15. Sort Products by Name (Insertion Sort)" << endl;
        cout << "  16. Search Product by Name (Linear Search)" << endl;
        cout << "  17. Search Product by ID (Binary Search)" << endl;
        cout << "  18. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            Product p;
            cout << "ID: ";       
            cin >> p.id;
            cin.ignore();
            cout << "Name: ";     
            getline(cin, p.name);
            cout << "Category: "; 
            getline(cin, p.category);
            cout << "Quantity: "; 
            cin >> p.quantity;
            cout << "Price: ";    
            cin >> p.price;
            root = insert(root, p);
            saveProductToDB(p.id, p.name, p.category, p.quantity, p.price);
            cout << "Product added." << endl;
        }
        else if (choice == 2) {
            int id; 
            cout << "Product ID to remove: "; 
            cin >> id;
            if (!search(root, id))
                cout << "Product ID " << id << " not found." << endl;
            else {
                root = deleteNode(root, id);
                deleteProductFromDB(id);
                cout << "Product removed." << endl;
            }
        }
        else if (choice == 3) {
            int id; 
            cout << "Product ID to search: "; 
            cin >> id;
            TreeNode* res = search(root, id);
            if (res)
                cout << "Found: ID=" << res->data.id << " | " << res->data.name
                     << " | Qty=" << res->data.quantity << " | Price=" << res->data.price << endl;
            else 
            cout << "Not found." << endl;
        }
        else if (choice == 4) {
            displayCatalog(root);
        }
        else if (choice == 5) {
            int pid, qty;
             string pname, type; 
             double cost;
            cout << "Product ID: ";   
            cin >> pid; cin.ignore();
            cout << "Product Name: "; 
            getline(cin, pname);
            cout << "Quantity: ";     
            cin >> qty;
            cout << "Total Cost: ";   
            cin >> cost;
            cout << "Type (in/out): "; 
            cin >> type;
            head = Insert_Front(head, pid, pname, qty, cost, type);
        }
        else if (choice == 6) {
            int pid, qty; 
            string pname, type; 
            double cost;
            cout << "Product ID: ";   
            cin >> pid; cin.ignore();
            cout << "Product Name: "; 
            getline(cin, pname);
            cout << "Quantity: ";     
            cin >> qty;
            cout << "Total Cost: ";   
            cin >> cost;
            cout << "Type (in/out): "; 
            cin >> type;
            head = Insert_End(head, pid, pname, qty, cost, type);
        }
        else if (choice == 7) {
            int pid, qty, pos; string pname, type; double cost;
            cout << "Position (0-based): "; 
            cin >> pos;
            cout << "Product ID: ";         
            cin >> pid; cin.ignore();
            cout << "Product Name: ";       
            getline(cin, pname);
            cout << "Quantity: ";           
            cin >> qty;
            cout << "Total Cost: ";         
            cin >> cost;
            cout << "Type (in/out): ";      
            cin >> type;
            head = Insert_Position(head, pos, pid, pname, qty, cost, type);
        }
        else if (choice == 8)  { 
            head = Delete_Front(head); 
        }
        else if (choice == 9)  { 
            head = Delete_End(head); 
        }
        else if (choice == 10) {
            int pos; cout << "Position (0-based): "; 
            cin >> pos;
            head = Delete_Position(head, pos);
        }
        else if (choice == 11) { 
            Display(head); 
        }
        else if (choice == 12) {
            int id; 
            cout << "Order Product ID to search: "; 
            cin >> id;
            bool found = false;
            for (OrderNode* cur = head; cur; cur = cur->next) {
                if (cur->productId == id) {
                    cout << "Order #" << cur->orderId << " | " << cur->productName
                         << " | Qty=" << cur->qty << " | Cost=" << cur->totalCost
                         << " | Type=" << cur->type << endl;
                    found = true;
                }
            }
            if (!found) cout << "No orders found for Product ID " << id << "." << endl;
        }
        else if (choice >= 13 && choice <= 15) {
            int n = countNodes(root);
            if (n == 0) { 
                cout << "Catalog is empty." << endl; 
                continue; 
            }
            Product arr[200];
            n = collectProducts(root, arr, 0);
            if (choice == 13) { 
                bubbleSort(arr, n);   
                printProducts(arr, n);
             }
            else if (choice == 14) { 
                selectionSort(arr, n); 
                printProducts(arr, n); 
            }
            else if (choice == 15) { 
                insertionSort(arr, n); 
                printProducts(arr, n); 
            }
        }
        else if (choice == 16) {
            int n = countNodes(root);
            if (n == 0) { 
                cout << "Catalog is empty." << endl; 
                continue; 
            }
            Product arr[200];
            n = collectProducts(root, arr, 0);
            string name; 
            cin.ignore();
            cout << "Product name to search: "; 
            getline(cin, name);
            int idx = linearSearch(arr, n, name);
            if (idx == -1) 
            {
                cout << "Not found." << endl;
            }
            else 
            {
                cout << "Found: ID=" << arr[idx].id << " | " << arr[idx].name
                      << " | Qty=" << arr[idx].quantity << endl;
            }
        }
        else if (choice == 17) {
            int n = countNodes(root);
            if (n == 0) { 
                cout << "Catalog is empty." << endl; 
                continue; 
            }
            Product arr[200];
            n = collectProducts(root, arr, 0);
            int id; 
            cout << "Product ID: "; cin >> id;
            sortById(arr, n);
            int idx = binarySearch(arr, n, id);
            if (idx == -1) 
            {
                cout << "Not found." << endl;
            }
            else 
            {
                cout << "Found: ID=" << arr[idx].id << " | " << arr[idx].name
                      << " | Qty=" << arr[idx].quantity << endl;
            }
        }
        else if (choice != 18) {
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 18);
    mysql_close(conn);
    return 0;
}