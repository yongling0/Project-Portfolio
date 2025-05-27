#include <iostream>
#include <iomanip>
#include <fstream>
//#include <ctime>
using namespace std;

struct Date
{
   int year;
   int month;
   int day;
};

struct Reservation
{
   char flightNo[ 8 ]; // flight number
   char id[ 12 ];      // ID number
   char name[ 8 ];     // name
   char mobile[ 12 ];  // mobile phone number
   char date[ 12 ];    // departure date
   int tickets[ 8 ];
   // tickets[ 0 ]: not used
   // tickets[ 1 ]: number of full fare tickets
   // tickets[ 2 ]: number of child tickets
   // tickets[ 3 ]: number of infant tickets
   // tickets[ 4 ]: number of senior citizen tickets
   // tickets[ 5 ]: number of impaired tickets
   // tickets[ 6 ]: number of impaired companion tickets
   // tickets[ 7 ]: number of military tickets
};

struct Flight
{
   char flightNo[ 8 ];      // flight number
   char aircraft[ 8 ];      // aircraft model
   int departureAirport;    // departure airport code
   int arrivalAirport;      // arrival airport code
   char departureTime[ 8 ]; // departure time
   char arrivalTime[ 8 ];   // arrival time
   int dayWeek[ 7 ];        // the value of dayWeek[ i ] is 0 or 1
   // dayWeek[ 0 ] == 1 if and only if the flight flies on Sunday
   // dayWeek[ 1 ] == 1 if and only if the flight flies on Monday
   // dayWeek[ 2 ] == 1 if and only if the flight flies on Tuesday
   // dayWeek[ 3 ] == 1 if and only if the flight flies on Wednesday
   // dayWeek[ 4 ] == 1 if and only if the flight flies on Thursday
   // dayWeek[ 5 ] == 1 if and only if the flight flies on Friday
   // dayWeek[ 6 ] == 1 if and only if the flight flies on Saturday
};

struct FlightSeats
{
   char date[ 12 ];      // date
   char flightNo[ 8 ];   // flight number
   int departureAirport; // departure airport code
   int arrivalAirport;   // arrival airport code
   int availableSeats;   // the number of available seats
};

const char airportName[ 12 ][ 12 ] = { "", "Taipei",    "Taichung", "Chiayi",  "Tainan",
                                         "Kaohsiung", "Hualien", "Taitung",
                                         "Penghu",    "Kinmen",   "Beigan",  "Nangan" };

// row 0 and column 0 are not used,
// fullFare[ i ][ j ] is the full fare from airport i to airport j
const int fullFare[ 12 ][ 12 ] = {
   { 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0,    0,    0,    0,    0,    0, 1781, 2460, 2296, 2618, 2197, 2197 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1769, 2236,    0, 2686 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1721, 2217,    0,    0 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1697, 2231,    0,    0 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1876, 2393,    0,    0 },
   { 0, 1781,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0, 2460,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0, 2296, 1769, 1721, 1697, 1876,    0,    0,    0, 1768,    0,    0 },
   { 0, 2618, 2236, 2217, 2231, 2393,    0,    0, 1768,    0,    0,    0 },
   { 0, 2197,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0, 2197, 2686,    0,    0,    0,    0,    0,    0,    0,    0,    0 } };

const int discount[ 8 ] = { 0, 100, 75, 0, 50, 50, 50, 95 };
// discount[ 0 ]: not used
// discount[ 1 ]: Full Fare Ticket (肂布)
// discount[ 2 ]: Child Ticket (ㄠ担布):                 25% off of the full fare
// discount[ 3 ]: Infant Ticket (捆ㄠ布):                Free of charge
// discount[ 4 ]: Senior Citizen Ticket (穛ρ布):        50% off of the full fare
// discount[ 5 ]: Impaired Ticket (稲み布):              50% off of the full fare
// discount[ 6 ]: Impaired Companion Ticket (稲み抄布): 50% off of the full fare
// discount[ 7 ]: Military Ticket (瓁布):               5% off of the full fare

const int maxNumReservations = 1000;

int enterChoice();

// perform booking
void booking();

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport( int departureAirport, int &arrivalAirport );

// input the departure date
void inputDepartureDate( char date[], int &departureDayWeek );

// return the number of days between currentDate and departureDate
int difference( Date departureDate, Date currentDate );

bool leapYear( int year );

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights );

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights,
     int departureAirport, int arrivalAirport, int departureDayWeek );

// load all flightSeats from departureAirport to arrivalAirport on departureDate,
// and put them into the array flightSeats
void loadFlightSeats( FlightSeats flightSeats[], int &numFlightSeats, char departureDate[],
                     int departureAirport, int arrivalAirport );

// display all flights from departureAirport to arrivalAirport
void displayFlights( const Flight flights[], int numFlights,
                    FlightSeats flightSeats[], int &numFlightSeats,
                    int departureAirport, int arrivalAirport, char date[] );

// choose a flight by input its number,
// and assign the number to reservation.flightNo
void chooseFlight( Reservation &reservation, const Flight flights[], int numFlights );

// input the number of tickets for each ticket type,
// and assign them to reservation.tickets
void inputNumTickets( Reservation &reservation, FlightSeats flightSeats[], int &numFlightSeats );

// during booking and refund application,
// the corresponding availableSeats in Flight Seats.dat should be modified
void modifyFlightSeatsFile( Reservation &reservation, int totalNumTickets );

void displayReservation( const Reservation reservation,
                        const Flight flights[], int numFlights );

// append reservation to the end of Reservations.dat
void saveReservation( const Reservation reservation );

// perform booking inquiry
bool bookingInquiry( const char id[] );

// perform refund application
void refundApplication();

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations( Reservation reservations[], int &numReservations );

int main()
{
   cout << "Welcome to UNI Air Domestic Flights Booking System\n";
   char id[ 12 ];
   int choice;

   while( ( choice = enterChoice() ) != 4 )
   {
      switch( choice )
      {
      case 1:
         booking(); // perform booking
         break;
      case 2:
         cout << "\nEnter ID number: ";
         cin >> id;
         bookingInquiry( id ); // perform booking inquiry
         break;
      case 3:
         refundApplication(); // perform refund application
         break;

      default:
         cerr << "\nInput error!\n";
         break;
      }
   }

   cout << endl;
   system( "pause" );
}

int enterChoice()
{
   cout << "\nInput your choice:\n"
        << "1. Booking\n"
        << "2. Booking enquiry\n"
        << "3. Refund application\n"
        << "4. End\n? ";

   int menuChoice;
   cin >> menuChoice;
   return menuChoice;
}

// perform booking
void booking()
{
    // choose departure airport by input its code,
    // and assign the code to departureAirport
    int departureAirport;
    do {
        cout << "\nDeparture airport:\n";
        for (int i = 1; i <= 11; i++)
            cout << setw(2) << i << ". " << airportName[i] << endl;
        cout << "? ";

        cin >> departureAirport;
    } while (departureAirport < 1 || departureAirport > 11);

    int arrivalAirport;
    // choose arrival airport by input its code, and assign the code to arrivalAirport
    chooseArrivalAirport(departureAirport, arrivalAirport);

    Reservation reservation = { "", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0 };

    int departureDayWeek; // departureDayWeek == 0 if departure date is Sunday
    // departureDayWeek == 1 if departure date is Monday
    // departureDayWeek == 2 if departure date is Tuesday
    // departureDayWeek == 3 if departure date is Wednesday
    // departureDayWeek == 4 if departure date is Thursday
    // departureDayWeek == 5 if departure date is Friday
    // departureDayWeek == 6 if departure date is Saturday

    inputDepartureDate(reservation.date, departureDayWeek);

    Flight flights[20];
    int numFlights = 0;

    // load all flights from departureAirport to arrivalAirport,
    // and put them into the array flights
    loadFlightSchedules(flights, numFlights, departureAirport, arrivalAirport,
        departureDayWeek);

    if (numFlights == 0)
    {
        cout << "No flights available!\n";
        return;
    }

    FlightSeats flightSeats[20];
    int numFlightSeats = 0;

    loadFlightSeats(flightSeats, numFlightSeats, reservation.date,
        departureAirport, arrivalAirport);

    int maxNumSeats = 0;
    for (int i = 1; i <= numFlights; i++)
        if (maxNumSeats < flightSeats[i].availableSeats)
            maxNumSeats = flightSeats[i].availableSeats;

    if (maxNumSeats == 0)
    {
        cout << "No seats available for Selected Flight!\n";
        return;
    }

    // display all flights from departureAirport to arrivalAirport
    displayFlights(flights, numFlights, flightSeats, numFlightSeats,
        departureAirport, arrivalAirport, reservation.date);

    // choose a flight by input its number, and assign the number to reservation.flightNo
    chooseFlight(reservation, flights, numFlights);

    // input the number of tickets for each ticket type,
    // and assign them to reservation.tickets
    inputNumTickets(reservation, flightSeats, numFlightSeats);

    int totalNumTickets = -reservation.tickets[3];
    for (int i = 1; i < 8; i++)
        totalNumTickets += reservation.tickets[i];

    if (totalNumTickets > 0)
        modifyFlightSeatsFile(reservation, totalNumTickets);
    else
    {
        cout << "Your booking could not be completed!\n";
        return;
    }

    displayReservation(reservation, flights, numFlights);

    cout << "\nID number: ";
    cin >> reservation.id;
    cout << "\nName: ";
    cin >> reservation.name;
    cout << "\nMobile: ";
    cin >> reservation.mobile;

    // append reservation to the end of Reservations.dat
    saveReservation(reservation);

    cout << "\nBooking completed!\n";
}

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport( int departureAirport, int &arrivalAirport )
{
    bool same = false;
    do {
        cout << "\nArrival airport:\n";
        for (int i = 1; i <= 11; i++) {
            if (fullFare[departureAirport][i] != 0)
                cout << setw(2) << i << ". " << airportName[i] << endl;
        }
        cout << "? ";

        cin >> arrivalAirport;
        for (int i = 1; i <= 11; i++) {
            if (fullFare[departureAirport][i] != 0 && arrivalAirport == i)
                same = true;
        }
    } while (!same);

}

void inputDepartureDate( char date[], int &departureDayWeek )
{
   tm structuredTime;
   time_t rawTime = time( nullptr );
   localtime_s( &structuredTime, &rawTime );

   int year = structuredTime.tm_year + 1900;
   int month = structuredTime.tm_mon + 1;
   int day = structuredTime.tm_mday;
   bool same = false;
   int num;
   const int lastdate1[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
   const int lastdate2[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
   Date currentDate{};
   currentDate.day = day;
   currentDate.month = month;
   currentDate.year = year;
   char date_year[5];
   char date_month[3];
   char date_day[3];

   do {
       cout << "\nDeparture Date (Month):\n";
       for (int i = 1; i <= 6; i++) {
           cout << setw(2) << i << ". " << year << "-" << month << endl;
           if (month == 12) {
               month = 0;
               year++;
           }
           month++;
       }
       cout << "? ";

       cin >> num;
       for (int i = 1; i <= 6; i++) {
           if (i == num)
               same = true;
       }
       if (leapYear(year)) {
           if (num == 1)
               cout << "Day" << "( " << day << " ~ " << lastdate2[month] << " ): ";
           else
               cout << "Day" << "( 1 ~ " << lastdate2[month] << " ): ";
       }
       else {
           if (num == 1)
               cout << "Day" << "( " << day << " ~ " << lastdate1[month] << " ): ";
           else
               cout << "Day" << "( 1 ~ " << lastdate1[month] << " ): ";
       }

   } while (!same);

   Date departureDate{};
   cin >> departureDate.day;
   departureDate.month = structuredTime.tm_mon + num;
   departureDate.year = structuredTime.tm_year + 1900;
   if (departureDate.month == 12) {
       departureDate.month = 1;
       departureDate.year++;
   }
   _itoa_s(departureDate.year, date_year, 10);
   _itoa_s(departureDate.month, date_month, 10);
   _itoa_s(departureDate.day, date_day, 10);
   
   strcat_s(date, 12, date_year);
   strcat_s(date, 12, "/");
   strcat_s(date, 12, date_month);
   strcat_s(date, 12, "/");
   strcat_s(date, 12, date_day);
   departureDayWeek = difference(departureDate, currentDate);

}

int difference( Date departureDate, Date currentDate )
{
    tm structuredTime;
    time_t rawTime = time(nullptr);
    localtime_s(&structuredTime, &rawTime);

    int differencedays = 0;
    const int lastdate1[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    const int lastdate2[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
    if (departureDate.year == currentDate.year) {
        if (leapYear(departureDate.year) || leapYear(currentDate.year)) {
            for (int i = currentDate.month; i <= departureDate.month; i++) {
                if (i == currentDate.month)
                    differencedays += lastdate2[currentDate.month] - currentDate.day;
                else if (i == departureDate.month)
                    differencedays += departureDate.day;
                else
                    differencedays += lastdate2[i];
            }
        }
        else {
            for (int i = currentDate.month; i <= departureDate.month; i++) {
                if (i == currentDate.month)
                    differencedays += lastdate1[currentDate.month] - currentDate.day;
                else if (i == departureDate.month)
                    differencedays += departureDate.day;
                else
                    differencedays += lastdate1[i];
            }
        }

    }
    else {
        for (int i = currentDate.month; i <= 12; i++) {
            if (i == currentDate.month)
                differencedays += lastdate1[currentDate.month] - currentDate.day;
            else
                differencedays += lastdate1[i];
        }
        if (leapYear(departureDate.year)) {
            for (int i = 1; i <= departureDate.month; i++) {
                if(i == departureDate.month)
                    differencedays += lastdate2[departureDate.month] - departureDate.day;
                else
                    differencedays += lastdate2[i];
            }
        }
        else
            for (int i = 1; i <= departureDate.month; i++) {
                if (i == departureDate.month)
                    differencedays += lastdate1[departureDate.month] - departureDate.day;
                else
                    differencedays += lastdate1[i];
            }

    }
    differencedays %= 7;
    differencedays += structuredTime.tm_wday;
    if (differencedays > 6)
        differencedays -= 7;
    return differencedays;

}

bool leapYear( int year )
{
   return ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) );
}

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights )
{
    Flight flightsdata;
    ifstream infile("Flight Schedule.dat", ios::in | ios :: binary);
    infile.seekg(0);
    while (!infile.eof()) {
        infile.read(reinterpret_cast<char*>(&flightsdata), sizeof(Flight));
        flights[numFlights] = flightsdata;
        numFlights++;
    }
    infile.clear();
    infile.close();

}

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights,
     int departureAirport, int arrivalAirport, int departureDayWeek )
{
    Flight flightsdata;
    ifstream infile("Flight Schedule.dat", ios::in | ios::binary);
    infile.seekg(0);
    numFlights++;
    while (!infile.eof()) {
        infile.read(reinterpret_cast<char*>(&flightsdata), sizeof(Flight));
        if (flightsdata.departureAirport == departureAirport && flightsdata.arrivalAirport == arrivalAirport 
            && flightsdata.dayWeek[departureDayWeek] == 1) {
            flights[numFlights] = flightsdata;
            numFlights++;
        }
    }
    numFlights--;
    infile.clear();
    infile.close();

}

void loadFlightSeats( FlightSeats flightSeats[], int &numFlightSeats, char departureDate[],
     int departureAirport, int arrivalAirport )
{
    FlightSeats flightSeatsdata;
    ifstream infile("Flight Seats.dat", ios::in | ios::binary);
    infile.seekg(0);
    numFlightSeats++;
    while (infile.read(reinterpret_cast<char*>(&flightSeatsdata), sizeof(FlightSeats))) {
        if (flightSeatsdata.departureAirport == departureAirport && flightSeatsdata.arrivalAirport == arrivalAirport
            && strcmp(flightSeatsdata.date , departureDate) == 0) {
            flightSeats[numFlightSeats] = flightSeatsdata;
            numFlightSeats++;
        }
    }
    numFlightSeats--;
    infile.clear();
    infile.close();

}

// display all flights from departureAirport to arrivalAirport
void displayFlights( const Flight flights[], int numFlights,
                   FlightSeats flightSeats[], int &numFlightSeats,
                    int departureAirport, int arrivalAirport, char date[] )
{
   cout << airportName[ departureAirport ] << " to " << airportName[ arrivalAirport ]
        << " / Depart date: " << date << ".\n\n";
   cout << "Flight   Departure   Arrival   Fare   No. Available Seats\n";

   for( int i = 1; i <= numFlights; i++ )
      if( flightSeats[ i ].availableSeats  > 0 )
         cout << setw( 6 ) << flights[ i ].flightNo
              << setw( 12 ) << flights[ i ].departureTime
              << setw( 10 ) << flights[ i ].arrivalTime
              << setw( 7 ) << fullFare[ departureAirport ][ arrivalAirport ]
              << setw( 22 ) << flightSeats[ i ].availableSeats << endl;
}

// choose a flight by input its number, and assign the number to reservation.flightNo
void chooseFlight( Reservation &reservation, const Flight flights[], int numFlights )
{
    bool fightnumber = false;
    do {
        cout << "\nFlight number:\n";
        cin >> reservation.flightNo;
        for (int i = 1; i <= numFlights; i++)
            if (strcmp(reservation.flightNo, flights[i].flightNo) == 0)
                fightnumber = true;
    } while (!fightnumber);

}

// input the number of tickets for each ticket type, and assign them to reservation.tickets
void inputNumTickets( Reservation &reservation, FlightSeats flightSeats[], int &numFlightSeats )
{
   int i;
   for( i = 1; i <= numFlightSeats; i++ )
      if( strcmp( reservation.flightNo, flightSeats[ i ].flightNo ) == 0 )
         break;

   int numAvailableSeats = flightSeats[ i ].availableSeats;
   int maxNumTickets = ( numAvailableSeats < 4 ) ? numAvailableSeats : 4;

   do {
      cout << "\nNumber of infant tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 3 ];
   } while( reservation.tickets[ 3 ] < 0 || reservation.tickets[ 3 ] > maxNumTickets );

   int numTickets = 0;
   int numAdults = 0;
   do
   {
      numTickets = 0;

      cout << "\nNumber of full fare tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 1 ];
      numTickets += reservation.tickets[ 1 ];

      cout << "\nNumber of child tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 2 ];
      numTickets += reservation.tickets[ 2 ];

      cout << "\nNumber of senior citizen tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 4 ];
      numTickets += reservation.tickets[ 4 ];

      cout << "\nNumber of impaired tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 5 ];
      numTickets += reservation.tickets[ 5 ];

      cout << "\nNumber of impaired companion tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 6 ];
      numTickets += reservation.tickets[ 6 ];

      cout << "\nNumber of military tickets (0 ~ " << maxNumTickets << "): ";
      cin >> reservation.tickets[ 7 ];
      numTickets += reservation.tickets[ 7 ];

      numAdults = reservation.tickets[ 1 ] + reservation.tickets[ 4 ] +
                 reservation.tickets[ 5 ] + reservation.tickets[ 6 ] +
                 reservation.tickets[ 7 ];

      if( numTickets > maxNumTickets )
         cout << "\nYou can books a maximum of " << maxNumTickets
              << " passengers ( including children ).\n";
      else if( numAdults < reservation.tickets[ 3 ] )
         cout << "\nEach infant must always be accompanied by at least one adult.\n";
   } while( numTickets > maxNumTickets || numAdults < reservation.tickets[ 3 ] );

   cout << endl;
}

void modifyFlightSeatsFile( Reservation &reservation, int totalNumTickets )
{
    FlightSeats flightseatsdata;
    fstream iofile("Flight Seats.dat", ios::out | ios::in | ios::binary);
    iofile.seekg(0);
    int num = 0;
    while (iofile.read(reinterpret_cast<char*>(&flightseatsdata), sizeof(FlightSeats))) {
        if (strcmp(flightseatsdata.date, reservation.date) == 0 && strcmp(flightseatsdata.flightNo, reservation.flightNo) == 0) {
            flightseatsdata.availableSeats -= totalNumTickets;
            iofile.seekp(num * sizeof(FlightSeats), ios::beg);
            //iofile.seekp(-32, ios::cur);
            iofile.write(reinterpret_cast<const char*>(&flightseatsdata), sizeof(FlightSeats));
            break;
        }
        num++;
    }
    iofile.clear();
    iofile.close();
}

void displayReservation( const Reservation reservation,
                         const Flight flights[], int numFlights )
{
   int k;
   for( k = 1; k <= numFlights; k++ )
      if( strcmp( reservation.flightNo, flights[ k ].flightNo ) == 0 )
         break;

   cout << "Ticket information:\n\n";
   cout << "Date: " << reservation.date << endl;
   cout << "Flight: B7-" << reservation.flightNo << endl << endl;

   int departureAirport = flights[ k ].departureAirport;
   int arrivalAirport = flights[ k ].arrivalAirport;

   cout << setw( 9 ) << right << airportName[ departureAirport ] << " -> "
        << setw( 9 ) << left << airportName[ arrivalAirport ] << endl;

   cout << setw( 9 ) << right << flights[ k ].departureTime << "    ";
   cout << setw( 9 ) << left << flights[ k ].arrivalTime << endl << endl;

   char ticketType[ 8 ][ 24 ] = { "", "Full Fare", "Child Fare", "Infant Fare",
                                      "Senior Citizen Fare",     "Impaired Fare",
                                      "Impaired Companion Fare", "Military Fare" };

   int total = 0;
   int fare;
   for( int i = 1; i <= 7; i++ )
      if( reservation.tickets[ i ] > 0 )
      {
         fare = fullFare[ departureAirport ][ arrivalAirport ] * discount[ i ] / 100;
         total += ( fare * reservation.tickets[ i ] );
         cout << right << setw( 23 ) << ticketType[ i ] << "  TWD "
              << setw( 4 ) << fare << " x " << reservation.tickets[ i ] << endl;
      }

   cout << "\nTotal: " << total << endl;
}

// append reservation to the end of Reservations.dat
void saveReservation( const Reservation reservation )
{
    ofstream outfile("Reservations.dat", ios::out |ios::app | ios::binary);
    outfile.write(reinterpret_cast<const char*>(&reservation), sizeof(Reservation));
}

// perform booking inquiry
bool bookingInquiry( const char id[] )
{
    Flight fights[200];
    int numflight = 1;
    loadFlightSchedules(fights, numflight);
    Reservation reservations[20];
    int numReservations = 0;
    loadReservations(reservations, numReservations);
    int num = 1;
    bool display = false;
    for (int i = 0; i < numReservations; i++) {
        if (strcmp(id, reservations[i].id) == 0) {
            cout << num << ". ";
            displayReservation(reservations[i], fights, numflight);
            num++;
            display = true;
        }
    }
    if (!display)
        cout << "NO reservations!" << endl;
    return display;

}

// perform refund application
void refundApplication()
{
   char id[ 12 ];

   cout << "\nEnter ID number: ";
   cin >> id;

   if( !bookingInquiry( id ) )
      return;

   int choice;
   cout << "\nEnter your choice: ";
   cin >> choice;

   Reservation reservations[ maxNumReservations ];
   int numReservations;
   loadReservations( reservations, numReservations);

   ofstream outfile("Reservations.dat", ios::out | ios::binary);
   outfile.seekp(0);
   int j = 0;
   for (int i = 0; i < numReservations; i++) {
       if (strcmp(id, reservations[i].id) == 0) {
           j++;
           if (j == choice) {
               int totalNumTickets = -reservations[i].tickets[3];
               for (int a = 1; a < 8; a++)
                   totalNumTickets += reservations[i].tickets[a];
               modifyFlightSeatsFile(reservations[i], -totalNumTickets);
               continue;
           }
       }
       outfile.write(reinterpret_cast<const char*>(&reservations[i]), sizeof(Reservation));
   }
   numReservations--;

   cout << "\nThe seleted booking has been deleted.\n";
}

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations( Reservation reservations[], int &numReservations )
{
    Reservation reservationdata;
    ifstream infile("Reservations.dat", ios::out | ios:: in |ios::binary);
    infile.seekg(0);
    numReservations = 0;
    while (!infile.eof()) {
        infile.read(reinterpret_cast<char*>(&reservationdata), sizeof(Reservation));
        reservations[numReservations] = reservationdata;
        numReservations++;
    }
    numReservations--;
    infile.clear();
    infile.close();

}