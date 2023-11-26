#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

const int MAX_SIZE = 20;
const string UD_FILENAME = "ud.txt";
const string PBT_FILENAME = "pbt.txt";
const string PVT_FILENAME = "pvt.txt";

void checkDatabase(fstream &, fstream &, fstream &, bool &);
void closeFiles(fstream &, fstream &, fstream &);
void changeFileMode(fstream &, int, string);
void createDatabase(fstream &, fstream &, fstream &, bool &);
void initializeDB(int[], string[], string[], int[]);
void readDatabase(fstream &, fstream &, fstream &);
void userData2Array(fstream &, int[], string[], string[], int[], int &);
void array2UserData(fstream &, int[], string[], string[], int[]);
void login(int[], string[], int[], int &, bool &, int &);
bool sIdExist(int[], int);
bool indexFinder(int[], int, int &);
bool matchUserPass(int[], string[], int[], bool &, int, string, int &);
int string2Int(string);
int adminMenu(int, string[]);
int userMenu(int, string[]);
void addUser(int[], string[], string[], int[], int &);
void editName(string[], int);
void editPassword(string[], int);
void editPermission(string[], int);
void editUser(int[], string[], string[], int[], bool, int, int &);
void addPublicTransaction(fstream &, int[]);
void addPrivateTransaction(fstream &, int[], int);
void viewPublic(fstream &);
void viewPrivate(fstream &, int, bool);
long int totalPublic(fstream &);
void publicBalancePerUser(fstream &, int[], long int[], int);
void privateBalancePerUser(fstream &, int[], long int[], int);
void finalBalance(double[], long int[], long int[], double, int);
void systemReport(int[], string[], fstream &, fstream &, bool, int, int);
void saveAndExit(int[], string[], string[], int[], fstream &, fstream &, fstream &);

int main(){

    fstream userData, publicTransactions, privateTransactions;
    bool newDB;

    checkDatabase(userData, publicTransactions, privateTransactions, newDB);
    closeFiles(userData, publicTransactions, privateTransactions);

    int studentID[MAX_SIZE] = {0}, admin[MAX_SIZE] = {0};
    string name[MAX_SIZE] = {""}, password[MAX_SIZE] = {""};
    int currentUser, userIndex, userQuantity;
    bool isAdmin = false;

    if (newDB){
        initializeDB(studentID, name, password, admin);
        array2UserData(userData, studentID, name, password, admin);
        closeFiles(userData, publicTransactions, privateTransactions);
    }
    userData2Array(userData, studentID, name, password, admin, userQuantity);
    closeFiles(userData, publicTransactions, privateTransactions);
    login(studentID, password, admin, currentUser, isAdmin, userIndex);
    if (isAdmin){
        while (true){
            int choice = adminMenu(userIndex, name);
            switch (choice){
            case 1: 
                addUser(studentID, name, password, admin, userQuantity);
                array2UserData(userData, studentID, name, password, admin);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 2: 
                editUser(studentID, name, password, admin, isAdmin, userIndex, currentUser);
                array2UserData(userData, studentID, name, password, admin);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 3: 
                addPublicTransaction(publicTransactions, studentID);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 4:
                addPrivateTransaction(privateTransactions, studentID, currentUser);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 5:
                viewPublic(publicTransactions);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 6:
                viewPrivate(privateTransactions, currentUser, isAdmin);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 7:
                viewPrivate(privateTransactions, currentUser, false);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 8:
                systemReport(studentID, name, publicTransactions, privateTransactions, isAdmin, userIndex, userQuantity);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 9:
                systemReport(studentID, name, publicTransactions, privateTransactions, false, userIndex, userQuantity);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 10:
                saveAndExit(studentID, name, password, admin, userData, publicTransactions, privateTransactions);
                break;
            default:
                break;
            }
        }
    } else {
        while (true){
            int choice = userMenu(userIndex, name);
            switch (choice){
            case 1: 
                editUser(studentID, name, password, admin, isAdmin, userIndex, currentUser);
                array2UserData(userData, studentID, name, password, admin);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 2:
                addPublicTransaction(publicTransactions, studentID);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 3:
                addPrivateTransaction(privateTransactions, studentID, currentUser);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 4:
                viewPublic(publicTransactions);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 5:
                viewPrivate(privateTransactions, currentUser, isAdmin);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 6:
                systemReport(studentID, name, publicTransactions, privateTransactions, isAdmin, userIndex, userQuantity);
                closeFiles(userData, publicTransactions, privateTransactions);
                break;
            case 7:
                array2UserData(userData, studentID, name, password, admin);
                closeFiles(userData, publicTransactions, privateTransactions);
                exit(0);
                break;
            default:
                break;
            }
        }
    }
}

void checkDatabase(fstream &ud, fstream &pbt, fstream &pvt, bool &newDB){
    changeFileMode(ud, 1, UD_FILENAME);
    changeFileMode(pbt, 1, PBT_FILENAME);
    changeFileMode(pvt, 1, PVT_FILENAME);
    if (! ud || ! pbt || ! pvt){
        cout << "Database does not exist! Creating a new database now." << endl;
        createDatabase(ud, pbt, pvt, newDB);
    } else {
        newDB = false;
    }
}

void closeFiles(fstream &f1, fstream &f2, fstream &f3){
    f1.close();
    f2.close();
    f3.close();
}

void changeFileMode(fstream &file, int mode, string fileName){
    switch (mode)
    {
    case 1:
        file.open(fileName, ios::in);
        break;
    case 2:
        file.open(fileName, ios::out);
        break;
    case 3:
        file.open(fileName, ios::app);
        break;
    case 4: 
        file.close();
        break;
    default:
        break;
    }
}

void createDatabase(fstream &ud, fstream &pbt, fstream &pvt, bool &newDB){
    changeFileMode(ud, 2, UD_FILENAME);
    changeFileMode(pbt, 2, PBT_FILENAME);
    changeFileMode(pvt, 2, PVT_FILENAME);
    if (! ud || ! pbt || ! pvt){
        cout << "Critical Error! Can not create database files!" << endl;
        exit(1);
    } else {
        newDB = true;
    }
}

void initializeDB(int sID[], string name[], string password[], int admin[]){
    int adminID;
    string adminName, adminPass;
    cout << "Initializing database." << endl;
    cout << "Admin account student ID : \t"; 
    cin >> adminID; 
    cout << "Admin account name : \t" << endl;
    cin.ignore();
    getline(cin, adminName);
    cout << "Admin account password : \t" << endl;
    getline(cin, adminPass);
    sID[0] = adminID;
    name[0] = adminName;
    password[0] = adminPass;
    admin[0] = 1;
}

void array2UserData(fstream &ud, int sID[], string name[], string password[], int admin[]){
    changeFileMode(ud, 2, UD_FILENAME);
    for (int i = 0; i < MAX_SIZE; i++){
        ud << sID[i] << ";" << name[i] << ";" << password[i] << ";" << admin[i] << endl;
    }
}

void userData2Array(fstream &ud, int sID[], string name[], string password[], int admin[], int &userQuantity){
    changeFileMode(ud, 1, UD_FILENAME);
    string sIdString, adminString;
    bool reachedLastUser = false;
    for (int i = 0; i < MAX_SIZE; i++){
        getline(ud, sIdString, ';');
        if (sIdString == "0" && reachedLastUser == false){
            userQuantity = i;
            reachedLastUser = true;
        }
        sID[i] = string2Int(sIdString);
        getline(ud, name[i], ';');
        getline(ud, password[i], ';');
        getline(ud, adminString, '\n');
        admin[i] = string2Int(adminString);
    }
}

bool sIdExist(int sID[], int givenID){
    for (int i = 0; i < MAX_SIZE; i++){
        if (sID[i] == givenID){
            return  true;
        }
    }
    return false;
}

bool indexFinder(int sID[], int givenID, int &index){
    for (index = 0; index < MAX_SIZE; index++){
        if (sID[index] == givenID){
            return true;
        }
    }
    return false;
}

bool matchUserPass(int sID[], string password[], int admin[], bool &isAdmin, int givenID, string givenPass, int &userIndex){
    for (userIndex = 0; userIndex < MAX_SIZE; userIndex++){
        if (sID[userIndex] == givenID){
            if (admin[userIndex] == 1){
                isAdmin = true;
            }
            if (password[userIndex] == givenPass){
                return true;
            }
            return false;
        }
    }
    return false;
}

void login(int sID[], string password[], int admin[], int &currentUser, bool &isAdmin, int &userIndex){
    int givenID; 
    string givenPass;
    while(true){
        cout << "Student ID : \t";
        cin >> givenID;
        cout << "Password : \t";
        cin.ignore();
        getline(cin, givenPass);
        if (matchUserPass(sID, password, admin, isAdmin, givenID, givenPass, userIndex)){
            currentUser = givenID;
            cout << "Welcome!" << endl;
            break;
        } else {
            cout << "Wrong credentials!" << endl;
            cout << "1- try again\n2- exit\n";
            int choice; 
            do {
                cin >> choice;
                if (choice == 2){
                    exit(1);
                }
            } while (choice > 2 || choice > 1);
        }
    }
}

int string2Int(string input){
    istringstream toBeConverted(input);
    int result;
    toBeConverted >> result;
    return result;
}

int adminMenu(int userIndex, string name[]){
    int choice;
    do {
        cout << "Hello " << name[userIndex] << "!" << endl;
        cout << "1- Add user\n2- Edit user info\n3- Add public transaction\n4- Add private transaction\n5- View all public transactions\n6- View all private transactions\n7- View all private transactions of yourself\n8- Full system report\n9- User report\n10- exit\noption:\t";
        cin >> choice;
        cin.ignore();
        if (choice > 10 || choice < 1){
            cout << "Invalid input!" << endl;
        }
    } while (choice > 10 || choice < 1);
    return choice;
}

int userMenu(int userIndex, string name[]){
    int choice;
    do {
        cout << "Hello " << name[userIndex] << "!" << endl;
        cout << "1- Edit user info\n2- Add public transaction\n3- Add private transaction\n4- View all public transactions\n5- View all private transactions of yourself\n6- User report\n7- exit\noption:\t";
        cin >> choice;
        cin.ignore();
        if (choice > 7 || choice < 1){
            cout << "Invalid input!" << endl;
        }
    } while (choice > 7 || choice < 1);
    return choice;
}

void viewPublic(fstream &pbt){
    changeFileMode(pbt, 1, PBT_FILENAME);
    cout << "ID\t\tAmount\t\tDate\t\tDescription\n--------------------------------------------------------------------------------" << endl;
    string data;
    while (getline(pbt, data,';')){
        cout << data << "\t\t";
        getline(pbt, data,';');
        cout << data << "\t\t";
        getline(pbt, data,';');
        cout << data << "\t";
        getline(pbt, data,'\n');
        cout << data << endl;
    }
}

void viewPrivate(fstream &pvt, int currentUser, bool isAdmin){
    changeFileMode(pvt, 1, PVT_FILENAME);
    cout << "Spender ID\tReceiver ID\tAmount\t\tDate\t\tDescription\n--------------------------------------------------------------------------------" << endl;
    string data;
    if (isAdmin){
        while (getline(pvt, data,';')){
            cout << data << "\t\t";
            getline(pvt, data,';');
            cout << data << "\t\t";
            getline(pvt, data,';');
            cout << data << "\t\t";
            getline(pvt, data,';');
            cout << data << "\t";
            getline(pvt, data,'\n');
            cout << data << endl;
        }
    } else {
        string id1, id2;
        while (getline(pvt, id1,';')){
            getline(pvt, id2, ';');
            if (! (string2Int(id1) == currentUser || string2Int(id2) == currentUser)){
                getline(pvt, data,';');
                getline(pvt, data,';');
                getline(pvt, data,'\n');
                continue;
            } else {
                cout << id1 << "\t\t" << id2 << "\t\t";
                getline(pvt, data,';');
                cout << data << "\t\t";
                getline(pvt, data,';');
                cout << data << "\t";
                getline(pvt, data,'\n');
                cout << data << endl;
            }
        }
    }
}

void addUser(int sID[], string name[], string password[], int admin[], int &userQuantity){
    if (userQuantity == MAX_SIZE){
        cout << "Sorry you've already reached 20 maximum users!" << endl;
        return;
    }
    int isAdmin = 0;
    char choice;
    int newSid;
    do {
        cout << "Student ID:\t";
        cin >> newSid;
        if (sIdExist(sID, newSid)){
            cout << "Student ID already exists!" << endl;
        }
    } while (sIdExist(sID, newSid));
    cin.ignore();
    sID[userQuantity] = newSid;
    cout << "Name:\t";
    getline(cin, name[userQuantity], '\n');
    cout << "Password:\t";
    getline(cin, password[userQuantity], '\n');
    do {
        cout << "Is the user an admin? (y/n)\t";
        cin >> choice;
        cin.ignore();
        if (! (choice == 'y' || choice == 'n')){
            cout << "Invalid input" << endl;
        }
    } while (! (choice == 'y' || choice == 'n'));
    if (choice == 'y'){
        isAdmin = 1;
    }
    admin[userQuantity] = isAdmin;
    userQuantity++;
    cout << "Done!" << endl;
}

void editName(string name[], int toBeEditedIndex){
    string newName;
    cout << "New name:\t";
    getline(cin, newName, '\n');
    name[toBeEditedIndex] = newName;
    cout << "Done!" << endl;
}

void editPassword(string password[], int toBeEditedIndex){
    string newName;
    cout << "New password:\t";
    getline(cin, newName, '\n');
    password[toBeEditedIndex] = newName;
    cout << "Done!" << endl;
}

void editPermission(int admin[], int toBeEditedIndex){
    char choice;
    do {
        cout << "Is user an admin? (y/n)\t";
        cin >> choice;
        cin.ignore();
        if (! (choice == 'y' || choice == 'n')){
            cout << "Invalid input" << endl;
        }
    } while (! (choice == 'y' || choice == 'n'));
    if (choice == 'y'){
        admin[toBeEditedIndex] = 1;
    } else {
        admin[toBeEditedIndex] = 0;
    }
    cout << "Done" << endl;
}

void editUser(int sID[], string name[], string password[], int admin[], bool isAdmin, int userIndex, int &currentUser){
    if (isAdmin){
        int toBeEditedID;
        do {
            cout << "Which user do you want to edit?\t";
            cin >> toBeEditedID;
            if (! sIdExist(sID, toBeEditedID)){
                cout << "Given student ID does not exist in database!" << endl;
            }
        } while (! sIdExist(sID, toBeEditedID));
        int toBeEditedIndex;
        indexFinder(sID, toBeEditedID, toBeEditedIndex);
        int choice;
        do {
            cout << "1- Edit Name\n2- Edit Password\n3- Edit permissions\noption?\t";
            cin >> choice;
            cin.ignore();
            if (choice > 3 || choice < 1){
                cout << "Invalid input" << endl;
            }
        } while (choice > 3 || choice < 1);
        switch (choice){
            case 1:
                editName(name, toBeEditedIndex);
                break;
            case 2: 
                editPassword(password, toBeEditedIndex);
                break;
            case 3:
                editPermission(admin, toBeEditedIndex);
                break;
            default:
                break;
        }
    } else {
        int choice;
        do {
            cout << "1- Edit Name\n2- Edit Password\noption?\t";
            cin >> choice;
            cin.ignore();
            if (choice > 2 || choice < 1){
                cout << "Invalid input" << endl;
            }
        } while (choice > 2 || choice < 1);
        switch (choice){
            case 1:
                editName(name, userIndex);
                break;
            case 2: 
                editPassword(password, userIndex);
                break;
            default:
                break;
        }
    }
}

void addPublicTransaction(fstream &pbt, int sID[]){
    changeFileMode(pbt, 3, PBT_FILENAME);
    int spender, amount;
    string date, detail;
    do {
        cout << "Spender Student ID:\t";
        cin >> spender;
        if (! sIdExist(sID, spender)){
            cout << "Given student ID does not exist in database!" << endl;
        }
    } while (! sIdExist(sID, spender));
    cout << "Amount spent:\t";
    cin >> amount;
    cin.ignore();
    cout << "Date of the transaction:\t";
    getline(cin, date, '\n');
    cout << "Details of the transaction:\t";
    getline(cin, detail, '\n');
    pbt << spender << ";" << amount << ";" << date << ";" << detail << endl;
    cout << "Done!" << endl;
}

void addPrivateTransaction(fstream &pvt, int sID[], int currentUser){
    changeFileMode(pvt, 3, PVT_FILENAME);
    int spender, receiver, amount;
    string date, detail;
    int mode;
    do {
        cout << "Are you...\n1-Reciever of private transaction.\n2-Spender of private transaction." << endl;
        cin >> mode;
        if (! (mode == 1 || mode == 2)){
            cout << "Invalid input." << endl;
        }
    } while (! (mode == 1 || mode == 2));
    if (mode == 1){
        do {
            cout << "Spender Student ID:\t";
            cin >> spender;
            if (! sIdExist(sID, spender)){
                cout << "Given student ID does not exist in database!" << endl;
            }
        } while (! sIdExist(sID, spender));
        receiver = currentUser;
    } else {
        spender = currentUser;
        do {
            cout << "Reciever Student ID:\t";
            cin >> receiver;
            if (! sIdExist(sID, receiver)){
                cout << "Given student ID does not exist in database!" << endl;
            }
        } while (! sIdExist(sID, receiver));
    }
    cout << "Amount spent:\t";
    cin >> amount;
    cin.ignore();
    cout << "Date of the transaction:\t";
    getline(cin, date, '\n');
    cout << "Details of the transaction:\t";
    getline(cin, detail, '\n');
    pvt << spender << ";" << receiver << ";" << amount << ";" << date << ";" << detail << endl;
    cout << "Done!" << endl;
}

long int totalPublic(fstream &pbt){
    long long int sum = 0;
    string data;
    while (getline(pbt, data, ';')){
        getline(pbt, data, ';');
        sum += string2Int(data);
        getline(pbt, data, ';');
        getline(pbt, data, '\n');
    }
    return sum;
}

void publicBalancePerUser(fstream &pbt, int sID[], long int publicBalance[], int userQuantity){
    pbt.clear();
    pbt.seekg(0, ios::beg);
    for (int i = 0; i < userQuantity; i++){
        int sum = 0;
        string userID;
        while (getline(pbt, userID, ';')){
            string data;
            getline(pbt, data, ';');
            if (sID[i] == string2Int(userID)){
                sum += string2Int(data);
            }
            getline(pbt, data, ';');
            getline(pbt, data, '\n');
        }
        publicBalance[i] = sum;
        pbt.clear();
        pbt.seekg(0, ios::beg);
    }
}

void privateBalancePerUser(fstream &pvt, int sID[], long int privateBalance[], int userQuantity){
    pvt.clear();
    pvt.seekg(0, ios::beg);
    for (int i = 0; i < userQuantity; i++){
        int sum = 0;
        string userID1, userID2;
        while (getline(pvt, userID1, ';')){
            getline(pvt, userID2, ';');
            string data;
            getline(pvt, data, ';');
            if (sID[i] == string2Int(userID1)){
                sum -= string2Int(data);
            } else if (sID[i] == string2Int(userID2)){
                sum += string2Int(data);
            }
            getline(pvt, data, ';');
            getline(pvt, data, '\n');
        }
        privateBalance[i] = sum;
        pvt.clear();
        pvt.seekg(0, ios::beg);
    }
}

void finalBalance(double balance[], long int publicBalance[], long int privateBalance[], double publicShare, int userQuantity){
    for (int i = 0; i < userQuantity; i++){
        balance[i] = publicShare - publicBalance[i] + privateBalance[i];
    }
}

void systemReport(int sID[], string name[], fstream &pbt, fstream &pvt, bool isAdmin, int userIndex, int userQuantity){
    changeFileMode(pbt, 1, PBT_FILENAME);
    changeFileMode(pvt, 1, PVT_FILENAME);
    long int publicBalance[MAX_SIZE] = {0};
    long int privateBalance[MAX_SIZE] = {0};
    double balance[MAX_SIZE] = {0};
    double publicShare = totalPublic(pbt) / (double) userQuantity;
    publicBalancePerUser(pbt, sID, publicBalance, userQuantity);
    privateBalancePerUser(pvt, sID, privateBalance, userQuantity);
    finalBalance(balance, publicBalance, privateBalance, publicShare, userQuantity);
    if (isAdmin){
        for (int i = 0; i < userQuantity; i++){
            cout << sID[i] << "\t" << name[i] << "\t" << balance[i] << endl;
        }
    } else {
        cout << sID[userIndex] << "\t" << name[userIndex] << "\t" << balance[userIndex] << endl;
    }
}

void saveAndExit(int sID[], string name[], string password[], int admin[], fstream &ud, fstream &pbt, fstream &pvt){
    closeFiles(ud, pbt, pvt);
    array2UserData(ud, sID, name, password, admin);
    closeFiles(ud, pbt, pvt);
    exit(0);
}