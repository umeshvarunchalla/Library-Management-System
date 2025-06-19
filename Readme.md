# Library Management System

This project implements a Library Management System in C++ using Object-Oriented Programming (OOP) principles. It was developed as part of an Assignment in CS253.

## Objective

The goal of this project is to demonstrate key OOP concepts—such as abstraction, inheritance, polymorphism, and encapsulation—while developing a system that efficiently manages library operations, including:
- Borrowing, reserving and returning books.
- Tracking overdue items and calculating fines.
- Managing different user roles (Student, Faculty, and Librarian) with role-specific permissions.

## Overview

Upon executing the code, the system automatically loads data from text files. It then presents a login menu that gives you the option to log in as a **Student**, **Faculty**, or **Librarian**. Choosing **Exit** stops the program by saving all data back to the text files.

---

## Role-Based Functionalities

### Librarian
- **Add Books:**  
  The librarian can add new books to the system. A unique Book ID is generated automatically and assigned to each new book.
  
- **Delete Books:**  
  The librarian can remove books from the system.(iff it is not borrowed and not reserved any user)

- **Add Users:**  
  The librarian can add new users (students, faculty, etc.). A unique User ID is automatically generated for each new user, and users must use that ID for subsequent logins.

- **Delete Users:**  
  The librarian can remove users from the system.(iff he returned all the books, cleared the fine, and has no reserved_book)

- **Update Student Fine:**  
  The librarian has the ability to update or modify the fine amount for any student.

  A student can pay an amount <= Fine.

- **View Lists:**  
  The librarian can view complete lists of both books and registered users.

### Student
- **Borrow Book:**  
  Students can borrow a book if it is available, with a maximum limit of 3 books at a time.

- **Return Book:**  
  Students can return books they have borrowed.

- **View List of Books:**  
  Students can view the complete list of available books.

- **View Account:**  
  Students can view their account details, which include:
  - The info of reserved book(is reserved any).
  - The list of currently borrowed books.
  - The current fine amount (if any).
  - Their borrowing history.

- **Reserve Book:**
  - Faculty can reserve atmost 1 book at a time(Irrespective of the overdues and fine).
### Faculty
- **Borrow Book:**  
  Faculty members can borrow books with a higher limit of up to 5 books at a time.

- **Return Book:**  
  Faculty members can return books they have borrowed.

- **View List of Books:**  
  Faculty can view the list of available books.

- **View Account:**  
  Faculty members can view their account details, which include:
  - The info of reserved book(is reserved any).
  - The list of currently borrowed books.
  - Their borrowing history.  
  *(Note: Faculty members are not subject to fines for overdue books.)*

- **Reserve Book:**
  - Student can reserve atmost 1 book at a time (Irrespective of the overdues).

## Files
- The text files already contains 3 faculties, 5 students, 1 librarian, 10 books.

- **Librarian_management_system.cpp**
  - This file contains all the code and the code uses the text file to load and save the data.

- **books.txt**
  - Saves and loads all books and their data.
  
  - Format - bookID, title, author, publisher, year, ISBN, status, reserver(either a facultyID or a studentID or "NULL"(if no one has reserved it)).

- **students.txt**
  - Saves and loads all students and their data.

  - Format- userID, username, password, fine, reserved_bookID("NULL" if have no reserved_book), borrowed_books(bookID,borrowed date), borrowing_history(starts with "HISTORY" followed by(if any)  bookID,borrowed date, returned date).

- **counters.txt**
  - Saves and loads counters which is used for userID generation or bookId generation.(Independent of no of objects)

  - Counter increases by as an new object created and doesnot alter if any object is deleted. 

- **faculties.txt**
  - Saves and loads all faculties and their data.
  
  - Format- userID, username, password, reserved_bookID("NULL" if have no reserved_book), borrowed_books(bookID,borrowed date), borrowing_history(starts with "HISTORY" followed by(if any)  bookID,borrowed date, returned date).

- **librarians.txt**
  - Saves and loads all librarians and their data(userID, username, password)

## Constraints
  - A book can be reserved by atmost 1 user at a time.
  - A user can reserve atmost 1 book at a time.

  - The code is such that it only consider days and truncates hours or minutes or seconds for calculating due periods.

  - Student have to consult Librarian to pay Fine.

  - Fine is calculated once the book is returned(for students).

  - Maximum due period of faculty is assumed to be 30 days.

  - The user have to enter the valid number among by reading displayed Menu properly for different actions according to their requirement(Menu will be displayed at every step).

  - UserId's and BookId's are generated automatically.

  - There is no chance of changing userid,bookid,username,password as they have to remember as it shows the ID at the time of creating new user or book .They have to consult the librarian(who can view all the data) if they have forgotten any of those.

  - bookID starts with "B".

  - userID for student starts with "S".

  - userID for faculty starts with "F".

  - userID for Librarian starts with "L".