
/*****************************************************************************
	CODE FILENAME: BaroneAssign5.cpp
	DESCRIPTION:   Program manages course information for a University
				   Program creates three seperate course options, then stores
				   associated info as objects defined by class courseList.
				   User can edit options for each course, and input students
				   by ID number into associated courses.
	CLASS/TERM:    CS362_XP72_Data Structures
	DESIGNER:	   Mark Barone
	FUNCTIONS	   UserMenu - User options on how to manipulate course
				   StudentMenu - Options to add/delete/print students in course
				   VerifyNumber - Verifies a valid course number is selected
				   VerifyTitle - Verifies a valid course title is selected
				   VerifyCap - Verifies a valid course capacity is selected
				   VerifyID - Verifies a valid student ID is selected
				    
*****************************************************************************/
#include <iostream>				//for I/O 
#include <iomanip>				//for formatting output
#include <fstream>				//for reading from text file
#include <cctype>				//for character checking
#include <string>				//for string checking

using namespace std;

//Global constants
const int MIN_CAP = 1,
		  MAX_CAP = 30;
		  
//courseList class specification
class courseList
{
	private:
		string courseNum;
		string courseTitle;
		int capacity;
		int numEnrolled;
		int idList [MAX_CAP];
	public:
		//default constructor
		courseList();
		//overload constructor
		courseList(string coNum, string coTitle, int cap = 4);
		//mutator functions			
		void setCourseNum(string num);
		void setCourseTitle(string title);
		void setCapacity(int cap);
		//accessor functions
		string getCourseNum() const;
		string getCourseTitle() const;
		int getCapacity() const;
		int getNumEnrolled() const;
		//facilitator functions             
		void printCourse(ostream& osObject, courseList& course);
		void printStudentIds(int numStu);
		void addOneStudent(int idNum);
		void dropOneStudent(int numStu);
		//overload functions
		friend courseList operator++(courseList& course);
		friend bool operator==(const courseList& course, string courseNum);
		friend ostream& operator<<(ostream&, const courseList&);
};
  	      
//Prototypes
void UserMenu(courseList& course);
void StudentMenu(courseList& course);
string VerifyNumber();
string VerifyTitle();
int VerifyCap();
int VerifyID();

/*****************************************************************************
   FUNCTION: Main
   DESCRIPTION: Creates three course objects calling constructors, then prompts
   			    user to enter information for courseThree. Main then loops 
				calling multiple functions to complete all tasks
   OUTPUT:	 	Return Value:  0 on exit
   CALLS TO: 	VerifyNumber, VerifyTitle, UserMenu
****************************************************************************/
int main()
{
	//variable declarations
	string courseNum = " ";				//valid course number
	string title = " ";					//valid course title

	//call constructors with parameters to define objects
	courseList courseOne ("CS361", "Control Structures", MAX_CAP);
	courseList courseTwo ("CS362", "Data Structures", 10);
	courseList courseThree;
	
	cout << " Welcome to Course Management, please enter course to add "
		 << endl << endl;

	cout << " Enter course number to add: "
		 << "format is (XX111): ";
	
	//loop until valid course number not including exit
	do{
		//prompt user to input course number and title for course three	 
		courseNum = VerifyNumber();

		//set course three values if exit not selected
		if (courseNum != "E"){
			title = VerifyTitle();
			courseThree.setCourseNum(courseNum);
			courseThree.setCourseTitle(title);
		}
		//prompt user to enter valid course number
		else
			cout << " Cannot enter 'E' to exit, enter a valid course number: ";
		
	}while (courseNum == "E");
	
	//loop until user decides to exit
	do{
		//get course number and title for each course and display
		cout << " Choose a course to manage:" << endl;
		
		courseNum = courseOne.getCourseNum(); 
		title = courseOne.getCourseTitle();
		cout << setw(7) << ' ' << courseNum << " - " << title << endl;
		
		courseNum = courseTwo.getCourseNum(); 
		title = courseTwo.getCourseTitle();
		cout << setw(7) << ' ' << courseNum << " - " << title << endl; 
		
		courseNum = courseThree.getCourseNum(); 
		title = courseThree.getCourseTitle();
		cout << setw(7) << ' ' << courseNum << " - " << title << endl;
		cout << " Enter the course number (e.g. CS200) or E to exit: ";
		 
		//user selected course to manage 
		courseNum = VerifyNumber();
		
		//if user chooses to exit program
		if (courseNum == "E"){
			//display course information before exit
			cout << courseOne << courseTwo << courseThree << endl;
			return 0;

		}
		if (courseOne == courseNum){
			UserMenu (courseOne);
		}
		else if (courseTwo == courseNum){
				
			UserMenu (courseTwo);
		}
		else if (courseThree == courseNum){
				
			UserMenu (courseThree);
		}
		else{		
			
			cout << " Entry does not match available courses, "
				 << "try again: " << endl << endl;
		}
			
	}while(courseNum != "E");
 }


	
/*************************************************************************
  FUNCTION:	 courseList::courseList
  DESCRIPTION: Constructor with parameter
  Input:	   Parameters: coNum - course number
  						   coTitle - course title
  						   cap - max students allowed to enroll
  						   		classList object
  Output:	   courseList object, set to parameter values
**************************************************************************/
courseList::courseList(string coNum, string coTitle, int cap)  
{
	courseNum = coNum;
	courseTitle = coTitle;
	capacity = cap; 
	numEnrolled = 0;	

}
	
/*************************************************************************
  FUNCTION:	    courseList::courseList
  DESCRIPTION:	Default constructor		 
  INPUT:		classList object
  OUTPUT:	    courseList object, initialized to all 0
**************************************************************************/
courseList::courseList()
{
		
	courseNum = " ";
	courseTitle = " ";
	capacity = MIN_CAP;
	numEnrolled = 0;
	
}

/*************************************************************************
  FUNCTION:	 courseList::setCourseNum
  DESCRIPTION:	sets course number for selected course 
  INPUT:		classList object
  		   	 Parameter:	num - user input for course number		 
**************************************************************************/
void courseList::setCourseNum(string num)
{
	courseNum = num;
	
	return;
}

/*************************************************************************
  FUNCTION:	 courseList::setCourseTitle
  DESCRIPTION:	sets course title
  INPUT:		classList object
  			 Parameter:	title - user entered course title		 
**************************************************************************/
void courseList::setCourseTitle(string title)
{
	courseTitle = title;
	
	return;
}
/*************************************************************************
  FUNCTION:	 courseList::setCapacity
  DESCRIPTION:	sets capacity of selected course
  INPUT:		classList object
  			Parameter: cap - capacity course it set to		 
**************************************************************************/
void courseList::setCapacity(int cap)
{	
	capacity = cap;
	cout << " New capacity is " << cap << endl << endl;
	
	return;
}

/*************************************************************************
  FUNCTION:	 courseList::getCourseNum
  DESCRIPTION:	gets course number of selected course					   
  INPUT:		classList object
  OUTPUT: 	 Return: courseNum - course number 		 
**************************************************************************/
string courseList::getCourseNum() const
{	
	return courseNum;
}

/*************************************************************************
  FUNCTION:	 courseList::getCourseTitle
  DESCRIPTION:	Obtains course title for selected course					   
  INPUT:		classList object
  OUTPUT: 	 Returns: courseTitle - course title 			 
**************************************************************************/
string courseList::getCourseTitle() const 
{	
	return courseTitle;
}

/*************************************************************************
  FUNCTION:	 courseList::getCapacity
  DESCRIPTION:	Obtains capacity of selected course					   
  INPUT:		classList object
  OUTPUT: 	 Returns: capacity -  max number of students for course			 
**************************************************************************/
int courseList::getCapacity() const 
{
	return capacity;
}
/*************************************************************************
  FUNCTION:	 courseList::getNumEnrolled
  DESCRIPTION:	Obtains number of students enrolled for selected course 					   
  INPUT:		classList object
  OUTPUT: 	 Returns: numEnrolled - number of enrolled students 		 
**************************************************************************/
int courseList::getNumEnrolled() const 
{
	return numEnrolled;
}

/*************************************************************************
  FUNCTION:	 courseList::printCourse
  DESCRIPTION:	Prints course name and number of seats taken
  INPUT:	    Parameters: osObject - overloaded << operator
  							course - object information to print
									classList object  			 
**************************************************************************/
void courseList::printCourse(ostream& osObject, courseList& course) 
{
	cout << course;
	
	return;
}
/*************************************************************************
  FUNCTION:	 courseList::printStudentIds
  DESCRIPTION:	Prints ID#'s from current student roster
  INPUT:		Paramter: numStu - quantity of students in course			 
  						  		classList object
**************************************************************************/
void courseList::printStudentIds(int numStu) 
{
	int index = 0;
	
	for (index = 0; index < numStu; index++){
		
		cout << " " << idList[index] << endl;
	}
	
	cout << endl << endl;
	
	return;
}

/*************************************************************************
  FUNCTION:	 courseList::addOneStudent
  DESCRIPTION:	adds one student to the array
  INPUT:		classList object
  			Parameter: idNum - valid student id number			 
**************************************************************************/
void courseList::addOneStudent(int idNum) 
{
	int number = 0;
	
	number = numEnrolled;
	
	idList[number] = idNum;
	
	numEnrolled++;
	
	return;	
}

/*************************************************************************
  FUNCTION:	 courseList::dropOneStudent
  DESCRIPTION:	Deletes one student from array 
  INPUT:	 Parameter:	numStu - number of students enrolled in class	
  				classList object		 
**************************************************************************/
void courseList::dropOneStudent(int numStu) 
{
	const int NOT_FOUND = -1;

	int foundIndex = NOT_FOUND;
	int indCount = 0;				//index of current cell searched
	int lastRecord = (numStu -1);	//index of last value stored in array
	int numDelete;
	
	cout << " Choose the ID number you wish to delete: ";
	cin >> numDelete;
	cout << endl << endl;
	
	//search array
	while ((indCount < numStu) &&				//still numbers to test 
		   (foundIndex == NOT_FOUND))          //target not yet found
	{
		if (idList[indCount] == numDelete)		//if record found
			foundIndex = indCount;						//store index of rec
		else
			indCount ++;
	}	   	
	
	//if record not found prompt user
	if (foundIndex == NOT_FOUND){
		cout << " Student ID " << numDelete << " could not be found"
			 << endl << endl;
	}
	//if record is found replace record to delete with last record in array
	else if (foundIndex == indCount){
		idList[foundIndex] = idList[lastRecord];
		idList[lastRecord] = 0;
		
		numEnrolled--;
	}
	
	return;
	
}

/*************************************************************************
  FUNCTION:	 operator++
  DESCRIPTION:	increments course capacity by one 
  INPUT:	 Parameter:	course - user selected course to increment					   
  OUTPUT: 	 Parameter: course - capacity incremented by one
  			 Return: course - object to increment		 
**************************************************************************/
courseList operator++(courseList& course)
{
	course.capacity++;

	return course;
}
/*************************************************************************
  FUNCTION:	 operator<<
  DESCRIPTION:	outputs course information 
  INPUT:	 Parameter:	osObject - output stream variable
  						course - object to retrieve data from
  OUTPUT:	 Returns:   osObject - 			 
**************************************************************************/
ostream& operator<<(ostream& osObject, const courseList& course)
{

	osObject << ' ' << course.courseNum << " - " << course.courseTitle
			 << endl << ' ' 
			 << course.numEnrolled << " seats taken, out of " << course.capacity 
			 << " total seats" << endl << endl;	
	
	return osObject;	
	
}

/*************************************************************************
  FUNCTION:	 operator==
  DESCRIPTION:	validates user choice of course 
  INPUT:	 Parameter:	course - object to compare					   
  						courseNum - user selected courseNum
  OUTPUT:	 Returns: true if user choice matches course			 
**************************************************************************/
bool operator==(const courseList& course, string courseNum)
{
	return (course.courseNum == courseNum);
	
}
/*************************************************************************
  FUNCTION:	 UserMenu
  DESCRIPTION:	Prompts user to select menu option 
  INPUT:	 Parameter:	course - object to maintain
  OUTPUT:	 Parameter: course - edited object
  CALLS: VerifyTitle, VerifyNum, VerifyCap, StudentMenu				   	 
**************************************************************************/
void UserMenu (courseList& course)
{
	//variable declarations
	char selection;				//user menu choice
	string num,					//course number
		   title;				//course title
	int valCap,					//user entered course capacity
		numEnr,					//number of students enrolled
		curCap,					//current course capacity
		diff;					//capacity change is too low if positive
	
	//loop until user selects to go back to choose course
	do{
		cout << " Choose from the following options" << endl << endl
			 << " Select           Option: " << endl
			 << " ------           ------- " << endl
			 << "   P:     Print Course Data" << endl
			 << "   N:     Modify course number" << endl
			 << "   T:     Modify course title" << endl
			 << "   C:     Modify capacity " << endl
			 << "   I:     Increment capacity" << endl
			 << "   S:     Student Management" << endl
			 << "   B:     Back to Choose Course Menu" << endl;
		 
		cin >> selection;
		cout << endl;
	
		switch (selection){
				case 'P':
				case 'p':
					course.printCourse(cout, course); 
					break;
				case 'N':
				case 'n':
					do{
						cout << " Enter new course number, format (XX111): ";
						num = VerifyNumber();
						if (num == "E")
							cout << " Error: you entered 'E', try again"
								 << endl << endl;
					}while(num == "E");
					course.setCourseNum(num);
					cout << " New course number is " << num << endl << endl;
					break;
				case 'T':
				case 't':
					title = VerifyTitle ();
					course.setCourseTitle(title);
					cout << " New course title is " << title << endl << endl;
					break;
				case 'C':
				case 'c':
					valCap = VerifyCap();
					curCap = course.getCapacity(); 
					numEnr = course.getNumEnrolled();
					if (valCap < numEnr){
						int diff = 0;
						diff = (numEnr - valCap);
						cout << " Cannot change capacity to " << valCap 
							 << " unless " << diff << " students are dropped," 
							 << endl << " because " << numEnr
		                     << " students are already registered."
		                     << endl << endl;
					}
					else 
						course.setCapacity(valCap);
					break;
				case 'I':
				case 'i':
					valCap = course.getCapacity();
					if (valCap < MAX_CAP){
						++course;
						valCap++;
						cout << " Capacity is now " << valCap << endl << endl;
					}
					else{
						cout << " Capacity is Full, cannot increment " << endl
							 << endl;
					}
					break;
				case 'S':
				case 's':
					StudentMenu(course);
					break;
				case 'B':
				case 'b':
					selection = toupper(selection);
					break;
				default:
					cout << " Invalid selection, choose a valid character"
						 << endl << endl;
					break;	
		}
		
	}while (selection != 'B');
	
	return;	
}

/*************************************************************************
  FUNCTION:	 StudentMenu
  DESCRIPTION:	Prompts user to make a selection for student maintenance
  INPUT:		Parameter: course - object to perform student maintenece for
  CALLS TO:	VerifyID		 
**************************************************************************/
void StudentMenu (courseList& course)
{
	//variable declarations
	char selection;					//user menu choice
	int numStu,						//number of students enrolled
		idNum,						//valid student id number
		cap;						//max number of students allowed in course
	string coNum;					//course number
	
	//loop until user chooses to go back to course management
	do{
		cout << " Choose from the following options" << endl << endl
			 << " Select           Option: " << endl
			 << " ------           ------- " << endl
			 << "   P:     Print Student ID's'" << endl
			 << "   A:     Add one student" << endl
			 << "   D:     Drop One Student" << endl
			 << "   B:     Back to Course Management" << endl;
		 
		cin >> selection;
		cout << endl;
	
		switch (selection){
				case 'P':
				case 'p':
					numStu = course.getNumEnrolled();
					coNum = course.getCourseNum(); 
					cout << " " << numStu << " students enrolled in " 
						 << coNum << endl;
					if (numStu > 0) 
						course.printStudentIds(numStu);
					break;
				case 'A':
				case 'a':
					numStu = course.getNumEnrolled();
					cap = course.getCapacity();
					if (numStu < cap){
						idNum = VerifyID();
						course.addOneStudent (idNum);
					}
					else{
						cout << " Course is full. Cannot add any more students"
							 << endl << endl;
					}
					break;
				case 'D':
				case 'd':
					numStu = course.getNumEnrolled();
					if (numStu == 0){
						cout << " Cannot drop a student -- no students are "
							 << "enrolled." << endl << endl;
					}
					else{
						cout << " Students currently enrolled " << endl;
						course.printStudentIds(numStu); 
						course.dropOneStudent(numStu);
					}
					break;
				case 'B':
				case 'b':
					selection = toupper(selection);
					break;
				default:
					cout << " Error: Invalid selection choose a valid character"
						 << endl << endl;
						
		}
		
	}while (selection != 'B');
	
	return;	
}


/*************************************************************************
  FUNCTION:	 VerifyNumber
  DESCRIPTION:	Prompts reads and validates user input for course number 
  OUTPUT:		Returns: num - valid course number		 
**************************************************************************/
string VerifyNumber ()
{
	//local constants
	const int INVALID = 99,					//exit loop if input invalid
		      MAX = 5;						//digits needed for valid number
	//variable declarations
	int x;									//index to check string characters
	string num = " ";						//user input
	bool valid = true;						//loops for valid string if false
	
	do{
		
		cin >> num;
		cout << endl;
		num[0] = toupper(num[0]);
		
		
		if (num == "E"){
			valid = true;
		}
		else if (num.length() < MAX){
			valid = false;
			x = INVALID;
			cout << " Course number entered does not have 5 characters" 
				 << endl << " Choose a course number: ";
		}
		else if (num.length() > MAX){
			valid = false;
			x = INVALID;
			cout << " Course number entered has more than 5 characters" 
				 << endl << " Choose a course number: ";
		}
		else if (num.length() == MAX){
				
			for (x = 0; x < MAX; x++){
			
				if (x < 2){
		 			if (!isalpha(num[x])){
		 				cout << " Error: course number must begin with two "
		 					 << "letters" << endl 
							 << " Choose a course number: ";
							  
		 				valid = false;
		 				x = INVALID;
		 			}
		 			else if (isalpha(num[x])){
		 				valid = true;
		 				num[x] = toupper(num[x]);
		 			}
		 		}
		 		else if ((x >= 2) && (x < 5 )){
		 			if (!isdigit(num[x])){
		 				cout << " Error: course number must end with three "
		 					 << "digits" << endl << endl;
		 				valid = false;
		 				x = INVALID;
		 			}
		 			else if (isdigit(num[x]))
		 				valid = true;
				}
		 	}
		}
		
		
	}while(!valid);
	
	return num;
}

/*************************************************************************
  FUNCTION:	 VerifyTitle
  DESCRIPTION:	Prompts, reads, validates, and formats user entry for course
  			    title 
  OUTPUT:     Returns: input - valid user entry	 
**************************************************************************/
string VerifyTitle ()
{
	//variable declarations
	bool valid = false;					//true if valid title input
	int maxChar;						//length of input
	int x;								//to check index of string characters
	string input;						//user input for title
	
	cin.ignore(100,'\n');
	
	while(!valid){
	
		cout << " Enter course title to add: "
			 << "format is (XXXX XXXXXX): ";
		
		getline (cin, input);
		cout << endl;
	
		maxChar = input.length();
	
		for (x = 0; x < maxChar; x++){
			if ((!isalpha(input[x])) && (!isspace(input[x]))){
			
				valid = false;
				cout << "Error: course title can only contain letters "
					 << "and spaces" << endl
					 << " You entered " << input << " , try again" << endl
					 << endl;
				input = " ";
						 
				x = maxChar;
			}
			else if ((isalpha(input[x])) || (isspace(input[x]))){
				//adjust input if user entered all caps
				input[x] = tolower(input[x]);
				valid = true;
			}			
		}
		
		if (valid){
			
			//capitalize first letter of each word
			for (x = 0; x < maxChar; x++){
		
				if (x == 0)
					input[x] = toupper(input[x]);
				if (isspace(input[x]))
					input[x+1] = toupper(input[x+1]);
			}
		}
	
	}
	
	return input;
}

/*************************************************************************
  FUNCTION:	 VerifyCap
  DESCRIPTION:	Prompts reads and validates user entry for course capacity 
  OUTPUT:     Returns: cap - valid user entry	 
**************************************************************************/
int VerifyCap ()
{
	//variable declarations
	int cap;							//value of capacity to verify
	bool valid = false;					//true if valid input
	
	while (!valid){
		
		cout << " Enter new course capacity: ";
	
		cin >> cap;
		cout << endl << endl;
	
		if ((cap >= MIN_CAP) && (cap <= MAX_CAP))
			valid = true;
		
		else{
			cout << " Error: Capacity must be between " << MIN_CAP << " and " 
				 << MAX_CAP << endl << " in order to be valid" << endl << endl;
		}
	}
	
	return cap;
}

/*************************************************************************
  FUNCTION:	 VerifyID
  DESCRIPTION:	Prompts, reads, and validates student ID number
  INPUT:	 Returns:	number - a valid 5 digit integer			 
**************************************************************************/
int VerifyID()
{
	//variable declarations
	const int MIN = 10000,					//5 digit integer not starting w/ 0
			  MAX = 99999;					//invalid if 6 or more digits
	int number;
	bool valid = false;
	
	do{
		cout << " Enter new student ID number, formatted as (12345): ";
	
		cin >> number;
	
		cout << endl << endl;
	
		if ((number >= MIN) && (number <= MAX))
			valid = true;
		else if (number < MIN){
			cout << " Error: ID# must be 5 digits and not start with zero "
				 << endl << endl;
		}
		else if (number > MAX){
			cout << " Error: ID# cannot be more than 5 digits " << endl << endl;
		}
	
	}while(!valid);
	
	return number;
}
