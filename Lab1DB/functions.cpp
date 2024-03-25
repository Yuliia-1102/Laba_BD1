#include <iostream>
#include "functions.h"
using namespace std;

void getClientInfo(string& name, string& address, string& number, string& dateOfB){
    cout << "Enter a name: " << endl;
    cin >> name;
    cout << "Enter a phone number: " << endl;
    cin >> number;
    cout << "Enter a date of birth: " << endl;
    cin >> dateOfB;
    cout << "Enter an email address: " << endl;
    cin >> address;
}

void getOrderInfo(string& dateOfArr, int& donations, string& movieName){
    cout << "Enter the film's name client wants to buy: " << endl;
    cin >> movieName;
    cout << "Enter a card number: " << endl;
    cin >> dateOfArr;
    cout << "Enter amount of money the client wants to donate for Ukrainian Armed Forces (ЗСУ): " << endl;
    cin >> donations;
}

void getOptions(){
    cout << "Choose your action: " << endl;
    cout << "1 -> Add a client (insert-m)." << endl;
    cout << "2 -> Delete a client (delete-m)." << endl;
    cout << "3 -> Edit a client (update-m)." << endl;
    cout << "4 -> Add an order (insert-s)." << endl;
    cout << "5 -> Delete an order (delete-s)." << endl;
    cout << "6 -> Edit an order (update-s)." << endl;
    cout << "7 -> Print all info about clients with number of their orders (ut-m)." << endl;
    cout << "8 -> Print all the info about all orders (ut-s)." << endl;
    cout << "9 -> Get all info about the selected client (get-m)." << endl;
    cout << "10 -> Get all info about the selected order of the selected client (get-s)." << endl;
    cout << "11 -> Count clients (calc-m)." << endl;
    cout << "12 -> Count orders (calc-s)." << endl;
    cout << "13 -> Exit." << endl;
}

