/*****************************************************************************
	CODE FILENAME: BaroneAssn3.cpp
	DESCRIPTION:   Program manages data used to create a budget. Data can be
				   loaded from a text file, or the user has the option to
				   create a new budget. Users will have the option to view
				   all budget info on file, see total income, expenses, and 
				   money left over or owed. The program also gives the option
				   to save any changes made before exit.
	CLASS/TERM:    CS362_XP72_Data Structures
	DESIGNER:	   Mark Barone
	FUNCTIONS	   main - calls functions to complete tasks
				   AssignArrays - verifies and reads text file data 
				   PromptUser - prompts user asking to load existing info
				   UserMenu - Show Cars, Add Cars, Delete Cars, Exit options
				   Show - shows list of budget items by type, and total calc
				   Add - adds valid budget item to array
				   Delete - deletes budget item from array
				   ValidateName - verifies and returns valid name as string
				   VerifyAmount - verifies and returns valid dollar amount
				   VerifyType - verifies and returns valid enumerated type
				   InsertItem - inserts budget item into sorted array
				   CheckName - determines if budget name is already stored
				   ExitProgram -  gives option to save changes before exit				    
*****************************************************************************/
#include <iostream>				//for I/O 
#include <iomanip>				//for formatting output
#include <fstream>				//for reading from text file
#include <cctype>				//for character checking
#include <string>				//for string checking
#include <cstddef>				//for definition of null

using namespace std;

//Global constants
const int MAX_REC = 1000,		//maximum amount of entries per array
		  EXIT = 0,				//return value for user selected exit
	      CONT = 1,			    //return value for continue
	      LOAD = 1,				//return value for load previous
	      NEW = 0,				//return value for create new
	      DELETED = 0,			//return value for record deleted
	      NO_REC = 1,			//return value for no record found
	      NOT_FOUND = -1,       //value of user license number not found
	      MAX_CHAR = 15;
	      
const string CUST_FILE = "BUDGET.TXT";
const string SAVE_FILE = "BUDGET.TXT";

const string MENU_1 = " Choose from the following options: ",
	         MENU_2 = "  |  MENU OPTION    | SELECT ",
	         MENU_3 = "  |   Show Items    |    S",
	         MENU_4 = "  |   Add  Items    |    A",
	         MENU_5 = "  |   Delete Items  |    D",
	         MENU_6 = "  |   Exit Program  |    E",
	         MENU_7 = " Enter your selection: ";
	         
const string catType[] = {"INCOME", "EXPENSE"}; 
	      
//enumerated type definition
enum category {INCOME, EXPENSE};

//Global structure definition	
struct budgetRec {
    category type;
    string name;
    float amount;
};
  
//Prototypes
void PromptUser (int& userChoice);
int AssignArray (ifstream& budgetInfo, budgetRec budget[], int& recCount);
void UserMenu (budgetRec budget[], int& recCount);
void Show (budgetRec budget[], int recCount);
void Add (budgetRec budget[], int& recCount);
void Delete (budgetRec budget[], int& recCount);
void DisplayName (budgetRec budget[], int recCount, category typeSel);
string ValidateName (string prompt);
float VerifyAmount ();
category VerifyType (string prompt);
void InsertItem (budgetRec budget[], int& recCount, category type, string name,
	             float amount );
bool CheckName (budgetRec budget[], int recCount, string name);
void ExitProgram (ofstream& updBudgInfo, budgetRec budget[], int recCount);

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
	//variable declaration
	ifstream budgetInfo;				//input file stream variable
	ofstream updBudgInfo;				//output file stream variable
	
	int fileCheck = 1;				//value representing file open success
	int recCount = 0;				//number of records in the array
 	int userChoice = 0;				//0 if exit, 1 if load file
	
	//array declaration
	budgetRec myBudget [MAX_REC];
	
	//give user option to load file
	PromptUser(userChoice);
	
	if(userChoice == LOAD){
		fileCheck = AssignArray (budgetInfo, myBudget, recCount);
	}
	//if user selected to exit program for failed file check
	if (fileCheck == EXIT)
		return EXIT;
	
	UserMenu (myBudget, recCount);
	
	//option to save before exit
	ExitProgram (updBudgInfo, myBudget, recCount);
	
	cout << endl << endl;
	return EXIT;
	
 }
/*************************************************************************
  FUNCTION:	 PromptUser
  DESCRIPTION: Asks user if they would like to load an existing file.						   
  OUTPUT: 	 Parameter:	userChoice - 0 to exit, 1 to continue		 
**************************************************************************/
void PromptUser (int& userChoice)
{
	char userInp = ' ';				//stores Y or N to continue
	
	
	cout << setw(6) << " " << "Budget Management System" << endl << endl 
		 << " Do you wish to load a previous file ('Y' or 'N')";

	//loop until valid entry is recieved from user
	do{
		cin >> userInp;
		userInp = toupper(userInp);
	
		if (userInp == 'Y')
			userChoice = LOAD;
		else if (userInp == 'N')
			userChoice = NEW;
		else
			cout << " Invalid entry: Please enter 'Y' to load, 'N' to exit "
				 << endl << endl;
			
	}while ((userInp != 'Y') && (userInp != 'N'));
	
	cout << endl << endl;
	
	return;
}
/*************************************************************************
  FUNCTION:	 AssignArrays
  DESCRIPTION: opens text file to read and validates information is in file
  			   copies data from file and passes data to InsertItem to 
			   populate array
  Input:	Parameter: budgetInfo - input file stream variable
  					   budget - array to populate
  					   recCount - number of records in array
  			File: 	Reads following budget info from each line of BUDGET
   					type, name, amount
   					Data on each line is formatted as follows
   					0 BUDGETNAME 1212.22				   
  OUTPUT: 	 Return value: 1 - if error opening file
			 Return value: 0 - if success
  CALLS TO:  InsertItem
**************************************************************************/
int AssignArray (ifstream& budgetInfo, budgetRec budget[], int& recCount)
{
	
	string name = " ";			//value for license number
	float amount = 0;			//value for mileage
	int value = 0,				//temp variable to store bool value as int 
		contResponse = 1;		//user response if file not found
	category type;				//value for rental status
	
	//open text file
	//Data on each line is formatted as follows
   	//0 BUDGETNAME 1212.22
	budgetInfo.open (CUST_FILE.c_str());
	
	//priming read
	budgetInfo >> value;		    
	budgetInfo >> name;          	
	budgetInfo >> amount;
	
	//user option if file not found
	if (!budgetInfo){                 
    	cout << endl << " Error opening input data file " << endl 
			 << " Enter (0) to exit, or (1) to create new file: ";
			 
    	cin >> contResponse;
    	cout << endl << endl;
    	
    	//check for valid user entry
		while ((contResponse != EXIT) && (contResponse != CONT)){
    		cout << " Invalid entry: must enter either (0) or (1) to proceed: ";
    		cin >> contResponse;
    		cout << endl << endl;
    	} //end while 	
    }	//end if

	//loop while more lines of data, and array not full
	while ((recCount < MAX_REC) && (budgetInfo)){
		//convert value read to enumerated type
		type = static_cast<category>(value);
		
		InsertItem (budget, recCount, type, name, amount);	
			
		//load next line of data
		budgetInfo.ignore (100, '\n');	
		budgetInfo >> value;
		budgetInfo >> name;
		budgetInfo >> amount;	
		
    }
    
    //if array is full and more data in file
    if ((recCount == MAX_REC) && (budgetInfo)){
    	cout << " More than 1000 lines of Data, remaining entries will be"
    		 << " ignored" << endl << endl;	 
    }
	//close file 
	budgetInfo.close ();
	 
	return contResponse;	
		
}

/*************************************************************************
  FUNCTION: 	UserMenu
  DESCRIPTION:  Function gives user 4 options to choose, then calls required
  				functions to carry out tasks. Determines which options to make
  				available depending upon array count
  INPUT:        Parameter:    budget - array holding budget info 	
							  recCount - number of budget items stored in array
  CALLS TO: Show, Add, Delete 			   				 
**************************************************************************/
void UserMenu (budgetRec budget[], int& recCount)
{
	char userEntry = ' ';				//selects menu option
	
	//loop until user exits
	do{
		//Array not full, and not empty all menu options given
		if ((recCount != 0) && (recCount < MAX_REC)){
			cout << MENU_1 << endl << endl << MENU_2 << endl << endl << MENU_3 
		     << endl << MENU_4 << endl << MENU_5 << endl << MENU_6 << endl 
			 << endl << MENU_7;
			 
			cin >> userEntry;
			userEntry = toupper(userEntry);
			
				switch (userEntry)
	         	{
			 		case 'S':
	         			Show (budget, recCount);
	         			break;
			 		case 'A':
	         			Add (budget, recCount);
	         			break;
	         		case 'D':
	         			Delete (budget, recCount);
	         			break;
	         		case 'E':
	         			return;
	         		default:
	         			cout << " Invalid entry: Must enter 'S', 'A',"
						 	 << " 'D', or 'E': ";
						break;
			 	} //end switch
		} //end if
		//array is empty, only add and exit options available
		else if (recCount == 0){
			cout << MENU_1 << endl << endl << MENU_2 << endl << endl << MENU_4 
				 << endl << MENU_6 << endl << endl << MENU_7;
			 
			cin >> userEntry;
			userEntry = toupper(userEntry);
			
			switch (userEntry)
	         {
			 	case 'S':
	         		cout << " Invalid entry: No items to show try again "
	         			 << endl << endl;
	         		break;
			 	case 'A':
	         		Add (budget, recCount);
	         		break;
	         	case 'D':
	         		cout << " Invalid entry: No items to delete try again"
	         			 << endl << endl;
	         		break;
	         	case 'E':
	         		return;
	         	default:
	         		cout << " Invalid entry: Must enter 'A', or 'E': ";
	         		break;
			 } //end switch
		} //end it
		//array is full, only show, delete, and exit options available
		else if (recCount == MAX_REC){
			cout << MENU_1 << endl << endl << MENU_2 << endl << endl << MENU_3 
		     << endl << MENU_5 << endl << MENU_6 << endl << endl << MENU_7;
			 
			cin >> userEntry;
			userEntry = toupper(userEntry);
			
				switch (userEntry)
	         	{
			 		case 'S':
	         			Show (budget, recCount);
	         			break;
			 		case 'A':
	         			cout << "Invalid entry: no room to add in array"
	         				 << endl << endl;
	         			break;
	         		case 'D':
	         			Delete (budget, recCount);
	         			break;
	         		case 'E':
	         			return;
	         		default:
	         			cout << " Invalid entry: Must enter 'S',"
						 	 << " 'D', or 'E': ";
						break;
			 	}	//end switch
		}// end if
	
	
	}while (userEntry != 'E'); //end loop on exit
		
}
/*************************************************************************
    FUNCTION:	 Show
    DESCRIPTION: Checks that array is not empty, then displays all budget item
				 by type: INCOME then EXPENSE, and shows totals to user 
    INPUT:       Parameter:    budget - array to be read
							   recCount - number of budget items in array			   
**************************************************************************/
void Show (budgetRec budget[], int recCount)
{
	int index = 0;				//loop counter

	float totalInc = 0.00,		//sum of total income
		  totalExp = 0.00,		//sum of total expenses
		  total = 0.00;			//total income minus total expense
		  
	//check if array is empty
	if (recCount == 0){
		cout << endl << " There are no entries on record " << endl;
		
		return;
	}
	
	//Income header
	cout << endl 
		 << " INCOME:" << endl
		 << "       Item               Amount" << endl
		 << "       ---------------    -------" << endl;
	
	cout << fixed << setprecision(2);
	//loop to show each income entry
	for (index = 0; index < recCount; index ++){
		if (budget[index].type == INCOME){
			cout << setw(7) << " "; 
			cout << left	 
				 << setw(19) << budget[index].name;
			cout << right
				 << setw(7) << budget[index].amount << endl;
				 
			totalInc = totalInc + budget[index].amount;
		}	     
	}
	//show total income
	cout << setw(33) << "-------" << endl;
	cout << " INCOME TOTAL:" << setw(19) << totalInc << endl << endl; 
	
	//reset index to loop for expense
	index = 0;
	 
	//expense header
	cout << " EXPENSES:" << endl
		 << "       Item               Amount" << endl
		 << "       ---------------    -------" << endl;
	
	//loop showing each expense entry
	for (index = 0; index < recCount; index ++){
		if (budget[index].type == EXPENSE){
			cout << setw(7) << " "; 
			cout << left	 
				 << setw(19) << budget[index].name;
			cout << right
				 << setw(7) << budget[index].amount << endl;
				 
			totalExp = totalExp + budget[index].amount;
		}	     
	}
	//show total expense
	cout << setw(33) << "-------" << endl;
	cout << " EXPENSE TOTAL:" << setw(18) << totalExp << endl << endl;
	
	//calculate remaining funds
	total = totalInc - totalExp;
	
	cout << " REMAINING FUNDS" << setw(17) << total << endl << endl;
	
	return;
}
				
/*************************************************************************
    FUNCTION:	 Add
    DESCRIPTION: Loops to call functions to verify user input, and calls
    			 InsertItem if input is valid and budget name is not a 
    			 dubplicate entry. Exits if array is full
    INPUT:       Parameter:    recCount - number of array entries
							   budget - array holding budget items			   
    OUTPUT: 	 Parameter:    budget - adds record if array not empty
							   recCount - increments count if vehicle added	 
	CALLS TO:    ValidateName, VerifyType, VerifyAmount, InsertItem
**************************************************************************/
void Add (budgetRec budget[], int& recCount)
{
	string inpName = " ",							//name of budget item
		   addPrompt1 = " Select type to add: ",	//pass to type function
		   addPrompt2 = " Select name to add: ";	//pass to name function
	float inpAmt = 0;								//amount of budget item
	category inpType;								//type of budget item
	bool check = false,						//true if name already exists
		 exit = false;						//true when user done adding lines
	char cont;								//determines continue or exit
	
	
	//add budget info until user exits or array is full
	do{

		if (recCount < MAX_REC){
			
			cout << endl;
	
			inpType = VerifyType (addPrompt1);
			
			while(!check){
				
				inpName = ValidateName (addPrompt2);
				check = CheckName (budget, recCount, inpName);
			}
			
			inpAmt = VerifyAmount ();
		
			if (check == true){
				InsertItem (budget, recCount, inpType, inpName, inpAmt);
		
				check = false;
			}
		
		}	
			//no option to continue if record full
			if (recCount == MAX_REC){
				cout << endl 
			 		 << " Warning: Record is now full, no further storage"
					 << " allowed " << endl << endl;
				exit = true;
			}
			//give user option to continue to add lines
			else{
				do{
	
					cout << endl
				 		 << " Select (A) to add another, or (E) to exit ";
					cin >> cont;
			
					cont = toupper(cont);
			
					if (cont == 'A')
						exit = false;
					else if (cont == 'E')
						exit = true;
					else
						cout << " Invalid entry, try again " << endl;
				
				}while ((cont != 'A') && (cont != 'E'));
			}
		
			 
	}while (!exit);
	
	return;
}

/*************************************************************************
     FUNCTION:	 Delete
    DESCRIPTION: Calls functions to verify budget type to delete, and shows
    			 and validates budget name to delete. Deletes entry and 
    			 resorts array if valid
    INPUT:       Parameter:    recCount - number of array entries
							   budget - array holding budget info
	OUTPUT:	     Parameter:    budget - deletes validated entry
							   recCount - decreases if item deleted	
	CALLS TO:    DisplayName, ValidateName, VerifyType		   
**************************************************************************/
void Delete (budgetRec budget[], int& recCount)
{
	string userDelete = " ",			//budget item name to delete
		   delPrompt1 = " Select valid item type to delete: ", //delete prompt
		   delPrompt2 = " Select item name to delete: ";       //delete prompt
	int result,							//value of record deleted or not found
	    foundIndex = NOT_FOUND,		    //index where name is found
	    indCount = 0,				    //index of current cell searched
	    lastRecord = (recCount -1),	    //index of last value stored in array
		num;							//index to delete in loop
	category typeSel;					//user choice for budget type to delete
	
	
	cout << endl;
	
	typeSel = VerifyType (delPrompt1);
	
	DisplayName (budget, recCount, typeSel);
	
	userDelete = ValidateName (delPrompt2);
	
		//search entire array looking for budget name
	while ((indCount <= recCount) &&				 
		   (foundIndex == NOT_FOUND))
	{          										
		//if record being searched is found
		if (budget[indCount].name == userDelete)		
			foundIndex = indCount;						//store index of rec
		else
			indCount ++;
	}	   	
	
	//if record not found prompt user
	if (foundIndex == NOT_FOUND){
		cout << " Record entry " << userDelete << " was not found, try again" 
			 << endl;
		
		return;
	}
	
	 //delete valid entry and move each value stored under it 
     for (num = foundIndex; num <= recCount; num++){
     	if (num < MAX_REC - 1){
         	budget[num].type = budget[num +1].type;
         	budget[num].name = budget[num +1].name;
         	budget[num].amount = budget[num +1].amount;
         }
         else if (num == MAX_REC - 1){
         	budget[num].type;
         	budget[num].name = " ";
         	budget[num].amount = 0.00;
         }
         	
     }
	  //decrease array size, since a value has been deleted from the list
	recCount--;
	cout << " Item: " << userDelete << " was deleted" << endl << endl;
	
	return;
	
}
/*************************************************************************
    FUNCTION:	 DisplayName
    DESCRIPTION: Shows all names for selected type (Income/Expense) 
				 stored in array
    INPUT:       Parameter:     budget - array to read
								recCount - number of records in array			   				 
**************************************************************************/
void DisplayName (budgetRec budget[], int recCount, category typeSel)
{
	int index;				//index to display budget item name
	
	cout << " Available " << catType[typeSel] << " entries to delete: " 
		 << endl << endl;
	
	//loop to show each budget item
	for (index = 0; index < recCount; index ++){
		//only show items of budget type selected
		if (budget[index].type == typeSel)
		cout << " " << budget[index].name << endl;   
	}
	
	cout << endl;
	
	return;	 
	
}

/*************************************************************************
    FUNCTION:	 ValidateName
    DESCRIPTION: Verifies that user input for name conforms to formatting
    			 requirements. Loops until valid entry is read. Input should
    			 be 15 characters or less with no spaces. Digits and letters
    			 only
    INPUT:      parameter: prompt - instruction based upon calling function
    OUTPUT:     return value: inpName - validated string value 			   				 
*************************************************************************/
string ValidateName (string prompt)
{
	//constant declaration
	const int invalid = 16;					//break loop if invalid entry
	//variable declaration
	string inpName;							//budget item name to validate
	int entryIdx,						//string index to check
		maxIdx;							//number of string characters in inpName
	bool validEntry = false;			//true if string is valid


	getline(cin, inpName);	

	while (!validEntry){
		
		cout << endl << prompt << endl
			 << " (must be 15 characters or less, without spaces): ";
		//read entire user entry	 
	    getline(cin, inpName);
		cout << endl;
		//measure string length
		maxIdx = inpName.length();
		
		//if input is not more than 15 characters
		if (maxIdx <= MAX_CHAR){
			//loop to check each character is valid
			for (entryIdx = 0; entryIdx <= maxIdx - 1; entryIdx ++){
				
					//if all characters are letters or number
					if (isalnum(inpName[entryIdx])){	
						validEntry = true;
						inpName[entryIdx] = toupper (inpName[entryIdx]);
					}
					//invalid if there are spaces
					else if (isblank(inpName[entryIdx])){						
						validEntry = false;
						cout << " Error: name entered has spaces" << endl 
							 << " You entered: " << inpName << endl;
						entryIdx = invalid;
					}
					//invalid if any character other than letter or number
					else{						
						validEntry = false;
						cout << " Error: name is formatted incorrectly" << endl 
							 << " You entered: " << inpName << endl;
						entryIdx = invalid;
					}
				
			}
			
		}	
		//invalid if string is more than 15 characters long
		else if (maxIdx >= MAX_CHAR){			//test character after max digit

			validEntry = false;
			cout << " Error: name entered is too long" << endl 
				 << " You entered: " << inpName << endl;
		}	
	
		cout << endl;
	}
	
	return inpName;
}

/*************************************************************************
    FUNCTION:	 VerifyAmount
    DESCRIPTION: Prompts user to input amount in dollars, verfies input is
				 a positive number 					   
	OUTPUT:		Return Value:	inpAmt - valid dollar amount 
**************************************************************************/
float VerifyAmount ()
{
	 float inpAmt = 0.00;							//budget amount in dollars
	 bool verified = false;							//true if positive number
	 
	 while (!verified){
	 	
	 	cout << " Enter the amount for record entry:"
		 	 << "(must be a positive value): ";
	 	cin >> inpAmt;
	 	cout << endl;
	 
		if (inpAmt >= .01){
			verified = true;
	 		return inpAmt;
	 	}
	 	else{
	 		cout << " Error: Amount entered is not a positive number" << endl 
			 	 << " You entered: " << inpAmt << endl << endl;
	 		inpAmt = 0;
	 	}
	 }
}

/*************************************************************************
    FUNCTION:	 VerifyType
    DESCRIPTION: Determines if budget record is Income or Expense
    INPUT:		 Parameter:	prompt - instruction based on calling function
    OUTPUT:      Return Value:  inpTyp - valid enumerated type entry 			   				 
**************************************************************************/
category VerifyType (string prompt)
{
	char inpTyp = ' ';						//holds user input for budget type
	
	//loop until user entry is valid
	do{
		cout << prompt << endl
		     << " Select (I) for INCOME, or (E) for EXPENSE: ";
		
		cin >> inpTyp;
		cout << endl;
		
		inpTyp = toupper(inpTyp);
		
		if (inpTyp == 'I')
			return INCOME;
		else if (inpTyp == 'E')
			return EXPENSE;
		else
			cout << " Error: you entered an invalid choice, (I) or (E) only " 
				 << endl << " You entered: " << inpTyp << endl;
		
	}while ((inpTyp != 'Y') || (inpTyp != 'N'));
}
/*************************************************************************
    FUNCTION:	 InsertItem
    DESCRIPTION: Checks array to determine correct index to store new record
				 correctly sorts array by moving all larger records down one
				 before inserting new record 
    INPUT:       Parameter:     budget - array to search and store new record
							    recCount - number of records in the array
							    type - budget type to store in new record
							    name - budget name to search and store 
							    amount - record amount to store
	OUTPUT:		 Parameter:		budget - creates new record and re-sorts array
								recCount - increments value for record stored			   				 
**************************************************************************/
void InsertItem (budgetRec budget[], int& recCount, category type, string name,
	             float amount )
{ 
  int num,                     // index into array
      placeToInsert = 0;       // index where new value will be inserted

     //increment index while more items to search and name not found
     while ((placeToInsert < recCount) &&      
            (budget[placeToInsert].name < name)) 
                 placeToInsert++;                 

     // Move larger values down one
     for (num = recCount -1; num >= placeToInsert; num--){
         budget[num+1].type = budget[num].type;
         budget[num+1].name = budget[num].name;
         budget[num+1].amount = budget[num].amount;
     }

     // Insert itemToAdd
     budget[placeToInsert].type = type;
     budget[placeToInsert].name = name;
     budget[placeToInsert].amount = amount;

     // Increment array size, since a value has been added to the list
     recCount++;
     
     
  return;
}
/*************************************************************************
    FUNCTION:	 CheckName
    DESCRIPTION: Verifies user entered budget item name does not already
				 exist in array 
    INPUT:       Parameter:     budget - budget info in array
							    recCount - number of records in the array
								nameSel - name to check in array
	OUTPUT:		 Returns:
						true - if name is not found and valid to use
						false - if name already in array			   				 
**************************************************************************/
bool CheckName (budgetRec budget[], int recCount, string nameSel)
{
	int foundInd = NOT_FOUND;	//index of name if found
	int ind = 0;				//index of current cell searched
	
	//search array
	while ((ind < recCount) &&				//still numbers to test 
		   (foundInd == NOT_FOUND))
	{          									
		if (budget[ind].name == nameSel)		//if record found
			foundInd = ind;						//store index of rec
		else
			ind ++;
	}	   	
	
	//if record not found entry can be added
	if (foundInd == NOT_FOUND){
		return true;
	}
	//if record is found prompt user to choose another
	else if (foundInd == ind){
		cout << " Record entry " << nameSel << " already used, try again" 
			 << endl;
		return false;
	}
}
/*************************************************************************
    FUNCTION:	 ExitProgram
    DESCRIPTION: Asks user if they would like to save any changes. Writes 
				 array data to text file if user selects save 
    INPUT:       Parameter:     updBudgInfo - output file stream variable
								budget - array data to be written to text
							    recCount - number of records in the array
	OUTPUT:		 File:	Writes following vehicle info from each record of budget
   					    type, name, amount
   					    Data on each line is formatted as follows
   				    	0 BUDGETNAME 1212.00			   				 
**************************************************************************/
void ExitProgram (ofstream& updBudgInfo, budgetRec budget[], int recCount)
{
	
	
	string name;					//budget item name
	float amount;					//budget item amount in dollars
	category type;					//budget item type
	int lineCount;					//how many lines of text written
	char userSel = 'N',				//'Y' if user wants to save to text file
		 skip = ' ';				//used for spacing in text file output
	
	
	
	//loop until valid user choice
	do{

		cout << " Would you like to save your changes? (Enter: Y/N) "
			 << endl << " WARNING: This will overwrite your existing file: ";
		 
		cin >> userSel;
	
		cout << endl;
	
		userSel = toupper(userSel);
		
		//save info to text file if user selects yes
		if (userSel == 'Y'){
			//open file to write
			//Data on each line is formatted as follows
   			//0 BUDGETNAME 1212.00
			updBudgInfo.open(SAVE_FILE.c_str()); 
			
			for (lineCount = 0; lineCount < recCount; lineCount ++){
				updBudgInfo << budget[lineCount].type << skip 
						   << budget[lineCount].name << skip
						   << budget[lineCount].amount << endl;
			}
			updBudgInfo.close();				//close output file
	
			return;
		}
		//exit without saving changes
		else if (userSel == 'N')
			return;
		//user entry was invalid
		else
			cout << " Invalid entry: Enter 'Y' to save, 'N' to exit " 
				 << endl << " You entered: " << userSel << endl;
				 
	}while ((userSel != 'Y') && (userSel != 'N'));
	
}



