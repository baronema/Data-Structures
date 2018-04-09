/*****************************************************************************
	CODE FILENAME: BaroneAssign6.cpp
	DESCRIPTION:   Program stores rental information for an apartment building
				   and includes phone number associated with rental, rent cost,
				   and current rental status. Files can be opened and saved 
				   by user defined filenames. 
	CLASS/TERM:    CS362_XP72_Data Structures
	DESIGNER:	   Mark Barone
	FUNCTIONS	   main - calls functions to complete program tasks
				   PromptUser - user option to load file
				   LoadFile - load save file, populates linked list w/ data
				   insertOrderedNode - creates list nodes, in ascending order
				   UserMenu - options to Show/Add/Delete/Modify/Exit
				   ShowRentals - displays all nodes in list
				   AddRentals - adds node to list
				   DeleteRentals - searchs for valid node to delete
				   ModifyRentals - modifies price of rent change from mod file
				   ExitProgram - saves list nodes to file of user choice
			       DeleteNode - removes valid node from list
				   ValidatePhone - validates phone number entry 
				   ValidateRent - validates cost of rent is whole number
				   ValidateStatus - validates rental status
				   ValidChoice - returns valid Yes or No input to continue
				    
*****************************************************************************/
#include <iostream>				//for I/O 
#include <iomanip>				//for formatting output
#include <fstream>				//for reading from text file
#include <cctype>				//for character checking
#include <string>				//for string checking
#include <cstddef>				//for definition of null

using namespace std;
	      
//Global structure definition	
struct rentalRec {
    string phone;
    float rent;
    bool status;
    rentalRec* next;
};
  
//Prototypes
char PromptUser ();
void LoadFile (rentalRec* &listTop);
void InsertNode (rentalRec* &listTop, rentalRec *newNode);
void UserMenu (rentalRec* &listTop);
void ShowRentals (rentalRec *listTop);
void AddRentals (rentalRec* &listTop);
void DeleteRentals (rentalRec* &listTop);
void ModifyRentals (rentalRec *listTop);
void ExitProgram (rentalRec *listTop);
void DeleteNode (rentalRec* &listTop, string userDelete);
string ValidatePhone ();
float ValidateRent ();
bool ValidateStatus ();
char ValidChoice ();

/*****************************************************************************
   FUNCTION: 	Main
   DESCRIPTION: Allocates memory for listTop node, calls functions to carry
   				out tasks
   OUTPUT:		Return Value:  0 on exit			
   CALLS TO: 	PromptUser, UserMenu, ExitProgram
****************************************************************************/
int main()
{

	//variable declarations
 	char choice;				//'Y' if user chooses to load file
	rentalRec* listTop;			//first node in list
	listTop = NULL;				//indicate list is empty
	
	choice = PromptUser();
	
	if(choice == 'Y'){
		LoadFile(listTop);
	}

	UserMenu (listTop);
	
	ExitProgram (listTop);	

	return 0;
	
 }

/*************************************************************************
  FUNCTION:	 PromptUser
  DESCRIPTION: Asks user if they would like to load an existing file.						   
  OUTPUT: 	 Returns: userInp - choice to load file		 
**************************************************************************/
char PromptUser ()
{
	char userInp = ' ';				//validated user input
	
	
	cout << " Happy Apartments Rental Management System" << endl << endl
		 << " Would you like to load from a previous file (Enter: Y or N): ";
	
	do{
		cin >> userInp;
		cin.ignore(1000, '\n');
		userInp = toupper(userInp);
		
		cout << endl;
	
		if (userInp == 'Y')
			return userInp;
		else if (userInp == 'N')
			return userInp;
		else
			cout << endl << endl << " Error: you entered: " << userInp
				 << " please select Y or N to continue: ";
			
	}while ((userInp != 'Y') && (userInp != 'N'));
	
}
/*************************************************************************
  FUNCTION:	 LoadFile
  DESCRIPTION: Validates text file opens, reads data from file and assigns
			   data to linked list
  Input:	File: 	Reads following rental info from each line of selected file
   					phone number, rent, status
   					Data on each line is formatted as follows
   					string, float, bool
   					111-111-1111 1234.56 0				   
  OUTPUT: 	 Parameter: listTop - pointer to top node of linked list, in case
  								  it is modified
  CALLS TO:  ValidChoice, InsertNode
**************************************************************************/
void LoadFile (rentalRec* &listTop)
{
	//constant declaration
	const string EXT = ".TXT";
	//variable declarations
	ifstream rentalInfo;			//input file stream
	string file,					//filename input by user
		   tempNum;					//phone number read from file
	float tempRent;					//rent value read from file
	bool tempSta,					//rent status read from file
		 found = false;				//true if file found
	char choice = 'N';				//used to search another file 
	int len = 0,					//character length of filename
		idx;						//index for string character checking
	rentalRec *newRec;				//temporary pointer to store file input

	//while file not found, and more files to search
	do{	
		cout << " Choose a text file to open, do not specify filetype " << endl
			 << " Formatted as (RENTAL): ";
		cin >> file;
		cout << endl << endl;
		
		file += EXT;
		
		len = file.length();
		
		for (idx = 0; idx <= len; idx++)
			file[idx] = toupper(file[idx]);
		
		//open file to read 
		//file has the following format:
		//phone, rent, status
		//string, float, bool	
		rentalInfo.open(file.c_str());
		
		//priming read
		rentalInfo >> tempNum;
		rentalInfo >> tempRent;
		rentalInfo >> tempSta;
		
		//exit loop if file found
		if(rentalInfo)
			found = true;
		
		//allow user to search another
		if(!rentalInfo){
			cout << " Error: File " << file << " not found" << endl
				 << " Would you like to choose another file? (Y or N): ";
			choice = ValidChoice();	
			rentalInfo.clear();
		}
		
	}while((!found) && (choice == 'Y'));

	//loop if file data to read
	while((rentalInfo) && (newRec != NULL)){
		
		//allocate memory for new node
		newRec = new (nothrow) rentalRec;
		
		//if heap is full
		if (newRec == NULL){
			cout << " Heap Error -- out of heap memory!" << endl
				 << " Cannot allocate space" << endl << endl;
		}
		//store data to new node
		else{
			newRec->phone = tempNum;
			newRec->rent = tempRent;
			newRec->status = tempSta;
			newRec->next = NULL;
			
			InsertNode(listTop, newRec);
			
			rentalInfo.ignore(100, '\n');
			rentalInfo >> tempNum;
			rentalInfo >> tempRent;
			rentalInfo >> tempSta;
	
		}
		
	}
	
	rentalInfo.close();
	
	
	return;
     
		
}
/*******************************************************************
   Function: InsertNode 
   Description: inserts nodes into the list, ordered by phone number.  
   INPUT:	Parameters: newNode - points to a new node containing data 
                                 including a NULL next pointer
                       listTop - points to top of list
   OUTPUT:  Parameter: listTop -  a pointer to the top of the list, 
								   in case it was modified.
********************************************************************/
void InsertNode (rentalRec* &listTop, rentalRec *newNode)
{        
  rentalRec *prev, 	       // New node will be inserted after prev
            *here;	       // New node will be inserted before here
                           // i.e. insert new node between prev and here

  //if list is empty
  if (listTop == NULL){
      listTop = newNode;   //  New node is first node in previously empty list
  }

  else{                  //  listTop != NULL (nodes in list already)
	  //if new node smaller than first node
      if ( newNode->phone < listTop->phone ){   
           newNode->next = listTop;          
           listTop = newNode;			//new node is first node in list
      }  
      
      else{						           
            // Find where to insert
            prev = listTop;			  
            here = listTop->next;	
			//search list until new node phone is in order
            while ((here != NULL) && (newNode->phone > here->phone)){
                                
                prev = here;
                here = here->next; // advance both pointers forward one node
            } 
            //end of list
            if(here == NULL){
            	newNode->next = prev->next;	
            	prev->next = newNode;
            }
			//do not store duplicate phone numbers
			else if((newNode->phone != prev->phone) && 
				(newNode->phone != here->phone))
            {
				// Insert into list at found spot
            	newNode->next = prev->next;	
            	prev->next = newNode;
            }
            //explain if duplicate number is skipped
            else if ((newNode->phone == prev->phone) ||
						(newNode->phone == here->phone) ||
							(newNode->phone == listTop->phone))
            {
				cout << " Error: duplicate phone number entered, entry ignored"
            		 << endl << endl;
            }
      }     
            
  }  
     
   return;
}

	
/*************************************************************************
  FUNCTION: 	UserMenu
  DESCRIPTION:  Function gives user 4 options to choose, then calls appropriate
  				functions to carry out tasks
  INPUT:		Parameter:  listTop - pointer to top node in list
  OUTPUT:       Parameter:  listTop - pointer to top of list if value is changed
  CALLS TO: ShowRentals, AddRentals, DeleteRentals, ModifyRentals,  			   				 
**************************************************************************/
void UserMenu (rentalRec* &listTop)
{
	char userEntry = ' ';			//valid user menu choice
	
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
		cin.ignore(1000, '\n');
		
		cout << listTop->next->phone ;
		
		cout << endl << endl;
	
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
    FUNCTION:	 ShowRentals
    DESCRIPTION: Checks that list is not empty, then displays all rentals 
    			 in list along with their rental status.
    INPUT:       Parameter:  listTop - pointer to top of list			   
**************************************************************************/
void ShowRentals (rentalRec *listTop)
{
	//local constant for string output of status
	const string rentSta[] = {"available", "rented"};
	//local variable
  	rentalRec* tempPtr = listTop;		//temp pointer set to top node in list
  
	if (tempPtr == NULL){
  		cout << " No apartments rentals are stored " << endl << endl;
  		
  		return;
	}
	
    cout << "                 Monthly" << endl;
	cout << " Phone Number       Rent     Status" << endl;
	cout << " -------------  --------     ---------" << endl;
	cout << fixed << setprecision(2);
	
	while (tempPtr != NULL) {
    	cout << setw(13) << tempPtr->phone;
    	cout << setw(11) << tempPtr->rent;
    	cout << setw(5) << ' '; 
    	cout << rentSta[tempPtr->status];

    	cout << endl;
    	tempPtr = tempPtr->next;
    }  
      
	cout << endl << endl;
	return;
}

/*************************************************************************
    FUNCTION:	 AddRentals
    DESCRIPTION: Checks that list is not full. Then calls functions to verify
                 variable input. Each function call returns a 
                 valid valued to create a new node
    INPUT:       Parameter:    listTop - points to top of list			   
    OUTPUT: 	 Parameter:    listTop - pointer to top of list, if case it is 
									     modified	 
	CALLS TO:    ValidatePhone, ValidateRent, ValidateStatus, ValidChoice
				 InsertNode
**************************************************************************/
void AddRentals (rentalRec* &listTop)
{
	//variable declarations
	rentalRec *newRental;				//new node
	string newNum;						//phone number to store
	float newRent;						//rent to store
	bool newStat;						//status to store
	char entry;							//option to continue adding
	
	//loop until user decides not to add more	
	do{
		newRental = new (nothrow) rentalRec;
		
		if (newRental == NULL){
			cout << " Heap Error -- out of heap memory!" << endl
				 << " Cannot allocate space" << endl << endl;
		}
		else{
			
			newNum = ValidatePhone();
			newRent = ValidateRent();
			newStat = ValidateStatus();

			newRental->phone = newNum;
			newRental->rent = newRent;
			newRental->status = static_cast<bool>(newStat);
			newRental->next = NULL;
			
			InsertNode(listTop, newRental);
		}
		
		cout << " Would you like to add another? ('Y' or 'N'): ";
		
		entry = ValidChoice();
		
	}while(entry != 'N');
	
	return;
}

/*************************************************************************
     FUNCTION:	 DeleteRentals
    DESCRIPTION: Checks that list is not empty. Then calls functions to verify
                 phone num. DeleteCars then calls another function to 
                 delete the node.
    INPUT:       Parameter:  listTop - pointer to top of list
	OUTPUT:      Parameter:  listTop - pointer to top of list if change occurs	
	CALLS TO:    ValidatePhone, DeleteNode		   
**************************************************************************/
void DeleteRentals (rentalRec* &listTop)
{
	string userDelete = " ";			//valid phone number to search/delete
	rentalRec* tempPtr;					//temp pointer for searching nodes
	int counter = 0;					//formatting counter for phone# output
	
	//begin search at first node in list
	tempPtr = listTop;
  
	if (tempPtr == NULL){
  		cout << " There are no rentals to delete " << endl << endl;
  		
  		return;
	}
	
	cout << " Phone Numbers" << endl;
	cout << " -------------" << endl;

	while (tempPtr != NULL) {
    	cout << ' ' << tempPtr->phone << ',';
    	counter++;
    	
    	//new line after every five numbers are displayed
    	if (counter == 5){
    		counter = 0;
    		cout << endl;
    	}
    	tempPtr = tempPtr->next;
    } 
	
	//validate user entry to delete
	userDelete = ValidatePhone ();
	//search/delete valid phone number
	DeleteNode (listTop, userDelete);
	
	return;
	
}
/*************************************************************************
    FUNCTION:	 DeleteNode
    DESCRIPTION: Searches list for target phone#, and deletes node
    			 if found.
    INPUT:       Parameter:     listTop - pointer to top of list
								userDelete - phone number to delete
	OUTPUT:		 Parameter:     listTop - pointer to top of list in case value
										  is changed			   				 
**************************************************************************/
void DeleteNode (rentalRec* &listTop, string userDelete)
{
	rentalRec *here,                        // Node being checked
              *prev;                        // Node prior to node being checked


	if ( listTop->phone == userDelete ) {    // If delName found in first node,
    	here = listTop;
        listTop = listTop->next;		     //  Delete top item
        delete here;
        cout << " Apartment using phone# " << userDelete << " was deleted"
        	 << endl << endl;
    }

	else {                                   //search

    	prev = listTop;                      // Initialize prev and here to 
        here = listTop->next;                // first 2 nodes in list

        //not end of list, and number not found
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

    }

	return;
			   
	
}


/*************************************************************************
  FUNCTION:	 ModifyRentals
  DESCRIPTION: Opens modification text file, and updates associated rentals
  			   with new rent charges
  Input:	File: 	Reads following vehicle info from each line of MODS
   					phone number, rent, status
   					Data on each line is formatted as follows
   					123-456-7890 12.12 1
					string, float, bool	
			Parameter: listTop - pointer to top of list			   
**************************************************************************/
void ModifyRentals (rentalRec *listTop)
{
	//local constant 
	const string MOD_FILE = "MODS.TXT";			//filename to open
	
	//variable declarations
	ifstream rentalMod;							//input filestream to read data
	rentalRec *tempRec;							//for modifying node rent value
	string tempNum;								//phone number read from file
	float tempRent;								//rent charge read from file
	int oneChange = 0;							//load display header if >0
	
	tempRec = listTop;					//set tempRec to second node
	
	if(tempRec == NULL){
		cout << " There are no apartment rentals that match" << endl << endl;
		
		return;
	}	
	
	rentalMod.open(MOD_FILE.c_str());
	
	//priming read
	rentalMod >> tempNum;
	rentalMod >> tempRent;
	rentalMod.ignore(100, '\n');
	
	if(!rentalMod){
		cout << " Error: No mod file found, no changes have been made"
			 << endl << endl;
			 
		return;
	}
	
	
	while(rentalMod){
		
		//search from top of list
		tempRec = listTop->next;
		
		if (listTop->phone == tempNum ) {    // If mod found in first node,
    		listTop->rent = (listTop->rent + tempRent);
    		oneChange++;
    	}
    	else{
    		//search list for matching phone number
    		while ((tempRec != NULL) && (tempRec->phone != tempNum)){
    			tempRec = tempRec->next;
    		}
    		//update rent if number matches
    		if (tempRec != NULL){
    			tempRec->rent = (tempRec->rent + tempRent);
    			oneChange++;
    		}
    	}
    	
    	if(oneChange == 1){
    		cout << " Phone Number     New Monthly Rent" << endl
    			 << " ------------     ----------------" << endl;
    		oneChange++;
    	}
    	
		if(tempRec != NULL){
    		if(listTop->phone == tempNum){
    			cout << ' ' << listTop->phone << setw(21) 
					 << listTop->rent << endl;
    		}
    		else if(tempRec->phone == tempNum){
    			cout << ' ' << tempRec->phone << setw(21) 
					 << tempRec->rent << endl;
    		}
    	}

    	
    	rentalMod >> tempNum;
    	rentalMod >> tempRent;
    	rentalMod.ignore(100, '\n');
	}
	
	if(oneChange == 0)
		cout << " No monthly rents have been modified" << endl << endl;
	
	rentalMod.close();
	return;
		
}
	

/*************************************************************************
    FUNCTION:	 ExitProgram
    DESCRIPTION: Asks user if they would like to save any changes. 
    INPUT:       Parameter:     listTop - pointer to top of list
	OUTPUT:		 File:	Writes following rental info from each node
   					    phone number, rent, status
   					    Data on each line is formatted as follows
   				    	123-456-7890 12.12 0
						string, float, bool	
	CALLS TO: ValidChoice	   				 
**************************************************************************/
void ExitProgram (rentalRec *listTop)
{	//const declaration
	const string EXT = ".TXT";
	//variable declarations
	ifstream fileCheck;				//to check if file exists
	ofstream updAptInfo;			//for writing data to file
	string phone,					//phone number
	       fileName;				//fileName to save to
	float rent;						//rent
	int status,						//rental status
		len,						//string character length
		idx;						//for string character checking
	char userSel;					//user option to save
	char saveFile = 'N';			//user option to write over existing
	rentalRec *nextNode;			//used to assign node data to variables
	
	nextNode = listTop;
	
	
	cout << " Would you like to save apartment data to file? ('Y' or 'N')";
	
	userSel = ValidChoice();
	
	//exit without saving
	if(userSel == 'N')
		return;
	
	//check files until new file entered or user chooses overwrite
	while(saveFile == 'N'){
		cout << " Enter filename to save to, do not specify filetype" << endl
			 << " Formatted as (RENTALS): ";
		cin >> fileName;
		
		fileName += EXT;
		
		len = fileName.length();
		
		for (idx = 0; idx <= len; idx++)
			fileName[idx] = toupper(fileName[idx]);
		
		//check if filename already exists
		//File format is
		//string, float, bool
		fileCheck.open(fileName.c_str());
 
		//if file exists warn user
		if(fileCheck){
			cout << endl;
			cout << " Warning: Filename " << fileName << " already exists" 
				 << endl << " you are about to overwrite file, would "
				 << " you like to proceed ('Y' or 'N'): ";
				 
			saveFile = ValidChoice ();		 
		}
		
		fileCheck.clear();
		fileCheck.close();
		//use new filename if none exist
		if(!fileCheck)
			saveFile = 'Y';
	}
	
	//open file for writing
	//file formatted as
	//123-456-7890 12.12 1
	//string, float, bool
	updAptInfo.open(fileName.c_str());

	do{
			 
	 	phone = nextNode->phone;
	 	rent = nextNode->rent;
	 	status = nextNode->status;
		 
	 	updAptInfo << phone << ' ' << rent << ' ' << status << endl;
		 
	 	nextNode = nextNode->next;	 
	}while(nextNode != NULL);
	
	updAptInfo.close();
	
	cout << " Your file was saved as: " << fileName << endl;
	
	return;
}

/*************************************************************************
    FUNCTION:	 ValidateLicense
    DESCRIPTION: Verifies that user input for phone number conforms to 
				 formatting requirements. Loops until valid entry is read.
    OUTPUT:     return value: inpNum - valid phone number 			   				 
*************************************************************************/
string ValidatePhone ()
{
	//declare constants
	const int INVALID = 99;				//break loop if invalid entry
	const int MAX = 12;					//valid phone number character length
	//declare variables
	string inpNum;						//phone number variable
	int entryIdx,						//index for string character checking
		len;							//length of string variable
	bool validEntry = false;			//represents user entry is in/valid


	
	while (!validEntry){
		
		cout << endl << " Enter a phone number, formated (111-111-1111)";
		cin.clear();
	    getline(cin, inpNum);
		cout << endl;
		
		len = inpNum.length();
		
		if(len > MAX)
			cout << " Invalid entry: number is too long" << endl;
		else if (len < MAX)
			cout << " Invalid entry: number is too short " << endl;
		else{

			for (entryIdx = 0; entryIdx <= len; entryIdx ++){	
				if (entryIdx < 3){			//check first two char are letters
					if (isdigit(inpNum[entryIdx])){	//continue if valid entry
						validEntry = true;
					}
					else{				
						//prompt user to enter valid
						validEntry = false;
						cout << " Invalid Entry " << inpNum << " does not start"
							 << " with 3 digits" << endl << endl;
						entryIdx = INVALID;
					}
				}
			
				if ((entryIdx == 3) || (entryIdx == 7)){//for dashes
					if (inpNum[entryIdx] == '-'){	//continue if valid entry
					validEntry = true;
					}
					else{				
						//prompt user to enter valid
						validEntry = false;
						cout << " Invalid Entry " << inpNum << " not formatted"
							 << " with proper dashes" << endl << endl;
						entryIdx = INVALID;
					}
				}
				if ((entryIdx > 3) && (entryIdx < 7)){//check middle digits
					if (isdigit(inpNum[entryIdx])){	//continue if valid entry
						validEntry = true;
					}
					else{				
						//prompt user to enter valid
						validEntry = false;
						cout << " Invalid Entry " << inpNum << " must be three"
							 << " digits after 1st dash" << endl << endl;
						entryIdx = INVALID;
					}
				}
				if ((entryIdx > 7) && (entryIdx < MAX)){
					if (isdigit(inpNum[entryIdx])){	//check last 4 digits
						validEntry = true;
					}
					else{				
						//prompt user to enter valid
						validEntry = false;
						cout << " Invalid Entry " << inpNum << " needs to end"
							 << " with four digits" << endl << endl;
						entryIdx = INVALID;
					}
				}
		
			}	
		}
	}
	cout << endl << endl;
	return inpNum;	
	
}

/*************************************************************************
    FUNCTION:	 ValidateRent
    DESCRIPTION: Prompts user to input rent, verfies input is positive # 					   
	OUTPUT:		Return Value:	inpRent - valid number greater than zero 			   				 
**************************************************************************/
float ValidateRent ()
{
	//declare variables
	 float inpRent = 0;						//rent variable
	 bool verified = false;					//true if valid entry
	 
	 while (!verified){
	 	
	 	cout << " Enter a positive number for rent amount: ";
		cin >> inpRent;
	 	cin.ignore(1000, '\n');
	 	cout << endl << endl;
	 	
	 	//if non-number entry clear input stream
	 	if (cin.fail()){
	 		cin.clear();
	 		cin.ignore(1000, '\n');
	 		cout << " Invalid entry: input must contain only numbers"
	 			 << endl;
	 		inpRent = 0;
	 	}
	 		
		else if (inpRent > 0.0){
			verified = true;
	 		return inpRent;
	 	}
	 	else{
	 		cout << " Invalid entry " << inpRent << " is not a positive"
			 	 << " non-zero value" << endl;
	 		inpRent = 0;
	 	}
	 }
	 
	 return inpRent;
}
/*************************************************************************
    FUNCTION:	 ValidateStatus
    DESCRIPTION: Validates whether apartment is currently rented
    OUTPUT:      Return Value:  inpSta - valid user entry 			   				 
**************************************************************************/
bool ValidateStatus ()
{
	//declare variables
	char inpSta = ' ';						//status variable
	bool status;							//true if valid
	
	do{
		cout << " Is the apartment currently rented (Y/N?): ";
		cin >> inpSta;
		cin.ignore(1000, '\n');
		cout << endl;
		
		inpSta = toupper(inpSta);
		
		if (inpSta == 'Y'){
			status = true;
			return status;
		}
		else if (inpSta == 'N'){
			status = false;
			return status;	
		}
		else
			cout << " Invalid entry: must answer (Y or N): " 
				 << " you entered " << inpSta << endl;
				 
		
	}while ((inpSta != 'Y') || (inpSta != 'N'));
}
/*************************************************************************
    FUNCTION:	 ValidChoice
    DESCRIPTION: Validates Yes and No choices
    OUTPUT:      Return Value:  choice - valid Y or N entry 			   				 
**************************************************************************/
char ValidChoice()
{
	char choice = ' ';
	
	do{
		cin >> choice;
		cin.ignore(1000, '\n');
		cout << endl;
		
		choice = toupper(choice);
		
		if (choice == 'Y'){
			return choice;
		}
		else if (choice == 'N'){
			return choice;	
		}
		else
			cout << " Invalid entry: must answer (Y or N): " 
				 << " you entered " << choice << endl;
				 
		
	}while ((choice != 'Y') || (choice != 'N'));
}


