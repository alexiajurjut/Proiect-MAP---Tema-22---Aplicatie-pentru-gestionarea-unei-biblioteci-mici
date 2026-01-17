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
    if (!lt) return "";
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

int main() {
    loadBooks();
    loadUsers();
    return 0;
}
