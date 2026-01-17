#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

struct Book {
    string title;
    string author;
    string isbn;
    string category;
    string year;
    string borrowed_by = "";
    int borrow_count = 0;
    long long due_ts = 0;
};

struct User {
    string name;
    string id;
    string email;
    int history_count = 0;
};

vector<Book> g_books;
vector<User> g_users;

const string BOOKS_FILE = "books.csv";
const string USERS_FILE = "users.csv";

int main() {
    return 0;
}
