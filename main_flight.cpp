// C++ program to implement basic airline reservation system using SQLite3
// objects in C++
#include <iostream> 
#include <fstream> 
#include <sqlite3.h>
#include <stdio.h>
#include <string>

using namespace std; 

static int no_flights = 0; //Number of flights available
static int seats_available = 0; //Number of seats available

// Function to print the flight details 
static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
	
	cout<<endl;
    for (i = 0; i < argc; i++) { 
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
    } 

	int x = stoi(argv[6]);
	no_flights++;
    cout << endl; 
    return 0; 
} 

// Function to print the seat details
static int bookback(void* data, int argc, char** argv, char** azColName) 
{ 
	int x = stoi(argv[0]);
	seats_available = x;
    cout << endl; 
    return 0; 
} 

// Class to define the properties of Flight 
class Flight { 
	private:
		int no_seats = 0; // Initialise the number of seats 
public: 
	// Instance variables 
	long int Flight_no; 
	int Src_id, Dst_id; 

    // Function to book Tickets
    int book_ticket();

	// Function to display flights
	int display_flights();
}; 

// Function to book tickets
int Flight::book_ticket()
{
         
    sqlite3* DB; 
    int exit = 0; 
    exit = sqlite3_open("FlightInfo.db", &DB); 
  
    // For error in opening the DataBase
    if (exit) { 
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
        return (-1); 
    } 
	
	string query = "SELECT seats FROM flights WHERE flight_no=" + to_string(Flight_no) + ";"; // 

	seats_available = 0;
    sqlite3_exec(DB, query.c_str(), bookback, NULL, NULL);


    // If seats are not available 
	if(!seats_available)
	{
		cout << "No seats available" << endl;
		return (0);
	}

	seats_available--; // Seats available decrements
	query = "UPDATE flights SET seats=" + to_string(seats_available) + " WHERE flight_no=" + to_string(Flight_no) + ";";

	int booked = sqlite3_exec(DB, query.c_str(), bookback, NULL, NULL);
	if(booked != SQLITE_OK)
	{
		cout << "Something went wrong! Please try again later." << endl;
	}
	else
	{
		cout << "Seat booked!!!" << endl;
	}
	
    sqlite3_close(DB); 
    return (0);
} 

// Functions to display
int Flight::display_flights()
{
    sqlite3* DB; 
    int exit = 0; 
    exit = sqlite3_open("FlightInfo.db", &DB); 
  
    if (exit) { 
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
        return (-1); 
    } 
	
	string query = "SELECT * FROM flights WHERE src_id=" + std::to_string(Src_id) + " AND dst_id=" + std::to_string(Dst_id) + ";";

	no_flights = 0;
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

    sqlite3_close(DB); 
    return (0);
} 
 
int main() 
{ 
	// Creating object of the class 
	Flight object; 

        cout<<"\t\tWelocome to Flight Booking"<<endl;
        cout<<"\t\t**************************"<<endl;
        cout<<"\n\t\tCity Name \t- Id"<<endl;
        cout<<"\t\t____________________"<<endl;
        cout<<"\n\t\tBangalore \t- 1"<<endl;
        cout<<"\t\tDelhi     \t- 2"<<endl;
        cout<<"\t\tMumbai    \t- 3"<<endl;
        cout<<"\t\tBelagavi  \t- 4"<<endl;

        cout<<"\nEnter the Source Id : "<<endl;
        cin>>object.Src_id;
        cout<<"Enter the Destination Id : "<<endl;
        cin>>object.Dst_id;
        object.display_flights();
        
		// If flights not available
		if(!no_flights)
		{
			cout << "No flights available" << endl; 
		}
		else
		{
			cout<<"Please enter the Flight Number : "<<endl;
        	cin>>object.Flight_no;
        	object.book_ticket();
		}
        
	return 0; 
} 

