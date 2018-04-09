/*****************************************************************************
	CODE FILENAME: BaroneAssn4.cpp
	DESCRIPTION:   Program maintains vehicle rental information for a car
				   rental company. It allows users to view vehicles available
				   for rent, and allows user to initiate turn in of previously
				   rented vehicle. All changes made to the data will be saved to
				   a binary file.
	CLASS/TERM:    CS362_XP72_Data Structures
	DESIGNER:	   Mark Barone
	FUNCTIONS	   main - calls functions to perform overall task of program
				   InitializeArray - Initializes array with all values Full if
				   					 no file is found. Loads data if found
				   DisplayStatus - Displays all vehicle inventory by level
				   ActionMenu - Prompts user for menu choice
				   SizeMenu - Validates user choice of car size 
				   ShowCars - Displays vehicle inventory of selected size by 
				   	          level
				   VerifyRental - Validates and updates user choice to rent
				   VerifyReturn - Validates and updates user choice to turn-in
				   CheckLevel - checks for valid level input from user
				   ValidateChoice - Validates user character entry for action
				   SaveFile - saves to binary file
				    
*****************************************************************************/
#include <iostream>				//for I/O 
#include <iomanip>				//for formatting output
#include <fstream>				//for reading from text file

using namespace std;

//Global constants
const int MAX_LEVEL = 6,					//max num of levels in garage
		  MAX_CAR_SIZE = 4,					//max num car sizes offered
		  GARAGE_FULL = 120,				//max num of cars garage stores
		  GARAGE_EMPTY = 0;					//no inventory in garage

//for string output of car size  
const string carType [] = {"Compact", "Mid-size", "Full-size", "SUV"};

//array alias
typedef int rental [MAX_CAR_SIZE] [MAX_LEVEL];

//declare enumerated type
enum carSize {COMPACT, MIDSIZE, FULLSIZE, SUV, NONE};

//Prototypes
void InitializeArray (ifstream&, rental);
void DisplayStatus (rental, int&);
char ActionMenu (int);
carSize SizeMenu (rental, char);
void ShowCars (rental, carSize, char);
void VerifyRental (rental, carSize, int);
void VerifyReturn (rental, carSize, int);
int CheckLevel ();
bool ValidateChoice (char, int);
void SaveFile (ofstream&, rental);

/*****************************************************************************
   FUNCTION: Main
   DESCRIPTION: Prompts user then loops calling multiple functions to complete 
   				all tasks
   OUTPUT:	 	Return Value:  0 on exit
   CALLS TO: 	InitializeArray, DisplayStatus, ActionMenu, SizeMenu,
   				ShowCars, SaveFile
****************************************************************************/
int main()
{	
	//constant declarations
	const char EXIT = 'E';					//user selected exit
	const char RENT_VEH = 'R',				//user selected Rent Vehicle
		  	   VEH_RETURN = 'T';			//user selected Turn-in Vehicle
	//variable declarations
	ifstream rentInfo;						//input filestream
	ofstream saveInfo;						//output filestream	
	
	int	carTotal = 0;						//total cars currently in garage
	carSize selection;						//car size to rent/turn-in
	char userSel;							//choice to rent/turn-in/exit
	
	//declare array
	rental garageInv;
	
	//program title
	cout << " Welcome to Speedy Rentals" << endl
		 << " The menu below shows cars available for rent" << endl << endl;
	
	//call function to Initialize an Array
	InitializeArray (rentInfo, garageInv);
	
	//loop until user selects exit
	do{

		DisplayStatus (garageInv, carTotal);
		
		userSel = ActionMenu (carTotal);
	
		if ((userSel == RENT_VEH) || (userSel == VEH_RETURN)){
		
			selection = SizeMenu (garageInv, userSel);
			
			if (selection != NONE)
				ShowCars (garageInv, selection, userSel);
		}			
		
	}while (userSel != EXIT);
	
	SaveFile (saveInfo, garageInv);

	return 0;	
 }

/*************************************************************************
  FUNCTION:	 InitializeArray
  DESCRIPTION:	Opens binary file and initializes array and sets all value to 
  				Full if file not found, or Empty if valid file. Then loads
				data into array if file is found 
  INPUT:	 Parameter:	rentInfo - input filestream variable
  						garageInv - array to initialize and update
			 File: 	Reads following rental info from each line of GARAGE.BIN
   					rowIdx, colIdx, qty
   					Data on each line is formatted as follows
   					binary character representation of an
   					int int int					   
  OUTPUT: 	 Parameter: garageInv - array to store rental info			 
**************************************************************************/
void InitializeArray (ifstream& rentInfo, rental garageInv)
{	
	//constant declarations
	const string OPEN_FILE = "GARAGE.BIN";	//filename of customer text.doc
	const int	FULL = 5,					//max cars stored per level
		  		EMPTY = 0;					//no cars stored on level
	//variable declarations	
	int qty,						//# of car sizes currently stored on level
		rowIdx,						//current array row index for loop
		colIdx;						//current array column index for loop
		
	//open file to read 
	//file has the following format:
	//rowIdx, colIdx, qty as
	//int, int, int	
	rentInfo.open(OPEN_FILE.c_str(), ios::binary | ios::in);

	
	//initialize array to Full if file not found
	if (!rentInfo){
		for (rowIdx = 0; rowIdx < MAX_CAR_SIZE; rowIdx++){
 		
			 for (colIdx = 0; colIdx < MAX_LEVEL; colIdx++){
		 		garageInv [rowIdx][colIdx] = FULL;
		 	}
		}
	
		return;
		
	}
	//initialize array to empty if file found
	else{
		for (rowIdx = 0; rowIdx < MAX_CAR_SIZE; rowIdx++){
 		
			 for (colIdx = 0; colIdx < MAX_LEVEL; colIdx++){
		 		garageInv [rowIdx][colIdx] = EMPTY;
		 	}
		}
		//update array values for vehicles stored in file
		do{
			rentInfo.read (reinterpret_cast<char*> 
			 			(&rowIdx), sizeof (int));
			rentInfo.read (reinterpret_cast<char*> 
			 			(&colIdx), sizeof (int));
			rentInfo.read (reinterpret_cast<char*> 
			 			(&qty), sizeof (int));
			
			garageInv [rowIdx][colIdx] = qty;	
			 				
		}while(rentInfo);			
	}
	
	rentInfo.clear();	
	rentInfo.close();	
	
	return;	
}
/*************************************************************************
  FUNCTION: 	DisplayStatus
  DESCRIPTION:  Displays total inventory of vehicles by car size and level
  INPUT:        Parameter: garageInv - array storing vehicle inventory
  OUTPUT:		Parameter: carTotal - total cars currently in garage			 
**************************************************************************/
void DisplayStatus (rental garageInv, int& carTotal)
{
	//variable declaration
	int sum = 0,						//total of car size on all levels
		rowIdx,							//array row index (car size)
		colIdx;							//array column index (level)
	
	//display header	
	cout << " Level:" << setw(10) << '0' << setw (4) << '1' << setw (4) << '2' 
		 << setw (4) << '3' << setw (4) << '4' << setw (4) << '5' << setw (8) 
	     << "Total" << endl << endl;
	
	//reset total to reflect current inventory     
	carTotal = 0;
	
	//loop displaying vehicle inventory for each level	
	for (rowIdx = 0; rowIdx < MAX_CAR_SIZE; rowIdx++){
		
		cout << ' ' << left << setw(15) << carType[rowIdx] << right;
 		
		for (colIdx = 0; colIdx < MAX_LEVEL; colIdx++){
		 	
			 cout << garageInv [rowIdx][colIdx] << setw(4);
			 
			 sum += garageInv [rowIdx][colIdx];
		}
		
		cout << setw(8) << sum << endl;
		carTotal += sum;
		sum = 0;
	}
	
	cout << setw(45) << "-----" << endl
		 << " Total Cars Available for Rental" << setw(13) << carTotal << endl
		 << endl;
	
	return;
		
}
/*************************************************************************
    FUNCTION:	ActionMenu 
    DESCRIPTION: Prompts user with menu of options. Rent/TurnIn/Exit
    INPUT:		 Parameter: carTotal - total amount of vehicles in garage
	OUTPUT:	 	 Return: character value of user choice
	CALLS TO:	 ValidateChoice	   
**************************************************************************/
char ActionMenu (int carTotal)
{
	//variable declaration
	char choice;						//rent/turn-in/exit
	bool valChar = false;				//true if choice is valid entry
	
	//all options if garage is not full or empty
	if ((carTotal > GARAGE_EMPTY) && (carTotal < GARAGE_FULL)){
		
		cout << " Please select from the following options: " 
			 << endl << endl
			 << " R - Rent a car" << endl
		 	 << " T - Turn in a Car" << endl
		 	 << " E - Exit the program" << endl << endl;
	}
	//no rent option from empty garage
	else if (carTotal == GARAGE_EMPTY){
		
		cout << " Please select from the following options: " << endl << endl
			 << " T - Turn in a Car" << endl
			 << " E - Exit the program" << endl << endl;
	}
	//no turn-in option from full garage
	else if (carTotal == GARAGE_FULL){
		
		cout << " Please select from the following options: " << endl << endl
			 << " R - Rent a car" << endl
			 << " E - Exit the program" << endl << endl;
	}
	//loop to validate user entry
	do{
		cout << " selection: ";
		cin >> choice;
		cin.sync();
		choice = toupper(choice);
		cout << endl;
		
		valChar = ValidateChoice (choice, carTotal);
		
	}while(!valChar);

	return choice;
}

/*************************************************************************
    FUNCTION:	SizeMenu 
    DESCRIPTION: Validates user entry for vehicle size
    INPUT:		 Parameter: garageInv - array of inventory totals
    						userSel - user choice to rent or turn-in
	OUTPUT:	 	 Return: entry - valid enumerated value of car size 	   
**************************************************************************/
carSize SizeMenu (rental garageInv, char userSel)
{
	//variable declaration
	char choice;						//mnemonic value of car size choice
	bool valChar = false;				//true if choice is valid entry
	carSize entry;						//valid entry to return

	if (userSel == 'R')
		cout << " Please select size of car to rent: " << endl << endl;
	else if (userSel == 'T')
		cout << " Please select size of car to turn in: " << endl << endl;
		
	//display options		 
	cout << " C - Compact" << endl
		 << " M - Mid-size" << endl
		 << " F - Full-size" << endl
		 << " S - SUV" << endl
		 << " R - Return to Main Menu" << endl << endl;
		 

	//loop to validate user input
	do{
		cout << " Selection: ";
		cin >> choice;
		cin.sync();
		choice = toupper(choice);
		cout << endl << endl;
	
		if (choice == 'C'){
			valChar = true;
			entry = COMPACT;
		}
		else if (choice == 'M'){
			valChar = true;
			entry = MIDSIZE;
		}
		else if (choice == 'F'){
			valChar = true;
			entry = FULLSIZE;
		}
		else if (choice == 'S'){
			valChar = true;
			entry =  SUV;
		}
		else if (choice == 'R'){
			valChar = true;
			entry = NONE;
		}
		else{
			cout << " Invalid choice: You must enter 'C', 'M', 'F', 'S', or 'R'"
			     << endl << endl;
		}
		
	}while(!valChar);

	return entry;
}
/*************************************************************************
    FUNCTION:	 ShowCars
    DESCRIPTION: Displays inventory on each level of selected car size
    INPUT:       Parameter: garageInv - array storing inventory info
							selection - car size selected by user
							userSel - user choice to rent or turn-in
	OUTPUT:		 Parameter: garageInv - updated array if change made
	CALLS TO:	 VerifyRental, VerifyReturn         	   				 
**************************************************************************/
void ShowCars (rental garageInv, carSize selection, char userSel)
{
	//variable declaration
	int colIdx,							//array column index (level)
		sum = 0;						//total inventory of car size in garage
	
	//display header for car size inventory
	cout << " Number of " << carType[selection] << " Cars Parked on each Level"
		 << endl << endl
		 << " Level: " << setw(10) << '0' << setw(4) << '1' << setw(4) << '2'
		 << setw(4) << '3' << setw(4) << '4' << setw(4) << '5' << setw(8)
		 << "Total" << endl << endl
		 << " " << left << setw(16) << carType[selection] << right; 
	
	//loop to display inventory on each level	 
	for (colIdx = 0; colIdx < MAX_LEVEL; colIdx++){
		 	
			 cout << garageInv [selection][colIdx] << setw(4);
			 
			 sum += garageInv [selection][colIdx];
	}	   
	
	cout << setw(8) << sum << endl << endl;
	
	//call functions to rent or turn-in
	if (userSel == 'T')
		VerifyReturn (garageInv, selection, sum);
	else if (userSel == 'R')
		VerifyRental (garageInv, selection, sum);
		
	return;
}

/*************************************************************************
    FUNCTION:	 VerifyRental
    DESCRIPTION: Verifies vehicle is available for rent, and updates array
    INPUT:       Parameter: garageInv - array storing inventory info
							selection - car size selected by user
							userSel - user choice to rent or turn-in
	OUTPUT:		 Parameter: garageInv - updated array if change made   	   				 
**************************************************************************/
void VerifyRental (rental garageInv, carSize selection, int sum)
{
	//constant declaration
	const int found = -1;				//value of level if vehicle is rented
	//variable declaration
	int level;							//column index of array (level)
	
	//unable to rent a vehicle if no vehicles of that size in garage
	if (sum == 0){
		cout << " Error: There are no cars available of selected size"
			 << " please choose another size" << endl << endl;
		return;
	}
	//loop to find highest level holding a vehicle
	for (level = 5; level >= 0; level--){
			
		if (garageInv [selection][level] > 0){
			
			garageInv [selection][level]--;
			sum--;
			
			cout << " Pick up car from " << carType [selection] << " row of"
				 << " level " << level << endl << ' ' << sum << ' ' 
				 << carType [selection] << " cars will now be available"
				 << endl << endl;
				 
			level = found;
		}	
	}
	
	system("pause");
	cout << endl << endl;
	return; 
}

/*************************************************************************
    FUNCTION:	 VerifyReturn
    DESCRIPTION: Verifies vehicle can be turned in allowing user to select
				 a level to return vehicle to, and updates array
    INPUT:       Parameter: garageInv - array storing inventory info
							selection - car size selected by user
							userSel - user choice to rent or turn-in
	OUTPUT:		 Parameter: garageInv - updated array if change made   	   				 
**************************************************************************/
void VerifyReturn (rental garageInv, carSize selection, int sum)
{
	//constant declaration
	const int MAX_INV = 30;					//car size max inventory
	//variable declaration
	int level = 0;						//column index of array (garage level)
	bool valid = false;					//true if valid level selected
	
	if (sum == MAX_INV){
		cout << " Error: Car Size entered is at max capacity" << endl
			 << " vehicle is being returned to wrong garage" << endl << endl;
		return;
	}
	
	cout << " You may park vehicle on any level with less than 5 cars" << endl;
		 
	do{	 
		//check for valid entry of level
		level = CheckLevel();
		//if level is not full return vehicle
		if (garageInv [selection][level] !=5){
			
			valid = true;
			garageInv [selection][level]++;
			sum++;
			
			cout << " Okay to park car in " << carType [selection] << " row"
				 << " of level " << level << endl << ' ' << sum << ' ' 
				 << carType [selection] << " cars will now be available" 
				 << endl << endl;
		}
		//have user choose another level if full
		else if (garageInv [selection][level] == 5){
			
			cout << " Error - Level " << level << " already contains 5 cars."
				 << endl << " Pick a level with less than 5 cars already"
				 << " parked" << endl << endl;
		}
		
	}while(!valid);
	

	system("pause");
	cout << endl << endl;
	return; 
}
/*************************************************************************
    FUNCTION:	 CheckLevel
    DESCRIPTION: Prompts reads and verifies valid integer input
    OUTPUT: 	 Return:  level - valid user entry  	 
**************************************************************************/
int CheckLevel()
{
	//constant declaration
	const int MIN = 0;							//lowest level available
	//variable declaration					
	bool valid = false;						//true if valid level selected
	int level;								//user selected level
	
	//loop until valid level is entered
	do{

		cout << " Select a level to park vehicle: ";
		 
		cin >> level;
		cout << endl << endl;
		
		if ((level >= MIN) && (level < MAX_LEVEL))
			valid = true;
		else{
			cout << " Error: must choose a level between 0 and 5"
				 << endl << endl;
		}
		
	}while(!valid);
	
	return level;
}
/*************************************************************************
    FUNCTION:	 ValidateChoice
    DESCRIPTION: Prompts reads and verifies valid character input
    INPUT		 Parameter:  choice - user choice to rent/turn-in/exit
    						 carTotal - total vehicles size in garage
    OUTPUT: 	 Return:  valChar - True if valid entry  	 
**************************************************************************/
bool ValidateChoice (char choice, int carTotal)
{
	//variable declaration
	bool valChar = false;					//true if valid entry
	
	//if there is room in garage
	if ((carTotal > GARAGE_EMPTY) && (carTotal < GARAGE_FULL)){
	
		switch (choice)
		{
			case 'R':
	         	valChar = true;
	       		break;
			case 'T':
	         	valChar = true;
	       		break;
	       	case 'E':
	         	valChar = true;
	         	break;
	       	default:
	         	cout << " Invalid entry: must enter 'R', 'T', or 'E'" << endl
	         		 << " You entered: " << choice << endl << endl;
				valChar = false;
				break;
		} //end switch
	}
	//no cars to rent if garage is empty
	else if (carTotal == GARAGE_EMPTY){
	
		switch (choice)
		{
			case 'R':
				cout << " Invalid entry: No cars to rent, choose Turn-In" 
					 << " or Exit" << endl << endl;
	         	valChar = false;
	       		break;
			case 'T':
	         	valChar = true;
	       		break;
	       	case 'E':
	         	valChar = true;
	         	break;
	       	default:
	         	cout << " Invalid entry: must enter 'R', 'T', or 'E'" << endl
	         		 << " You entered: " << choice << endl << endl;
				valChar = false;
				break;
		} //end switch
	}
	//no room to turn-in if garage is full
	else if (carTotal == GARAGE_FULL){
	
		switch (choice)
		{
			case 'R':
	         	valChar = true;
	       		break;
			case 'T':
				cout << " Invalid entry: Garage is full, choose Rent or Exit"
					 << endl << endl;
	         	valChar = false;
	       		break;
	       	case 'E':
	         	valChar = true;
	         	break;
	       	default:
	         	cout << " Invalid entry: must enter 'R', 'T', or 'E'" << endl
	         		 << " You entered: " << choice << endl << endl;
				valChar = false;
				break;
		} //end switch
	}
	
	return valChar;

}
/*************************************************************************
    FUNCTION:	 SaveFile
    DESCRIPTION: Saves array values (rowIdx, colIdx, garageInv) to binary
    			 file
    INPUT:       Parameter: saveInfo - output filestream variable
							garageInv - array storing data to write
	OUTPUT:		 File: Writes following rental info from each line of GARAGE.BIN
   					   rowIdx, colIdx, garageInv[][]
   					   Data on each line is formatted as follows
   					   binary character representation of an
   					   int int int	         	   				 
**************************************************************************/
void SaveFile (ofstream& saveInfo, rental garageInv)
{
	const string SAVE_FILE = "GARAGE.BIN";	//filename of customer text.doc
	
	int rowIdx = 0,
		colIdx = 0;
	
	//open file to write 
	//file has the following format:
	//rowIdx, colIdx, garageInv[][] as
	//int, int, int
	saveInfo.open(SAVE_FILE.c_str(), ios::binary | ios::out | ios::trunc); 
	
	for (rowIdx = 0; rowIdx < MAX_CAR_SIZE; rowIdx++){
 		
		 for (colIdx = 0; colIdx < MAX_LEVEL; colIdx++){
		 	saveInfo.write (reinterpret_cast<char*> 
			 			(&rowIdx), sizeof (int));
			saveInfo.write (reinterpret_cast<char*> 
			 				(&colIdx), sizeof (int));
			 				
			saveInfo.write (reinterpret_cast<char*> 
							(&garageInv [rowIdx][colIdx]), sizeof (int));
		 }
	}

	saveInfo.close();			//close file

	return;		   
	
}
	
