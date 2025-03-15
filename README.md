# Personal Data Storage System

## Overview
The **Personal Data Storage System (PDS)** is a structured data storage framework designed to provide **efficient storage, retrieval, indexing, and deletion** of records. The system leverages **Binary Search Trees (BST)** and **linked data structures** to enhance data access and organization.

## Features
- **Persistent Data Storage (PDS):** Enables structured storage of records in a file-based system.
- **Indexing with Binary Search Trees (BST):** Ensures efficient searching and retrieval.
- **Record Deletion with Index Management:** Supports safe removal of records while maintaining integrity.
- **Linked Data Handling:** Establishes relationships between stored entities.
- **Modular Design:** Supports various data types and structures.
- **Scalability:** Suitable for applications requiring structured data organization.
- **JDBC Integration:** Provides database connectivity for structured data handling.

## Project Structure

### **Lab 05 - Record Deletion and Indexing**
This module implements efficient **record deletion** while maintaining an indexed file system.

#### **Key Components**
- **`IMT2022102_pds.c`** – Core implementation of the **Persistent Data Storage (PDS)** system.
- **`IMT2022102_contact.c`** – Contact management, including **insertion, retrieval, and deletion**.
- **`bst.c` & `bst.h`** – Binary Search Tree (BST) for **fast indexing and lookup**.
- **`pds_tester.c`** – Test suite for validating PDS operations.

### **Lab 06 - Linked Data Structures**
This module extends PDS by incorporating **linked data structures**, enabling entity relationships.

#### **Key Components**
- **`IMT2022102_pds.c`** – Extended PDS with linked data management.
- **`artist_musical_instrument.c`** – Demonstrates entity relationships in a music dataset.
- **`linked_data_demo.c`** – Implements linked data retrieval.
- **`structures.h`** – Defines the data structures for linked entity handling.

### **Lab 07 - JDBC Project**
This module integrates **JDBC (Java Database Connectivity)** for **structured data storage and retrieval** using SQL.

#### **Key Components**
- **`sql/schema.sql`** – Defines the **Music Database Schema**:
  - **Musical Instruments**
  - **Music Artists**
  - **Concerts**
  - **Record Labels**
  - Normalized to **BCNF**.
- **`src/imt2022102_music.java`** – Implements **CRUD operations** using JDBC.
- **`mysql-connector-j-8.3.0.jar`** – Required for **MySQL database connectivity**.

## Installation & Setup
To download and run the project, follow these steps:

### **1️⃣ Clone the Repository**
```sh
 git clone https://github.com/Owais-Md/Personal-Data-Storage-System.git
 cd Personal-Data-Storage-System
```

### **2️⃣ Compile the Source Code**
For **basic PDS operations**:
```sh
 gcc IMT2022102_pds.c IMT2022102_contact.c bst.c -o pds_system
```
For **linked data operations**:
```sh
 gcc IMT2022102_pds.c linked_data_demo.c bst.c -o linked_pds
```
For **JDBC-based database management**:
```sh
 javac -cp .:mysql-connector-j-8.3.0.jar src/imt2022102_music.java -d class/
```

### **3️⃣ Run the Program**
To execute **PDS operations**:
```sh
 ./pds_system
```
To run **linked data management**:
```sh
 ./linked_pds
```
To run **JDBC database operations**:
```sh
 java -cp class/:mysql-connector-j-8.3.0.jar imt2022102_music
```

## Dependencies
- **GCC** for compilation.
- **Java JDK 8+** for JDBC operations.
- **MySQL Database** (Ensure MySQL server is running).
- **Standard C libraries** (`stdio.h`, `stdlib.h`, `string.h`).
- **Binary Search Tree (BST) implementation** for indexing.
- **JDBC MySQL Connector (`mysql-connector-j-8.3.0.jar`)**.

## License
This project is licensed under the **MIT License**.

## Contributions
Contributions are welcome! Feel free to submit issues and pull requests.

---
This README provides a **professional** and **structured** guide to understanding, installing, and running the PDS and JDBC system.
