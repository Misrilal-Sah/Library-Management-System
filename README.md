<div align="center">

```
██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗
██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝
██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝ 
██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝  
███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║   
╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝  
```

### ⚡ MANAGEMENT SYSTEM ⚡
#### `v2.0` — Built in C · Runs on Windows · Zero Dependencies

<br/>

![Language](https://img.shields.io/badge/Language-C-00d4ff?style=for-the-badge&logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078d4?style=for-the-badge&logo=windows&logoColor=white)
![Version](https://img.shields.io/badge/Version-2.0-ffe600?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-00ff88?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-00ff88?style=for-the-badge)

</div>

---

<div align="center">

> **⚡ A blazing-fast, terminal-native library management system**  
> **Purpose-built for colleges and educational institutions.**  
> Handles books, students, transactions — all from a single `.c` file.

</div>

---

## ⚡ FEATURE MATRIX

<table>
<tr>
<td width="50%">

### 🔐 Authentication Engine
```
✦ Secure admin login
✦ Live password masking (getch)
✦ 3-attempt lockout protection
✦ Instant session invalidation
```

### 📚 Book Management
```
✦ Add / Edit / Delete books
✦ Multi-copy inventory tracking
✦ Search: ID · Name · Author · Category
✦ Categories: Fiction · Non-Fiction
              Academic · Reference
✦ Capacity: 500 book records
```

### 💾 Data Persistence
```
✦ Binary file I/O (no database needed)
✦ Auto-save on every write operation
✦ Separate stores per entity
✦ Survives restarts — data intact
```

</td>
<td width="50%">

### 👨‍🎓 Student Management
```
✦ Register / Edit / Delete students
✦ Department & contact tracking
✦ Per-student borrow history
✦ Hard limit: 3 books per student
✦ Capacity: 200 student records
```

### 📋 Issue / Return System
```
✦ Issue books to registered students
✦ Auto due-date calculation (14 days)
✦ Late return fine: ₹5/day
✦ Overdue books quick-view
✦ Capacity: 1000 transactions
```

### 📊 Reports & Analytics
```
✦ Library stats dashboard
✦ Full transaction history log
✦ Per-student borrowing history
✦ Live available copies counter
```

</td>
</tr>
</table>

---

## ⚡ TECH STACK

| Layer | Technology | Details |
|---|---|---|
| **Language** | `C (C99)` | Pure standard C, no external libs |
| **UI Engine** | `Windows Console API` | `SetConsoleTextAttribute` for 16-color output |
| **Storage** | `Binary File I/O` | `fread` / `fwrite` — no SQL, no ORM |
| **Time** | `<time.h>` | Native date diff for fine calculation |
| **Input** | `<conio.h>` | `getch()` for real-time password masking |
| **Build** | `GCC / MSVC / Code::Blocks` | Single-file compile |

---

## ⚡ QUICK START

### Step 1 — Clone or Download

```bash
git clone https://github.com/yourusername/library-management.git
cd library-management
```

### Step 2 — Compile

**GCC (MinGW on Windows) — Recommended:**
```bash
gcc -o library main.c -Wall -std=c99
```

**MSVC (Visual Studio Developer Prompt):**
```bash
cl main.c /Fe:library.exe
```

**Code::Blocks / Dev-C++ / Turbo C:**
```
1. Open main.c
2. Press F9 → Build and Run
```

### Step 3 — Run

```bash
./library.exe
```

> Login prompt appears immediately. See credentials below.

---

## ⚡ DEFAULT CREDENTIALS

```
╔══════════════════════════════════╗
║   👤  Username  :  admin         ║
║   🔑  Password  :  library123    ║
╚══════════════════════════════════╝
```

> ⚠️ Change the password constants in `main.c` before deploying in production.

---

## ⚡ CONFIGURATION

All tuneable constants live at the top of `main.c`:

```c
// ── Limits ─────────────────────────────────────────────────
#define MAX_BOOKS            500      // Max book records
#define MAX_STUDENTS         200      // Max student records
#define MAX_TRANSACTIONS     1000     // Max transaction records

// ── Business Rules ─────────────────────────────────────────
#define FINE_PER_DAY         5.0      // ₹ Fine per overdue day
#define DEFAULT_ISSUE_DAYS   14       // Loan window (days)
#define MAX_BOOKS_PER_STUDENT 3       // Borrow limit per student

// ── Auth ───────────────────────────────────────────────────
#define ADMIN_USERNAME       "admin"
#define ADMIN_PASSWORD       "library123"
```

---

## ⚡ DATA FILES

The application auto-creates three binary stores on first run:

```
library-management/
├── main.c                  ← entire source (single file)
├── books.dat               ← book records (binary)
├── students.dat            ← student records (binary)
└── transactions.dat        ← issue/return history (binary)
```

> These files are created automatically. Do **not** edit them manually.

---

## ⚡ CONSOLE UI PREVIEW

```
  ╔══════════════════════════════════════════════════════════════╗
  ║              📚 LIBRARY MANAGEMENT SYSTEM 📚                 ║
  ╠══════════════════════════════════════════════════════════════╣
  ║  MAIN MENU                                                   ║
  ╚══════════════════════════════════════════════════════════════╝

  [1] 📚  Book Management
  [2] 👨‍🎓  Student Management
  [3] 📋  Issue / Return Books
  [4] 📊  Reports & Statistics
  [0] 🚪  Logout & Exit
```

Color-coded feedback across the entire UI:
- 🟢 **Green** — success operations
- 🔴 **Red** — errors & blocked actions
- 🟡 **Yellow** — warnings & prompts
- 🔵 **Cyan** — headers & navigation
- 🟣 **Magenta** — sub-menus & sections

---

## ⚡ SYSTEM CONSTRAINTS

| Constraint | Limit |
|---|---|
| Max books in system | `500` |
| Max students registered | `200` |
| Max transactions logged | `1,000` |
| Books per student at once | `3` |
| Loan period default | `14 days` |
| Overdue fine rate | `₹5.00 / day` |
| Login attempts before lockout | `3` |

---

<div align="center">

---

```
⚡ ─────────────────────────────────────────────────────── ⚡
      LIBRARY MANAGEMENT SYSTEM  ·  v2.0  ·  Built in C
      Single-file · Zero dependencies · Terminal-native
⚡ ─────────────────────────────────────────────────────── ⚡
```

**Built with ⚡ by a developer who believes great software**  
**doesn't need a framework — just a good `.c` file.**

<br/>

![Made with C](https://img.shields.io/badge/Made%20with-C-00d4ff?style=flat-square&logo=c)
![Console UI](https://img.shields.io/badge/UI-Windows%20Console-0078d4?style=flat-square)
![No Dependencies](https://img.shields.io/badge/Dependencies-Zero-00ff88?style=flat-square)
![Single File](https://img.shields.io/badge/Source-Single%20File-ffe600?style=flat-square)

</div>

