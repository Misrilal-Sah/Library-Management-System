# 📚 Library Management System v2.0

A comprehensive C-based library management system for colleges and educational institutions.

## ✨ Features

### 🔐 Authentication
- Secure admin login with password masking
- Default credentials: `admin` / `library123`

### 📚 Book Management
- Add, edit, delete books
- Search by ID, name, author, category
- Track multiple copies per book
- Categories: Fiction, Non-Fiction, Academic, Reference

### 👨‍🎓 Student Management
- Register, edit, delete students
- Track department and contact info
- View borrowing history per student
- Maximum 3 books per student limit

### 📋 Issue/Return System
- Issue books to registered students
- Return books with automatic fine calculation
- View currently issued books
- View overdue books list
- Fine: Rs. 5/day for late returns

### 📊 Reports & Statistics
- Library statistics dashboard
- Transaction history
- Student borrowing history

### 💾 Data Persistence
- Automatic save/load to binary files
- Separate files for books, students, transactions

---

## 🚀 Compilation

### Using GCC (MinGW on Windows):
```bash
gcc -o library main.c -Wall
```

### Using Visual Studio:
```bash
cl main.c /Fe:library.exe
```

### Using Code::Blocks / Dev-C++:
1. Open `main.c`
2. Press F9 to Build and Run

---

## 📖 Usage

1. Run the compiled executable
2. Login with admin credentials
3. Navigate using the menu options

### Default Login
- **Username:** `admin`
- **Password:** `library123`

---

## 📁 Data Files

The program creates these files in the same directory:
- `books.dat` - Book records
- `students.dat` - Student records  
- `transactions.dat` - Issue/return history

---

## 🛠️ Configuration

Edit these constants in `main.c` to customize:

```c
#define FINE_PER_DAY 5.0           // Fine amount per day
#define DEFAULT_ISSUE_DAYS 14       // Days before due date
#define MAX_BOOKS_PER_STUDENT 3     // Max books per student
#define ADMIN_USERNAME "admin"      // Admin username
#define ADMIN_PASSWORD "library123" // Admin password
```

---

## 📸 Screenshots

The system features a colorful console UI with:
- Box-drawing characters for menus
- Color-coded messages (green=success, red=error, yellow=warning)
- Formatted tables for data display

---

