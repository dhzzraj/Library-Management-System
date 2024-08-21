#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class Book {
public:
    int bookId;
    char title[50];
    char author[50];
    bool isAvailable;

    void addBook();
    void displayBook() const;
    void getAllBooks() const;
    void updateBook();
    void deleteBook();
    bool isBookExists(int) const;
};

class Patron {
public:
    void borrowBook();
    void returnBook();
    void viewBorrowedBooks() const;
};

bool Book::isBookExists(int id) const {
    ifstream checkFile("books.dat", ios::binary);
    Book b;
    while (checkFile.read((char*)&b, sizeof(b))) {
        if (id == b.bookId) {
            checkFile.close();
            return true;
        }
    }
    checkFile.close();
    return false;
}

void Book::addBook() {
    cout << "\nEnter book title: ";
    cin.ignore();
    cin.getline(title, 50);

    cout << "Enter book author: ";
    cin.getline(author, 50);

    cout << "Enter book ID: ";
    while (!(cin >> bookId)) {
        cout << "Enter number only: ";
        cin.clear();
        cin.ignore(123, '\n');
    }

    if (isBookExists(bookId)) {
        cout << "Error: Book with ID " << bookId << " already exists\n";
    } else {
        isAvailable = true;
        ofstream file("books.dat", ios::binary | ios::app);
        if (file) {
            file.write((char*)this, sizeof(*this));
            file.close();
            cout << "\nBook added successfully...\n";
        } else {
            cout << "Error: Could not open file to save book data.\n";
        }
    }
}

void Book::displayBook() const {
    cout << setw(5) << bookId << setw(25) << title << setw(25) << author << setw(10) << (isAvailable ? "Available" : "Not Available") << endl;
}

void Book::getAllBooks() const {
    ifstream file("books.dat", ios::binary);
    if (!file) {
        cerr << "Error: Could not open file.\n";
        return;
    }

    Book b;
    bool found = false; // To check if any book exists

    // Print headings
    cout << setw(5) << "ID" << setw(25) << "Title" << setw(25) << "Author" << setw(10) << "Status" << endl;
    cout << "--------------------------------------------------------------" << endl;

    while (file.read((char*)&b, sizeof(b))) {
        found = true;
        b.displayBook();
    }

    if (!found) {
        cout << "There are no books.\n";
    }

    file.close();
}

void Book::updateBook() {
    int id, pos;
    bool found = false;
    cout << "\nEnter book ID to update: ";
    cin >> id;
    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book b;
    while (file.read((char*)&b, sizeof(b))) {
        if (b.bookId == id) {
            cout << "Enter new book title: ";
            cin.ignore();
            cin.getline(b.title, 50);
            cout << "Enter new book author: ";
            cin.getline(b.author, 50);
            pos = -1 * static_cast<int>(sizeof(b));
            file.seekp(pos, ios::cur);
            file.write((char*)&b, sizeof(b));
            found = true;
            cout << "\nBook updated successfully...\n";
            break;
        }
    }
    if (!found) {
        cout << "Invalid Book ID\n";
    }
    file.close();
}

void Book::deleteBook() {
    int id;
    bool found = false;
    cout << "\nEnter book ID to delete: ";
    cin >> id;
    ifstream file("books.dat", ios::binary);
    ofstream tempFile("temp.dat", ios::binary);
    Book b;
    while (file.read((char*)&b, sizeof(b))) {
        if (b.bookId != id) {
            tempFile.write((char*)&b, sizeof(b));
        } else {
            found = true;
        }
    }
    file.close();
    tempFile.close();
    remove("books.dat");
    rename("temp.dat", "books.dat");
    if (found) {
        cout << "\nBook deleted successfully...\n";
    } else {
        cout << "Invalid Book ID\n";
    }
}

void Patron::borrowBook() {
    int id;
    bool found = false;
    cout << "Enter book ID to borrow: ";
    cin >> id;
    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book b;
    while (file.read((char*)&b, sizeof(b))) {
        if (b.bookId == id) {
            if (b.isAvailable) {
                b.isAvailable = false;
                int pos = -1 * static_cast<int>(sizeof(b));
                file.seekp(pos, ios::cur);
                file.write((char*)&b, sizeof(b));
                found = true;
                cout << "\nBook borrowed successfully...\n";
                break;
            } else {
                cout << "Book is not available for borrowing.\n";
                break;
            }
        }
    }
    if (!found) {
        cout << "Invalid Book ID\n";
    }
    file.close();
}

void Patron::returnBook() {
    int id;
    bool found = false;
    cout << "Enter book ID to return: ";
    cin >> id;
    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book b;
    while (file.read((char*)&b, sizeof(b))) {
        if (b.bookId == id) {
            if (!b.isAvailable) {
                b.isAvailable = true;
                int pos = -1 * static_cast<int>(sizeof(b));
                file.seekp(pos, ios::cur);
                file.write((char*)&b, sizeof(b));
                found = true;
                cout << "\nBook returned successfully...\n";
                break;
            } else {
                cout << "Book is already available.\n";
                break;
            }
        }
    }
    if (!found) {
        cout << "Invalid Book ID\n";
    }
    file.close();
}

void Patron::viewBorrowedBooks() const {
    ifstream file("books.dat", ios::binary);
    if (!file) {
        cerr << "Error: Could not open file.\n";
        return;
    }

    Book b;
    bool found = false; // To check if any book exists

    // Print headings
    cout << setw(5) << "ID" << setw(25) << "Title" << setw(25) << "Author" << setw(25) << "Status" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;

    while (file.read((char*)&b, sizeof(b))) {
        if (!b.isAvailable) {
            found = true;
            b.displayBook();
        }
    }

    if (!found) {
        cout << "There are no borrowed books.\n";
    }

    file.close();
}

// Function for admin login
bool adminLogin() {
    string username, password;
    const string adminUsername = "dhzzraj";
    const string adminPassword = "dheeraj167"; // For demonstration purposes

    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    return (username == adminUsername && password == adminPassword);
}

// Function for customer login
bool customerLogin() {
    string username, password;
    const string customerUsername = "customer";
    const string customerPassword = "customer167"; // For demonstration purposes

    cout << "Enter customer username: ";
    cin >> username;
    cout << "Enter customer password: ";
    cin >> password;

    return (username == customerUsername && password == customerPassword);
}

int main() {
    int choice;
    bool isAdmin = false;
    bool isCustomer = false;

    while (true) {
        cout << "\n::::::::::||LIBRARY MANAGEMENT SYSTEM||::::::::::\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // Admin Login
                isAdmin = adminLogin();
                if (isAdmin) {
                    while (true) {
                        cout << "\n::::::::::||ADMIN MENU||::::::::::\n";
                        cout << "1. Add Book\n";
                        cout << "2. Display All Books\n";
                        cout << "3. Update Book\n";
                        cout << "4. Delete Book\n";
                        cout << "5. Exit\n";
                        cout << "Enter your choice: ";
                        cin >> choice;
                        Book book;
                        switch (choice) {
                            case 1:
                                book.addBook();
                                break;
                            case 2:
                                book.getAllBooks();
                                break;
                            case 3:
                                book.updateBook();
                                break;
                            case 4:
                                book.deleteBook();
                                break;
                            case 5:
                                isAdmin = false;
                                break;
                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                        if (!isAdmin) break;
                    }
                } else {
                    cout << "Invalid admin credentials.\n";
                }
                break;
            case 2: // Customer Login
                isCustomer = customerLogin();
                if (isCustomer) {
                    while (true) {
                        cout << "\n::::::::::||CUSTOMER MENU||::::::::::\n";
                        cout << "1. Borrow Book\n";
                        cout << "2. Return Book\n";
                        cout << "3. View Borrowed Books\n";
                        cout << "4. Exit\n";
                        cout << "Enter your choice: ";
                        cin >> choice;
                        Patron patron;
                        switch (choice) {
                            case 1:
                                patron.borrowBook();
                                break;
                            case 2:
                                patron.returnBook();
                                break;
                            case 3:
                                patron.viewBorrowedBooks();
                                break;
                            case 4:
                                isCustomer = false;
                                break;
                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                        if (!isCustomer) break;
                    }
                } else {
                    cout << "Invalid customer credentials.\n";
                }
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
