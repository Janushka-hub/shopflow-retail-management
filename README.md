# ShopFlow — E-commerce Management System

![Language](https://img.shields.io/badge/Language-C11-blue)
![Build](https://img.shields.io/badge/Build-CMake-064F8C)
![Interface](https://img.shields.io/badge/Interface-CLI-lightgrey)

## Overview

ShopFlow is a command-line e-commerce management application developed in C as part of the Computer Science program at EFREI Paris.

The application simulates the core operations of an online store, including product inventory, customer management, order processing, automated pricing calculations, and sales analytics.

The project focuses on modular programming, dynamic memory management, structured data modeling, and the implementation of business rules in C.

## Features

### Product Management
- Add, update, search, and deactivate products.
- Manage inventory levels and stock valuation.
- Monitor low-stock alerts.
- Search products by reference, keyword, category, and price.

### Customer Management
- Create and manage customer profiles.
- Manage contact information and delivery addresses.
- Access customer order history.
- Calculate and update loyalty points.

### Order Management
- Create orders containing multiple products.
- Validate product availability and quantities.
- Automatically update inventory levels.
- Manage order status transitions.
- Cancel orders and restore stock automatically.
- Calculate discounts, VAT, shipping fees, and final prices.

### Sales Analytics
- Calculate total, monthly, and annual revenue.
- Identify the top five best-selling products.
- Rank customers by generated revenue.
- Analyze inventory quantities and values by category.
- Exclude cancelled orders from sales statistics.

## Technologies

| Technology | Purpose |
|---|---|
| C11 | Core application development |
| CMake | Build configuration |
| C Standard Library | Memory management, strings, and input validation |
| CLI | Interactive terminal-based interface |

## Project Architecture

The application follows a modular architecture, separating business logic, data structures, and utility functions.

| File | Responsibility |
|---|---|
| `main.c` | Application entry point and main menu |
| `types.h` | Shared data structures and constants |
| `produit.c / .h` | Product and inventory management |
| `client.c / .h` | Customer management |
| `commande.c / .h` | Order processing and pricing calculations |
| `statistique.c / .h` | Sales analytics and rankings |
| `utils.c / .h` | Input validation, search, and memory allocation |
| `initialisation.c / .h` | Demonstration data and memory cleanup |
| `CMakeLists.txt` | Build configuration |

## Data Model

The application is organized around seven interconnected structures:

- `Boutique` — Central store structure.
- `Produit` — Product information and inventory.
- `Client` — Customer information and order history.
- `Commande` — Order details and financial calculations.
- `LigneCommande` — Individual order items.
- `Adresse` — Customer and delivery addresses.
- `Date` — Order dates.

Products, customers, orders, and order items are stored in dynamically allocated arrays that can grow according to application needs.

## Business Rules

### Automatic Discounts

| Order subtotal (excluding VAT) | Discount |
|---|---:|
| Below €100 | 0% |
| €100 – €299.99 | 5% |
| €300 – €599.99 | 10% |
| €600 and above | 15% |

- **VAT:** 20%, applied after the discount.
- **Shipping:** €5.90, free for orders starting at €60 before discount and VAT.
- **Loyalty program:** One point for every complete €10 of the final order total, including shipping.

Order cancellation automatically restores inventory and updates customer loyalty points.

## Getting Started

### Prerequisites

- A C11-compatible compiler, such as GCC or Clang.
- CMake 3.16 or later.

### Build and Run

Open a terminal in the repository directory and execute:

```bash
cmake -S . -B build
cmake --build build
./build/ShopFlow
```

The application starts with preloaded demonstration data, allowing users to explore its features immediately.

### Main Menu

```text
========== SHOPFLOW ==========
1. Gestion des produits
2. Gestion des clients
3. Gestion des commandes
4. Statistiques
0. Quitter
```

The application interface is available in French.

## Technical Highlights

- Dynamic memory allocation and resizing using `realloc`.
- Nested structures and pointer manipulation.
- Modular programming with separate header and source files.
- Memory cleanup using `free`.
- Input validation and allocation failure handling.
- Case-insensitive string comparison.
- Search and sorting algorithms.
- Inventory consistency and order lifecycle management.

## Current Scope

ShopFlow is an in-memory application. Data is initialized at startup and is not permanently saved after the application closes.

This design intentionally focuses on C programming fundamentals, memory management, and business logic without relying on external databases or frameworks.

## Academic Context

Developed as a collaborative academic project for the XTI101 C Programming module at EFREI Paris, 2026.

The project demonstrates the practical application of fundamental computer science concepts to an e-commerce management system.
