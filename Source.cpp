#define _CRT_SECURE_NO_WARNINGS

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

long long nowTs() {
    return (long long)time(nullptr);
}

string tsToDate(long long ts) {
    if (ts <= 0) return "";
    time_t t = (time_t)ts;
    tm* lt = localtime(&t);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    return string(buf);
}

string opt(const vector<string>& args, const string& key, const string& def = "") {
    for (size_t i = 0; i + 1 < args.size(); i++)
        if (args[i] == key) return args[i + 1];
    return def;
}

bool hasFlag(const vector<string>& args, const string& key) {
    return find(args.begin(), args.end(), key) != args.end();
}

int safeToInt(const string& s, int def = 0) {
    try {
        if (s.empty()) return def;
        return stoi(s);
    }
    catch (...) { return def; }
}

vector<string> split(const string& line, char sep) {
    vector<string> out;
    string cur;
    for (char c : line) {
        if (c == sep) {
            out.push_back(cur);
            cur.clear();
        }
        else cur.push_back(c);
    }
    out.push_back(cur);
    return out;
}

void loadBooks() {
    g_books.clear();
    ifstream f(BOOKS_FILE);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto p = split(line, ',');
        if (p.size() < 7) continue;
        Book b;
        b.title = p[0];
        b.author = p[1];
        b.isbn = p[2];
        b.category = p[3];
        b.year = p[4];
        b.borrowed_by = p[5];
        b.borrow_count = safeToInt(p[6]);
        if (p.size() > 7) b.due_ts = stoll(p[7]);
        g_books.push_back(b);
    }
}

void loadUsers() {
    g_users.clear();
    ifstream f(USERS_FILE);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto p = split(line, ',');
        if (p.size() < 4) continue;
        User u;
        u.name = p[0];
        u.id = p[1];
        u.email = p[2];
        u.history_count = safeToInt(p[3]);
        g_users.push_back(u);
    }
}

void saveBooks() {
    ofstream out(BOOKS_FILE);
    for (auto& b : g_books) {
        out << b.title << "," << b.author << "," << b.isbn << ","
            << b.category << "," << b.year << ","
            << b.borrowed_by << "," << b.borrow_count << ","
            << b.due_ts << "\n";
    }
}

void saveUsers() {
    ofstream out(USERS_FILE);
    for (auto& u : g_users) {
        out << u.name << "," << u.id << "," << u.email << "," << u.history_count << "\n";
    }
}

void cmdAddBook(const vector<string>& args) {
    if (args.size() < 3) {
        cout << "Folosire: add_book \"Titlu\" \"Autor\" --isbn <isbn>\n";
        return;
    }

    Book b;
    b.title = args[1];
    b.author = args[2];
    b.isbn = opt(args, "--isbn");
    b.category = opt(args, "--category", "N/A");
    b.year = opt(args, "--year", "N/A");

    if (b.isbn.empty()) {
        cout << "Lipseste --isbn.\n";
        return;
    }

    for (auto& x : g_books)
        if (x.isbn == b.isbn) {
            cout << "Exista deja o carte cu acest ISBN.\n";
            return;
        }

    g_books.push_back(b);
    saveBooks();
    cout << "Carte adaugata.\n";
}

void cmdAddUser(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "Folosire: add_user \"Nume\" --id <id>\n";
        return;
    }

    User u;
    u.name = args[1];
    u.id = opt(args, "--id");
    u.email = opt(args, "--email", "N/A");

    if (u.id.empty()) {
        cout << "Lipseste --id.\n";
        return;
    }

    for (auto& x : g_users)
        if (x.id == u.id) {
            cout << "Exista deja un utilizator cu acest ID.\n";
            return;
        }

    g_users.push_back(u);
    saveUsers();
    cout << "Utilizator adaugat.\n";
}

int main(int argc, char** argv) {
    loadBooks();
    loadUsers();

    vector<string> args;
    for (int i = 1; i < argc; i++) args.push_back(argv[i]);

    if (args.empty()) {
        cout << "Comenzi disponibile: add_book, add_user\n";
        return 0;
    }

    string cmd = args[0];

    if (cmd == "add_book") cmdAddBook(args);
    else if (cmd == "add_user") cmdAddUser(args);
    else cout << "Comanda necunoscuta.\n";

    return 0;
}
