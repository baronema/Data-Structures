/*****************************************************************************
	CODE FILENAME: BaroneAssign6.cpp
	DESCRIPTION:   Program stores vehical information for a car rental company
				   neccessary to determine a vehicles current rental status.
				   The user will be prompted with the option to load a previous
				   file, or create one from new. The user is then given an 
				   options menu, which allows them to view which vehicles are 
				   currently on record, and what their mileage and rented status
				   are. The user will also have the option to add or delete 
				   vehicles from the record. After the user decides to exit the 
				   program they will be given the option to save any changes 
				   made to the data in a text file.
	CLASS/TERM:    CS362_XP41_Data Structures
	DESIGNER:	   Mark Barone
	FUNCTIONS	   main - opens text file, calls functions to complete tasks
				   AssignArrays - verifies and reads text file data into arrays
				   PromptUser - prompts user asking to load existing info
				   UserMenu - Show Cars, Add Cars, Delete Cars, Exit options
				   ShowCars - shows cars currently stored in array of records
				   AddCars - adds cars to array
				   ValidateLicense - checks that license# entered XX0000 format
				   VerifyMileage - verifies mileage is an integer
				   VerifyStatus - verifies status is a boolean value
				   DeleteCar - prompts user to enter license#, verifies input
				   DeleteEntry - finds record and deletes or notifies user
				                 that the record does not exist 
				   ExitProgram -  prompts user to save changes
				    
*****************************************************************************/
#include <iostream>				//for I/O 
#include <iomanip>				//for formatting output
#include <fstream>				//for reading from text file
#include <cctype>				//for character checking
#include <string>				//for string checking
#include <cstddef>				//for definition of null

using namespace std;

//Global constants
const int EXIT = 0,				//return value for user selected exit
	      CONT = 1,			    //return value for continue
	      LOAD = 1,				//return value for load previous
	      NEW = 0,				//return value for create new
	      DELETED = 0,			//return value for record deleted
	      NO_REC = 1,			//return value for no record found
	      NOT_FOUND = -1;       //value of user license number not found 
	      
//Global structure definition	
struct rentalRec {
    string phone;
    float rent;
    bool status;
    rentalRec* next;
};
  

//output prompts to user
const string INTRO_1 = "          OverPriced Rentals",
	         INTRO_2 = " Rental Car Inventory Management System ",
	         EXIT_1 = " Would you like to save your changes? (Enter: Y/N) ",
	         EXIT_2 = " WARNING: This will overwrite your existing file: ",
	         LOAD_PROMPT = " Do you wish to load a previous file ('Y' or 'N')",
	         SHOW_CAR_1 = " License",
	         SHOW_CAR_2 = " Number     Mileage     Rented ",
	         SHOW_CAR_3 = " ------     --------    ------ ",
	         SHOW_CAR_4 = " Number ",
	         SHOW_CAR_5 = " ------ ",
	         ERROR_1 = " Invalid entry: Please enter 'Y' to load, 'N' to exit ",
	         ERROR_2 = ,
	         ERROR_3 = " There are no vehicles on record ",
	         ERROR_4 = " Warning: Record is full, no further storage allowed ",
	         ERROR_5 = " License number: ",
	         ERROR_6 = " was not found",
	         ERROR_7 = " Invalid entry: License # must begin with 2 letters ",
	         ERROR_8 = " Invalid entry: License # must be 2 letters followed "
	                   " by 4 digits. ",
	         ERROR_9 = " Invalid entry: License # can only be 6 digits long ",
	         ERROR_10 = " Invalid entry: License # cannot contain a space ",
	         ERROR_11 = " Invalid entry: Mileage must be a number > zero ",
	         ERROR_12 = " Invalid entry: Enter 'Y' for yes, or 'N' for no",
	         ERROR_13 = " Invalid entry: Enter 'Y' to save, 'N' to exit ",
	         ERROR_14 = " Error opening input data file ",
	         ERROR_15 = " Enter <0> to exit, or <1> to create new file: ",
	         MENU_1 = ,
	         MENU_2 = ,
	       
	         LIC_PROMPT = " Enter a license number in ( XX1111 ) format: ",
	         MIL_PROMPT = " Enter mileage: ",
	         STA_PROMPT = " Is the car currently rented (ENTER: Y or N): ",
	         SHOW_ENTRY = " You entered: ";
	         




//Prototypes
void PromptUser (string& custFile, int& userChoice);
int SetLinkedList (ifstream& rentalInfo, rentalRec store[], int& carCount);
void UserMenu (rentalRec[], int& carCount);
void ShowCars (rentalRec[], int carCount);
void AddCars (rentalRec[], int& carCount);
void DeleteCars (rentalRec[], int& carCount);
void IncreaseMiles (rentalRec[], )
void ExitProgram (rentalRec[], int carCount);
string ValidateLicense ();
int VerifyMileage ();
char VerifyStatus ();
void DisplayLicense (rentalRec store[], int carCount);
int DeleteEntry (rentalRec store[], int& carCount, string userDelete);

/*****************************************************************************
   FUNCTION: Main
   
   DESCRIPTION: Calls function to determine if users would like to load existing
                file, or create new. Then main calls functions to create
                record of array, as well as calls a function to give the user
                a list of options to select
   OUTPUT:	Return Value:  EXIT on exit
			
   CALLS TO: AssignArrays, PromptUser, UserMenu, ExitProgram
****************************************************************************/
int main()
{


	int fileCheck = 1;				//value representing file open success
	int carCount = 0;				//number of cars in the array
	int loadPrev = 0;
 	int userChoice = 0;				//
	
	rentalRec* listTop = NULL;

	
	PromptUser(custFile, userChoice);
	
	if(userChoice == LOAD){
		LoadFile(listTop);
	}

	
	UserMenu (localStore, carCount);
	
	ExitProgram (localStore, carCount);
	

	return EXIT;
	
 }

/*************************************************************************
  FUNCTION:	 PromptUser
  DESCRIPTION: Asks user if they would like to load an existing file.						   
  OUTPUT: 	 Parameter:	userChoice - value of user entry		 
**************************************************************************/
void PromptUser ()
{
	char userInp = ' ';				//value of user input
	
	
	cout << " Happy Apartments Rental Management System" << endl << endl
		 << " Would you like to load from a previous file (Enter: Y or N): "
	
	do{
		cin >> userInp;
		cin.ignore(100, '/n');
		userInp = toupper(userInp);
	
		if (userInp == 'Y')
			userChoice = LOAD;
		else if (userInp == 'N')
			userChoice = NEW;
		else
			cout << endl << endl << " Error: you entered: " << userInp
				 << " please select Y or N to continue: ";
			
	}while ((userInp != 'Y') && (userInp != 'N'));
	
	return;
}
/*************************************************************************
  FUNCTION:	 LoadFile
  DESCRIPTION: Validates text file opens, reads data from file and assigns
			   data to an array
  Input:	File: 	Reads following vehicle info from each line of RENTALS
   					license number, mileage, rental status
   					Data on each line is formatted as follows
   					xx1234 1234 0				   
  OUTPUT: 	 Return value: 1 - if error opening file
			 Return value: 0 - if success
			 Parameter: carCount - number of customers stored in sorted file
			 Parameter: store - vehicle information from file
**************************************************************************/
void LoadFile (rentalRec* &listTop)
{
	ifstream rentalInfo;
	string file;
	string tempNum;
	float tempRent;
	bool tempSta;
	char choice = 'N';
	rentalRec *newRec;

	do{	
		cout << " Choose a text file to open (formatted as 'RENTAL.TXT'): ";
		cin >> file;
		cout << endl << endl;
	
		rentalInfo.open(file.c_str());
		
		rentalInfo >> tempNum;
		rentalInfo >> tempRent;
		rentalInfo >> tempSta;
		
		if(!rentalInfo){
			cout << " Error: File " << string << " not found" << endl;
				 << " Would you like to choose another file? (Y or N): ";
			choice = validChoice();	
			rentalInfo.clear();
		}
	}while((!rentalInfo) && (choice == 'Y'));

	while((rentalInfo) && (newRec != NULL)){
		
		newRec = new (notthrow) rentalRec;
		
		if (newRec = NULL){
			cout << " Heap Error -- out of heap memory!" << endl
				 << " Cannot allocate space" << endl << endl;
		}
		else{
			newRec->phone = tempNum;
			newRec->rent = tempRent;
			newRec->status = tempSta;
			newRec->next = NULL;
			
			listTop = insertOrderedNode(listTop, newRec);
			
			rentalRec.ignore(100, '/n');
			rentalInfo >> tempNum;
			rentalInfo >> tempRent;
			rentalInfo >> tempSta;
		}
		
	}
	
	rentalInfo.close();
	
	return;
     
		
}
//*******************************************************************
//   insertOrderedNode inserts nodes into the list, ordered by age.  
//   newNode points to a new node containing data, 
//           including a NULL next pointer
//   Returns a pointer to the top of the list, in case it was modified.
//*******************************************************************
student* insertOrderedNode (rentalRec *listTop, rentalRec *newNode)
{        
  student *prev, 	       // New node will be inserted after prev
          *here;	       // New node will be inserted before here
                           // i.e. insert new node between prev and here

  if (listTop == NULL)
      listTop = newNode;   //  New node is first node in previously empty list

  else  {                  //  listTop != NULL (nodes in list already)

      if ( newNode->phone < listTop->phone ) {   // Node will be new top node
           newNode->next = listTop;          // Insert at top of list
           listTop = newNode;
      }  // end if
      
      else {						           
            // Find where to insert
            prev = listTop;			  
            here = listTop->next;	

            while ((here != NULL) && (newNode->phone > here->phone)) {
                                   // while not at end of list and
                                   // new node's age > age of node here points to
                prev = here;
                here = here->next; // advance both pointers forward one node
            }  // end while

            // Insert into list at found spot
            newNode->next = prev->next;	
            prev->next = newNode;
      }     // end else (find where to insert)
            
   }  // end else (listTop != NULL)
         
   return listTop;
}

	
/*************************************************************************
  FUNCTION: 	UserMenu
  DESCRIPTION:  Function gives user 4 options to choose, then calls required
  				functions to carry out subsequent tasks
  INPUT:        Parameter:    store - array to be read 	
							  carCount - number of vehicles stored in array
  CALLS TO: ShowCars, AddCars, DeleteCar 			   				 
**************************************************************************/
void UserMenu (rentalRec store[], int& carCount)
{
	char userEntry = ' ';
	
	do{

		cout << " Choose from the following options: " << endl << endl 
			 << "  |   MENU OPTION      | SELECT " << endl << endl 
			 << "  |   Show Rentals     |    S" << endl 
			 << "  |   Add Rentals      |    A" << endl 
			 << "  |   Delete Rentals   |    D" << endl 
			 << "  |   Modify Rentals   |    M" << endl
			 << "  |   Exit Program     |    E" << endl << endl 
			 << " Make your selection: ";
		cin >> userEntry;
	
		userEntry = toupper(userEntry);
	
		if (userEntry == 'S')
			ShowRentals (listTop);
		else if (userEntry == 'A')
			AddRentals (listTop);
		else if (userEntry == 'D')
			DeleteRentals (listTop);
		else if (userEntry == 'M')
			ModifyRentals (listTop);
		else if (userEntry == 'E')
			return;
		else{
			cout << " Invalid entry: Must enter 'S', 'A', 'D', 'M', or 'E': "
				 << endl << endl;
		}
		
	}while (userEntry != 'E');	
}
				
/*************************************************************************
    FUNCTION:	 ShowCars
    DESCRIPTION: Checks that array is not empty, then displays all vehicles 
    			 along with their rental status.
    INPUT:       Parameter:    store - array to be read
							   carCount - number of vehicles in array			   
**************************************************************************/
void ShowRentals (rentalRec *listTop)
{
  	rentalRec* tempPtr = listTop;
  
	if (tempPtr == NULL){
  		cout << " No apartments rentals are stored " << endl << endl;
  		
  		return;
	}
    cout << "                 Monthly" << endl;
	cout << " Phone Number       Rent     Status" << endl;
	cout << " -------------  --------     ---------" << endl;

	while (tempPtr != NULL) {
    	cout << setw(12) << tempPtr->phone;
    	cout << setw(5) << tempPtr->rent;
    	cout << setw(6) << tempPtr->status;

    	cout << endl;
    	tempPtr = tempPtr->next;
    }   // end while
      
	cout << endl << endl;
	return;
}

/*************************************************************************
    FUNCTION:	 AddCars
    DESCRIPTION: Checks that array is not full. Then calls functions to verify
                 input of array fields. After each function call returns a 
                 verified valued the new entries are added to the record
    INPUT:       Parameter:    carCount - number of array entries			   
    OUTPUT: 	 Parameter:    store - adds record if array not empty
							   carCount - adds one if vehicle added	 
	CALLS TO:    ValidateLicense, VerifyMileage, VerifyStatus
**************************************************************************/
void AddRentals (rentalRec* &listTop)
{
	rentalRec *newRental;
	string newNum;
	float newRent;
	bool newStat;
	char entry;
	
		
	do{
		newRental = new (notthrow) rentalRec;
		
		if (newRental = NULL){
			cout << " Heap Error -- out of heap memory!" << endl
				 << " Cannot allocate space" << endl << endl;
		}
		else{
			
			newNum = VerifyPhone();
			newRent = VerifyRent();
			newStat = VerifyStatus();
			

			newRental->phone = tempNum;
			newRental->rent = tempRent;
			newRental->status = tempSta;
			newRental->next = NULL;
			
			insertOrderedNode(listTop, newRental);
		}
		
		cout << " Would you like to add another? ('Y' or 'N'): ";
		
		entry = ValidChoice();
		
	}while(entry != 'N');
	
	return;
}

/*************************************************************************
     FUNCTION:	 DeleteCars
    DESCRIPTION: Checks that array is not empty. Then calls functions to verify
                 license. DeleteCars then calls another function to search and
                 delete the record.
    INPUT:       Parameter:    carCount - number of array entries	
	CALLS TO:    DisplayLicense, ValidateLicense, DeleteEntry		   
**************************************************************************/
void DeleteRentals (rentalRec* &listTop)
{
	string userDelete = " ";			//validated license# to search/delete
	rentalRec* tempPtr;
	int counter = 0;
	

	tempPtr = listTop;
  
	if (tempPtr == NULL){
  		cout << " There are no rentals to delete " << endl << endl;
  		
  		return;
	}
	
	cout << " Phone Numbers" << endl;
	cout << " -------------" << endl;

	while (tempPtr != NULL) {
    	cout << ' ' << tempPtr->phone
    	counter++;
    	
    	if (counter == 5){
    		counter = 0;
    		cout << endl;
    	}
    	tempPtr = tempPtr->next;
    }   // end while
	
	userDelete = ValidateLicense ();
	
	DeleteNode (listTop, userDelete);
	
	return;
	
}
/*************************************************************************
    FUNCTION:	 DeleteEntry
    DESCRIPTION: Searches array for target license#, and deletes record
    			 if found.
    INPUT:       Parameter:     store - array to read
								carCount - total vehicles in array
								userDelete - users entry
	OUTPUT:		 Parameter:     store - deletes record if found
			                    carCount - minus one if record deleted
				 Return Value:  NO_REC - record not found
				 				DELETED - record was deleted			   				 
**************************************************************************/
void DeleteNode (rentalRec* &listTop, string userDelete)
{
	rentalRec *here,                           // Node being checked
              *prev;                           // Node prior to node being checked


	if ( listTop->phone == userDelete ) {        // If delName found in first node,
    	here = listTop;
        listTop = listTop->next;		      //  Delete top item
        delete here;
        cout << " Apartment using phone# " << userDelete << " was deleted"
        	 << endl << endl;
    }

	else {                                   // Not top item, so search...

    	prev = listTop;                      // Initialize prev and here to 
        here = listTop->next;                // first 2 nodes in list

        // Look for delName (while not end of list and name doesn't match)
        while ((here != NULL) && (here->phone != userDelete)) {   
        	prev = here;               // move prev down one
            here = here->next;         // move here down one
       	}

        if (here != NULL) {               // If found, delete item from list
            prev->next = here->next;
            delete here;
            cout << " Apartment using phone# " << userDelete << " was deleted"
        	 << endl << endl;
        }
        else                              // Otherwise, give error message
            cout << userDelete << " not found in list." << endl;

    }	// End else (not top Item)

	return;
			   
	
}


/*************************************************************************
  FUNCTION:	 IncreaseMiles ()
  DESCRIPTION: Validates text file opens, reads data from file and assigns
			   data to an array
  Input:	File: 	Reads following vehicle info from each line of RENTALS
   					license number, mileage, rental status
   					Data on each line is formatted as follows
   					xx1234 1234 0				   
  OUTPUT: 	 Return value: 1 - if error opening file
			 Return value: 0 - if success
			 Parameter: carCount - number of customers stored in sorted file
			 Parameter: store - vehicle information from file
**************************************************************************/
void ModifyRentals (rentalRec *listTop)
{
	
	const string MOD_FILE = "MODS.TXT";
	ifstream rentalMod;
	rentalRec tempRec;
	string tempNum;
	float tempRent;
	int oneChange = 0;
	bool changed = false;
	
	tempRec = listTop->next;
	
	if(listTop == NULL){
		cout << " There are no apartment rentals that match" << end << endl;
		
		return;
	}	
	
	rentalMod.open(MOD_FILE.c_std());
	
	rentalMod >> tempNum;
	rentalMod >> tempRent;
	rentalMod.ignore(100, '/n');
	
	if(!rentalMod){
		cout << " Error: No mod file found, no changes have been made"
			 << endl << endl;
			 
		return;
	}
	
	while(rentalMod){
		
		if ( listTop->phone == tempNum ) {        // If delName found in first node,
    		listTop->rent = tempRent;
    		oneChange++;
    		changed = true;
    	}
    	else{
    		while ((tempRec != NULL) && (tempRec->phone != tempNum)){
    			tempRec = tempRec->next;
    		}
    		if (tempRec != NULL){
    			tempRec->rent = tempRent;
    			oneChange++;
    			changed = true;
    		}
    	}
    	
    	if(oneChange == 1){
    		cout << " Phone Number     New Monthly Rent" << endl
    			 << " ------------     ----------------" << endl;
    	}
    	
    	if(changed){
    		cout << ' ' << tempRec->phone << setw(22) << tempRec->rent << endl;
			changed = false; 
    	}
    	
    	tempRec = listTop->next;
    	rentalMod >> tempNum;
    	rentalMod >> tempRent;
    	rentalMod.ignore(100, '/n');
	}
	
	if(oneChange == 0)
		cout << " No monthly rents have been modified" << endl << endl;
	
	return;
		
}
	

/*************************************************************************
    FUNCTION:	 ExitProgram
    DESCRIPTION: Asks user if they would like to save any changes. 
    INPUT:       Parameter:     store - data to be written to text
							    carCount - number of cars in the array
	OUTPUT:		 File:	Writes following vehicle info from each index of store
   					    license plate, mileage, rented status
   					    Data on each line is formatted as follows
   				    	xx1111 1111 1			   				 
**************************************************************************/
void ExitProgram (rentalRec *listTop)
{	
	ofstream updAptInfo;				//output file stream variable
	string phone;					//social security number
	       fileName;
	float rent;			//credit rating index
	int status;
	char userSel;
	char overWrite = 'N';
	rentalRec *nextNode;
	
	nextNode = listTop;
	
	
	cout << " Would you like to save apartment data to file? ('Y' or 'N')";
	
	userSel = ValidChoice();
	
	if(userSel == 'N')
		return;
	
	while(overWrite == 'N'){
		cout << " Enter filename to save to, must be .TXT file type" << endl
			 << " Formatted as (RENTALS.TXT): ";
		cin >> fileName;
		
		updAptInfo.open(fileName.c_str()); //open file to write
		
		if(updAptInfo){
			cout << " Warning: Filename " << fileName << " already exists" 
				 << endl << " you are about to write over file, would "
				 << " you like to overwrite ('Y' or 'N'): ";
			overWrite = ValidChoice; 
		}
	}
		
	if((!updAptInfo) || (overWrite == 'Y')){
		 do{
		 
		 	phone = nextNode->phone;
		 	rent = nextNode->rent;
		 	status = nextNode->status;
		 
		 	updAptInfo << phone << ' ' << rent << ' ' << status << endl;
		 
		 	nextNode = nextNode->next;	 
		}while(nextNode != NULL);
	}
	
	return;
}

/*************************************************************************
    FUNCTION:	 ValidateLicense
    DESCRIPTION: Verifies that user input for license conforms to formatting
    			 requirements. Loops until valid entry is read.
    OUTPUT:     return value: inpLic - validated string value 			   				 
*************************************************************************/
string ValidateLicense ()
{
	string inpLic;
	const int invalid = 13;					//break loop if invalid entry

	int entryIdx;							//index position of string to check
	bool validEntry = false;				//represents user entry is in/valid
	char ch1;

	getline(cin, inpLic);
	
	while (!validEntry){
		
		cout << endl << LIC_PROMPT;
	    getline(cin, inpLic);
		cout << endl;
	
		for (entryIdx = 0; entryIdx <= 6; entryIdx ++){
				
				if (entryIdx < 2)				//check first two char are letters
					if (isalpha(inpLic[entryIdx])){	//continue if valid entry
						validEntry = true;
						inpLic[entryIdx] = toupper (inpLic[entryIdx]);
					}
					else{						//prompt user to enter valid
						validEntry = false;
						cout << ERROR_7 << endl << SHOW_ENTRY << inpLic << endl;
						entryIdx = invalid;
					}	
			
				if ((entryIdx >= 2) && (entryIdx < 6))	//checks last 4 are numbers
					if (isdigit(inpLic[entryIdx]))			//continue if valid
						validEntry = true;
					else{								//prompt user to enter valid
						validEntry = false;
						cout << ERROR_8 << endl << SHOW_ENTRY << inpLic << endl;
						entryIdx = invalid;
					}
				if (entryIdx == 6){				//test character after max digit
					if (isblank(inpLic[entryIdx])){
						validEntry = true;
						return inpLic;
					}
					else if (isalnum(inpLic[entryIdx])){
						validEntry = false;
						cout << ERROR_9 << endl << SHOW_ENTRY << inpLic << endl;
						entryIdx = invalid;
					}
				}	
		}
	
		cout << endl;
	}
}

/*************************************************************************
    FUNCTION:	 VerifyMileage
    DESCRIPTION: Prompts user to input vehicle mileage, verfies input is pos# 					   
	OUTPUT:		Return Value:	inpMil - verified user input 
 			   				 
**************************************************************************/
int VerifyMileage ()
{
	 int inpMil = 0;
	 bool verified = false;
	 
	 while (!verified){
	 	
	 	cout << MIL_PROMPT;
	 	cin >> inpMil;
	 	cout << endl;
	 
		if (inpMil >= 1){
			verified = true;
	 		return inpMil;
	 	}
	 	else{
	 		cout << ERROR_11 << endl << SHOW_ENTRY << inpMil << endl;
	 		inpMil = 0;
	 	}
	 }
}
/*************************************************************************
    FUNCTION:	 VerifyStatus
    DESCRIPTION: Determines whether vehicle to add is currently rented
    			 Calls: 	   DisplayResults - to display rating of selection
    OUTPUT:      Return Value:  inpSta - validated user entry 			   				 
**************************************************************************/
char VerifyStatus ()
{
	char inpSta = ' ';
	
	do{
		cout << STA_PROMPT;
		cin >> inpSta;
		cout << endl;
		
		inpSta = toupper(inpSta);
		
		if (inpSta == 'Y')
			return inpSta;
		else if (inpSta == 'N')
			return inpSta;
		else
			cout << ERROR_12 << endl << SHOW_ENTRY << inpSta << endl;
		
	}while ((inpSta != 'Y') || (inpSta != 'N'));
}



