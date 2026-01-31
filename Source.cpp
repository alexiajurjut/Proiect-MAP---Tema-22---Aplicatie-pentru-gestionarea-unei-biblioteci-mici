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

void cmdBorrow(const vector<string>& args) {
    string isbn = opt(args, "--isbn");
    string uid = opt(args, "--user_id");
    int days = safeToInt(opt(args, "--days", "14"), 14);

    if (isbn.empty() || uid.empty()) {
        cout << "Trebuie --isbn si --user_id.\n";
        return;
    }

    auto uit = find_if(g_users.begin(), g_users.end(),
        [&](const User& u) { return u.id == uid; });
    if (uit == g_users.end()) {
        cout << "Utilizator inexistent.\n";
        return;
    }

    for (auto& b : g_books) {
        if (b.isbn == isbn) {
            if (!b.borrowed_by.empty()) {
                cout << "Cartea este deja imprumutata.\n";
                return;
            }

            b.borrowed_by = uid;
            b.borrow_count++;
            b.due_ts = nowTs() + (long long)days * 24LL * 3600LL;

            uit->history_count++;

            saveBooks();
            saveUsers();

            cout << "Imprumut inregistrat. Termen: " << tsToDate(b.due_ts) << "\n";
            return;
        }
    }

    cout << "ISBN inexistent.\n";
}

void cmdReturn(const vector<string>& args) {
    string isbn = opt(args, "--isbn");
    string uid = opt(args, "--user_id");

    if (isbn.empty() || uid.empty()) {
        cout << "Trebuie --isbn si --user_id.\n";
        return;
    }

    for (auto& b : g_books) {
        if (b.isbn == isbn) {
            if (b.borrowed_by.empty()) {
                cout << "Cartea nu este imprumutata.\n";
                return;
            }
            if (b.borrowed_by != uid) {
                cout << "Aceasta carte nu a fost imprumutata de acest utilizator.\n";
                return;
            }

            long long now = nowTs();
            int overdue = (now > b.due_ts)
                ? (int)((now - b.due_ts) / (24LL * 3600LL))
                : 0;

            b.borrowed_by = "";
            b.due_ts = 0;
            saveBooks();

            cout << "Cartea a fost returnata.\n";
            if (overdue > 0)
                cout << "Intarziere: " << overdue << " zile.\n";
            else
                cout << "Returnata la timp.\n";

            return;
        }
    }

    cout << "ISBN inexistent.\n";
}

void cmdSearch(const vector<string>& args) {
    string t = opt(args, "--title");
    string a = opt(args, "--author");
    string c = opt(args, "--category");
    string i = opt(args, "--isbn");

    cout << "Rezultate:\n";
    for (auto& b : g_books) {
        if ((t.empty() || b.title == t) &&
            (a.empty() || b.author == a) &&
            (c.empty() || b.category == c) &&
            (i.empty() || b.isbn == i)) {

            cout << " - " << b.title << " | "
                << (b.borrowed_by.empty() ? "DISPONIBILA" : "IMPRUMUTATA")
                << "\n";
        }
    }
}

void cmdReport(const vector<string>& args) {
    if (hasFlag(args, "--borrowed")) {
        cout << "Carti imprumutate:\n";
        for (auto& b : g_books)
            if (!b.borrowed_by.empty())
                cout << " - " << b.title << " la " << b.borrowed_by << "\n";
    }
    else if (hasFlag(args, "--popular")) {
        cout << "Popularitate carti:\n";
        for (auto& b : g_books)
            cout << " - " << b.title << " (" << b.borrow_count << " imprumuturi)\n";
    }
    else if (hasFlag(args, "--active-users")) {
        cout << "Utilizatori activi:\n";
        for (auto& u : g_users)
            if (u.history_count > 0)
                cout << " - " << u.name << " (" << u.history_count << " imprumuturi)\n";
    }
    else if (hasFlag(args, "--stats")) {
        cout << "Total carti: " << g_books.size() << "\n";
        cout << "Total utilizatori: " << g_users.size() << "\n";
    }
}

int main(int argc, char** argv) {
    loadBooks();
    loadUsers();

    vector<string> args;
    for (int i = 1; i < argc; i++) args.push_back(argv[i]);

    if (args.empty()) {
        cout << "Comenzi disponibile: add_book, add_user, borrow, return, search, report\n";
        return 0;
    }

    string cmd = args[0];

    if (cmd == "add_book") cmdAddBook(args);
    else if (cmd == "add_user") cmdAddUser(args);
    else if (cmd == "borrow") cmdBorrow(args);
    else if (cmd == "return") cmdReturn(args);
    else if (cmd == "search") cmdSearch(args);
    else if (cmd == "report") cmdReport(args);
    else cout << "Comanda necunoscuta.\n";

    return 0;
}
