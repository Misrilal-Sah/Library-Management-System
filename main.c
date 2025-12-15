/*
 *
 * Features:
 * - Admin Authentication System
 * - Book Management (Add, Edit, Delete, Search)
 * - Student Management (Add, Edit, Delete, Search)
 * - Issue/Return Books with Fine Calculation
 * - File-based Data Persistence
 * - Reports and Statistics
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>

// ═══════════════════════════════════════════════════════════════════════════
// CONSTANTS AND CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

#define MAX_BOOKS 500
#define MAX_STUDENTS 200
#define MAX_TRANSACTIONS 1000
#define MAX_STRING 50
#define FINE_PER_DAY 5.0
#define DEFAULT_ISSUE_DAYS 14
#define MAX_BOOKS_PER_STUDENT 3

// Admin credentials
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "library123"

// File paths
#define BOOKS_FILE "books.dat"
#define STUDENTS_FILE "students.dat"
#define TRANSACTIONS_FILE "transactions.dat"

// Console colors
#define CLR_DEFAULT 7
#define CLR_HEADER 11      // Cyan
#define CLR_SUCCESS 10     // Green
#define CLR_ERROR 12       // Red
#define CLR_WARNING 14     // Yellow
#define CLR_MENU 13        // Magenta
#define CLR_HIGHLIGHT 15   // White

// ═══════════════════════════════════════════════════════════════════════════
// DATA STRUCTURES
// ═══════════════════════════════════════════════════════════════════════════

typedef struct {
    int id;
    char name[MAX_STRING];
    char author[MAX_STRING];
    char category[MAX_STRING];
    int pages;
    float price;
    int totalCopies;
    int availableCopies;
    bool isActive;
} Book;

typedef struct {
    int id;
    char name[MAX_STRING];
    char department[MAX_STRING];
    char contact[15];
    int booksIssued;
    bool isActive;
} Student;

typedef struct {
    int id;
    int bookId;
    int studentId;
    char bookName[MAX_STRING];
    char studentName[MAX_STRING];
    char issueDate[30];
    char dueDate[30];
    char returnDate[30];
    float fineAmount;
    bool isReturned;
} Transaction;

// ═══════════════════════════════════════════════════════════════════════════
// GLOBAL VARIABLES
// ═══════════════════════════════════════════════════════════════════════════

Book books[MAX_BOOKS];
Student students[MAX_STUDENTS];
Transaction transactions[MAX_TRANSACTIONS];

int bookCount = 0;
int studentCount = 0;
int transactionCount = 0;
int nextBookId = 1;
int nextStudentId = 1;
int nextTransactionId = 1;

bool isLoggedIn = false;

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS - UI Helpers
// ═══════════════════════════════════════════════════════════════════════════

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void pressEnterToContinue() {
    setColor(CLR_WARNING);
    printf("\n\n  Press ENTER to continue...");
    setColor(CLR_DEFAULT);
    while (getchar() != '\n');
    getchar();
}

void printHeader(const char* title) {
    clearScreen();
    setColor(CLR_HEADER);
    printf("\n  ╔══════════════════════════════════════════════════════════════╗\n");
    printf("  ║              📚 LIBRARY MANAGEMENT SYSTEM 📚                 ║\n");
    printf("  ╠══════════════════════════════════════════════════════════════╣\n");
    setColor(CLR_HIGHLIGHT);
    printf("  ║  %-60s║\n", title);
    setColor(CLR_HEADER);
    printf("  ╚══════════════════════════════════════════════════════════════╝\n\n");
    setColor(CLR_DEFAULT);
}

void printSubHeader(const char* title) {
    setColor(CLR_MENU);
    printf("\n  ┌──────────────────────────────────────────────────────────────┐\n");
    printf("  │  %-60s│\n", title);
    printf("  └──────────────────────────────────────────────────────────────┘\n\n");
    setColor(CLR_DEFAULT);
}

void printSuccess(const char* message) {
    setColor(CLR_SUCCESS);
    printf("\n  ✓ %s\n", message);
    setColor(CLR_DEFAULT);
}

void printError(const char* message) {
    setColor(CLR_ERROR);
    printf("\n  ✗ %s\n", message);
    setColor(CLR_DEFAULT);
}

void printWarning(const char* message) {
    setColor(CLR_WARNING);
    printf("\n  ⚠ %s\n", message);
    setColor(CLR_DEFAULT);
}

void printDivider() {
    setColor(CLR_HEADER);
    printf("  ────────────────────────────────────────────────────────────────\n");
    setColor(CLR_DEFAULT);
}

// Safe string input function
void getStringInput(char* buffer, int maxLen) {
    fgets(buffer, maxLen, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
}

// Safe integer input function
int getIntInput() {
    int value;
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }
    return value;
}

// Safe float input function
float getFloatInput() {
    float value;
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", &value) != 1) {
        return -1.0f;
    }
    return value;
}

// ═══════════════════════════════════════════════════════════════════════════
// DATE UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void getCurrentDate(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 30, "%d-%m-%Y", tm_info);
}

void getDateAfterDays(char* buffer, int days) {
    time_t t = time(NULL);
    t += days * 24 * 60 * 60;
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 30, "%d-%m-%Y", tm_info);
}

int daysBetweenDates(const char* date1, const char* date2) {
    struct tm tm1 = {0}, tm2 = {0};
    sscanf(date1, "%d-%d-%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year);
    sscanf(date2, "%d-%d-%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year);
    tm1.tm_mon -= 1; tm1.tm_year -= 1900;
    tm2.tm_mon -= 1; tm2.tm_year -= 1900;
    
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    
    return (int)difftime(t2, t1) / (24 * 60 * 60);
}

// ═══════════════════════════════════════════════════════════════════════════
// FILE OPERATIONS - Data Persistence
// ═══════════════════════════════════════════════════════════════════════════

void saveBooks() {
    FILE* file = fopen(BOOKS_FILE, "wb");
    if (file == NULL) {
        printError("Could not save books data!");
        return;
    }
    fwrite(&bookCount, sizeof(int), 1, file);
    fwrite(&nextBookId, sizeof(int), 1, file);
    fwrite(books, sizeof(Book), bookCount, file);
    fclose(file);
}

void loadBooks() {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (file == NULL) {
        return; // File doesn't exist yet
    }
    fread(&bookCount, sizeof(int), 1, file);
    fread(&nextBookId, sizeof(int), 1, file);
    fread(books, sizeof(Book), bookCount, file);
    fclose(file);
}

void saveStudents() {
    FILE* file = fopen(STUDENTS_FILE, "wb");
    if (file == NULL) {
        printError("Could not save students data!");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(&nextStudentId, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
}

void loadStudents() {
    FILE* file = fopen(STUDENTS_FILE, "rb");
    if (file == NULL) {
        return;
    }
    fread(&studentCount, sizeof(int), 1, file);
    fread(&nextStudentId, sizeof(int), 1, file);
    fread(students, sizeof(Student), studentCount, file);
    fclose(file);
}

void saveTransactions() {
    FILE* file = fopen(TRANSACTIONS_FILE, "wb");
    if (file == NULL) {
        printError("Could not save transactions data!");
        return;
    }
    fwrite(&transactionCount, sizeof(int), 1, file);
    fwrite(&nextTransactionId, sizeof(int), 1, file);
    fwrite(transactions, sizeof(Transaction), transactionCount, file);
    fclose(file);
}

void loadTransactions() {
    FILE* file = fopen(TRANSACTIONS_FILE, "rb");
    if (file == NULL) {
        return;
    }
    fread(&transactionCount, sizeof(int), 1, file);
    fread(&nextTransactionId, sizeof(int), 1, file);
    fread(transactions, sizeof(Transaction), transactionCount, file);
    fclose(file);
}

void saveAllData() {
    saveBooks();
    saveStudents();
    saveTransactions();
}

void loadAllData() {
    loadBooks();
    loadStudents();
    loadTransactions();
}

// ═══════════════════════════════════════════════════════════════════════════
// AUTHENTICATION MODULE
// ═══════════════════════════════════════════════════════════════════════════

bool login() {
    char username[MAX_STRING];
    char password[MAX_STRING];
    int attempts = 3;
    
    while (attempts > 0) {
        printHeader("ADMIN LOGIN");
        
        setColor(CLR_DEFAULT);
        printf("  Enter Username: ");
        getStringInput(username, MAX_STRING);
        
        printf("  Enter Password: ");
        
        // Password masking
        int i = 0;
        char ch;
        while ((ch = getch()) != '\r') {
            if (ch == '\b' && i > 0) {
                i--;
                printf("\b \b");
            } else if (ch != '\b' && i < MAX_STRING - 1) {
                password[i++] = ch;
                printf("*");
            }
        }
        password[i] = '\0';
        printf("\n");
        
        if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
            printSuccess("Login successful! Welcome, Admin.");
            Sleep(1500);
            return true;
        } else {
            attempts--;
            setColor(CLR_ERROR);
            printf("\n  ✗ Invalid credentials! %d attempt(s) remaining.\n", attempts);
            setColor(CLR_DEFAULT);
            Sleep(1500);
        }
    }
    
    printError("Too many failed attempts. Exiting...");
    Sleep(2000);
    return false;
}

// ═══════════════════════════════════════════════════════════════════════════
// BOOK MANAGEMENT MODULE
// ═══════════════════════════════════════════════════════════════════════════

int findBookById(int id) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id && books[i].isActive) {
            return i;
        }
    }
    return -1;
}

int findBookByName(const char* name) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive && _stricmp(books[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void displayBookDetails(int index) {
    Book* b = &books[index];
    setColor(CLR_DEFAULT);
    printf("  │ %-4d │ %-22s │ %-18s │ %-12s │ %3d │ Rs.%-7.2f │ %d/%-2d │\n",
           b->id, b->name, b->author, b->category, b->pages, b->price,
           b->availableCopies, b->totalCopies);
}

void displayBookTableHeader() {
    setColor(CLR_HEADER);
    printf("  ┌──────┬────────────────────────┬────────────────────┬──────────────┬─────┬────────────┬──────┐\n");
    printf("  │  ID  │       Book Name        │       Author       │   Category   │ Pg  │   Price    │ Avl  │\n");
    printf("  ├──────┼────────────────────────┼────────────────────┼──────────────┼─────┼────────────┼──────┤\n");
    setColor(CLR_DEFAULT);
}

void displayBookTableFooter() {
    setColor(CLR_HEADER);
    printf("  └──────┴────────────────────────┴────────────────────┴──────────────┴─────┴────────────┴──────┘\n");
    setColor(CLR_DEFAULT);
}

void addBook() {
    printHeader("ADD NEW BOOK");
    
    if (bookCount >= MAX_BOOKS) {
        printError("Maximum book limit reached!");
        pressEnterToContinue();
        return;
    }
    
    Book* b = &books[bookCount];
    b->id = nextBookId++;
    b->isActive = true;
    
    printf("  Enter Book Name: ");
    getStringInput(b->name, MAX_STRING);
    
    if (strlen(b->name) == 0) {
        printError("Book name cannot be empty!");
        pressEnterToContinue();
        return;
    }
    
    // Check for duplicate
    if (findBookByName(b->name) >= 0) {
        printWarning("A book with this name already exists!");
        printf("  Do you want to add anyway? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        while (getchar() != '\n');
        if (choice != 'y' && choice != 'Y') {
            return;
        }
    }
    
    printf("  Enter Author Name: ");
    getStringInput(b->author, MAX_STRING);
    
    printf("  Enter Category (Fiction/Non-Fiction/Academic/Reference): ");
    getStringInput(b->category, MAX_STRING);
    
    printf("  Enter Number of Pages: ");
    b->pages = getIntInput();
    if (b->pages <= 0) b->pages = 1;
    
    printf("  Enter Price (Rs.): ");
    b->price = getFloatInput();
    if (b->price < 0) b->price = 0;
    
    printf("  Enter Total Copies: ");
    b->totalCopies = getIntInput();
    if (b->totalCopies <= 0) b->totalCopies = 1;
    
    b->availableCopies = b->totalCopies;
    
    bookCount++;
    saveBooks();
    
    printSuccess("Book added successfully!");
    printf("\n  Book ID: %d\n", b->id);
    pressEnterToContinue();
}

void displayAllBooks() {
    printHeader("ALL BOOKS IN LIBRARY");
    
    int activeBooks = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive) activeBooks++;
    }
    
    if (activeBooks == 0) {
        printWarning("No books available in the library.");
        pressEnterToContinue();
        return;
    }
    
    printf("  Total Books: %d\n\n", activeBooks);
    displayBookTableHeader();
    
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive) {
            displayBookDetails(i);
        }
    }
    
    displayBookTableFooter();
    pressEnterToContinue();
}

void searchBooks() {
    printHeader("SEARCH BOOKS");
    
    printSubHeader("Search Options");
    printf("  1. Search by Book ID\n");
    printf("  2. Search by Book Name\n");
    printf("  3. Search by Author\n");
    printf("  4. Search by Category\n");
    printf("  5. Back to Menu\n");
    
    printf("\n  Enter your choice: ");
    int choice = getIntInput();
    
    char searchTerm[MAX_STRING];
    int found = 0;
    
    switch (choice) {
        case 1: {
            printf("  Enter Book ID: ");
            int id = getIntInput();
            int index = findBookById(id);
            if (index >= 0) {
                printf("\n");
                displayBookTableHeader();
                displayBookDetails(index);
                displayBookTableFooter();
                found = 1;
            }
            break;
        }
        case 2:
            printf("  Enter Book Name: ");
            getStringInput(searchTerm, MAX_STRING);
            displayBookTableHeader();
            for (int i = 0; i < bookCount; i++) {
                if (books[i].isActive && strstr(books[i].name, searchTerm) != NULL) {
                    displayBookDetails(i);
                    found++;
                }
            }
            displayBookTableFooter();
            break;
        case 3:
            printf("  Enter Author Name: ");
            getStringInput(searchTerm, MAX_STRING);
            displayBookTableHeader();
            for (int i = 0; i < bookCount; i++) {
                if (books[i].isActive && strstr(books[i].author, searchTerm) != NULL) {
                    displayBookDetails(i);
                    found++;
                }
            }
            displayBookTableFooter();
            break;
        case 4:
            printf("  Enter Category: ");
            getStringInput(searchTerm, MAX_STRING);
            displayBookTableHeader();
            for (int i = 0; i < bookCount; i++) {
                if (books[i].isActive && strstr(books[i].category, searchTerm) != NULL) {
                    displayBookDetails(i);
                    found++;
                }
            }
            displayBookTableFooter();
            break;
        case 5:
            return;
        default:
            printError("Invalid choice!");
            pressEnterToContinue();
            return;
    }
    
    if (found == 0) {
        printWarning("No books found matching your search.");
    } else {
        setColor(CLR_SUCCESS);
        printf("\n  Found %d book(s).\n", found);
        setColor(CLR_DEFAULT);
    }
    
    pressEnterToContinue();
}

void editBook() {
    printHeader("EDIT BOOK");
    
    printf("  Enter Book ID to edit: ");
    int id = getIntInput();
    
    int index = findBookById(id);
    if (index < 0) {
        printError("Book not found!");
        pressEnterToContinue();
        return;
    }
    
    Book* b = &books[index];
    char input[MAX_STRING];
    
    printf("\n  Current Details:\n");
    displayBookTableHeader();
    displayBookDetails(index);
    displayBookTableFooter();
    
    printf("\n  (Press ENTER to keep current value)\n\n");
    
    printf("  Book Name [%s]: ", b->name);
    getStringInput(input, MAX_STRING);
    if (strlen(input) > 0) strcpy(b->name, input);
    
    printf("  Author [%s]: ", b->author);
    getStringInput(input, MAX_STRING);
    if (strlen(input) > 0) strcpy(b->author, input);
    
    printf("  Category [%s]: ", b->category);
    getStringInput(input, MAX_STRING);
    if (strlen(input) > 0) strcpy(b->category, input);
    
    printf("  Pages [%d]: ", b->pages);
    int pages = getIntInput();
    if (pages > 0) b->pages = pages;
    
    printf("  Price [%.2f]: ", b->price);
    float price = getFloatInput();
    if (price >= 0) b->price = price;
    
    printf("  Total Copies [%d]: ", b->totalCopies);
    int copies = getIntInput();
    if (copies > 0) {
        int issued = b->totalCopies - b->availableCopies;
        b->totalCopies = copies;
        b->availableCopies = copies - issued;
        if (b->availableCopies < 0) b->availableCopies = 0;
    }
    
    saveBooks();
    printSuccess("Book updated successfully!");
    pressEnterToContinue();
}

void deleteBook() {
    printHeader("DELETE BOOK");
    
    printf("  Enter Book ID to delete: ");
    int id = getIntInput();
    
    int index = findBookById(id);
    if (index < 0) {
        printError("Book not found!");
        pressEnterToContinue();
        return;
    }
    
    // Check if book is issued
    if (books[index].availableCopies < books[index].totalCopies) {
        printError("Cannot delete! Some copies of this book are currently issued.");
        pressEnterToContinue();
        return;
    }
    
    printf("\n  Book to delete:\n");
    displayBookTableHeader();
    displayBookDetails(index);
    displayBookTableFooter();
    
    setColor(CLR_WARNING);
    printf("\n  Are you sure you want to delete this book? (y/n): ");
    setColor(CLR_DEFAULT);
    
    char choice;
    scanf(" %c", &choice);
    while (getchar() != '\n');
    
    if (choice == 'y' || choice == 'Y') {
        books[index].isActive = false;
        saveBooks();
        printSuccess("Book deleted successfully!");
    } else {
        printWarning("Deletion cancelled.");
    }
    
    pressEnterToContinue();
}

void bookManagementMenu() {
    int choice = 0;
    
    while (choice != 6) {
        printHeader("BOOK MANAGEMENT");
        
        setColor(CLR_MENU);
        printf("  ┌────────────────────────────────────┐\n");
        printf("  │  1. 📚 Add New Book                │\n");
        printf("  │  2. 📋 Display All Books           │\n");
        printf("  │  3. 🔍 Search Books                │\n");
        printf("  │  4. ✏️  Edit Book                   │\n");
        printf("  │  5. 🗑️  Delete Book                 │\n");
        printf("  │  6. ⬅️  Back to Main Menu           │\n");
        printf("  └────────────────────────────────────┘\n");
        setColor(CLR_DEFAULT);
        
        printf("\n  Enter your choice: ");
        choice = getIntInput();
        
        switch (choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3: searchBooks(); break;
            case 4: editBook(); break;
            case 5: deleteBook(); break;
            case 6: break;
            default:
                printError("Invalid choice! Please try again.");
                Sleep(1000);
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// STUDENT MANAGEMENT MODULE
// ═══════════════════════════════════════════════════════════════════════════

int findStudentById(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id && students[i].isActive) {
            return i;
        }
    }
    return -1;
}

void displayStudentDetails(int index) {
    Student* s = &students[index];
    printf("  │ %-4d │ %-24s │ %-18s │ %-14s │  %d  │\n",
           s->id, s->name, s->department, s->contact, s->booksIssued);
}

void displayStudentTableHeader() {
    setColor(CLR_HEADER);
    printf("  ┌──────┬──────────────────────────┬────────────────────┬────────────────┬─────┐\n");
    printf("  │  ID  │      Student Name        │     Department     │    Contact     │ Bks │\n");
    printf("  ├──────┼──────────────────────────┼────────────────────┼────────────────┼─────┤\n");
    setColor(CLR_DEFAULT);
}

void displayStudentTableFooter() {
    setColor(CLR_HEADER);
    printf("  └──────┴──────────────────────────┴────────────────────┴────────────────┴─────┘\n");
    setColor(CLR_DEFAULT);
}

void addStudent() {
    printHeader("ADD NEW STUDENT");
    
    if (studentCount >= MAX_STUDENTS) {
        printError("Maximum student limit reached!");
        pressEnterToContinue();
        return;
    }
    
    Student* s = &students[studentCount];
    s->id = nextStudentId++;
    s->isActive = true;
    s->booksIssued = 0;
    
    printf("  Enter Student Name: ");
    getStringInput(s->name, MAX_STRING);
    
    if (strlen(s->name) == 0) {
        printError("Student name cannot be empty!");
        pressEnterToContinue();
        return;
    }
    
    printf("  Enter Department: ");
    getStringInput(s->department, MAX_STRING);
    
    printf("  Enter Contact Number: ");
    getStringInput(s->contact, 15);
    
    studentCount++;
    saveStudents();
    
    printSuccess("Student registered successfully!");
    printf("\n  Student ID: %d\n", s->id);
    pressEnterToContinue();
}

void displayAllStudents() {
    printHeader("ALL REGISTERED STUDENTS");
    
    int activeStudents = 0;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].isActive) activeStudents++;
    }
    
    if (activeStudents == 0) {
        printWarning("No students registered.");
        pressEnterToContinue();
        return;
    }
    
    printf("  Total Students: %d\n\n", activeStudents);
    displayStudentTableHeader();
    
    for (int i = 0; i < studentCount; i++) {
        if (students[i].isActive) {
            displayStudentDetails(i);
        }
    }
    
    displayStudentTableFooter();
    pressEnterToContinue();
}

void searchStudents() {
    printHeader("SEARCH STUDENTS");
    
    printf("  1. Search by Student ID\n");
    printf("  2. Search by Name\n");
    printf("  3. Search by Department\n");
    printf("  4. Back\n");
    
    printf("\n  Enter your choice: ");
    int choice = getIntInput();
    
    char searchTerm[MAX_STRING];
    int found = 0;
    
    switch (choice) {
        case 1: {
            printf("  Enter Student ID: ");
            int id = getIntInput();
            int index = findStudentById(id);
            if (index >= 0) {
                printf("\n");
                displayStudentTableHeader();
                displayStudentDetails(index);
                displayStudentTableFooter();
                found = 1;
            }
            break;
        }
        case 2:
            printf("  Enter Student Name: ");
            getStringInput(searchTerm, MAX_STRING);
            displayStudentTableHeader();
            for (int i = 0; i < studentCount; i++) {
                if (students[i].isActive && strstr(students[i].name, searchTerm) != NULL) {
                    displayStudentDetails(i);
                    found++;
                }
            }
            displayStudentTableFooter();
            break;
        case 3:
            printf("  Enter Department: ");
            getStringInput(searchTerm, MAX_STRING);
            displayStudentTableHeader();
            for (int i = 0; i < studentCount; i++) {
                if (students[i].isActive && strstr(students[i].department, searchTerm) != NULL) {
                    displayStudentDetails(i);
                    found++;
                }
            }
            displayStudentTableFooter();
            break;
        case 4:
            return;
        default:
            printError("Invalid choice!");
            pressEnterToContinue();
            return;
    }
    
    if (found == 0) {
        printWarning("No students found.");
    } else {
        setColor(CLR_SUCCESS);
        printf("\n  Found %d student(s).\n", found);
        setColor(CLR_DEFAULT);
    }
    
    pressEnterToContinue();
}

void editStudent() {
    printHeader("EDIT STUDENT");
    
    printf("  Enter Student ID to edit: ");
    int id = getIntInput();
    
    int index = findStudentById(id);
    if (index < 0) {
        printError("Student not found!");
        pressEnterToContinue();
        return;
    }
    
    Student* s = &students[index];
    char input[MAX_STRING];
    
    printf("\n  Current Details:\n");
    displayStudentTableHeader();
    displayStudentDetails(index);
    displayStudentTableFooter();
    
    printf("\n  (Press ENTER to keep current value)\n\n");
    
    printf("  Student Name [%s]: ", s->name);
    getStringInput(input, MAX_STRING);
    if (strlen(input) > 0) strcpy(s->name, input);
    
    printf("  Department [%s]: ", s->department);
    getStringInput(input, MAX_STRING);
    if (strlen(input) > 0) strcpy(s->department, input);
    
    printf("  Contact [%s]: ", s->contact);
    getStringInput(input, 15);
    if (strlen(input) > 0) strcpy(s->contact, input);
    
    saveStudents();
    printSuccess("Student updated successfully!");
    pressEnterToContinue();
}

void deleteStudent() {
    printHeader("DELETE STUDENT");
    
    printf("  Enter Student ID to delete: ");
    int id = getIntInput();
    
    int index = findStudentById(id);
    if (index < 0) {
        printError("Student not found!");
        pressEnterToContinue();
        return;
    }
    
    // Check if student has issued books
    if (students[index].booksIssued > 0) {
        printError("Cannot delete! Student has books issued. Return books first.");
        pressEnterToContinue();
        return;
    }
    
    printf("\n  Student to delete:\n");
    displayStudentTableHeader();
    displayStudentDetails(index);
    displayStudentTableFooter();
    
    setColor(CLR_WARNING);
    printf("\n  Are you sure you want to delete this student? (y/n): ");
    setColor(CLR_DEFAULT);
    
    char choice;
    scanf(" %c", &choice);
    while (getchar() != '\n');
    
    if (choice == 'y' || choice == 'Y') {
        students[index].isActive = false;
        saveStudents();
        printSuccess("Student deleted successfully!");
    } else {
        printWarning("Deletion cancelled.");
    }
    
    pressEnterToContinue();
}

void studentManagementMenu() {
    int choice = 0;
    
    while (choice != 6) {
        printHeader("STUDENT MANAGEMENT");
        
        setColor(CLR_MENU);
        printf("  ┌────────────────────────────────────┐\n");
        printf("  │  1. 👨‍🎓 Add New Student             │\n");
        printf("  │  2. 📋 Display All Students        │\n");
        printf("  │  3. 🔍 Search Students             │\n");
        printf("  │  4. ✏️  Edit Student                │\n");
        printf("  │  5. 🗑️  Delete Student              │\n");
        printf("  │  6. ⬅️  Back to Main Menu           │\n");
        printf("  └────────────────────────────────────┘\n");
        setColor(CLR_DEFAULT);
        
        printf("\n  Enter your choice: ");
        choice = getIntInput();
        
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudents(); break;
            case 4: editStudent(); break;
            case 5: deleteStudent(); break;
            case 6: break;
            default:
                printError("Invalid choice! Please try again.");
                Sleep(1000);
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// ISSUE/RETURN BOOKS MODULE
// ═══════════════════════════════════════════════════════════════════════════

void displayTransactionDetails(int index, bool detailed) {
    Transaction* t = &transactions[index];
    if (detailed) {
        printf("  │ %-4d │ %-20s │ %-18s │ %-10s │ %-10s │ %-8s │ Rs.%-6.2f │\n",
               t->id, t->bookName, t->studentName, t->issueDate, t->dueDate,
               t->isReturned ? "Returned" : "Pending", t->fineAmount);
    } else {
        printf("  │ %-4d │ %-20s │ %-18s │ %-10s │ %-10s │\n",
               t->id, t->bookName, t->studentName, t->issueDate, t->dueDate);
    }
}

void issueBook() {
    printHeader("ISSUE BOOK");
    
    // Check if there are books and students
    int activeBooks = 0, activeStudents = 0;
    for (int i = 0; i < bookCount; i++) if (books[i].isActive && books[i].availableCopies > 0) activeBooks++;
    for (int i = 0; i < studentCount; i++) if (students[i].isActive) activeStudents++;
    
    if (activeBooks == 0) {
        printError("No books available for issue!");
        pressEnterToContinue();
        return;
    }
    
    if (activeStudents == 0) {
        printError("No students registered. Please add a student first.");
        pressEnterToContinue();
        return;
    }
    
    printf("  Enter Student ID: ");
    int studentId = getIntInput();
    
    int studentIndex = findStudentById(studentId);
    if (studentIndex < 0) {
        printError("Student not found!");
        pressEnterToContinue();
        return;
    }
    
    if (students[studentIndex].booksIssued >= MAX_BOOKS_PER_STUDENT) {
        printError("Student has reached maximum book limit!");
        printf("\n  Current books issued: %d (Max: %d)\n", 
               students[studentIndex].booksIssued, MAX_BOOKS_PER_STUDENT);
        pressEnterToContinue();
        return;
    }
    
    printf("  Enter Book ID: ");
    int bookId = getIntInput();
    
    int bookIndex = findBookById(bookId);
    if (bookIndex < 0) {
        printError("Book not found!");
        pressEnterToContinue();
        return;
    }
    
    if (books[bookIndex].availableCopies <= 0) {
        printError("No copies available for this book!");
        pressEnterToContinue();
        return;
    }
    
    // Check if student already has this book
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].studentId == studentId && 
            transactions[i].bookId == bookId && 
            !transactions[i].isReturned) {
            printError("Student already has a copy of this book!");
            pressEnterToContinue();
            return;
        }
    }
    
    // Create transaction
    Transaction* t = &transactions[transactionCount];
    t->id = nextTransactionId++;
    t->bookId = bookId;
    t->studentId = studentId;
    strcpy(t->bookName, books[bookIndex].name);
    strcpy(t->studentName, students[studentIndex].name);
    getCurrentDate(t->issueDate);
    getDateAfterDays(t->dueDate, DEFAULT_ISSUE_DAYS);
    t->returnDate[0] = '\0';
    t->fineAmount = 0;
    t->isReturned = false;
    
    // Update counts
    books[bookIndex].availableCopies--;
    students[studentIndex].booksIssued++;
    transactionCount++;
    
    saveAllData();
    
    printSuccess("Book issued successfully!");
    printf("\n  Transaction ID: %d\n", t->id);
    printf("  Book: %s\n", t->bookName);
    printf("  Student: %s\n", t->studentName);
    printf("  Issue Date: %s\n", t->issueDate);
    printf("  Due Date: %s\n", t->dueDate);
    
    pressEnterToContinue();
}

void returnBook() {
    printHeader("RETURN BOOK");
    
    printf("  Enter Transaction ID: ");
    int transactionId = getIntInput();
    
    int transIndex = -1;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].id == transactionId && !transactions[i].isReturned) {
            transIndex = i;
            break;
        }
    }
    
    if (transIndex < 0) {
        printError("Transaction not found or book already returned!");
        pressEnterToContinue();
        return;
    }
    
    Transaction* t = &transactions[transIndex];
    
    // Display transaction details
    printf("\n  Transaction Details:\n");
    printDivider();
    printf("  Book: %s\n", t->bookName);
    printf("  Student: %s\n", t->studentName);
    printf("  Issue Date: %s\n", t->issueDate);
    printf("  Due Date: %s\n", t->dueDate);
    printDivider();
    
    // Calculate fine
    char currentDate[30];
    getCurrentDate(currentDate);
    int daysLate = daysBetweenDates(t->dueDate, currentDate);
    
    float fine = 0;
    if (daysLate > 0) {
        fine = daysLate * FINE_PER_DAY;
        setColor(CLR_WARNING);
        printf("\n  ⚠️  Book is %d day(s) overdue!\n", daysLate);
        printf("  Fine Amount: Rs. %.2f (Rs. %.2f/day)\n", fine, FINE_PER_DAY);
        setColor(CLR_DEFAULT);
    } else {
        printSuccess("Book returned on time! No fine.");
    }
    
    printf("\n  Confirm return? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    while (getchar() != '\n');
    
    if (choice == 'y' || choice == 'Y') {
        t->isReturned = true;
        strcpy(t->returnDate, currentDate);
        t->fineAmount = fine;
        
        // Update book and student
        int bookIndex = findBookById(t->bookId);
        int studentIndex = findStudentById(t->studentId);
        
        if (bookIndex >= 0) books[bookIndex].availableCopies++;
        if (studentIndex >= 0) students[studentIndex].booksIssued--;
        
        saveAllData();
        
        printSuccess("Book returned successfully!");
        if (fine > 0) {
            setColor(CLR_WARNING);
            printf("\n  Please collect fine of Rs. %.2f from the student.\n", fine);
            setColor(CLR_DEFAULT);
        }
    } else {
        printWarning("Return cancelled.");
    }
    
    pressEnterToContinue();
}

void displayIssuedBooks() {
    printHeader("CURRENTLY ISSUED BOOKS");
    
    int issuedCount = 0;
    
    setColor(CLR_HEADER);
    printf("  ┌──────┬──────────────────────┬────────────────────┬────────────┬────────────┐\n");
    printf("  │  ID  │      Book Name       │    Student Name    │ Issue Date │  Due Date  │\n");
    printf("  ├──────┼──────────────────────┼────────────────────┼────────────┼────────────┤\n");
    setColor(CLR_DEFAULT);
    
    for (int i = 0; i < transactionCount; i++) {
        if (!transactions[i].isReturned) {
            displayTransactionDetails(i, false);
            issuedCount++;
        }
    }
    
    setColor(CLR_HEADER);
    printf("  └──────┴──────────────────────┴────────────────────┴────────────┴────────────┘\n");
    setColor(CLR_DEFAULT);
    
    if (issuedCount == 0) {
        printWarning("No books are currently issued.");
    } else {
        printf("\n  Total issued books: %d\n", issuedCount);
    }
    
    pressEnterToContinue();
}

void displayOverdueBooks() {
    printHeader("OVERDUE BOOKS");
    
    char currentDate[30];
    getCurrentDate(currentDate);
    
    int overdueCount = 0;
    
    setColor(CLR_HEADER);
    printf("  ┌──────┬──────────────────────┬────────────────────┬────────────┬────────────┬──────────┐\n");
    printf("  │  ID  │      Book Name       │    Student Name    │ Issue Date │  Due Date  │ Days Late│\n");
    printf("  ├──────┼──────────────────────┼────────────────────┼────────────┼────────────┼──────────┤\n");
    setColor(CLR_DEFAULT);
    
    for (int i = 0; i < transactionCount; i++) {
        if (!transactions[i].isReturned) {
            int daysLate = daysBetweenDates(transactions[i].dueDate, currentDate);
            if (daysLate > 0) {
                Transaction* t = &transactions[i];
                setColor(CLR_ERROR);
                printf("  │ %-4d │ %-20s │ %-18s │ %-10s │ %-10s │    %-5d │\n",
                       t->id, t->bookName, t->studentName, t->issueDate, t->dueDate, daysLate);
                setColor(CLR_DEFAULT);
                overdueCount++;
            }
        }
    }
    
    setColor(CLR_HEADER);
    printf("  └──────┴──────────────────────┴────────────────────┴────────────┴────────────┴──────────┘\n");
    setColor(CLR_DEFAULT);
    
    if (overdueCount == 0) {
        printSuccess("No overdue books! All books returned on time.");
    } else {
        setColor(CLR_WARNING);
        printf("\n  Total overdue books: %d\n", overdueCount);
        printf("  Estimated total fine: Rs. %.2f\n", overdueCount * FINE_PER_DAY);
        setColor(CLR_DEFAULT);
    }
    
    pressEnterToContinue();
}

void issueReturnMenu() {
    int choice = 0;
    
    while (choice != 5) {
        printHeader("ISSUE / RETURN BOOKS");
        
        setColor(CLR_MENU);
        printf("  ┌────────────────────────────────────┐\n");
        printf("  │  1. 📤 Issue Book                  │\n");
        printf("  │  2. 📥 Return Book                 │\n");
        printf("  │  3. 📋 View Issued Books           │\n");
        printf("  │  4. ⚠️  View Overdue Books          │\n");
        printf("  │  5. ⬅️  Back to Main Menu           │\n");
        printf("  └────────────────────────────────────┘\n");
        setColor(CLR_DEFAULT);
        
        printf("\n  Enter your choice: ");
        choice = getIntInput();
        
        switch (choice) {
            case 1: issueBook(); break;
            case 2: returnBook(); break;
            case 3: displayIssuedBooks(); break;
            case 4: displayOverdueBooks(); break;
            case 5: break;
            default:
                printError("Invalid choice! Please try again.");
                Sleep(1000);
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// REPORTS MODULE
// ═══════════════════════════════════════════════════════════════════════════

void displayLibraryStats() {
    printHeader("LIBRARY STATISTICS");
    
    int totalBooks = 0, availableBooks = 0, issuedBooks = 0;
    int totalStudents = 0;
    int totalTransactions = 0, pendingReturns = 0;
    float totalFineCollected = 0;
    
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive) {
            totalBooks += books[i].totalCopies;
            availableBooks += books[i].availableCopies;
        }
    }
    issuedBooks = totalBooks - availableBooks;
    
    for (int i = 0; i < studentCount; i++) {
        if (students[i].isActive) totalStudents++;
    }
    
    for (int i = 0; i < transactionCount; i++) {
        totalTransactions++;
        if (!transactions[i].isReturned) pendingReturns++;
        totalFineCollected += transactions[i].fineAmount;
    }
    
    setColor(CLR_MENU);
    printf("  ╔══════════════════════════════════════════════════════════════╗\n");
    printf("  ║                    📊 LIBRARY STATISTICS                     ║\n");
    printf("  ╠══════════════════════════════════════════════════════════════╣\n");
    setColor(CLR_HIGHLIGHT);
    printf("  ║  📚 BOOKS                                                    ║\n");
    setColor(CLR_DEFAULT);
    printf("  ║     Total Book Titles    : %-10d                        ║\n", bookCount);
    printf("  ║     Total Copies         : %-10d                        ║\n", totalBooks);
    printf("  ║     Available Copies     : %-10d                        ║\n", availableBooks);
    printf("  ║     Issued Copies        : %-10d                        ║\n", issuedBooks);
    setColor(CLR_MENU);
    printf("  ╠══════════════════════════════════════════════════════════════╣\n");
    setColor(CLR_HIGHLIGHT);
    printf("  ║  👨‍🎓 STUDENTS                                                 ║\n");
    setColor(CLR_DEFAULT);
    printf("  ║     Registered Students  : %-10d                        ║\n", totalStudents);
    setColor(CLR_MENU);
    printf("  ╠══════════════════════════════════════════════════════════════╣\n");
    setColor(CLR_HIGHLIGHT);
    printf("  ║  📋 TRANSACTIONS                                             ║\n");
    setColor(CLR_DEFAULT);
    printf("  ║     Total Transactions   : %-10d                        ║\n", totalTransactions);
    printf("  ║     Pending Returns      : %-10d                        ║\n", pendingReturns);
    printf("  ║     Fine Collected       : Rs. %-10.2f                   ║\n", totalFineCollected);
    setColor(CLR_MENU);
    printf("  ╚══════════════════════════════════════════════════════════════╝\n");
    setColor(CLR_DEFAULT);
    
    pressEnterToContinue();
}

void displayTransactionHistory() {
    printHeader("TRANSACTION HISTORY");
    
    if (transactionCount == 0) {
        printWarning("No transactions recorded yet.");
        pressEnterToContinue();
        return;
    }
    
    setColor(CLR_HEADER);
    printf("  ┌──────┬──────────────────────┬────────────────────┬────────────┬────────────┬──────────┬───────────┐\n");
    printf("  │  ID  │      Book Name       │    Student Name    │ Issue Date │  Due Date  │  Status  │   Fine    │\n");
    printf("  ├──────┼──────────────────────┼────────────────────┼────────────┼────────────┼──────────┼───────────┤\n");
    setColor(CLR_DEFAULT);
    
    for (int i = transactionCount - 1; i >= 0 && i >= transactionCount - 20; i--) {
        displayTransactionDetails(i, true);
    }
    
    setColor(CLR_HEADER);
    printf("  └──────┴──────────────────────┴────────────────────┴────────────┴────────────┴──────────┴───────────┘\n");
    setColor(CLR_DEFAULT);
    
    printf("\n  Showing last %d transactions.\n", transactionCount > 20 ? 20 : transactionCount);
    
    pressEnterToContinue();
}

void studentBorrowingHistory() {
    printHeader("STUDENT BORROWING HISTORY");
    
    printf("  Enter Student ID: ");
    int studentId = getIntInput();
    
    int studentIndex = findStudentById(studentId);
    if (studentIndex < 0) {
        printError("Student not found!");
        pressEnterToContinue();
        return;
    }
    
    printf("\n  Student: %s (%s)\n\n", students[studentIndex].name, students[studentIndex].department);
    
    setColor(CLR_HEADER);
    printf("  ┌──────┬──────────────────────┬────────────┬────────────┬──────────┬───────────┐\n");
    printf("  │  ID  │      Book Name       │ Issue Date │  Due Date  │  Status  │   Fine    │\n");
    printf("  ├──────┼──────────────────────┼────────────┼────────────┼──────────┼───────────┤\n");
    setColor(CLR_DEFAULT);
    
    int found = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].studentId == studentId) {
            Transaction* t = &transactions[i];
            printf("  │ %-4d │ %-20s │ %-10s │ %-10s │ %-8s │ Rs.%-6.2f │\n",
                   t->id, t->bookName, t->issueDate, t->dueDate,
                   t->isReturned ? "Returned" : "Pending", t->fineAmount);
            found++;
        }
    }
    
    setColor(CLR_HEADER);
    printf("  └──────┴──────────────────────┴────────────┴────────────┴──────────┴───────────┘\n");
    setColor(CLR_DEFAULT);
    
    if (found == 0) {
        printWarning("No borrowing history for this student.");
    }
    
    pressEnterToContinue();
}

void reportsMenu() {
    int choice = 0;
    
    while (choice != 4) {
        printHeader("REPORTS & STATISTICS");
        
        setColor(CLR_MENU);
        printf("  ┌────────────────────────────────────┐\n");
        printf("  │  1. 📊 Library Statistics          │\n");
        printf("  │  2. 📜 Transaction History         │\n");
        printf("  │  3. 👨‍🎓 Student Borrowing History   │\n");
        printf("  │  4. ⬅️  Back to Main Menu           │\n");
        printf("  └────────────────────────────────────┘\n");
        setColor(CLR_DEFAULT);
        
        printf("\n  Enter your choice: ");
        choice = getIntInput();
        
        switch (choice) {
            case 1: displayLibraryStats(); break;
            case 2: displayTransactionHistory(); break;
            case 3: studentBorrowingHistory(); break;
            case 4: break;
            default:
                printError("Invalid choice! Please try again.");
                Sleep(1000);
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN MENU AND PROGRAM ENTRY
// ═══════════════════════════════════════════════════════════════════════════

void mainMenu() {
    int choice = 0;
    
    while (choice != 6) {
        printHeader("MAIN MENU");
        
        setColor(CLR_MENU);
        printf("  ╔════════════════════════════════════╗\n");
        printf("  ║  1. 📚 Book Management             ║\n");
        printf("  ║  2. 👨‍🎓 Student Management          ║\n");
        printf("  ║  3. 📋 Issue/Return Books          ║\n");
        printf("  ║  4. 📊 Reports & Statistics        ║\n");
        printf("  ║  5. 💾 Save All Data               ║\n");
        printf("  ║  6. 🚪 Exit                        ║\n");
        printf("  ╚════════════════════════════════════╝\n");
        setColor(CLR_DEFAULT);
        
        printf("\n  Enter your choice: ");
        choice = getIntInput();
        
        switch (choice) {
            case 1: bookManagementMenu(); break;
            case 2: studentManagementMenu(); break;
            case 3: issueReturnMenu(); break;
            case 4: reportsMenu(); break;
            case 5:
                saveAllData();
                printSuccess("All data saved successfully!");
                Sleep(1500);
                break;
            case 6:
                saveAllData();
                printSuccess("Data saved. Thank you for using Library Management System!");
                Sleep(2000);
                break;
            default:
                printError("Invalid choice! Please try again.");
                Sleep(1000);
        }
    }
}

int main() {
    // Set console title
    SetConsoleTitle("Library Management System v2.0");
    
    // Set console to use UTF-8
    SetConsoleOutputCP(65001);
    
    // Load existing data
    loadAllData();
    
    // Login
    if (!login()) {
        return 1;
    }
    
    isLoggedIn = true;
    
    // Show welcome message
    printHeader("WELCOME");
    setColor(CLR_SUCCESS);
    printf("  Welcome to Library Management System v2.0!\n\n");
    setColor(CLR_DEFAULT);
    printf("  Loaded:\n");
    printf("    • %d book(s)\n", bookCount);
    printf("    • %d student(s)\n", studentCount);
    printf("    • %d transaction(s)\n", transactionCount);
    
    Sleep(2000);
    
    // Main menu loop
    mainMenu();
    
    return 0;
}
