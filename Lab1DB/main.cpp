#include <iostream>
#include <cstdio>
#include <cstring>
#include "functions.h"
using namespace std;

struct Client {
public:
    char name[40];
    char emailAddress[40];
    char phoneNumber[40];
    char dateOfBirth[40];
    int orderCount;
};

struct Identificator {
public:
    int cliId;
    int32_t clientAddress;
};

struct Order {
public:
    int orderId;
    int clientId;
    char cardNumber[40];
    char filmName[40];
    int donation;
};

Identificator getClientIndex(int position){
    Identificator index{};
    FILE* IndexDB = fopen("clientIndex.bin", "rb");
    fseek(IndexDB, position * sizeof(Identificator), SEEK_SET);
    fread(&index, sizeof(Identificator), 1, IndexDB);
    fclose(IndexDB);
    return index;
}

Client getClient(int position) {
    Client client{};
    Identificator index = getClientIndex(position);
    FILE *clientDB = fopen("clients.bin", "rb");
    fseek(clientDB, index.clientAddress, SEEK_SET);
    fread(&client, sizeof(Client), 1, clientDB);
    fclose(clientDB);
    return client;
}

Order getOrder(int position){
    Order order{};
    FILE* ordersDB = fopen("orders.bin", "rb");
    fseek(ordersDB, position * sizeof(Order), SEEK_SET);
    fread(&order, sizeof(Order), 1, ordersDB);
    fclose(ordersDB);
    return order;
}


void writeClient(Client& client, int position){
    Identificator index = getClientIndex(position);
    FILE* clientsDB = fopen("clients.bin", "rb+");
    fseek(clientsDB, index.clientAddress, SEEK_SET);
    fwrite(&client, sizeof(Client), 1, clientsDB);
    fclose(clientsDB);
}

void writeOrder(Order& order, int position){
    FILE* ordersDB = fopen("orders.bin", "rb+");
    fseek(ordersDB, position * sizeof(Order), SEEK_SET);
    fwrite(&order, sizeof(Order), 1, ordersDB);
    fclose(ordersDB);
}

void writeIndex(Identificator index, int position){
    FILE* clientIndex = fopen("clientIndex.bin", "rb+");
    fseek(clientIndex, position * sizeof(Identificator), SEEK_SET);
    fwrite(&index, sizeof(Identificator), 1, clientIndex);
    fclose(clientIndex);
}

int getAmountOfClients(){
    FILE* cliDb = fopen("clients.bin", "ab+");
    fseek(cliDb, 0, SEEK_END);
    int count = ftell(cliDb)/sizeof(Client);
    fclose(cliDb);
    return count;
}

int getAmountOfOrders(){
    FILE* ordersDB = fopen("orders.bin", "ab+");
    fseek(ordersDB, 0, SEEK_END);
    int count = ftell(ordersDB) / sizeof(Order);
    fclose(ordersDB);
    return count;
}

void addClient(Client& client) {
    Identificator index{};
    FILE* IndexDB = fopen("clientIndex.bin", "ab+");
    fseek(IndexDB, 0, SEEK_END);
    int count = ftell(IndexDB)/sizeof(Identificator) + 1;

    index.cliId = count;
    index.clientAddress = (count - 1) * sizeof(Client);

    fseek(IndexDB, 0, SEEK_SET);
    fwrite(&index, sizeof(Identificator), 1, IndexDB);
    fclose(IndexDB);

    FILE* cliDb = fopen("clients.bin", "ab+");
    fwrite(&client, sizeof(Client), 1, cliDb);
    fclose(cliDb);
}

void addOrder(Order& order, int clientId){
    Client client = getClient(clientId - 1);
    client.orderCount++;
    writeClient(client, clientId - 1);

    FILE* ordersDB = fopen("orders.bin", "ab+");
    fseek(ordersDB, 0, SEEK_END);
    order.orderId = ftell(ordersDB)/sizeof(Order) + 1;
    fwrite(&order, sizeof(Order), 1, ordersDB);
    fclose(ordersDB);
}

int idClientValidator(int id){
    int all = getAmountOfClients();
    if(id > all || id < 1){
        cout << "Incorrect Id." << endl;
        cout << endl;
        return -1;
    }

    Identificator index = getClientIndex(id - 1);
    if(index.cliId == -1){
        cout << "This client is already deleted." << endl;
        cout << endl;
        return -1;
    }
    return 0;
}

int idOrderValidator(int id){
    int all = getAmountOfOrders();
    if(id > all || id < 1){
        cout << "Incorrect id." << endl;
        cout << endl;
        return -1;
    }

    Order order = getOrder(id - 1);
    if(order.orderId == -1){
        cout << "This order is already deleted." << endl;
        cout << endl;
        return -1;
    }
    return 0;
}

void deleteClient(int id) {
    int result = idClientValidator(id);
    if(result == -1) return;

    Identificator index = getClientIndex(id - 1);
    Client client = getClient(id - 1);
    Order order{};
    int amount = getAmountOfOrders();

    if(client.orderCount > 0){
        for(int i = 0; i < amount; i++){
            order = getOrder(i);
            if(index.cliId == order.clientId){
                order.orderId = -1;
                writeOrder(order, i);
            }
        }
    }

    index.cliId = -1;
    writeIndex(index, id - 1);
    cout << "Client has been deleted." << endl;
    cout << endl;
}

void deleteOrder(int clientId, int orderId) {
    Client client = getClient(clientId - 1);
    client.orderCount--;
    writeClient(client, clientId - 1);

    Order reservation = getOrder(orderId - 1);
    reservation.orderId = -1;
    writeOrder(reservation, orderId - 1);
}

int countValidClients() {
    int amount = getAmountOfClients();
    int totalClients = 0;
    Client client{};
    Identificator index{};

    for (int i = 0; i < amount; i++) {
        client = getClient(i);
        index = getClientIndex(i);
        if (index.cliId != -1) {
            totalClients++;
        }
    }

    return totalClients;
}

int countValidOrders() {
    int amount = getAmountOfOrders();
    int totalOrders = 0;
    Order order{};

    for (int i = 0; i < amount; i++) {
        order = getOrder(i);
        if (order.orderId != -1 && order.orderId != 0) {
            totalOrders++;
        }
    }

    return totalOrders;
}

void displayClientInfo(int index) {
    int totalClients = getAmountOfClients();
    if (index < 0 || index >= totalClients) {
        cout << "No client found at this index. Please enter a valid index." << endl;
        return;
    }

    Identificator identificator = getClientIndex(index);
    if (identificator.cliId == -1) {
        cout << "This client has been deleted." << endl;
        return;
    }

    Client client = getClient(index);
    cout << "Client Information:" << endl;
    cout << "ID: " << identificator.cliId << endl;
    cout << "Name: " << client.name << endl;
    cout << "Email Address: " << client.emailAddress << endl;
    cout << "Phone Number: " << client.phoneNumber << endl;
    cout << "Date of Birth: " << client.dateOfBirth << endl;
    cout << "Order Count: " << client.orderCount << endl;
}


int main() {
    Client client{};
    Order order{};
    Identificator index{};
    int choice = 0;
    int id, clientId, donations, amount;
    string name, address, number, dateOfB, cardNum, movieName;
    int totalOrders, totalClients = 0;

    getOptions();
    while (true) {
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Wrong number." << endl;
        }

        switch (choice) {
            case 1:
                getClientInfo(name, address, number, dateOfB);
                strcpy(client.name, name.c_str());
                strcpy(client.emailAddress, address.c_str());
                strcpy(client.phoneNumber, number.c_str());
                strcpy(client.dateOfBirth, dateOfB.c_str());
                client.orderCount = 0;
                addClient(client);
                break;

            case 2:
                cout << "Enter Id of the client you want to delete: " << endl;
                cin >> id;
                deleteClient(id);
                break;

            case 3:
                cout << "Enter Id of the client you want to edit: " << endl;
                cin >> id;
                if (idClientValidator(id) == 0) {
                    getClientInfo(name, address, number, dateOfB);
                    client = getClient(id - 1);
                    strcpy(client.name, name.c_str());
                    strcpy(client.emailAddress, address.c_str());
                    strcpy(client.phoneNumber, number.c_str());
                    strcpy(client.dateOfBirth, dateOfB.c_str());
                    writeClient(client, id - 1);
                }
                break;

            case 4:
                cout << "Enter Id of the client for who you want to make an order: " << endl;
                cin >> id;
                if (idClientValidator(id) == 0) {
                    getOrderInfo(cardNum, donations, movieName);
                    order.donation = donations;
                    order.clientId = id;
                    strcpy(order.cardNumber, cardNum.c_str());
                    strcpy(order.filmName, movieName.c_str());
                    addOrder(order, id);
                }
                break;

            case 5:
                cout << "Enter the client's Id: " << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    client = getClient(clientId - 1);
                    if (client.orderCount > 0) {
                        cout << "Enter the order's Id: " << endl;
                        cin >> id;
                        if (idOrderValidator(id) == 0) deleteOrder(clientId, id);

                    } else cout << "The selected client has got no orders." << endl;
                }
                break;

            case 6:
                cout << "Enter the client's id: " << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    client = getClient(clientId - 1);
                    if (client.orderCount > 0) {
                        cout << "Enter the order's id: " << endl;
                        cin >> id;
                        if (idOrderValidator(id) == 0) {
                            order = getOrder(id - 1);
                            getOrderInfo(cardNum, donations, movieName);
                            order.donation = donations;
                            strcpy(order.cardNumber, cardNum.c_str());
                            strcpy(order.filmName, movieName.c_str());
                            writeOrder(order, id - 1);
                            cout << "The selected order has been edited." << endl;
                        }
                    } else cout << "The selected client has got no orders." << endl;
                    cout << endl;
                }
                break;

            case 7:
                amount = getAmountOfClients();
                cout << "All clients in database with their all info:" << endl;
                for (int i = 0; i < amount; i++) {
                    client = getClient(i);
                    index = getClientIndex(i);
                    if (index.cliId != -1) {
                        cout << "The client's Id: " << index.cliId << endl;
                        cout << "The client's name: " << client.name << endl;
                        cout << "The client's email address: " << client.emailAddress << endl;
                        cout << "The client's phone number: " << client.phoneNumber << endl;
                        cout << "The client's date of birth: " << client.dateOfBirth << endl;
                        cout << "The number of orders: " << client.orderCount << endl;
                        cout << endl;
                    }
                }

                break;

            case 8:
                amount = getAmountOfOrders();
                cout << "All reservations in database:" << endl;
                for (int i = 0; i < amount; i++) {
                    order = getOrder(i);
                    if (order.orderId != -1 && order.orderId != 0) {
                        cout << "The reservation's id: " << order.orderId << endl;
                        cout << "The client's id: " << order.clientId << endl;
                        cout << "The name of the film that client has bought: " << order.filmName << endl;
                        cout << "The amount of money client donated for ЗСУ: " << order.donation << endl;
                        cout << "The client's card number: " << order.cardNumber << endl;
                        cout << endl;
                    }
                }
                break;

            case 9:
                cout << "Enter the index of the client you want to display: ";
                int index;
                cin >> index;
                if (!cin.fail() && index >= 0) {
                    displayClientInfo(index - 1);
                } else {
                    cout << "Invalid input. Please enter a valid index." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;

            case 10:
                cout << "Enter client's Id: " << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    cout << "Enter order's Id: " << endl;
                    int orderId;
                    cin >> orderId;
                    if (idOrderValidator(orderId) == 0) {
                        order = getOrder(orderId - 1);
                        if (order.clientId == clientId && order.orderId != -1 && order.orderId != 0) {
                            cout << "\nThe order's id: " << order.orderId << endl;
                            cout << "The name of the film that client has bought: " << order.filmName << endl;
                            cout << "The amount of money client donated for ЗСУ: " << order.donation << endl;
                            cout << "The client's card number: " << order.cardNumber << endl;
                        } else {
                            cout << "This client does not have an order with the specified Id." << endl;
                        }
                    } else {
                        cout << "Invalid order Id or order does not exist." << endl;
                    }
                } else {
                    cout << "Invalid client Id or client does not exist." << endl;
                }
                cout << endl;
                break;

            case 11:
                cout << "Total number of valid clients: " << countValidClients() << endl;
                break;

            case 12:
                cout << "Total number of valid orders: " << countValidOrders() << endl;
                break;

            case 13:
                return 0;

            default:
                cout << "Wrong input." << endl;
                break;
        }
        cout << "What do you want to do now?" << endl;
    }
}