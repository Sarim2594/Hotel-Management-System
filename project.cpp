#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<conio.h>
#include<chrono>
#include<thread>
#include<windows.h>
using namespace std;
class Reservation;
class RevenueFile
{
    string filename="Revenue.txt";
public:
    RevenueFile() 
    {
        fstream f(filename);
        try {if(!f.is_open()) throw "Error opening file: Revenue.txt";}
        catch(const char* error) 
        {
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        f.close();
    }
    void readRevenueFile(unsigned int *maintenance, unsigned int *totalSalary, unsigned int *totalIncome)
    {
        ifstream f(filename);
        string line;
        getline(f, line);
        istringstream iss(line);
        string token;
        getline(iss, token, ':');
        *maintenance = stoi(token);
        getline(iss, token, ':');
        *totalSalary = stoi(token);
        getline(iss, token, ':');
        *totalIncome = stoi(token);
        f.close();
    }
    void updateRevenueFile(unsigned int *maintenance, unsigned int *totalSalary, unsigned int *totalIncome)
    {
        ofstream f(filename);
        f<<*maintenance<<":"<<*totalSalary<<":"<<*totalIncome<<":"<<endl;
        f.close();
    }
};
class Date 
{
private:
    int day;
    int month;
    int year;
public:
    Date(){}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    int getDay() {return day;}
    int getMonth() {return month;}
    int getYear() {return year;}
    bool operator<(const Date& other) const 
    {
        if(year<other.year) return true;
        if(year>other.year) return false;
        if(month<other.month) return true;
        if(month>other.month) return false;
        return day<other.day;
    }
    bool operator==(const Date& other) const 
    {
        return (day==other.day)&&(month==other.month)&&(year==other.year);
    }
    bool operator!=(const Date& other) const 
    {
        return !(*this==other);
    }
    bool operator>(const Date& other) const 
    {
        return !(*this<other||*this==other);
    }
    int operator-(const Date& other) const 
    {
        int thisDays=day+month*30+year*365;
        int otherDays=other.day+other.month*30+other.year*365;
        return thisDays-otherDays;
    }
    bool datedorrect()
    {
        switch(month) 
        {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                if(day>31||day<0) return false; break;
            case 4: case 6: case 9: case 11:
                if(day>30||day<0) return false; break;
            case 2:
                if(year%4==0) if(day>29||day<0) return false;
                else if(day>28||day<0) return false; break;
            default:
                return false;
        } 
        return true;
    }
};
Date stodate(const string& date)
{
    int day, month, year;
    sscanf(date.c_str(), "%d-%d-%d", &day, &month, &year);
    return Date(day, month, year);
}
class HotelManagementSystem
{
protected:
    const string reservationFile="Reservations.txt";
    const string revenueFile="Revenue.txt";
    const string ratesFile="RoomRates.txt";
    const string salaryFile="Salary.txt";
    // int virtual get(const string& room)=0; //Overriden in rates class
    // void virtual display()=0;//overriden in rates and reservation class
    // bool virtual roomCorrect(const string& a)=0;//overriden in reservation class
    // bool virtual roomAvailable(const string& room, Date checkIn, Date checkOut)=0;//overriden in reservation class
    // bool virtual formatCorrect(const string &date)=0;//overriden in reservation class
    // bool virtual bookingCorrect(const string& room, const Date& checkIn, const Date& checkOut)=0;//overriden in reservation class
    // void virtual generateReceipt(const string& room, int stay, const string& name, const string& contact, const string& date1, const string& date2)=0;//overriden in reservation class
    // bool virtual numberCheck(const string& number)=0;//overriden in reservation class
    // void virtual addRateToTotal(unsigned int num)=0;//overriden in reservation class
    // void virtual add()=0;//overriden in reservation class
    // void virtual searchByName(string name)=0;//overriden in reservation class
    // void virtual searchByRoom(string room)=0;//overriden in reservation class
    // void virtual maintenanceCost()=0;//overriden in admin class
    // void virtual staffSalary()=0;//overriden in admin class
    // void virtual updateRate()=0;//overriden in admin class
    // void virtual annualIncome()=0;//overriden in admin class
    // void virtual menu()=0;//overriden in admin class
};
class Rates:public HotelManagementSystem
{
    vector<string> rooms;
    vector<int> rates;
public:
    Rates(){}
    int get(const string& room)
    {
        string line, temp;
        int rate;
        ifstream file(ratesFile);
        if(file.is_open()) 
        {
            while(getline(file, line))
            {
                stringstream ss(line);
                getline(ss, temp, ':'); 
                ss>>rate;    
                if(ss.fail()) throw "Parsing failed. Invalid input format.";
                rates.push_back(rate);  
                rooms.push_back(temp);  
            }
            file.close();
        }
        else 
        {
            throw "Unable to open file: RoomRates.txt";
            return -1;
        }
        for (int i=0; i<rooms.size(); i++)
        {
            if(rooms[i]==room) return rates[i];
        }
        throw "Room not found";
        return -1;
    }
    void display() 
    {
        string line, temp;
        int rate;
        ifstream file(ratesFile);
        if(file.is_open()) 
        {
            while(getline(file, line))
            {
                stringstream ss(line);
                getline(ss, temp, ':'); 
                ss>>rate;    
                if(ss.fail()) 
                {
                    throw "Parsing failed. Invalid input format.";
                    continue; 
                }
                cout<<"Rate of room "<<temp<<": "<<rate<<endl;
            }
            file.close();
        }
        else throw "Unable to open file: RoomRates.txt";
    }
    ~Rates(){}
};
class Reservation:public HotelManagementSystem
{
protected:
    bool roomCorrect(const string& a)
    {
        string room, line;
        ifstream file(ratesFile);
        if(file.is_open()) 
        {
            while(getline(file, line))
            {
                stringstream ss(line);
                getline(ss, room, ':');
                if(room==a)
                {
                    file.close();
                    return true;
                }
            }
            file.close();
            return false;
        }
        else 
        {
            throw "Unable to open file: Reservations.txt";
            return false;
        }
    }
    bool roomAvailable(const string& room, Date checkIn, Date checkOut)
    {
        string line, a, b, c, d;
        fstream file(reservationFile);
        if(file.is_open())
        {
            while(getline(file, line))
            {
                stringstream ss(line);
                getline(ss, a, ':');
                getline(ss, b, ':');
                getline(ss, c, ':');
                getline(ss, d, ':');
                if(a==room)
                {
                    Date datec=stodate(c);
                    Date dated=stodate(d);
                    if(checkOut>datec&&checkOut<dated) return false;
                    if(checkIn==datec) return false;
                    if(checkIn>datec&&checkIn<dated) return false;
                    if(checkOut==dated) return false;
                    if(checkIn<datec&&checkOut>dated) return false;
                }
            }
            return true;
        }
        else 
        {
            throw "Unable to open file: Reservations.txt";
            return false;
        }
    }
    bool formatCorrect(const string &date) 
    {
    if(date.length()!=10) return false;
    if(date[2]!='-'||date[5]!='-') return false;
    for(int i=0; i<10; i++) if(i!=2&&i!=5&&(date[i]<'0'||date[i]>'9')) return false;
    return true;
    }
    bool bookingCorrect(const string& room, const Date& checkIn, const Date& checkOut)
    {
        if(checkOut<checkIn)
        {
            throw "Please re-book a reservation with a valid date where Check-Out date is later than the Check-In date.";
            return false;
        }
		if(checkOut-checkIn>50) 
        {
			throw "Hotel policy: Stay cannot be more than 50 days at a time.\nIf you want to stay for more than 50 days then you will have to book multiple reservations of less than 50 days.";
			return false;
		}
        if(!roomAvailable(room, checkIn, checkOut)) 
        {
            cout<<"Room "<<room<<" is not available for the selected dates."<<endl;
            return false;
        }
        return true;
    }
    void generateReceipt(const string& room, int stay, const string& name, const string& contact, const string& date1, const string& date2)
    {
        Rates r;
        try
        {
            int discount=0;
            switch(room[0]) 
            {
                case 'A':
                    if(stay>5) discount=4000;
                    break;
                case 'B':
                    if(stay>5) discount=2500;
                    break;
                case 'C':
                    if(stay>5) discount=1500;
                    break;
                case 'D':
                    if(stay>5) discount=1000;
                    break;
            }
            int total=stay*r.get(room);
            unsigned int final=total-discount;
            addRateToTotal(final);
            ofstream file("receipt.txt");
            if(!file.is_open()) 
            {
                throw "Error generating a receipt. Cannot open file for writing.";
                return;
            }
            file<<"\n\t\tFAST NUCES HOTEL\n";
            file<<"______________________________________________________\n";
            file<<"\t\tContact: 1234-5678910\n";
            file<<"\tShah Latif Town, National Highway(N5)\n";
            file<<"------------------------------------------------------\n";
            file<<"Check in: "<<date1<<"\t|\tCheck out: "<<date2<<endl;
            file<<"Name: "<<name<<"\t\t|\tPhone: "<<contact<<endl;
            file<<"______________________________________________________\n\n";
            file<<"\t\tRESERVATION RECEIPT\n";
            file<<"______________________________________________________\n";
            file<<"Room Number\tDays Booked\tPrice per night\n";
            file<<"------------------------------------------------------\n";
            file<<"   "<<room<<"\t\t     "<<stay<<"\t\t  Rs."<<r.get(room)<<endl;
            file<<"------------------------------------------------------\n";
            file<<"Total\t\t\tRs."<<total<<endl;
            file<<"Discount\t\tRs."<<discount<<endl;
            file<<"Grand Total\t\tRs."<<final<<endl;
            file<<"______________________________________________________\n";
            file<<"\t\tReceipt design by Sarim\n";
            file.close();
            system("start notepad receipt.txt");
        }
        catch(const char* error) 
        {
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    bool numberCheck(const string& number)
    {
        for(int i=0; i<number.length(); i++)
            if(number[i]<'0'||number[i]>'9') return false;
        return true;
    }
    void addRateToTotal(unsigned int num)
    {
        unsigned int a=0, b=0, c=0;
        RevenueFile r;
        r.readRevenueFile(&a, &b, &c);
        c+=num;
        r.updateRevenueFile(&a, &b, &c);
    }
public:
    Reservation(){};
    void add()
    {
        string roomNum, guestName, tempIn, tempOut, contact, account;
        Date checkIn, checkOut;
        int day, month, year;
        try{
            cout<<"Enter the details. Enter -1 anytime to go back to the main menu without completing the reservation.\n";
            do
            {    
                cout<<"Enter your room number: ";
                cin.ignore();
                getline(cin, roomNum);
                if(roomNum=="-1") return;
                while(!roomCorrect(roomNum))
                {
                    cerr<<"Invalid room number. Please re-enter: ";
                    cin>>roomNum;
                    if(roomNum=="-1") return;
                }
                cout<<"Enter your name: ";
                getline(cin, guestName);
                if(guestName=="-1") return;
                cout<<"Enter Check-In Date (DD-MM-YYYY): ";
            startIn:
                cin>>tempIn;
                if(tempIn=="-1") return;
                if(!formatCorrect(tempIn)) 
                {
                    cerr<<"Date not in format DD-MM-YYYY. Please re-enter: ";
                    goto startIn;
                }
                checkIn=stodate(tempIn);
                if(!checkIn.datedorrect()) 
                {
                    cerr<<"Invalid date. Please re-enter: ";
                    goto startIn;
                }
                cout<<"Enter Check-Out Date (DD-MM-YYYY): ";
            startOut:
                cin>>tempOut;
                if(tempOut=="-1") return;
                if(!formatCorrect(tempOut)) 
                {
                    cerr<<"Date not in format DD-MM-YYYY. Please re-enter: ";
                    goto startOut;
                }
                checkOut=stodate(tempOut);
                if(!checkOut.datedorrect()) 
                {
                    cerr<<"Invalid date. Please re-enter: ";
                    goto startOut;
                }
            } while(!bookingCorrect(roomNum, checkIn, checkOut));
            cout<<"Enter your contact number: ";
        startCon:
            cin>>contact;
            if(contact.length()!=11||!numberCheck(contact))
            {
                cerr<<"Incorrect contact number. Please re-enter: ";
                goto startCon;
            }
            cout<<"Enter your account number: ";
        startAcc:
            cin>>account;
            if(account.length()!=16||!numberCheck(account))
            {
                cerr<<"Incorrect account number. Please re-enter: ";
                goto startAcc;
            }
            ofstream file(reservationFile, ios::app);
            if(file.is_open()) 
            {
                file<<endl<<roomNum<<":"<<guestName<<":"<<tempIn<<":"<<tempOut<<":"<<contact<<":"<<account;
                file.close();
            }
            else throw "Unable to open file: Reservations.txt";
            int stay=checkOut-checkIn;
            generateReceipt(roomNum, stay, guestName, contact, tempIn, tempOut);
            cout<<"Reservation added successfully! Room "<<roomNum<<" is booked for "<<stay<<" days."<<endl;
        }
        catch(const char* error) 
        {
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout<<"\nPress any key to continue...";
        while(!_kbhit()) {}
        char c=_getch();
        system("cls");
    }
    void display()
    {
        try
        {
            string line;
            ifstream file(reservationFile);
            if(file.is_open())
            {
                cout<<"Room Number\tGuest Name\tCheck-In Date\t Check-Out Date\t  Contact Number\tAccount Number\n";
                while(getline(file, line))
                {
                    string a, b, c, d, e, f;
                    stringstream ss(line);
                    getline(ss, a, ':');
                    getline(ss, b, ':');
                    getline(ss, c, ':');
                    getline(ss, d, ':');
                    getline(ss, e, ':');
                    getline(ss, f);
                    cout<<"   "<<a<<"\t\t  "<<b;
                    for(int i=0; i<15-b.size(); i++) cout<<" ";
                    cout<<c<<"\t   "<<d<<"\t    "<<e<<"\t       "<<f<<endl;
                }
            } 
            else throw "Unable to open file: Reservations.txt";
        }
        catch(const char* error) 
        {
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout<<"\nPress any key to continue...";
        while(!_kbhit()) {}
        char c=_getch();
    }
    void searchByName(string name)
    {
        try
        {
            system("cls");
            string line;
            int var=0;
            ifstream file(reservationFile);
            if(file.is_open())
            {
                cout<<"Room Number\tGuest Name\tCheck-In Date\t Check-Out Date\t  Contact Number\tAccount Number\n ";
                while(getline(file, line))
                {
                    string a, b, c, d, e, f;
                    stringstream ss(line);
                    getline(ss, a, ':');
                    getline(ss, b, ':');
                    getline(ss, c, ':');
                    getline(ss, d, ':');
                    getline(ss, e, ':');
                    getline(ss, f);
                    if(b==name) 
                    {
                        cout<<"   "<<a<<"\t\t  "<<b;
                        for(int i=0; i<15-b.size(); i++) cout<<" ";
                        cout<<c<<"\t   "<<d<<"\t    "<<e<<"\t       "<<f<<endl;
                        var=1;
                    }
                }
            } 
            else throw "Unable to open file: Reservations.txt";
            if(!var) 
            {
                system("cls");
                cout<<"No reservation found for guest: "<<name<<endl<<"Check the spelling and capitalization if you think this is an error"<<endl;
            }
        }
        catch(const char* error) 
        {
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout<<"\nPress any key to continue...";
        while(!_kbhit()) {}
        char c=_getch();
    }
    void searchByRoom(string room)
    {
        try
        {
            string line;
            int var=0;
            ifstream file(reservationFile);
            if(file.is_open())
            {
                cout<<"Room Number\tGuest Name\tCheck-In Date\t Check-Out Date\t  Contact Number\tAccount Number\n ";
                while(getline(file, line))
                {
                    string a, b, c, d, e, f;
                    stringstream ss(line);
                    getline(ss, a, ':');
                    getline(ss, b, ':');
                    getline(ss, c, ':');
                    getline(ss, d, ':');
                    getline(ss, e, ':');
                    getline(ss, f);
                    if(a==room) 
                    {
                        cout<<"   "<<a<<"\t\t  "<<b;
                        for(int i=0; i<15-b.size(); i++) cout<<" ";
                        cout<<c<<"\t   "<<d<<"\t    "<<e<<"\t       "<<f<<endl;
                        var=1;
                    }
                }
            } 
            else throw "Unable to open file: Reservations.txt";
            if(!var) 
            {
                system("cls");
                cout<<"No reservation found for room: "<<room<<endl;
            }
        }
        catch(const char* error) 
        {
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout<<"\nPress any key to continue...";
        while(!_kbhit()) {}
        char c=_getch();
        system("cls");
    }
    ~Reservation(){};
};
class Admin:public Reservation
{
    unsigned int maintenance=0;
	unsigned int totalSalary=0;
    unsigned int totalIncome=0;
    RevenueFile r;
    void maintenanceCost() 
    {
        string room;
        cout<<"Enter room number: ";
        cin>>room;
        while(!roomCorrect(room))
        {
            cerr<<"Invalid room number. Please re-enter: ";
            cin>>room;
        }
        switch(room[0]) 
        {
            case 'A':
                maintenance+=600;
                break;
            case 'B':
                maintenance+=500;
                break;
            case 'C':
                maintenance+=400;
                break;
            case 'D':
                maintenance+=200;
                break;
        }
        cout<<"\nMaintenance cost for room "<<room<<" added to total maintenance cost."<<endl;
        cout<<"\n\nPress any key to return to menu";
        while(!_kbhit()) {}
        char c=_getch();
    }
    void staffSalary() 
    {
        string name="Salary.txt";
        vector <string> staffs;
        vector<int> sals;
        try
        {
            string line, staff, salary;
            int i=0;
            ifstream file(name);
            if(file.is_open()) 
            {
                while(getline(file, line))
                {
                    stringstream ss(line);
                    getline(ss, staff, ':'); 
                    getline(ss, salary);    
                    if(ss.fail()) 
                    {
                        throw "Parsing failed. Invalid format.";
                        continue; 
                    }
                    staffs.push_back(staff);
                    sals.push_back(stoi(salary));
                    cout<<++i<<": "<<staff<<": $"<<stoi(salary)<<endl;
                }
                file.close();
            }
            else throw "Error opening salary.txt for reading";
            cout<<"\nEnter the number of the staff whose salary you want to update or enter 0 to go back to the menu: ";
            int choice;
            cin>>choice;
            if(choice==0) return;
            ofstream file1(name);
            if(file1.is_open())
            {
                string line, staff, salary;
                cout<<"Enter new salary: ";
                int newSalary;
                cin>>newSalary;
                for(int i=0; i<sals.size(); i++)
                {
                    file1<<staffs[i]<<":";
                    if(choice-1==i) 
                    {
                        totalSalary-=sals[i];
                        totalSalary+=newSalary;                            
                        file1<<newSalary<<endl;
                    }
                    else file1<<sals[i]<<endl;
                }
                cout<<"\nSalary updated successfully!";
                this_thread::sleep_for(chrono::seconds(1));
                file1.close();
            }
            else throw "Error opening salary.txt for writing";
        }
        catch(const char* error) 
        {
            system("cls");
            cout<<error<<endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    void updateRate() 
    {
        string room;
        int newRate;
        vector<int>rates;
        vector<string>rooms;
        cout<<"Enter room number: ";
        cin>>room;
        while(!roomCorrect(room))
        {
            cerr<<"Invalid room number. Please re-enter: ";
            cin>>room;
        }
        cout<<"Enter new rate: ";
        cin>>newRate;
        while(newRate<0||newRate==0)
        {
            cerr<<"Rate cannot be less than or equal to 0. Please a valid new rate: ";
            cin>>newRate;
        }
        int rate;
        string line,temp;
        ifstream file(ratesFile);
        if(file.is_open()) 
        {
            while(getline(file, line))
            {
                stringstream ss(line);
                getline(ss, temp, ':');
                ss>>rate;
                rates.push_back(rate);
                rooms.push_back(temp);
            }
            file.close();
        }
        else throw "Unable to open file RoomRates.txt for reading";
        ofstream file1(ratesFile);
        if(file1.is_open())
        {
            for(int i=0; i< rooms.size(); i++)
            {
                if(rooms[i]==room) rates[i]=newRate;
                file1<<rooms[i]<<":"<<rates[i]<<endl;
            }
            cout<<"Update successful\n";
            file1.close();
            cout<<"\nPress any key to continue...";
            while(!_kbhit()) {}
            char c=_getch(); 
        }
        else throw "Unable to open file RoomRates.txt for writing";
    }
    void annualIncome() 
    {
        int tempIncome=totalIncome;
        cout<<"\nAnnual Income:"<<endl;
        cout<<"Total Room Rent: $"<<totalIncome<<endl;
        cout<<"Maintenance Expenses: $"<<maintenance<<endl;
        cout<<"Total Staff Salary: $"<<totalSalary<<endl;
        cout<<"Electricity bill: $"<<tempIncome*0.3<<endl;
        tempIncome=tempIncome-(tempIncome*0.3);
        cout<<"Food bill: $"<<tempIncome*0.1<<endl;
        tempIncome=tempIncome-(tempIncome*0.1);
        cout<<"Telephone bill: $5000"<<endl;
        tempIncome-=5000;
        cout<<"Internet bill: $10000"<<endl;
        tempIncome-=10000;
        cout<<"Water bill: $"<<tempIncome*0.55<<endl;
        tempIncome=tempIncome-(tempIncome*0.55);
        cout<<"Gas bill: $"<<tempIncome*0.03<<endl;
        tempIncome=tempIncome-(tempIncome*0.03);
        int profit=tempIncome-(maintenance+totalSalary);
        profit>=0?cout<<"\n\nProfit of $"<<profit<<endl:cout<<"\n\nLoss of $"<<profit*-1<<endl;
        cout<<"\nPress any key to continue...";
        while(!_kbhit()) {}
        char c=_getch();
    }
public:
    Admin() {r.readRevenueFile(&maintenance, &totalSalary, &totalIncome);}
    void menu()
    {
        int option;
        Reservation re;
        do {
            cout<<"Admin Options"<<endl;
            cout<<"1. Add room for maintenance"<<endl;
            cout<<"2. Staff salary"<<endl;
            cout<<"3. Update room rate"<<endl;
            cout<<"4. Annual Revenue"<<endl;
            cout<<"5. Back"<<endl;
            cout<<"Enter your choice: ";
            cin>>option;
            switch (option) 
            {
                case 1:
                    system("cls");
                    maintenanceCost();
                    system("cls");
                    break;
                case 2:
                    system("cls");
                    staffSalary();
                    system("cls");
                    break;
                case 3:
                    system("cls");
                    try{re.display();}
                    catch(const char* error) {cout<<error<<endl;}
                    cout<<endl;
                    updateRate();
                    system("cls");
                    break;
                case 4:
                    system("cls");
                    annualIncome();
                    system("cls");
                    break;
                case 5:
                    return;
                default:
                    cout<<"Invalid choice. Please try again."<<endl;
            }
        } while(1);
    }
    ~Admin()
    {
        r.updateRevenueFile(&maintenance, &totalSalary, &totalIncome);
    }
};
void mainMenu()
{
    string choice;
    Rates r;
    Reservation re;
    Admin a;
    while(1) 
    {
        choice="0";
        cout<<"\t\tHotel Management System Menu:\n";
        cout<<"1. Add reservation\n";
        cout<<"2. Display reservations\n";
        cout<<"3. Search reservation by guest name\n";
        cout<<"4. Search reservation by room number\n";
        cout<<"5. Display room rates\n";
        cout<<"6. Admin rights\n";
        cout<<"7. Exit\n";
        cout<<"Enter your choice: ";
        while(choice<"1"||choice>"7")
        {
            cin>>choice;
            if(choice<"1"||choice>"7") cout<<"Invalid choice, try again: ";
        }
        if(choice=="1")
        {
            system("cls");
            try{r.display();}
            catch(const char* error) {cout<<error<<endl;}
            cout<<"\tAdd Reservation\n";
            re.add();
            system("cls");
        }
        else if(choice=="2")
        {
            system("cls");
            re.display();
            system("cls");
        }
        else if(choice=="3")
        {
            string searchName;
            system("cls");
            cout<<"Enter guest name to search: ";
            cin.ignore();
            getline(cin, searchName);
            re.searchByName(searchName);       
			system("cls");          
        }
        else if(choice=="4")
        {
            string searchRoom;
            system("cls");
            cout<<"Enter room number to search: ";
            cin>>searchRoom;
            re.searchByRoom(searchRoom);
        }
        else if(choice=="5")
        {
            system("cls");
            r.display();    
            string back=" ";
            cout<<"\nPress any key to continue...";
            while(!_kbhit()) {}
            char c=_getch();
            system("cls");
        }
        else if(choice=="6")
        {
            system("cls");
            string pass;
            cout<<"Enter password: ";
            cin>>pass;
            if(pass=="password123") 
            {
                system("cls");
                a.menu();
            }
            else 
            {
                cout<<"Incorrect password";
                this_thread::sleep_for(chrono::seconds(1));
            }
            system("cls");
        }
        else if(choice=="7") return;
    }
}
int main() 
{
    ifstream file("welcome.txt");
    if(!file) 
    {  
        cerr<<"Error opening file"<<endl;
        return 1;
    }
    string line;
    while(getline(file, line)) if(line!="stop") cout<<line<<endl; else break;
    file.close();
    this_thread::sleep_for(chrono::seconds(1));
    cout<<"\nPress any key to continue...";
    while(!_kbhit()) {}
    char c=_getch();
    system("cls");
    mainMenu();
    return 0;
}