#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Client.txt";
const string UsersFileName = "Users.txt";

enum enMainMenuePermissions {
    eAll = -1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClients = 8,
    pFindClient = 16,
    pTranactions = 32,
    pManageUsers = 64
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool deletmark = false;
};

struct stUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};


vector<string> SplitString(string S1, string Delim);
void youroption(vector<sClient>& vClients);
vector<sClient> LoadCleintsDataFromFile(string FileName);
string ConvertUserRecordToLine(stUser User, string Seperator = "#//#");
void ShowAllUsersScreen();

stUser CurrentUser;

stUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData = SplitString(Line, Seperator);

    if (vUserData.size() >= 3)
    {
        User.UserName = vUserData[0];
        User.Password = vUserData[1];
        try
        {
            User.Permissions = stoi(vUserData[2]);
        }
        catch (...)
        {
            User.Permissions = 0;
        }
    }
    return User;
}

vector<stUser> LoadUsersDataFromFile(string FileName)
{
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line == "")
                continue;

            stUser User = ConvertUserLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

bool FindUserByUsernameAndPassword(string UserName, string Password, stUser& User)
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUser U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string UserName, string Password)
{
    return FindUserByUsernameAndPassword(UserName, Password, CurrentUser);
}

void Login()
{
    bool LoginFailed = false;
    string Username, Password;

    do
    {
        system("cls");
        cout << "-----------------------------------\n";
        cout << "\tLogin Screen\n";
        cout << "-----------------------------------\n";

        if (LoginFailed)
        {
            cout << "Invalid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFailed = !LoadUserInfo(Username, Password);

    } while (LoginFailed);

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    youroption(vClients);
}

void ShowManageUsersMenuScreen()
{
    system("cls");
    cout << "==================================================\n";
    cout << "\tManage Users Menue Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "==================================================\n";
}

void PrintUserRecord(stUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(12) << left << User.Password;
    cout << "| " << setw(12) << left << User.Permissions;
}

void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUser List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(12) << "Password";
    cout << "| " << left << setw(12) << "Permissions";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
    {
        cout << "\t\t\t\tNo Users Available In the System!";
    }
    else
    {
        for (stUser User : vUsers)
        {
            PrintUserRecord(User);
            cout << endl;
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        return enMainMenuePermissions::eAll;
    }

    cout << "\nDo you want to give access to :\n";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pListClients;

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pAddNewClient;

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pDeleteClient;

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pUpdateClients;

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pFindClient;

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pTranactions;

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
        Permissions += enMainMenuePermissions::pManageUsers;

    return Permissions;
}

bool UserExistsByUsername(string UserName, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line == "") continue;

            stUser User = ConvertUserLinetoRecord(Line);
            if (User.UserName == UserName)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

stUser ReadNewUser()
{
    stUser User;

    cout << "Enter Username? ";
    cin >> User.UserName;

    while (UserExistsByUsername(User.UserName, UsersFileName))
    {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
        cin >> User.UserName;
    }

    cout << "Enter Password? ";
    cin >> User.Password;

    User.Permissions = ReadPermissionsToSet();

    return User;
}

string ConvertUserRecordToLine(stUser User, string Seperator)
{
    string UserRecord = "";
    UserRecord += User.UserName + Seperator;
    UserRecord += User.Password + Seperator;
    UserRecord += to_string(User.Permissions);
    return UserRecord;
}

void AddNewUser()
{
    stUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddUsers()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New User:\n\n";
        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more users? Y / N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void ShowAccessDeniedMessage()
{
    system("cls");
    cout << "\n------------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You dont Have Permission To Do this,\n";
    cout << "Please Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";
}

string ReadUserName()
{
    string UserName = "";
    cout << "\nPlease enter UserName? ";
    cin >> UserName;
    return UserName;
}

bool FindUserByUsername(string UserName, vector<stUser> vUsers, stUser& User)
{
    for (stUser U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}

vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open())
    {
        for (stUser U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUsers;
}

bool MarkUserForDeleteByUsername(string UserName, vector<stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteUserByUsername(string UserName, vector<stUser>& vUsers)
{
    if (UserName == "Admin")
    {
        cout << "\n\nYou cannot Delete This User!";
        return false;
    }

    stUser User;
    char Answer = 'n';

    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkUserForDeleteByUsername(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }

    return false;
}

stUser ChangeUserRecord(string UserName)
{
    stUser User;
    User.UserName = UserName;

    cout << "\n\nEnter Password? ";
    cin >> User.Password;

    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool UpdateUserByUsername(string UserName, vector<stUser>& vUsers)
{
    stUser User;
    char Answer = 'n';

    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\nAre you sure you want to update this User? y/n? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with UserName [" << UserName << "] is Not Found!";
        return false;
    }

    return false;
}

void finduser()
{
    system("cls");
    cout << "-----------------------------------\n";
    cout << "\tFind User Screen\n";
    cout << "-----------------------------------\n";
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    stUser User;
    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "\nUser with UserName [" << UserName << "] is Not Found!\n";
    }
}

void ManageUsers()
{
    short Option = 0;

    do
    {
        ShowManageUsersMenuScreen();

        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> Option;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            Option = 0;
        }

        switch (Option)
        {
        case 1:
            system("cls");
            ShowAllUsersScreen();
            break;

        case 2:
            AddUsers();
            break;

        case 3:
        {
            system("cls");
            cout << "-----------------------------------\n";
            cout << "\tDelete User Screen\n";
            cout << "-----------------------------------\n";
            vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
            string UserName = ReadUserName();
            DeleteUserByUsername(UserName, vUsers);
            break;
        }
        case 4:
        {
            system("cls");
            cout << "-----------------------------------\n";
            cout << "\tUpdate User Screen\n";
            cout << "-----------------------------------\n";

            vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
            string UserName = ReadUserName();
            UpdateUserByUsername(UserName, vUsers);
            break;
        }

        case 5:
            finduser();
            break;

        case 6:
            break;

        default:
            cout << "\nWrong Option! Please choose from 1 to 6.\n";
        }

        if (Option != 6)
        {
            cout << "\n\nPress any key to go back to Manage Users Menu...";
            system("pause>0");
        }

    } while (Option != 6);
}

void ShowMainMenuScreen()
{
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage users\n";
    cout << "\t[8] log out.\n";
    cout << "==================================================\n";
}

void ShowTransactionsMenuScreen()
{
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "==================================================\n";
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddClients()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y / N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        vString.push_back(sWord);
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    if (vClientData.size() >= 5)
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }
    return Client;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            if (Line == "")
            {
                continue;
            }

            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool PrintClientRecord(string AccountNumber, vector<sClient> vClients, sClient& FoundClient)
{
    for (sClient Client : vClients) {
        if (Client.AccountNumber == AccountNumber)
        {
            FoundClient = Client;
            return true;
        }
    }
    return false;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& c : vClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            c.deletmark = true;
            return true;
        }
    }
    return false;
}

vector<sClient> saveclientdatatofile(string FileName, vector<sClient> vClient)
{
    fstream myfile;
    myfile.open(FileName, ios::out);
    string dataline;
    if (myfile.is_open())
    {
        for (sClient c : vClient)
        {
            if (c.deletmark == false) {
                dataline = ConvertRecordToLine(c);
                myfile << dataline << endl;
            }
        }
        myfile.close();
    }
    return vClient;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            saveclientdatatofile(ClientsFileName, vClients);

            vClients = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

sClient changeclientrecord(string AccountNumber) {
    sClient Client;
    Client.AccountNumber = AccountNumber;
    cout << "\n\n\n enter pincode ";
    getline(cin >> ws, Client.PinCode);
    cout << "enter name  ";
    getline(cin, Client.Name);
    cout << "enter your phone ";
    getline(cin, Client.Phone);
    cout << "enter Account Balance ";
    cin >> Client.AccountBalance;
    return Client;
}

bool updateclient(vector<sClient>& vClients, string AccountNumber) {
    sClient Client;
    char answer = 'n';
    if (PrintClientRecord(AccountNumber, vClients, Client)) {
        PrintAllClientsData(vClients);
        cout << "\n are you sure you want update this client y / n? ";
        cin >> answer;
        if (tolower(answer) == 'y')
        {
            for (sClient& c : vClients)
            {
                if (c.AccountNumber == AccountNumber)
                {
                    c = changeclientrecord(AccountNumber);
                    break;
                }
            }
            saveclientdatatofile(ClientsFileName, vClients);
            cout << "\n\n\nupdate successfully";
            return true;
        }
    }
    else { cout << "\nI do not found it \n"; }
    return false;
}

void ShowTotalBalancesScreen(vector<sClient> vClients)
{
    system("cls");
    cout << "\n\t\tBalances List (" << vClients.size() << ") Client(s).\n";
    cout << "_____________________________________________________________________\n";
    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(25) << left << "Client Name";
    cout << "| " << setw(12) << left << "Balance";
    cout << "\n_____________________________________________________________________\n";
    double TotalBalances = 0;
    for (sClient Client : vClients)
    {
        cout << "| " << setw(15) << left << Client.AccountNumber;
        cout << "| " << setw(25) << left << Client.Name;
        cout << "| " << setw(12) << left << Client.AccountBalance << endl;

        TotalBalances += Client.AccountBalance;
    }
    cout << "_____________________________________________________________________\n";
    cout << "\n\t\t Total Balances = " << TotalBalances << endl;
}

void withdraw(vector<sClient>& vClients)
{
    system("cls");
    cout << "-----------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "-----------------------------------\n";
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double withdrawAmount = 0;
    cout << "Please enter withdraw amount? \n";
    cin >> withdrawAmount;
    char Answer = 'n';
    cout << "\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance -= withdrawAmount;
                Client = C;
                break;
            }
        }
        saveclientdatatofile(ClientsFileName, vClients);
        cout << "\nDone Successfully. New balance is: " << Client.AccountBalance << endl;
    }
}

void deposet(vector<sClient>& vClients)
{
    system("cls");
    cout << "-----------------------------------\n";
    cout << "\tDeposit Screen\n";
    cout << "-----------------------------------\n";
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double DepositAmount = 0;
    cout << "please deposite amount! \n";
    cin >> DepositAmount;
    char Answer = 'n';
    cout << "\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += DepositAmount;
                Client = C;
                break;
            }
        }
        saveclientdatatofile(ClientsFileName, vClients);
        cout << "\nDone Successfully. New balance is: " << Client.AccountBalance << endl;
    }
}

void trans(vector<sClient>& vClients) {
    short option = 0;

    do
    {
        ShowTransactionsMenuScreen();

        cout << "Choose what do you want to do? [1 to 4]? ";
        cin >> option;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            option = 0;
        }

        switch (option)
        {
        case 1:
            deposet(vClients);
            break;
        case 2:
            withdraw(vClients);
            break;
        case 3:
            ShowTotalBalancesScreen(vClients);
            break;
        case 4:
            break;
        default:
            cout << "\nWrong Option! Please choose from 1 to 4.\n";
        }

        if (option != 4)
        {
            cout << "\n\nPress any key to go back to Transactions Menu...";
            system("pause>0");
        }

    } while (option != 4);
}

void youroption(vector<sClient>& vClients)
{
    short option = 0;
    string AccountNumber = "";
    sClient client;

    do
    {
        ShowMainMenuScreen();

        cout << "Choose what do you want to do? [1 to 8]? ";
        cin >> option;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            option = 0;
        }

        switch (option)
        {
        case 1:
            if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
            {
                ShowAccessDeniedMessage();
                break;
            }
            PrintAllClientsData(vClients);
            break;

        case 2:
            if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
            {
                ShowAccessDeniedMessage();
                break;
            }
            AddClients();
            vClients = LoadCleintsDataFromFile(ClientsFileName);
            break;

        case 3:
            if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
            {
                ShowAccessDeniedMessage();
                break;
            }
            AccountNumber = ReadClientAccountNumber();
            DeleteClientByAccountNumber(AccountNumber, vClients);
            break;

        case 4:
            if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
            {
                ShowAccessDeniedMessage();
                break;
            }
            AccountNumber = ReadClientAccountNumber();
            updateclient(vClients, AccountNumber);
            break;

        case 5:
            if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
            {
                ShowAccessDeniedMessage();
                break;
            }
            AccountNumber = ReadClientAccountNumber();
            if (FindClientByAccountNumber(AccountNumber, vClients, client))
                PrintClientCard(client);
            else
                cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
            break;

        case 6:
            if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
            {
                ShowAccessDeniedMessage();
                break;
            }
            trans(vClients);
            break;

        case 7:
            if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
            {
                ShowAccessDeniedMessage();
                break;
            }
            ManageUsers();
            break;

        case 8:
            break;

        default:
            cout << "\nWrong Option! Please choose from 1 to 8.\n";
            break;
        }

        if (option != 8)
        {
            cout << "\n\nPress any key to go back to Main Menu...";
            system("pause>0");
        }

    } while (option != 8);
}

int main()
{
    while (true)
    {
        Login();
    }

    return 0;
}