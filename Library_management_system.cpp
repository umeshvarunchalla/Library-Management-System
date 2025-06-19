#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

enum Status {available,borrowed,reserved};
//struct for storing borrowing history
struct history
{
    string bookID;
    time_t borrowed_time;
    time_t returned_time;
};
//forward declaration
class Book;
class Account;
class User;
class Student;
class Faculty;
class Librarian;

class Library
{
private:
    vector<Book*> books;
    vector<Student*> students;
    vector<Faculty*> faculties;
    vector<Librarian*> librarians;
public:
    void Load(){
        //load from database
        LoadBooksFromFile();
        LoadStudentsFromFile(); 
        LoadFacultiesFromFile();
        LoadLibrariansFromFile();
        LoadCountersFromFile();
    }
    void LoadBooksFromFile();
    void LoadStudentsFromFile();
    void LoadFacultiesFromFile();
    void LoadLibrariansFromFile();
    void LoadCountersFromFile();
    void Save(){
        //save to database
        SaveBooksToFile();
        SaveStudentsToFile();
        SaveFacultiesToFile();
        SaveLibrariansToFile();
        SaveCountersToFile();
    }
    void SaveBooksToFile();
    void SaveStudentsToFile();
    void SaveFacultiesToFile();
    void SaveLibrariansToFile();
    void SaveCountersToFile();
    void Add_book(Librarian* librarian);
    void Delete_book(Librarian* librarian);
    void Add_student(Librarian* librarian);
    void Delete_student(Librarian* librarian);
    void Add_faculty(Librarian* librarian);
    void Delete_faculty(Librarian* librarian);
    void Add_librarian(Librarian* librarian);
    void Delete_librarian(Librarian* librarian);
    Book* Get_book(string bookID);
    Student* Get_student(string studentID);
    Faculty* Get_faculty(string facultyID);
    Librarian* Get_librarian(string librarianID);
    void View_books(Librarian* librarian);
    void View_books(Student* student);
    void View_books(Faculty* faculty);
    void View_students(Librarian* librarian);
    void View_faculties(Librarian* librarian);
    void View_librarians(Librarian* librarian);
    void Update_fine(Librarian* librarian);
    void Student_home(Student* student);
    void Faculty_home(Faculty* faculty);
    void Librarian_home(Librarian* librarian);
    void Home();
};

class Book
{ 
private:
    string reserved_student;//userID of student who reserved the book(if any else "NULL")
    string reserved_faculty;//userID of faculty who reserved the book(if any else "NULL")
public:
    string bookID;
    string  title;
    string author;
    string publisher;
    int year;
    long long ISBN;
    Status status;
    static long long book_num;
    Book(string  title,string author,string publisher,int year,long long ISBN,Status status){
        this->title=title;
        this->author=author;
        this->ISBN=ISBN;
        this->publisher=publisher;
        this->year=year;
        this->status=status;
        this->bookID='B'+to_string(book_num);
        this->reserved_student="NULL";
        this->reserved_faculty="NULL";
        book_num++;
    }
    string Get_bookID(){
        return this->bookID;
    }
    string Get_title(){
        return this->title;
    }
    string Get_author(){
        return this->author;
    }
    string Get_publisher(){
        return this->publisher;
    }
    Status Get_status(){
        return this->status;
    }
    int Get_year(){
        return this->year;
    }
    long long Get_ISBN(){
        return this->ISBN;
    }
    string Get_reserved_student(){
        return this->reserved_student;
    }
    string Get_reserved_faculty(){
        return this->reserved_faculty;
    }
    void Set_status(Status status){
        this->status=status;
    }
    void Set_bookID(string bookID){
        this->bookID=bookID;
    }
    void Set_counter(long long book_num){
        this->book_num=book_num;
    }
    void Set_reserved_student(string studentID){
        this->reserved_student=studentID;
    }
    void Set_reserved_faculty(string facultyID){
        this->reserved_faculty=facultyID;
    }
};

class Account
{
private:
    vector<pair<Book*,time_t>> borrowed_books;
    vector<history> borrowing_history;
    Book* reserved_book;
public:
    vector<pair<Book*,time_t>> Get_borrowed_books(){
        return borrowed_books;
    }
// to get borrowed books along with the due days(how many days back they have borrowed for)
    vector<pair<string,long long>> Get_books_with_duedays(){
        vector<pair<string,long long>> books_days;
        for(int i=0;i<borrowed_books.size();i++){
            long long fine=0;
            time_t now = time(nullptr);
            long long difference = difftime(now, borrowed_books[i].second) / (60 * 60 * 24);
            books_days.push_back({borrowed_books[i].first->Get_bookID(),difference});
        }
        return books_days;
    }
    //returns null pointer if no reserved book
    Book* Get_Reserved_book(){
        return reserved_book;
    }
    vector<history> Get_borrowing_history(){
        return borrowing_history;
    }
    //to add book to borrowed_books
    void Add_book(string bookID,Library& library){
        Book* book=library.Get_book(bookID);
        book->Set_status(borrowed);
        time_t now = time(nullptr);
        borrowed_books.push_back({book,now});
    }
    //to delete book from borrowed_books
    void Delete_book(string bookID,Library& library){
        for(int i=0;i<borrowed_books.size();i++){
            if(borrowed_books[i].first->Get_bookID()==bookID){
                Book* book=library.Get_book(bookID);
                book->Set_status(available);
                history h;
                h.bookID=book->Get_bookID();
                h.borrowed_time=borrowed_books[i].second;
                h.returned_time=time(nullptr);
                borrowing_history.push_back(h);
                borrowed_books.erase(borrowed_books.begin()+i);
            }
        }
    }
    //to add borrowed_books or borrowing history to account while loading data from files
    void Add_history(string bookID,time_t borrowed_time,time_t returned_time){
        history h;
        h.bookID=bookID;
        h.borrowed_time=borrowed_time;
        h.returned_time=returned_time;
        borrowing_history.push_back(h);
    }
    void Add_borrowed_books(string bookID,time_t borrowed_time,Library& library){
        Book* book=library.Get_book(bookID);
        book->Set_status(borrowed);
        borrowed_books.push_back({book,borrowed_time});
    }
    void Set_Reserved_book(Book* book){
        reserved_book=book;
    }
};

class User
{
protected:
    string userID;
    string username;
    string password;
    int fine_per_day;
    int max_borrow_period;
    int max_books;
public:
    string Get_userID(){
        return this->userID;
    }
    string Get_username(){
        return this->username;
    }
    string Get_password(){
        return this->password;
    }
    int Get_fine_per_day(){
        return this->fine_per_day;
    }
    int Get_max_borrow_period(){
        return this->max_borrow_period;
    }
    int Get_max_books(){
        return this->max_books;
    }
    void Set_userID(string userID){
        this->userID=userID;
    }
    void Set_username(string username){
        this->username=username;
    }
    void Set_password(string password){
        this->password=password;
    }
    void Set_fine_per_day(int fine_per_day){
        this->fine_per_day=fine_per_day;
    }
    void Set_max_borrow_period(int max_borrow_period){
        this->max_borrow_period=max_borrow_period;
    }
    void Set_max_books(int max_books){
        this->max_books=max_books;
    }
};

class Student : public User{
private:
    Account account;
    long long fine;//calculated when the book is returned
    static long long student_num;
public:
    //constructor overloading(one is for creating new student and other is for loading from data)
    Student(string username,string password){
        this->username=username;
        this->password=password;
        this->userID='S'+to_string(student_num);
        cout<<"student's userID is :"<<this->userID<<endl;
        this->fine=0;
        this->max_books=3;
        this->fine_per_day=10;
        this->max_borrow_period=15;
        student_num++;
        this->account.Set_Reserved_book(NULL);
    }
    Student(string username,string password,string userID,long long fine){
        this->username=username;
        this->password=password;
        this->userID=userID;
        this->fine=fine;
        this->max_books=3;
        this->fine_per_day=10;
        this->max_borrow_period=15;
        this->account.Set_Reserved_book(NULL);
    }
    //view account details(borrowed_books,fine,reserved_book,borrowing_history)
    void View_account(Library & library){
        vector<pair<Book*,time_t>> borrowed_books=this->Get_account()->Get_borrowed_books();
        cout<<"Your fine is:"<<this->fine<<endl;
        cout<<"Your reserved book is:"<<endl;
        if(this->account.Get_Reserved_book()!=NULL){
            cout<<"BookID:"<<this->account.Get_Reserved_book()->Get_bookID()<<endl;
            cout<<"Title:"<<this->account.Get_Reserved_book()->Get_title()<<endl;
            cout<<"Author:"<<this->account.Get_Reserved_book()->Get_author()<<endl;
            cout<<"Publisher:"<<this->account.Get_Reserved_book()->Get_publisher()<<endl;
            cout<<"Year:"<<this->account.Get_Reserved_book()->Get_year()<<endl;
            cout<<"ISBN:"<<this->account.Get_Reserved_book()->Get_ISBN()<<endl;
        }
        else cout<<"No reserved book"<<endl;
        cout<<"Your borrowed books are:"<<endl;
        if(borrowed_books.size()==0) cout<<"No borrowed books"<<endl;
        for(int i=0;i<borrowed_books.size();i++){
            cout<<"Book"<<i+1<<":"<<endl;
            cout<<"BookID:"<<borrowed_books[i].first->Get_bookID()<<endl;
            cout<<"Title:"<<borrowed_books[i].first->Get_title()<<endl;
            cout<<"Author:"<<borrowed_books[i].first->Get_author()<<endl;
            cout<<"Publisher:"<<borrowed_books[i].first->Get_publisher()<<endl;
            cout<<"Year:"<<borrowed_books[i].first->Get_year()<<endl;
            cout<<"ISBN:"<<borrowed_books[i].first->Get_ISBN()<<endl;
            char Date[11];
            strftime(Date, sizeof(Date), "%d-%m-%Y", localtime(&borrowed_books[i].second));
            cout<<"Borrowed date:"<<Date<<endl;
            time_t now = time(nullptr);
            long long difference = difftime(now, borrowed_books[i].second) / (60 * 60 * 24);
            cout<<"Overdue of:"<<((difference>max_borrow_period)?(difference-max_borrow_period):0)<<" days"<<endl;
        }
        cout<<"Your borrowing history is:"<<endl;
        vector<history> borrowing_history=this->account.Get_borrowing_history();
        if(borrowing_history.size()==0) cout<<"No borrowing history"<<endl;
        for(int i=0;i<borrowing_history.size();i++){
            cout<<"Book"<<i+1<<":"<<endl;
            cout<<"BookID:"<<borrowing_history[i].bookID<<endl;
            char borrowDate[11], returnDate[11];
            strftime(borrowDate, sizeof(borrowDate), "%d-%m-%Y", localtime(&borrowing_history[i].borrowed_time));
            strftime(returnDate, sizeof(returnDate), "%d-%m-%Y", localtime(&borrowing_history[i].returned_time));
            cout<<"Borrowed date:"<<borrowDate<<endl;
            cout<<"Returned date:"<<returnDate<<endl;
        }
        cout<<"Enter any key to return to Home"<<endl;
        cin.ignore();
        cin.get();
        library.Student_home(this);
    }
    //BorrowBook - if the student has reached the maximum borrow limit he can't borrow more.
    //if the student has fine he can't borrow more.
    //if the student has overdue books he can't borrow more.
    void BorrowBook(Library & library){
        if(this->account.Get_borrowed_books().size()==this->Get_max_books()) {
            cout<<"You have reached your maximum borrow limit"<<endl;
            cout<<"Enter 1 to borrow another book"<<endl;
            cout<<"Enter 2 to return to home"<<endl;
            int t;
            cin>>t;
            if(t==1) BorrowBook(library);
            if(t==2) library.Student_home(this);
        }
        vector<pair<string,long long>> books_overdues=this->account.Get_books_with_duedays();
        int overdues=0;
        for(int i=0;i<books_overdues.size();i++){
            overdues+=((books_overdues[i].second)>max_borrow_period);
        }
        if(fine>0){
            cout<<"You have to clear fine to borrow books"<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            cin.get();
            library.Student_home(this);
        }
        else if(overdues>0){
            cout<<"You have overdues so you can not borrow a new book"<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            cin.get();
            library.Student_home(this);
        }
        else{
            cout<<"Enter the bookID of the book you want:";
            string bookID;
            cin>>bookID;
            Book* book=library.Get_book(bookID);//returns "null pointer" if book not found
            if(book==NULL){
                cout<<"Book not found"<<endl;
                cout<<"Enter 1 to try again"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) BorrowBook(library);
                if(t==2) library.Student_home(this);
            }
            else{
            Status status=book->Get_status();
            if(status==borrowed) {
                cout<<"It is borrowed by some other person"<<endl;
                cout<<"Enter 1 to borrow another book"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) BorrowBook(library);
                if(t==2) library.Student_home(this);
            }
            if(status==reserved) {
                if((book->Get_reserved_student())==this->Get_userID()){
                    book->Set_status(borrowed);
                    book->Set_reserved_student("NULL");
                    this->account.Add_book(bookID,library);
                    this->account.Set_Reserved_book(NULL);
                    cout<<"You have borrowed your reserved book successfully"<<endl;
                    cout<<"Enter any key to return to Home"<<endl;
                    cin.ignore();
                    cin.get();
                    library.Student_home(this);
                }
                else{
                    cout<<"You can not borrow this book since it is reserved by some other person"<<endl;
                    cout<<"Enter 1 to borrow another book"<<endl;
                    cout<<"Enter 2 to return to home"<<endl;
                    int t;
                    cin>>t;
                    if(t==1) this->BorrowBook(library);
                    if(t==2) library.Student_home(this);
                }
            }
            if(status==available){
                this->account.Add_book(bookID,library);
                cout<<"Book borrowed successfully";
                cout<<"Enter any key to return to Home"<<endl;
                cin.ignore();
                cin.get();
                library.Student_home(this);
            }
            }
        }
    }
    //ReturnBook - if book is in borrowed_books it will return the book and add it to borrowing history.
    //if the book is reserved by someone else it will set the status to reserved.
    //if the book is not in borrowed_books it will show "Book is not borrowed by you".
    void ReturnBook(Library & library){
        cout<<"Enter the bookID of the book you want to return"<<endl;
        string bookID;
        cin>>bookID;
        vector<pair<Book*,time_t>> borrowed_books=this->account.Get_borrowed_books();
        int f=1;
        for(int i=0;i<borrowed_books.size();i++){
            if(borrowed_books[i].first->Get_bookID()==bookID){
                f=0;//flag
                this->account.Delete_book(bookID,library);
                cout<<"Book returned successfully"<<endl;
                if(borrowed_books[i].first->Get_reserved_faculty()!="NULL" or borrowed_books[i].first->Get_reserved_student()!="NULL"){ 
                    borrowed_books[i].first->Set_status(reserved);
                }
                time_t now = time(nullptr);
                long long difference = difftime(now, borrowed_books[i].second) / (60 * 60 * 24);
                fine+=((difference>max_borrow_period)?(difference-max_borrow_period)*(this->fine_per_day):0);
                    cout<<"You have total fine of "<<fine<<" rupees can clear it by consulting librarian"<<endl;
                    cout<<"Enter any key to return to home"<<endl;
                    cin.ignore();
                    cin.get();
                    library.Student_home(this);
            }
        }
        if(f){
            cout<<"Book not borrowed by you"<<endl;
            cout<<"Enter 1 to try again"<<endl;
            cout<<"Enter 2 to return to home"<<endl;
            int t;
            cin>>t;
            if(t==1) this->ReturnBook(library);
            if(t==2) library.Student_home(this);
        }
    }
    //ReserveBook - if the student has already reserved a book he can't reserve another one.
    //if the book is reserved by someone else he can't reserve it.
    void ReserveBook(Library & library){
        cout<<"Enter the bookID of the book you want to reserve"<<endl;
        string bookID;
        cin>>bookID;
        Book* book=library.Get_book(bookID);
        if(this->account.Get_Reserved_book()!=NULL){
            cout<<"You have already reserved a book and cannot reserve another one "<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            cin.get();
            library.Student_home(this);
        }
        else if(book==NULL){
            cout<<"Book not found"<<endl;
            cout<<"Enter 1 to try another book"<<endl;
            cout<<"Enter 2 to return to home"<<endl;
            int t;
            cin>>t;
            if(t==1) this->ReserveBook(library);
            if(t==2) library.Student_home(this);
        }
        else{
            if(book->Get_status()==reserved){
                cout<<"Book already reserved by someone else"<<endl;
                cout<<"Enter 1 to try another book"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) this->ReserveBook(library);
                if(t==2) library.Student_home(this);
            }
            else if(book->Get_status()==borrowed){
                if(book->Get_reserved_student()=="NULL" and book->Get_reserved_faculty()=="NULL"){
                    book->Set_reserved_student(this->Get_userID());
                    this->account.Set_Reserved_book(book);
                    cout<<"Book reserved successfully"<<endl;
                    cout<<"Enter any key to return to home"<<endl;
                    cin.ignore();
                    cin.get();
                    library.Student_home(this);
                }
                else{
                    cout<<"Book already reserved by someone else"<<endl;
                    cout<<"Enter 1 to try another book"<<endl;
                    cout<<"Enter 2 to return to home"<<endl;
                    int t;
                    cin>>t;
                    if(t==1) this->ReserveBook(library);
                    if(t==2) library.Student_home(this);
                }
            }
            else{
                cout<<"Book is available you can not reserve can only borrow"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                library.Student_home(this);
            }
        }
    }
    Account* Get_account(){
        return &(this->account);
    }
    long long Get_fine(){
        return this->fine;
    }
    void Set_fine(long long fine){
        this->fine=fine;
    }
    void Set_counter(long long student_num){
        this->student_num=student_num;
    }
};

class Faculty : public User{
private:
    Account account;
    static long long faculty_num;// used to assign userID
public:
    Account* Get_account(){
        return &(this->account);
    }
    Faculty(string username,string password){
        this->username=username;
        this->password=password;
        this->userID='F'+to_string(faculty_num);
        cout<<"your userID is :"<<this->userID<<endl;
        this->fine_per_day=0;
        this->max_borrow_period=30;
        this->account.Set_Reserved_book(NULL);
        faculty_num++;
    }
    Faculty(string username,string password, string userID){
        this->username=username;
        this->password=password;
        this->userID=userID;
        this->fine_per_day=0;
        this->max_borrow_period=30;
        this->account.Set_Reserved_book(NULL);
    }
    //view account details(borrowed_books,reserved_book,borrowing_history)
    void View_account(Library & library){
        vector<pair<Book*,time_t>> borrowed_books=this->account.Get_borrowed_books();
        cout<<"Your reserved book is:"<<endl;
        if(this->account.Get_Reserved_book()!=NULL){
            cout<<"BookID:"<<this->account.Get_Reserved_book()->Get_bookID()<<endl;
            cout<<"Title:"<<this->account.Get_Reserved_book()->Get_title()<<endl;
            cout<<"Author:"<<this->account.Get_Reserved_book()->Get_author()<<endl;
            cout<<"Publisher:"<<this->account.Get_Reserved_book()->Get_publisher()<<endl;
            cout<<"Year:"<<this->account.Get_Reserved_book()->Get_year()<<endl;
            cout<<"ISBN:"<<this->account.Get_Reserved_book()->Get_ISBN()<<endl;
        }
        else cout<<"No reserved book"<<endl;
        cout<<"Your borrowed books are:"<<endl;
        if(borrowed_books.size()==0) cout<<"No borrowed books"<<endl;
        for(int i=0;i<borrowed_books.size();i++){
            cout<<"Book"<<i+1<<":"<<endl;
            cout<<"BookID:"<<borrowed_books[i].first->Get_bookID()<<endl;
            cout<<"Title:"<<borrowed_books[i].first->Get_title()<<endl;
            cout<<"Author:"<<borrowed_books[i].first->Get_author()<<endl;
            cout<<"Publisher:"<<borrowed_books[i].first->Get_publisher()<<endl;
            cout<<"Year:"<<borrowed_books[i].first->Get_year()<<endl;
            cout<<"ISBN:"<<borrowed_books[i].first->Get_ISBN()<<endl;
            char Date[11];
            strftime(Date, sizeof(Date), "%d-%m-%Y",localtime(&borrowed_books[i].second));
            cout << "Borrowed date: " << Date << endl;
            time_t now = time(nullptr);
            long long difference = difftime(now, borrowed_books[i].second) / (60 * 60 * 24);
            cout<<"Fine:"<<((difference>max_borrow_period)?(difference-max_borrow_period)*(this->fine_per_day):0)<<endl;
        }
        cout<<"Your borrowing history is:"<<endl;
        vector<history> borrowing_history=this->account.Get_borrowing_history();
        if(borrowing_history.size()==0) cout<<"No borrowing history"<<endl;
        for(int i=0;i<borrowing_history.size();i++){
            cout<<"Book"<<i+1<<":"<<endl;
            cout<<"BookID:"<<borrowing_history[i].bookID<<endl;
            char borrowDate[11], returnDate[11];
            strftime(borrowDate, sizeof(borrowDate), "%d-%m-%Y", localtime(&borrowing_history[i].borrowed_time));
            strftime(returnDate, sizeof(returnDate), "%d-%m-%Y", localtime(&borrowing_history[i].returned_time));
            cout<<"Borrowed date:"<<borrowDate<<endl;
            cout<<"Returned date:"<<returnDate<<endl;
        }
        cout<<"Enter any key to return to Home"<<endl;
        cin.ignore();
        cin.get();
        library.Faculty_home(this);
    }
    //BorrowBook - if the faculty has reached the maximum borrow limit he can't borrow more.
    //if the faculty has overdues he can't borrow more.
    void BorrowBook(Library & library){
        if(this->account.Get_borrowed_books().size()==this->Get_max_books()) {
            cout<<"You have reached your maximum borrow limit"<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            cin.get();
            library.Faculty_home(this);
        }
        long long overdues=0;
        vector<pair<string,long long>> books_fine=this->account.Get_books_with_duedays();
        for(int i=0;i<books_fine.size();i++){
            overdues+=((books_fine[i].second)>max_borrow_period);
        }
        if(overdues>0){
            cout<<"You have overdues so you can not borrow a new book"<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            cin.get();
            library.Faculty_home(this);
        }
        else{
            cout<<"Enter the bookID of the book you want:";
            string bookID;
            cin>>bookID;
            Book* book=library.Get_book(bookID);//returns "null pointer" if book not found
            if(book==NULL){
                cout<<"Book not found"<<endl;
                cout<<"Enter 1 to try again"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) BorrowBook(library);
                if(t==2) library.Faculty_home(this);
            }
            else{
            Status status=book->Get_status();
            if(status==borrowed) {
                cout<<"It is borrowed by some other person"<<endl;
                cout<<"Enter 1 to borrow another book"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) this->BorrowBook(library);
                else library.Faculty_home(this);
            }
            if(status==reserved) {
                if(book->Get_reserved_faculty()==this->Get_userID()){
                    book->Set_status(borrowed);
                    book->Set_reserved_faculty("NULL");
                    this->account.Add_book(bookID,library);
                    this->account.Set_Reserved_book(NULL);
                    cout<<"Book borrowed successfully"<<endl;
                    cout<<"Enter any key to return to Home"<<endl;
                    cin.ignore();
                    cin.get();
                    library.Faculty_home(this);
                }
                else{
                    cout<<"It is reserved by some other person"<<endl;
                    cout<<"Enter 1 to borrow another book"<<endl;
                    cout<<"Enter 2 to return to home"<<endl;
                    int t;
                    cin>>t;
                    if(t==1) this->BorrowBook(library);
                    else library.Faculty_home(this);
                }
            }
            if(status==available){
                this->account.Add_book(bookID,library);
                cout<<"Transaction successful";
                cout<<"Enter any key to return to Home"<<endl;
                cin.ignore();
                cin.get();
                library.Faculty_home(this);
            }
        }
        }
    }
    //ReturnBook - if book is in borrowed_books it will return the book and add it to borrowing history.
    //if the book is reserved by someone else it will set the status to reserved.
    //if the book is not in borrowed_books it will show "Book is not borrowed by you".
    void ReturnBook(Library & library){
        cout<<"Enter the bookID of the book you want to return"<<endl;
        string bookID;
        cin>>bookID;
        vector<pair<Book*,time_t>> borrowed_books=this->account.Get_borrowed_books();
        int f=1;
        for(int i=0;i<borrowed_books.size();i++){
            if(borrowed_books[i].first->Get_bookID()==bookID){
                f=0;
                this->account.Delete_book(bookID,library);
                if(borrowed_books[i].first->Get_reserved_faculty()!="NULL" or borrowed_books[i].first->Get_reserved_student()!="NULL"){ 
                    borrowed_books[i].first->Set_status(reserved);
                }
                cout<<"Book returned successfully"<<endl;
                cout<<"Enter any key to return to Home"<<endl;
                cin.ignore();
                cin.get();
                library.Faculty_home(this);
            }
        }
        if(f){
            cout<<"Book not borrowed by you"<<endl;
            cout<<"Enter 1 to try again"<<endl;
            cout<<"Enter 2 to return to home"<<endl;
            int t;
            cin>>t;
            if(t==1) ReturnBook(library);
            if(t==2) library.Faculty_home(this);
        }
    }
    //ReserveBook - if the faculty has already reserved a book he can't reserve another one.
    //if the book is reserved by someone else he can't reserve it.
    void ReserveBook(Library & library){
        cout<<"Enter the bookID of the book you want to reserve"<<endl;
        string bookID;
        cin>>bookID;
        Book* book=library.Get_book(bookID);
        if(this->account.Get_Reserved_book()!=NULL){
            cout<<"You have already reserved a book and cannot reserve another one "<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            cin.get();
            library.Faculty_home(this);
        }
        else if(book==NULL){
            cout<<"Book not found"<<endl;
            cout<<"Enter 1 to try again"<<endl;
            cout<<"Enter 2 to return to home"<<endl;
            int t;
            cin>>t;
            if(t==1) this->ReserveBook(library);
            if(t==2) library.Faculty_home(this);
        }
        else{
            if(book->Get_status()==reserved){
                cout<<"Book already reserved by someone else"<<endl;
                cout<<"Enter 1 to try again"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) this->ReserveBook(library);
                if(t==2) library.Faculty_home(this);
            }
            else if(book->Get_status()==borrowed){
                if(book->Get_reserved_student()=="NULL" and book->Get_reserved_faculty()=="NULL"){
                    book->Set_reserved_faculty(this->Get_userID());
                    this->account.Set_Reserved_book(book);
                    cout<<"Book reserved successfully"<<endl;
                    cout<<"Enter any key to return to home"<<endl;
                    cin.ignore();
                    cin.get();
                    library.Faculty_home(this);
                }
                else{
                    cout<<"Book already reserved by someone else"<<endl;
                    cout<<"Enter 1 to try another book"<<endl;
                    cout<<"Enter 2 to return to home"<<endl;
                    int t;
                    cin>>t;
                    if(t==1) this->ReserveBook(library);
                    if(t==2) library.Faculty_home(this);
                }
            }
            else{
                cout<<"Book is available you can not reserve can only borrow"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                library.Faculty_home(this);
            }
        }
    }
    void Set_counter(long long faculty_num){
        this->faculty_num=faculty_num;
    }
};

class Librarian : public User{
private:
    static long long librarian_num;// used to assign userID
public:
    Librarian(string username,string password){
        this->username=username;
        this->password=password;
        this->userID='L'+to_string(librarian_num);
        cout<<"your userID is :"<<this->userID<<endl;
        librarian_num++;
    }
    Librarian(string username,string password,string userID){
        this->username=username;
        this->password=password;
        this->userID=userID;
    }
    void Set_counter(long long librarian_num){
        this->librarian_num=librarian_num;
    }
};
long long Book::book_num=1;
long long Student::student_num=1;
long long Faculty::faculty_num=1;
long long Librarian::librarian_num=1;
//loading data from text files
void Library::LoadBooksFromFile() {
    ifstream file("books.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string bookID, title, author, publisher, yearStr, isbnStr, statusStr ,reserverStr;
            int year;
            long long ISBN;
            getline(ss, bookID, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, publisher, ',');
            getline(ss, yearStr, ',');
            getline(ss, isbnStr, ',');
            getline(ss, statusStr, ',');
            getline(ss, reserverStr, ',');
            try {
                year = stoi(yearStr);
                ISBN = stoll(isbnStr);
                Status status = (statusStr == "available") ? available : ((statusStr == "borrowed") ? borrowed : reserved);
                Book* book = new Book(title, author, publisher, year, ISBN,status);
                book->Set_bookID(bookID);
                books.push_back(book);
                if(reserverStr!="NULL"){
                    if(reserverStr[0]=='S'){
                        book->Set_reserved_student(reserverStr);
                    }
                    else{
                        book->Set_reserved_faculty(reserverStr);
                    }
                }
            } catch (const invalid_argument& e) {
                cerr << "Invalid data format in file: " << e.what() << endl;
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::LoadStudentsFromFile() {
    ifstream file("students.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string userID, username, password, fineStr, reservedBookID;
            getline(ss, userID, ',');
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, fineStr, ',');
            getline(ss, reservedBookID, ',');
            Student* student = new Student(username, password, userID, stoll(fineStr));
            if(reservedBookID!="NULL"){
                Book* book=Get_book(reservedBookID);
                student->Get_account()->Set_Reserved_book(book);
            }
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) tokens.push_back(token);
            int historyIndex;
            for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens[i] == "HISTORY") {
                    historyIndex = static_cast<int>(i);
                    break;
                }
            }
            for (size_t i = 0; i + 1 < static_cast<size_t>(historyIndex); i += 2) {
                string bookID = tokens[i];
                tm Date = {0};
                sscanf(tokens[i+1].c_str(), "%d-%d-%d",&Date.tm_mday, &Date.tm_mon, &Date.tm_year);
                Date.tm_year -= 1900; 
                Date.tm_mon  -= 1;
                Date.tm_hour = 0;
                Date.tm_min  = 0;
                Date.tm_sec  = 0;
                time_t dateBorrowed = mktime(&Date);
                student->Get_account()->Add_borrowed_books(bookID, dateBorrowed,*this);
            }
            for (size_t i = historyIndex + 1; i + 2 < tokens.size(); i += 3) {
                string bookID = tokens[i];
                tm Date = {0};
                sscanf(tokens[i+1].c_str(), "%d-%d-%d", &Date.tm_mday, &Date.tm_mon, &Date.tm_year);
                Date.tm_year -= 1900; 
                Date.tm_mon  -= 1;
                Date.tm_hour = 0;
                Date.tm_min  = 0;
                Date.tm_sec  = 0;
                time_t histBorrowDate = mktime(&Date);
                Date = {0};
                sscanf(tokens[i+2].c_str(), "%d-%d-%d", &Date.tm_mday, &Date.tm_mon, &Date.tm_year);
                Date.tm_year -= 1900; 
                Date.tm_mon  -= 1;
                Date.tm_hour = 0;
                Date.tm_min  = 0;
                Date.tm_sec  = 0;
                time_t histReturnDate = mktime(&Date);
                student->Get_account()->Add_history(bookID, histBorrowDate, histReturnDate);
            }
            students.push_back(student);
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::LoadFacultiesFromFile() {
    ifstream file("faculties.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string userID, username, password, reservedBookID;
            getline(ss, userID, ',');
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, reservedBookID, ',');
            Faculty* faculty = new Faculty(username, password, userID);
            if(reservedBookID!="NULL"){
                Book* book=Get_book(reservedBookID);
                faculty->Get_account()->Set_Reserved_book(book);
            }
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) tokens.push_back(token);
            int histIndex;
            for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens[i] == "HISTORY") {
                    histIndex = static_cast<int>(i);
                    break;
                }
            }
            for (size_t i = 0; i + 1 < static_cast<size_t>(histIndex); i += 2) {
                string bookID = tokens[i];
                tm Date = {0};
                sscanf(tokens[i+1].c_str(), "%d-%d-%d", &Date.tm_mday, &Date.tm_mon, &Date.tm_year);
                Date.tm_year -= 1900; 
                Date.tm_mon  -= 1;
                Date.tm_hour = 0;
                Date.tm_min  = 0;
                Date.tm_sec  = 0;
                time_t dateBorrowed = mktime(&Date);
                faculty->Get_account()->Add_borrowed_books(bookID, dateBorrowed, *this);
            }
            for (size_t i = histIndex + 1; i + 2 < tokens.size(); i += 3) {
                string bookID = tokens[i];
                tm Date = {0};
                sscanf(tokens[i+1].c_str(), "%d-%d-%d", &Date.tm_mday, &Date.tm_mon, &Date.tm_year);
                Date.tm_year -= 1900; 
                Date.tm_mon  -= 1;
                Date.tm_hour = 0;
                Date.tm_min  = 0;
                Date.tm_sec  = 0;
                time_t histBorrowDate = mktime(&Date);
                Date = {0};
                sscanf(tokens[i+2].c_str(), "%d-%d-%d", &Date.tm_mday, &Date.tm_mon, &Date.tm_year);
                Date.tm_year -= 1900; 
                Date.tm_mon  -= 1;
                Date.tm_hour = 0;
                Date.tm_min  = 0;
                Date.tm_sec  = 0;
                time_t histReturnDate = mktime(&Date);
                faculty->Get_account()->Add_history(bookID, histBorrowDate, histReturnDate);
            }
            faculties.push_back(faculty);
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::LoadLibrariansFromFile() {
    ifstream file("librarians.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string userID, username, password;
            getline(ss, userID, ',');
            getline(ss, username, ',');
            getline(ss, password, ',');
            Librarian* librarian = new Librarian(username, password, userID);
            librarians.push_back(librarian);
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::LoadCountersFromFile() {
    ifstream file("counters.txt");
    string line;
    if (file.is_open()) {
        getline(file, line);
        stringstream ss(line);
        string book_num_str, student_num_str, faculty_num_str, librarian_num_str;
        getline(ss, book_num_str, ',');
        getline(ss, student_num_str, ',');
        getline(ss, faculty_num_str, ',');
        getline(ss, librarian_num_str, ',');
        try {
            if(books.size()) books.back()->Set_counter(stoll(book_num_str)+1);
            if(students.size()) students.back()->Set_counter(stoll(student_num_str)+1);
            if(faculties.size()) faculties.back()->Set_counter(stoll(faculty_num_str)+1);
            if(librarians.size()) librarians.back()->Set_counter(stoll(librarian_num_str)+1);
        } catch (const invalid_argument& e) {
            cerr << "Invalid data format in file: " << e.what() << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
//saving data to text files
void Library::SaveBooksToFile() {
    ofstream file("books.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < books.size(); i++) {
            file << books[i]->Get_bookID() << "," << books[i]->Get_title() << "," << books[i]->Get_author() << "," << books[i]->Get_publisher() << "," << books[i]->Get_year() << "," << books[i]->Get_ISBN() <<","<<((books[i]->Get_status()==available)?"available":((books[i]->Get_status()==borrowed)?"borrowed":"reserved"))
            <<","<<((books[i]->Get_reserved_faculty()!="NULL")?books[i]->Get_reserved_faculty():(books[i]->Get_reserved_student()!="NULL"?books[i]->Get_reserved_student():"NULL"))<< endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::SaveStudentsToFile() {
    ofstream file("students.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < students.size(); i++) {
            file << students[i]->Get_userID() << "," << students[i]->Get_username() << "," << students[i]->Get_password() << ","<< students[i]->Get_fine() << ","<<((students[i]->Get_account()->Get_Reserved_book()!=NULL)?students[i]->Get_account()->Get_Reserved_book()->Get_bookID():"NULL")<<",";
            vector<pair<Book*,time_t>> borrowed_books = students[i]->Get_account()->Get_borrowed_books();
            for (size_t j = 0; j < borrowed_books.size(); j++) {
                char Date[11];
                strftime(Date, sizeof(Date), "%d-%m-%Y", localtime(&borrowed_books[j].second) );
                file << borrowed_books[j].first->Get_bookID() << "," << Date<< ",";
            }
            file << "HISTORY";
            vector<history> borrowing_history = students[i]->Get_account()->Get_borrowing_history();
            for (size_t j = 0; j < borrowing_history.size(); j++) {
                char borrowDate[11], returnDate[11];
                strftime(borrowDate, sizeof(borrowDate), "%d-%m-%Y", localtime(&borrowing_history[j].borrowed_time));
                strftime(returnDate, sizeof(returnDate), "%d-%m-%Y", localtime(&borrowing_history[j].returned_time));
                file << "," << borrowing_history[j].bookID << "," << borrowDate<< "," <<returnDate;
            }
            file << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::SaveFacultiesToFile() {
    ofstream file("faculties.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < faculties.size(); i++) {
            file << faculties[i]->Get_userID() << "," << faculties[i]->Get_username() << "," << faculties[i]->Get_password() << ","<<((faculties[i]->Get_account()->Get_Reserved_book()!=NULL)?faculties[i]->Get_account()->Get_Reserved_book()->Get_bookID():"NULL") <<",";
            vector<pair<Book*,time_t>> borrowed_books = faculties[i]->Get_account()->Get_borrowed_books();
            for (size_t j = 0; j < borrowed_books.size(); j++) {
                char Date[11];
                strftime(Date, sizeof(Date), "%d-%m-%Y", localtime(&borrowed_books[j].second) );
                file << borrowed_books[j].first->Get_bookID() << "," << Date<< ",";
            }
            file << "HISTORY";
            vector<history> borrowing_history = faculties[i]->Get_account()->Get_borrowing_history();
            for (size_t j = 0; j < borrowing_history.size(); j++) {
                char borrowDate[11], returnDate[11];
                strftime(borrowDate, sizeof(borrowDate), "%d-%m-%Y", localtime(&borrowing_history[j].borrowed_time));
                strftime(returnDate, sizeof(returnDate), "%d-%m-%Y", localtime(&borrowing_history[j].returned_time));
                file << ","<< borrowing_history[j].bookID << "," << borrowDate << "," << returnDate ;
            }
            file << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::SaveLibrariansToFile() {
    ofstream file("librarians.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < librarians.size(); i++) {
            file << librarians[i]->Get_userID() << "," << librarians[i]->Get_username() << "," << librarians[i]->Get_password() << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
void Library::SaveCountersToFile() {
    ofstream file("counters.txt");
    if (file.is_open()) {
        file << (books.size()?books.back()->Get_bookID().substr(1):"0") << "," << (students.size()?students.back()->Get_userID().substr(1):"0") << "," << (faculties.size()?faculties.back()->Get_userID().substr(1):"0") << "," <<( librarians.size()?librarians.back()->Get_userID().substr(1):"0") << endl;
        file.close();
    } else {
        cerr << "Unable to open file" << endl;
    }
}
//adding and deleting functions(can be used only by librarian)
void Library::Add_book(Librarian* librarian){
    cout<<"Enter title:";
    string title;
    cin>>title;
    cout<<"Enter author:";
    string author;
    cin>>author;
    cout<<"Enter publisher:";
    string publisher;
    cin>>publisher;
    cout<<"Enter year:";
    int year;
    cin>>year;
    cout<<"Enter ISBN:";
    long long ISBN;
    cin>>ISBN;
    Book* book=new Book(title,author,publisher,year,ISBN,available);
    books.push_back(book);
    cout<<"Book added successfully with bookID:"<<books.back()->bookID<<endl;
    cout<<"Enter any key to return to home"<<endl;
    cin.ignore();
    cin.get();
    this->Librarian_home(librarian);
}
void Library::Delete_book(Librarian* librarian){
    cout<<"Enter bookID of the book you want to delete"<<endl;
    string bookID;
    cin>>bookID;
    int f=1;
    for(int i=0;i<books.size();i++){
        if(books[i]->Get_bookID()==bookID){
            f=0;
            if(books[i]->Get_status()==borrowed){
                cout<<"Book is borrowed by someone so can not be deleted"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
            else if(books[i]->Get_status()==reserved){
                cout<<"Book is reserved by someone so can not be deleted"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
            else {
                books.erase(books.begin()+i);
                cout<<"Book deleted successfully"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
        }
    }
    if(f==1){
        cout<<"Book not found"<<endl;
        cout<<"Enter any key to return to home"<<endl;
        cin.ignore();
        cin.get();
        this->Librarian_home(librarian);
    }
}
void Library::Add_student(Librarian* librarian){
    cout<<"Enter username:";
    string username;
    cin>>username;
    cout<<"Enter password:";
    string password;
    cin>>password;
    Student* student=new Student(username,password);
    students.push_back(student);
    cout<<"Student added successfully"<<endl;
    this->Librarian_home(librarian);
}
void Library::Delete_student(Librarian* librarian){
    cout<<"Enter userID of the student you want to delete"<<endl;
    string userID;
    cin>>userID;
    int f=1;
    for(int i=0;i<students.size();i++){
        if(students[i]->Get_userID()==userID){
            f=0;
            if(students[i]->Get_account()->Get_borrowed_books().size()){
                cout<<"Student has borrowed books so can not be deleted"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
            else if(students[i]->Get_fine()){
                cout<<"Student has fine so can not be deleted"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
            else if(students[i]->Get_account()->Get_Reserved_book()!=NULL){
                cout<<"Student has reserved book so can not be deleted"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
            else{
                students.erase(students.begin()+i);
                cout<<"Student deleted successfully"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
        }
    }
    if(f){
        cout<<"Student not found"<<endl;
        this->Librarian_home(librarian);
    }
}
void Library::Add_faculty(Librarian* librarian){
    cout<<"Enter username:";
    string username;
    cin>>username;
    cout<<"Enter password:";
    string password;
    cin>>password;
    Faculty* faculty=new Faculty(username,password);
    faculties.push_back(faculty);
    cout<<"Faculty added successfully"<<endl;
    this->Librarian_home(librarian);
}
void Library::Delete_faculty(Librarian* librarian){
    cout<<"Enter userID of the faculty you want to delete"<<endl;
    string userID;
    cin>>userID;
    int f=1;
    for(int i=0;i<faculties.size();i++){
        if(faculties[i]->Get_userID()==userID){
            f=0;
            if(faculties[i]->Get_account()->Get_borrowed_books().size()){
                cout<<"Faculty has borrowed books so can not be deleted"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
            else{
                faculties.erase(faculties.begin()+i);
                cout<<"Faculty deleted successfully"<<endl;
                cout<<"Enter any key to return to home"<<endl;
                cin.ignore();
                cin.get();
                this->Librarian_home(librarian);
            }
        }
    }
    if(f) {
        cout<<"Faculty not found"<<endl;
        cout<<"Enter any key to return to home"<<endl;
        cin.ignore();
        cin.get();
        this->Librarian_home(librarian);
    }
}
void Library::Add_librarian(Librarian* librarian){
    cout<<"Enter username:";
    string username;
    cin>>username;
    cout<<"Enter password:";
    string password;
    cin>>password;
    Librarian* librarian_new=new Librarian(username,password);
    librarians.push_back(librarian_new);
    cout<<"Librarian added successfully"<<endl;
    this->Librarian_home(librarian);
}
void Library::Delete_librarian(Librarian* librarian){
    cout<<"Enter userID of the librarian you want to delete"<<endl;
    string userID;
    cin>>userID;
    int f=1;
    for(int i=0;i<librarians.size();i++){
        if(librarians[i]->Get_userID()==userID){
            f=0;
            librarians.erase(librarians.begin()+i);
            cout<<"Librarian deleted successfully"<<endl;
            cout<<"Enter any key to return to home"<<endl;
            cin.ignore();
            this->Librarian_home(librarian);
        }
    }
    if(f){
        cout<<"Librarian not found"<<endl;
        cout<<"Enter any key to return to home"<<endl;
        cin.ignore();
        cin.get();
        this->Librarian_home(librarian);
    }
}
//Any user can view list of books and its status
void Library::View_books(Librarian* librarian){
    for (int i = 0; i < books.size(); i++) {
        cout << "Book" << i + 1 << ":" << endl;
        cout << "BookID: " << books[i]->Get_bookID() << endl;
        cout << "Title: " << books[i]->Get_title() << endl;
        cout << "Author: " << books[i]->Get_author() << endl;
        cout << "Publisher: " << books[i]->Get_publisher() << endl;
        cout << "Year: " << books[i]->Get_year() << endl;
        cout << "ISBN: " << books[i]->Get_ISBN() << endl;
        cout << "Status: " <<( (books[i]->Get_status()==0)?"available":((books[i]->Get_status()==1)?"borrowed":"reserved") )<< endl;
    }
    cout << "Enter any key to return to home" << endl;
    cin.ignore();
    cin.get();
    this->Librarian_home(librarian);
}
void Library::View_books(Student* student){
    for (int i = 0; i < books.size(); i++) {
        cout << "Book" << i + 1 << ":" << endl;
        cout << "BookID: " << books[i]->Get_bookID() << endl;
        cout << "Title: " << books[i]->Get_title() << endl;
        cout << "Author: " << books[i]->Get_author() << endl;
        cout << "Publisher: " << books[i]->Get_publisher() << endl;
        cout << "Year: " << books[i]->Get_year() << endl;
        cout << "ISBN: " << books[i]->Get_ISBN() << endl;
        cout << "Status: " <<( (books[i]->Get_status()==0)?"available":((books[i]->Get_status()==1)?"borrowed":"reserved") )<< endl;
    }
    cout << "Enter any key to return to home" << endl;
    cin.ignore();
    cin.get();
    this->Student_home(student);
}
void Library::View_books(Faculty* faculty){
    for (int i = 0; i < books.size(); i++) {
        cout << "Book" << i + 1 << ":" << endl;
        cout << "BookID: " << books[i]->Get_bookID() << endl;
        cout << "Title: " << books[i]->Get_title() << endl;
        cout << "Author: " << books[i]->Get_author() << endl;
        cout << "Publisher: " << books[i]->Get_publisher() << endl;
        cout << "Year: " << books[i]->Get_year() << endl;
        cout << "ISBN: " << books[i]->Get_ISBN() << endl;
        cout << "Status: " <<( (books[i]->Get_status()==0)?"available":((books[i]->Get_status()==1)?"borrowed":"reserved") )<< endl;
    }
    cout << "Enter any key to return to home" << endl;
    cin.ignore();
    cin.get();
    this->Faculty_home(faculty);
}
//Only librarian can view list of users
void Library::View_students(Librarian* librarian){
    for(int i=0;i<students.size();i++){
        cout<<"Student"<<i+1<<":"<<endl;
        cout<<"UserID:"<<students[i]->Get_userID()<<endl;
        cout<<"Username:"<<students[i]->Get_username()<<endl;
        cout<<"Password:"<<students[i]->Get_password()<<endl;
    }
    cout<<"Enter any key to return to home"<<endl;
    cin.ignore();
    cin.get();
    this->Librarian_home(librarian);
}
void Library::View_faculties(Librarian* librarian){
    for(int i=0;i<faculties.size();i++){
        cout<<"Faculty"<<i+1<<":"<<endl;
        cout<<"UserID:"<<faculties[i]->Get_userID()<<endl;
        cout<<"Username:"<<faculties[i]->Get_username()<<endl;
        cout<<"Password:"<<faculties[i]->Get_password()<<endl;
    }
    cout<<"Enter any key to return to home"<<endl;
    cin.ignore();
    cin.get();
    this->Librarian_home(librarian);
}
void Library::View_librarians(Librarian* librarian){
    for(int i=0;i<librarians.size();i++){
        cout<<"Librarian"<<i+1<<":"<<endl;
        cout<<"UserID:"<<librarians[i]->Get_userID()<<endl;
        cout<<"Username:"<<librarians[i]->Get_username()<<endl;
        cout<<"Password:"<<librarians[i]->Get_password()<<endl;
    }
    cout<<"Enter any key to return to home"<<endl;
    cin.ignore();
    cin.get();
    this->Librarian_home(librarian);
}
//Getter functions
Book* Library::Get_book(string bookID){
    for(int i=0;i<books.size();i++){
        if(books[i]->Get_bookID()==bookID) return books[i];
    }
    return NULL;
}
Student* Library::Get_student(string userID){
    for(int i=0;i<students.size();i++){
        if(students[i]->Get_userID()==userID) return students[i];
    }
    return NULL;
}
Faculty* Library::Get_faculty(string userID){
    for(int i=0;i<faculties.size();i++){
        if(faculties[i]->Get_userID()==userID) return faculties[i];
    }
    return NULL;
}
Librarian* Library::Get_librarian(string userID){
    for(int i=0;i<librarians.size();i++){
        if(librarians[i]->Get_userID()==userID) return librarians[i];
    }
    return NULL;
}
// Fine updation can be only done by librarian
void Library::Update_fine(Librarian* librarian){
    cout<<"Enter student's userID:";
    string userID;
    cin>>userID;
    Student* student=this->Get_student(userID);
    if(student==NULL){
        cout<<"Student not found"<<endl;
        cout<<"Enter 1 to try again"<<endl;
        cout<<"Enter 2 to return to home"<<endl;
        int t;
        cin>>t;
        if(t==1) this->Update_fine(librarian);
        if(t==2) this->Librarian_home(librarian);
    }
    else{
        cout<<"Student have fine of :"<< student->Get_fine()<<" rupees"<<endl;
        cout<<"Enter 1 to clear fine"<<endl;
        cout<<"Enter 2 to return to home"<<endl;
        int t;
        cin>>t;
        if(t==1){
            cout<<"Enter the amount that student paid:";
            long long fine_paid;
            cin>>fine_paid;
            if(fine_paid>student->Get_fine()){
                cout<<"Amount paid is greater than fine"<<endl;
                cout<<"Enter 1 to try again"<<endl;
                cout<<"Enter 2 to return to home"<<endl;
                int t;
                cin>>t;
                if(t==1) this->Update_fine(librarian);
                else if(t==2) this->Librarian_home(librarian);
            }
            else {
                cout<<"Fine updated successfully"<<endl;
                student->Set_fine(student->Get_fine()-fine_paid);
                this->Librarian_home(librarian);
            }
        }
        else if(t==2) this->Librarian_home(librarian);
        else{
            cout<<"Invalid input"<<endl;
            this->Update_fine(librarian);
        }
    }
}
void Library::Student_home(Student* student){
    cout<<"Welcome student \""<<student->Get_username()<<"\""<<endl;
    cout<<"Enter 1 to view account"<<endl;
    cout<<"Enter 2 to borrow book"<<endl;
    cout<<"Enter 3 to return book"<<endl;
    cout<<"Enter 4 to view books"<<endl;
    cout<<"Enter 5 to reserve book"<<endl;
    cout<<"Enter 6 to logout"<<endl;
    int t;
    cin>>t;
    if(t==1) student->View_account(*this);
    else if(t==2) student->BorrowBook(*this);
    else if(t==3) student->ReturnBook(*this);
    else if(t==4) View_books(student);
    else if(t==5) student->ReserveBook(*this);
    else if(t==6) Home();
    else{
        cout<<"Invalid input"<<endl;
        Student_home(student);
    }
}
void Library::Faculty_home(Faculty* faculty){
    cout<<"Welcome faculty \""<<faculty->Get_username()<<"\""<<endl;
    cout<<"Enter 1 to view account"<<endl;
    cout<<"Enter 2 to borrow book"<<endl;
    cout<<"Enter 3 to return book"<<endl;
    cout<<"Enter 4 to view books"<<endl;
    cout<<"Enter 5 to reserve book"<<endl;
    cout<<"Enter 6 to logout"<<endl;
    int t;
    cin>>t;
    if(t==1) faculty->View_account(*this);
    else if(t==2) faculty->BorrowBook(*this);
    else if(t==3) faculty->ReturnBook(*this);
    else if(t==4) View_books(faculty);
    else if(t==5) faculty->ReserveBook(*this);
    else if(t==6) Home();
    else{
        cout<<"Invalid input"<<endl;
        Faculty_home(faculty);
    }
}
void Library::Librarian_home(Librarian* librarian){
    cout<<"Welcome librarian \""<<librarian->Get_username()<<"\""<<endl;
    cout<<"Enter 1 to add book"<<endl;
    cout<<"Enter 2 to delete book"<<endl;
    cout<<"Enter 3 to add student"<<endl;
    cout<<"Enter 4 to delete student"<<endl;
    cout<<"Enter 5 to add faculty"<<endl;
    cout<<"Enter 6 to delete faculty"<<endl;
    cout<<"Enter 7 to add librarian"<<endl;
    cout<<"Enter 8 to delete librarian"<<endl;
    cout<<"Enter 9 to view books"<<endl;
    cout<<"Enter 10 to view students"<<endl;
    cout<<"Enter 11 to view faculties"<<endl;
    cout<<"Enter 12 to view librarians"<<endl;
    cout<<"Enter 13 to update fine of student"<<endl;
    cout<<"Enter 14 to logout"<<endl;
    int t;
    cin>>t;
    if(t==1) Add_book(librarian);
    else if(t==2) Delete_book(librarian);
    else if(t==3) Add_student(librarian);
    else if(t==4) Delete_student(librarian);
    else if(t==5) Add_faculty(librarian);
    else if(t==6) Delete_faculty(librarian);
    else if(t==7) Add_librarian(librarian);
    else if(t==8) Delete_librarian(librarian);
    else if(t==9) View_books(librarian);
    else if(t==10) View_students(librarian);
    else if(t==11) View_faculties(librarian);
    else if(t==12) View_librarians(librarian);
    else if(t==13) Update_fine(librarian);
    else if(t==14) Home();
    else{
        cout<<"Invalid input"<<endl;
        Librarian_home(librarian);
    }
}
//Main login page
void Library::Home(){
    cout<<"Enter 1 for student login"<<endl;
    cout<<"Enter 2 for faculty login"<<endl;
    cout<<"Enter 3 for librarian login"<<endl;
    cout<<"Enter 4 to exit"<<endl;
    int t;
    cin>>t;
    if(t==1){
        cout<<"Enter userID:";
        string userID;
        cin>>userID;
        Student* student=Get_student(userID);
        if(student==NULL){
            cout<<"Student not found"<<endl;
            Home();
        }
        else{
            string password;
                cout<<"Enter password:";
                cin>>password;
                if(student->Get_password()!=password){
                cout<<"Incorrect password"<<endl;
                Home();
            }
            else Student_home(student);
        }
    }
    else if(t==2){
        cout<<"Enter userID:";
        string userID;
        cin>>userID;
        Faculty* faculty=Get_faculty(userID);
        if(faculty==NULL){
            cout<<"Faculty not found"<<endl;
            Home();
        }
        else{
            string password;
            cout<<"Enter password:";
            cin>>password;
            if(faculty->Get_password()!=password){
                cout<<"Incorrect password"<<endl;
                Home();
            }
            else Faculty_home(faculty);
        }
    }
    else if(t==3){
        cout<<"Enter userID:";
        string userID;
        cin>>userID;
        Librarian* librarian=Get_librarian(userID);
        if(librarian==NULL){
            cout<<"Librarian not found"<<endl;
            Home();
        }
        else{
            string password;
            cout<<"Enter password:";
            cin>>password;
            if(librarian->Get_password()!=password){
                cout<<"Incorrect password"<<endl;
                Home();
            }
            else Librarian_home(librarian);
        }
    }
    else if(t==4) return;
    else{
        cout<<"Invalid input"<<endl;
        Home();
    }
}

int main() {
    Library library;
    library.Load();
    library.Home();
    library.Save();
    return 0;
}