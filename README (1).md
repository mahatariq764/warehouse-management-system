# Warehouse Management System (WMS)

A dual-component Warehouse Management System built as a Semester 4 Data Structures project — combining a C++ console application with custom data structures and a full-stack web application, both backed by MySQL.

## Project Structure

```
wms-repo/
├── cpp-console-app/
│   └── warehouse.cpp        # C++ console app (BST for products, linked list for orders, MySQL integration)
└── web-app/
    ├── login.html            # Login page
    ├── signup.html           # Registration page
    ├── wms.html               # Main dashboard UI
    ├── auth.css               # Shared styling for login/signup
    ├── auth.js                 # Shared client-side logic for login/signup
    ├── auth.php                 # Backend: register/login (PHP + MySQL)
    ├── products.php              # Backend: product CRUD operations
    ├── orders.php                 # Backend: order placement & stock updates
    └── dashboard.php               # Backend: dashboard stats (totals, low stock, recent orders)
```

## Component 1: C++ Console Application

A console-based inventory management tool using core data structures:

- **Binary Search Tree (BST)** for storing and searching products efficiently
- **Singly Linked List** for managing order history
- **MySQL integration** via the MySQL C Connector for persistent storage

### Requirements
- A C++ compiler (e.g. MinGW/g++)
- MySQL Server
- MySQL C Connector (`libmysql.dll` / `.lib`) — not included in this repo; download from the [official MySQL site](https://dev.mysql.com/downloads/connector/c/) and place alongside the compiled executable

### Build & Run
```bash
g++ warehouse.cpp -o warehouse -I"<path-to-mysql-include>" -L"<path-to-mysql-lib>" -lmysql
./warehouse
```

## Component 2: Full-Stack Web Application

A browser-based warehouse management interface.

- **Frontend:** HTML, CSS, JavaScript
- **Backend:** PHP
- **Database:** MySQL

### Features
- User registration & login (with password hashing)
- Product management (add, view, delete)
- Order placement with automatic stock updates
- Dashboard with live stats: total products, low-stock alerts, total orders, and recent order history

### Setup
1. Install [XAMPP](https://www.apachefriends.org/) (or any Apache + PHP + MySQL stack).
2. Place the `web-app` folder inside your `htdocs` directory.
3. Create a MySQL database named `warehouse_management_system` with `users`, `products`, and `orders` tables.
4. Start Apache and MySQL via XAMPP.
5. Open `login.html` in your browser (e.g. `http://localhost/wms/login.html`).

## Tech Stack

- C++ (Data Structures: BST, Linked List)
- HTML, CSS, JavaScript
- PHP
- MySQL
