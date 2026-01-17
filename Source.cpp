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

int main() {
    return 0;
}
