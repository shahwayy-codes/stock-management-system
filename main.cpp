#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
const int MAX_USERS = 10; // Global constant for maximum users
const int STOCK_COUNT = 4; // Global constant for number of stocks
string stockNames[STOCK_COUNT] = {"Apple", "Tesla", "Netflix", "Amazon"}; 
double stockPrices[STOCK_COUNT] = {150.0, 200.0, 300.0, 350.0};

// Use of struct to hold user account information
struct UserAccount {
    string username;            //we use strings to store username and pass for better handling instead of char 
    string password;           // User is a single entity with multiple attributes, so struct
                               // helps us to store them in an organized way instead of using separate arrays
    double balance = 1000;
    int shares[STOCK_COUNT] = {0}; // Array to store number of shares for each stock
};
UserAccount users[MAX_USERS];
    int totalUsers = 0;  // Total users start from 0 because it counts how many users exist
int loggedInUserIndex = -1; // No user logged in at start

//file handling functions
void loadUsersFromFile(){
    ifstream file("users.txt");  //function load the user data from fle and check if given attributes are valid and also check the max number of users 
    if (!file) return;
    totalUsers = 0;
    while(file >> users[totalUsers].username 
                >> users[totalUsers].password // loads the userdata from file into users array
                >> users[totalUsers].balance) {
        for(int i = 0; i < STOCK_COUNT; i++)
            file >> users[totalUsers].shares[i]; // read number of shares for each stock
        totalUsers++;
        if(totalUsers >= MAX_USERS) break; // check that users do not exceed max limit
    }
    file.close();
}
void saveUsersToFile(){
    ofstream file("users.txt"); // save user data into file
    for (int i = 0; i < totalUsers; i++) {
        file << users[i].username << " "
             << users[i].password << " "
             << users[i].balance << " "; //this function saves all user data into the file, including the username, password, balance, and the number of shares each user holds for every stock
        for (int j = 0; j < STOCK_COUNT; j++)
            file << users[i].shares[j] << " ";//user owns number of shares for each stock
        file << endl;
    }
    file.close();
}
//Account creation and login
void createNewAccount(){
    if (totalUsers >= MAX_USERS) 
    { cout << "Max accounts reached"<<endl;
 return; }
    string username, password;
    cout << "Enter username (no spaces): ";
     cin >> username;
    for (int i = 0; i < totalUsers; i++)
        if (users[i].username == username)
      { cout << "Username taken"<<endl;
 return; }
    cout << "Enter password (no spaces): "; 
    cin >> password;
    users[totalUsers].username = username; 
    users[totalUsers].password = password;  //We store username and password at the next empty index in the users array
    totalUsers++;
    saveUsersToFile(); // save new user data into file
    cout << "Account created! Starting balance $1000"<<endl;
}
bool loginUser(){
    string u, p; // temporary variables to hold username and password input
    cout << "Enter username: ";
     cin >> u;
    cout << "Enter password: ";
     cin >> p; 
    for (int i = 0; i < totalUsers; i++)
        if (users[i].username == u && users[i].password == p){ 
            loggedInUserIndex = i; // set the logged-in user index
            cout << "Login successful! Welcome, " << u << endl;//if the user credentials match, we set the loggedInUserIndex to the index of that user in the users array
            return true;
        } 
    cout << "Invalid username or password"<<endl; 
    return false;
}
//user portfolio view
void viewPortfolio() {
    UserAccount &u = users[loggedInUserIndex]; // &avoid copying the struct we use a reference to directly access the logged-in user's data
    cout << fixed << setprecision(2); 
    cout << "Balance: $" << u.balance << endl;
    for (int i = 0; i < STOCK_COUNT; i++)
        cout << stockNames[i] << " Shares: " << u.shares[i] 
             << " Price: $" << stockPrices[i] << "\n";
}   
//buying/selling of shares
void buySellShares(bool isBuy) { //bool is a data type that can hold one of two values true or false
    UserAccount &u = users[loggedInUserIndex];//& allows us to access and modify the user’s data directly without copying the struct
    int choice, qty;
    cout << "Select stock:"<<endl;
    for (int i = 0; i < STOCK_COUNT; i++)
        cout << i + 1 << ". " << stockNames[i] << " ($" << stockPrices[i] << ")"<<endl;
    cin >> choice;
    if (choice < 1 || choice > STOCK_COUNT) return;
    int index = choice - 1;
    cout << "Enter quantity: ";
     cin >> qty;
    if (qty <= 0) return;
    double amount = qty * stockPrices[index];
    if (isBuy) {
        if (u.balance >= amount) { u.balance -= amount; u.shares[index] += qty; }
        else { cout << "Not enough balance."<<endl;
             return; 
            }
            cout << "Bought " << qty << " " << stockNames[index] << " shares."<<endl;
    } else {
        if (u.shares[index] >= qty) { u.shares[index] -= qty; u.balance += amount; }
        else { cout << "Not enough shares."<<endl;
             return; }
            cout << "Sold " << qty << " " << stockNames[index] << " shares."<<endl;
    }
    saveUsersToFile();
}

///mainmenu
void showMainMenu() {
    int choice;
    do{
        if(loggedInUserIndex == -1) {
           cout << "\n 1. Create Account\n";
           cout << "2. Login\n";
           cout << "3. Exit\n";
           cout << "Choice: ";
            cin >> choice;
            if (choice == 1) createNewAccount();
            else if (choice == 2) loginUser();
        } else{
            cout << "\n1. View Portfolio\n";
            cout << "2. Buy Shares\n";
            cout << "3. Sell Shares\n";
            cout << "4. Logout\n";
            cout << "Choice: ";

          cin >> choice;
            if (choice == 1) viewPortfolio();
            else if (choice == 2) buySellShares(true);
            else if (choice == 3) buySellShares(false);
            else if (choice == 4) loggedInUserIndex = -1;
        }
    }
         while (!(loggedInUserIndex == -1 && choice == 3));
}
    int main() {    
        int a = 5;              
        int *p = &a;            // pointer declaration & initialization
        int b = *p;             // dereferencing
        int &r = a;             // reference variable,r is another name for a

        double *pa = stockPrices;       // pointer to array (points to first index)
        string *ps = &stockNames[0];    // pointer to string 
        loadUsersFromFile(); 
        showMainMenu();      
        return 0;
    }
