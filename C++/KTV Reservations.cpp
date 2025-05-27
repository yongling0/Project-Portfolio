#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;

#include <vector>
using std::vector;

struct Date
{
    int year;
    int month;
    int day;
    int hour;
};

struct MemberRecord
{
    char idNumber[12];   // account number
    char password[24];   // password
    char name[8];        // name
};

struct ReservationRecord
{
    char idNumber[12]; // account number
    int branchCode;      // branch code
    Date date;           // reservation date
    int numCustomers;    // number of customers
};

char branchNames[19][24] = { "", "Taipei Dunhua South",   "Taipei Zhongxiao",
                                     "Taipei Songjiang",    "Taipei Nanjing",
                                     "Taipei Linsen",       "Taipei Zhonghua New",
                                     "Banqiao Guanqian",    "Yonghe Lehua",
                                     "Taoyuan Zhonghua",    "Taoyuan Nankan",
                                     "Zhongli Zhongyang",   "Hsinchu Beida",
                                     "Taichung Ziyou",      "Chiayi Ren'ai",
                                     "Tainan Ximen",        "Kaohsiung Zhonghua New",
                                     "Kaohsiung Jianxing",  "Pingtung Kending" };

// read all memberDetails from the file Members.dat
void loadMemberDetails(vector< MemberRecord >& memberDetails);

// read all reservations that are not out of date, from the file Reservations.dat
void loadReservations(vector< ReservationRecord >& reservations);

// compute the current date
Date compCurrentDate();

// return true if and only if date1 <= date2
bool lessEqual(const Date& date1, const Date& date2);

// input an integer in the range [ begin, end ]
int inputAnInteger(int begin, int end);

// login and display the submenu
void login(const vector< MemberRecord >& memberDetails,
    vector< ReservationRecord >& reservations);

// there exists a member with specified idNumber and password
bool valid(char idNumber[], char password[],
    const vector< MemberRecord >& memberDetails);

// add a new reservation for the member with specified IDnumber
void reservation(char idNumber[], vector< ReservationRecord >& reservations);

// compute 7 dates which is starting from the current date
void compAvailableDates(Date availableDates[]);

// display all fields of reservation
void output(ReservationRecord reservation);

// display all reservations for the member with specified IDnumber,
// then let the member to choose one of them to delete
void queryDelete(char idNumber[], vector< ReservationRecord >& reservations);

// add a new member
void registration(vector< MemberRecord >& memberDetails);

// return true if idNumber is a legal ID number
//bool legalID( char idNumber[] );

// return true if idNumber belongs to memberDetails
bool existingID(char idNumber[], const vector< MemberRecord >& memberDetails);

// write all memberDetails into the file Members.dat
void saveMemberDetails(const vector< MemberRecord >& memberDetails);

// write all reservations that are not out of date, into the file Reservations.dat
void saveReservations(const vector< ReservationRecord >& reservations);

bool leapyear(int year);

int main()
{
    vector< MemberRecord > memberDetails; // member details for all members
    vector< ReservationRecord > reservations; // all reservations

    loadMemberDetails(memberDetails);
    loadReservations(reservations);
    cout << "Welcome to the Cashbox Party World!\n\n";
    int choice;
    while (true)
    {
        cout << "1 - Login\n";
        cout << "2 - Registration\n";
        cout << "3 - End\n";

        do cout << "\nEnter your choice (1~3): ";
        while ((choice = inputAnInteger(1, 3)) == -1);
        cout << endl;

        switch (choice)
        {
        case 1:
            login(memberDetails, reservations);
            break;

        case 2:
            registration(memberDetails);
            break;

        case 3:
            saveMemberDetails(memberDetails);
            saveReservations(reservations);
            cout << "Thank you! Goodbye!\n\n";
            system("pause");
            return 0;

        default:
            cout << "\nIncorrect choice!\n";
            break;
        }
    }
    system("pause");
}

void loadMemberDetails(vector< MemberRecord >& memberDetails)
{
    MemberRecord memberdata;
    ifstream inFile("Members.dat", ios::in | ios::binary);
    inFile.seekg(0);
    while (inFile.read(reinterpret_cast<char*>(&memberdata), sizeof(MemberRecord))) {
        memberDetails.push_back(memberdata);
    }
    inFile.clear();
    inFile.close();
}

void loadReservations(vector< ReservationRecord >& reservations)
{
    ReservationRecord reservationsdata;
    ifstream inFile("Reservations.dat", ios::in | ios::binary);
    inFile.seekg(0);
    while (inFile.read(reinterpret_cast<char*>(&reservationsdata), sizeof(ReservationRecord))) {
        reservations.push_back(reservationsdata);
    }
    inFile.clear();
    inFile.close();
}

Date compCurrentDate()
{
    Date currentDate;
    tm structuredTime;
    time_t rawTime = time(0);
    localtime_s(&structuredTime, &rawTime);

    currentDate.year = structuredTime.tm_year + 1900;
    currentDate.month = structuredTime.tm_mon + 1;
    currentDate.day = structuredTime.tm_mday;
    currentDate.hour = structuredTime.tm_hour;

    return currentDate;
}

bool lessEqual(const Date& date1, const Date& date2)
{
    int date1days = 0;
    int date2days = 0;
    int differiencedasys;
    int lastdate[13] = { 0,31,0,31,30,31,30,31,31,30,31,30,31 };
    if (date1.year != date2.year) {
        if (leapyear(date2.year)) {
            date2days += 366;
            lastdate[2] = 29;
            for (int i = 1; i < date1.month; i++)
                date1days += lastdate[i];
            date1days += date1.day;
            for (int i = 1; i < date2.month; i++)
                date2days += lastdate[i];
            date2days += date2.day;
        }
        else {
            date2days += 0;//modify
            lastdate[2] = 28;
            for (int i = 1; i < date1.month; i++)
                date1days += lastdate[i];
            date1days += date1.day;
            for (int i = 1; i < date2.month; i++)
                date2days += lastdate[i];
            date2days += date2.day;
        }
    }
    else {
        if (leapyear(date2.year)) {
            date2days += 366;
            lastdate[2] = 29;
            for (int i = 1; i < date1.month; i++)
                date1days += lastdate[i];
            date1days += date1.day;
            for (int i = 1; i < date2.month; i++)
                date2days += lastdate[i];
            date2days += date2.day;
        }
        else {
            date2days += 0;//modify
            lastdate[2] = 28;
            for (int i = 1; i < date1.month; i++)
                date1days += lastdate[i];
            date1days += date1.day;
            for (int i = 1; i < date2.month; i++)
                date2days += lastdate[i];
            date2days += date2.day;
        }
    }
    differiencedasys = date2days - date1days;
    if (differiencedasys >= 0)
        return true;
    else 
        return false;
}

int inputAnInteger(int begin, int end)
{
    char string[80];
    cin.getline(string, 80, '\n');

    if (strlen(string) == 0)
        exit(0);

    for (unsigned int i = 0; i < strlen(string); i++)
        if (string[i] < '0' || string[i] > '9')
            return -1;

    int number = atoi(string);
    if (number >= begin && number <= end)
        return number;
    else
        return -1;
}

void login(const vector< MemberRecord >& memberDetails,
    vector< ReservationRecord >& reservations)
{
    char idNumber[12] = "";
    char password[24] = "";

    do {
        cout << "Please enter your ID number: ";
        cin >> idNumber;
        cout << "Enter your password: ";
        cin >> password;
    } while (!valid(idNumber, password, memberDetails));

    cin.ignore();

    int choice;

    while (true)
    {
        cout << "\n1 - Make Reservation\n";
        cout << "2 - Reservation Enquiry/Canceling\n";
        cout << "3 - End\n";

        do cout << "\nEnter your choice (1~3): ";
        while ((choice = inputAnInteger(1, 3)) == -1);
        cout << endl;
        switch (choice)
        {
        case 1:
            reservation(idNumber, reservations);
            break;

        case 2:
            queryDelete(idNumber, reservations);
            break;

        case 3:
            return;

        default:
            cout << "\nIncorrect choice!\n";
            break;
        }
    }
}

bool valid(char idNumber[], char password[], const vector< MemberRecord >& memberDetails)
{
    for (int i = 0; i < memberDetails.size(); i++) {
        if (strcmp(idNumber, memberDetails[i].idNumber) == 0 && strcmp(password, memberDetails[i].password) == 0)
            return true;
    }
    cout << "Invalid account number or password. Please try again." << endl;
    return false;
}

void reservation(char idNumber[], vector< ReservationRecord >& reservations)
{
    ReservationRecord newReservation;

    strcpy_s(newReservation.idNumber, 12, idNumber);
    for (int i = 1; i < 19; i++)
        cout << setw(2) << i << ". " << branchNames[i] << endl;
    do {
        cout << "Enter your choice ( 0 to end ): ";
        cin >> newReservation.branchCode;
        if (newReservation.branchCode == 0) {
            cin.ignore();//add
            return;
        }
    } while (newReservation.branchCode < 0 || newReservation.branchCode > 18);

    cout << "The current hour is " << compCurrentDate().year << "/" << compCurrentDate().month << "/" <<
        compCurrentDate().day << ":" << compCurrentDate().hour << endl;

    tm structuredTime;
    time_t rawTime = time(0);
    localtime_s(&structuredTime, &rawTime);
    int year = structuredTime.tm_year + 1900;
    int month = structuredTime.tm_mon + 1;
    int day = structuredTime.tm_mday;
    int hour = structuredTime.tm_hour;
    int num;
    cout << "Available days:" << endl;
    if (hour == 23) day++;//add
    for (int i = 1; i < 8; i++) {
        if (day > 31) { day = 1; month = 1; year++; }//add
        cout << setw(2) << i << ". " << year << "/" << month << "/" << day << endl;
        day++;
    }
    do {
        cout << "Enter your choice ( 0 to end ): ";
        cin >> num;
        if (num == 0) {
            cin.ignore();//add
            return;
        }
    } while (num < 0 || num > 7);
    newReservation.date.year = structuredTime.tm_year + 1900;
    newReservation.date.month = structuredTime.tm_mon + 1;//modify
    newReservation.date.day = structuredTime.tm_mday + num - 1;
    if (newReservation.date.day > 31) { newReservation.date.day = 1; newReservation.date.month = 1; newReservation.date.year++; }//add
    bool displace = false;
    do {
        if (num == 1) {

            cout << "Enter hour ( " << hour + 1 << " ~ 23 ) : ";//modify
        }
        else
            cout << "Enter hour (0 ~ 23): ";
        cin >> newReservation.date.hour;
        if (num == 1) {
            if (newReservation.date.hour > hour && newReservation.date.hour <= 23)//modify
                displace = true;
        }
        else {
            if (newReservation.date.hour >= 1 && newReservation.date.hour <= 23)
                displace = true;
        }
    } while (!displace);

    do {
        cout << "Enter the number of customers (1~30, 0 to end): ";
        cin >> newReservation.numCustomers;
        if (newReservation.numCustomers == 0) {
            cin.ignore();//add
            return;
        }
    } while (newReservation.numCustomers < 0 || newReservation.numCustomers > 30);

    cout << endl << setw(26) << "Branch"
        << setw(14) << "Date" << setw(8) << "Hour"
        << setw(19) << "No of Customers" << endl;

    output(newReservation);

    cout << "\nReservation Completed!\n";
    reservations.push_back(newReservation);
    cin.ignore();//add
}

void compAvailableDates(Date availableDates[])
{
    Date currentDate = compCurrentDate();
    for (int i = 1; i <= 7; i++) {
        availableDates[i].year = currentDate.year;
        availableDates[i].month = currentDate.month;
        if (currentDate.month == 12) {
            currentDate.month = 1;
        }
        availableDates[i].day = currentDate.day;
        currentDate.day++;
    }
}

void output(ReservationRecord reservation)
{
    cout << setw(26) << branchNames[reservation.branchCode]
        << setw(8) << reservation.date.year << '-'
        << setw(2) << setfill('0') << reservation.date.month << '-'
        << setw(2) << setfill('0') << reservation.date.day
        << setw(8) << setfill(' ') << reservation.date.hour
        << setw(19) << reservation.numCustomers << endl;
}

void queryDelete(char idNumber[], vector< ReservationRecord >& reservations)
{
    Date currentDate = compCurrentDate();
    int num = 0;//modify
    bool same = false;//add
    if (reservations.size() == 0) { cout << "No reservations!" << endl; return; }//add
    for (int i = 0; i < reservations.size(); i++) {//modify
        if (strcmp(idNumber, reservations[i].idNumber) == 0 && lessEqual(currentDate, reservations[i].date)) {//modify
            /*cout << "No reservations!" << endl;*///delete
            same = true;//add
            /*return;*///delete
        }
    }
    if(!same){ cout << "No reservations!" << endl;  return;}//add
    cout << endl << setw(26) << "Branch" << setw(14) << "Date" << setw(8) << "Hour" << setw(19) << "No of Customers" << endl;//add
    for (int i = 0; i < reservations.size(); i++) {//modify
        if (lessEqual(currentDate, reservations[i].date)) {
            if (strcmp(idNumber, reservations[i].idNumber) == 0) {
                num++;//add
                cout << setw(8) << num << ". ";
                output(reservations[i]);
                cout << endl;
            }
            //delete
        }
    }
    int choice;

    do {
        cout << "Choose a reservation to cancel (0: keep all reservations):";
        cin >> choice;
        if (choice == 0) {
            cin.ignore();//add
            return;
        }
    } while (choice > num);

    ofstream outFile("Reservations.dat", ios::out | ios::binary);
    outFile.seekp(0);
    int j = 0;
    for (int i = 0; i < reservations.size(); i++) {
        if (strcmp(idNumber, reservations[i].idNumber) == 0) {
            j++;
        }
        if (j == choice) {
            reservations.erase(reservations.begin() + i);//add
            continue;
        }
        outFile.write(reinterpret_cast<const char*>(&reservations[i]), sizeof(ReservationRecord));
    }
    outFile.close();
    cin.ignore();//add
}

void registration(vector< MemberRecord >& memberDetails)
{
    MemberRecord newMember;

    cout << "Input your ID Number: ";
    cin >> newMember.idNumber;
    cin.ignore();

    if (existingID(newMember.idNumber, memberDetails))
    {
        cout << "\nYou are already a member!\n\n";
        return;
    }

    cout << "Input your Name: ";
    cin >> newMember.name;

    cout << "Choose a password: ";
    cin >> newMember.password;

    cin.ignore();

    memberDetails.push_back(newMember);

    cout << "\nRegistration Completed!\n\n";
}

bool existingID(char idNumber[], const vector< MemberRecord >& memberDetails)
{
    for (int i = 0; i < memberDetails.size(); i++)
        if (strcmp(idNumber, memberDetails[i].idNumber) == 0)
            return true;
    return false;
}

void saveMemberDetails(const vector< MemberRecord >& memberDetails)
{
    ofstream outFile("Members.dat", ios::out | ios::app | ios::binary);
    for (int i = 0; i < memberDetails.size(); i++) {//add
        outFile.write(reinterpret_cast<const char*>(&memberDetails[i]), sizeof(MemberRecord)); }
    outFile.close();
}

void saveReservations(const vector< ReservationRecord >& reservations)
{
    Date currentDate = compCurrentDate();
    ofstream outFile("Reservations.dat", ios::out | ios::binary);
    for (int i = 0; i < reservations.size(); i++) {
        if (lessEqual(currentDate, reservations[i].date)) {
            outFile.write(reinterpret_cast<const char*>(&reservations[i]), sizeof(ReservationRecord)); //modify
        }
    }
    outFile.close();
}

bool leapyear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    else
        return false;
}