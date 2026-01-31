# Proiect-MAP---Tema-22---Aplicatie-pentru-gestionarea-unei-biblioteci-mici

Library Manager (C++) este o aplicatie de consola pentru gestionarea unei biblioteci mici: carti + utilizatori, imprumut/returnare cu termen limita, cautare, rapoarte si import/export CSV. Datele se salveaza automat in books.csv si users.csv (in acelasi folder cu executabilul).

Compilare:
g++ -O2 -std=c++17 library_manager.cpp -o library_manager

Rulare:
./library_manager <comanda> [optiuni]

Fisiere:
books.csv – catalogul de carti (include status imprumut + termen)
users.csv – lista de utilizatori (include istoric imprumuturi)

COMENZI

Adaugare carte:
./library_manager add_book "1984" "George Orwell" --isbn 9780451524935 --category Fiction --year 1949

Adaugare utilizator:
./library_manager add_user "Popescu Ion" --id 1001 --email ion@example.com

Imprumut carte:
Format : ./library_manager borrow --isbn <isbn> --user_id <id> [--days <zile>]
./library_manager borrow --isbn 9780451524935 --user_id 1001
./library_manager borrow --isbn 9780451524935 --user_id 1001 --days 7

Returnare carte:
Format: ./library_manager return --isbn <isbn> --user_id <id>
./library_manager return --isbn 9780451524935 --user_id 1001

Cautare:
./library_manager search --title "1984"
./library_manager search --author "George Orwell"
./library_manager search --category Fiction
./library_manager search --isbn 9780451524935

Rapoarte:
./library_manager report --borrowed
./library_manager report --popular
./library_manager report --active-users
./library_manager report --stats

Export catalog:
./library_manager export catalog.csv

Import catalog:
./library_manager import catalog.csv






