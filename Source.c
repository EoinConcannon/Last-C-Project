#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	//variables user will assign
	int ID;
	char firstName[30];
	char lastName[30];
	int year;
	float rent;
	char email[30];
	char accomType[15];
	int bedrooms;
	char pets[3];
	int staying;

	//Reference to the next node
	struct node* NEXT;
}nodeT;

//miscellaneous functions
int listLength(nodeT* top);
int findPos(nodeT* top, int pos);
void nodeAssignment(nodeT* newNode, nodeT* top, int f);
void displayText(nodeT* temp);
void loadPreviousFirst(nodeT** top, FILE* db);
void loadPrevious(nodeT* top, FILE* db);
void nodeAssignmentFile(newNode, top, db);

//main functions
void initialiseDatabase(nodeT** top);
void addToDatabase(nodeT* top);
void displayList(nodeT* top);
void displaySpecific(nodeT* top);
void updateData(nodeT* top);
void deleteStart(nodeT** top);
void deleteEnd(nodeT* top);
void deleteAccommodation(nodeT* top, int pos);
void statistics(nodeT* top, int i, FILE* db);
void printReport(nodeT* top, FILE* db, int i);
void orderedList(nodeT* top);

main()
{
	nodeT* headPtr = NULL;
	FILE* db;

	db = fopen("Database.txt", "r");
	db = fopen("Report.txt", "r");

	//password variables
	int admin = 0;
	char input;
	char password[30];
	char filePassword[30];

	//login screen
	//admin can create a new database and use extras functions in main method
	//fail once, login as guest who uses previous database
	while (admin < 1 || admin > 2)
	{
		printf("1: Login\n2: Guest\n");
		scanf("%d", &admin);
		if (admin < 1 || admin > 2)
			printf("ERROR: INVALID INPUT\n\n");
	}

	if (admin == 1)
	{
		db = fopen("password.txt", "r");
		int i = 0;

		printf("Password:\n");
		while ((input = getch()) != 13)//masks user inputs as *
		{
			password[i] = input;
			i++;
			printf("*");
		}
		password[i++] = '\0';
		fscanf(db, "%s", filePassword);//assign filePassword with value in textfile

		//compare user's inputs
		if (strcmp(password, filePassword) == 0)
			printf("\nLOGIN SUCCESSFUL\n");
		else
		{
			printf("\nINCORRECT PASSWORD\nLOGGING AS GUEST\n");
			admin = 0;
		}
		fclose(db);
	}
	
	int reload = 0;

	//if login was successful, ask admin about loading the previous database
	if (admin == 1)
	{
		while (reload < 1 || reload > 2)
		{
			printf("1: Create New Database\n2: Use Previous Database\n");
			scanf("%d", &reload);
			if (reload < 1 || reload > 2)
				printf("ERROR: INVALID INPUT\n\n");
		}
	}
	//if admin doesn't create new database or guest login
	if (reload != 1)
	{
		db = fopen("Database.txt", "r");

		loadPreviousFirst(&headPtr, db);
		while (!feof(db))
		{
			loadPrevious(headPtr, db);
		}
		deleteEnd(headPtr);//deletes empty node created at end by file
		fclose(db);
	}

	int menuNav = 0;//controls for main

	printf("\n\n\t\tXYZ Accommodation Ltd\n");
	if (db == NULL)
	{
		printf("ERROR: FILE NOT RESPONDING\n");
	}
	else
	{
		while (menuNav != -1)
		{
			printf("\n1: Add Accommodation\n");
			printf("2: Display All Accommodation\n");
			if (admin == 1)
				printf("3: Display Specific Accommodation\n");
			if(admin == 1)
				printf("4: Update Accommodation\n");
			printf("5: Delete Accommodation\n");
			printf("6: Generate Statistics\n");
			printf("7: Print All Accommodation To File\n");
			if (admin == 1)
				printf("8: List All Accommodation Ordered By Monthly Rental\n");//not working properly
			printf("-1: Save And Quit Database\n");
			scanf("%d", &menuNav);

			if (menuNav == 1)//add node
			{
				if (headPtr == NULL)
					initialiseDatabase(&headPtr);//if database empty
				else
				{
					addToDatabase(headPtr);//adds nodes to end of list
				}
			}
			else if (menuNav == 2)//display all
			{
				if (headPtr != NULL)
				{
					displayList(headPtr);
				}
				else
					printf("Database Empty\n");
			}
			else if (menuNav == 3 && admin == 1)//display specific
			{
				if (headPtr != NULL)
				{
					displaySpecific(headPtr);
				}
				else
					printf("Database Empty\n");
			}
			else if (menuNav == 4 && admin == 1)//update node
			{
				if (headPtr != NULL)
				{
					updateData(headPtr);
				}
				else
					printf("Database Empty\n");
			}
			else if (menuNav == 5)//delete node
			{
				if (headPtr == NULL)
					printf("Database Empty\n");
				else
				{
					int deleteNo = 0;

					printf("Input the ID of the Accommodation you want to remove from the database?\n");
					scanf("%d", &deleteNo);

					findPos(headPtr, deleteNo);//finds the position

					if (deleteNo <= 1)//if first node
					{
						deleteStart(&headPtr);
					}
					else if (deleteNo >= listLength(headPtr))//if last node
					{
						deleteEnd(headPtr);
					}
					else
					{
						deleteAccommodation(headPtr, deleteNo);
					}
				}
			}
			else if (menuNav == 6)//generate statistics
			{
				if (headPtr != NULL)
				{
					int i = 0;//not printing to report file
					statistics(headPtr, i, db);
				}
				else
					printf("Database Empty\n");
			}
			else if (menuNav == 7)//print to report file
			{
				int i = 1;//printing to report file
				if (headPtr != NULL)
				{
					db = fopen("Report.txt", "w");

					printf("SAVING TO FILE...\n");
					printReport(headPtr, db, i);
					printf("SAVED.\n");
				}
				else
					printf("Database Empty\n");
			}
			else if (menuNav == 8 && admin == 1)//ordered list
			{
				if (headPtr != NULL)
				{
					orderedList(headPtr);
				}
				else
					printf("Database Empty\n");
			}
			else if (menuNav == -1)//saving/exiting database
			{
				int i = 0;//not printing statistics
				if (headPtr != NULL)
				{
					db = fopen("Database.txt", "w");

					printf("SAVING TO FILE...\n");
					printReport(headPtr, db, i);
					printf("SAVED.\n");
				}
			}
			else
				printf("ERROR: INVALID INPUT\n");
		}//end of while loop
	}
}//end of main

void initialiseDatabase(nodeT** top)
{
	nodeT* newNode;
	int f = 1;//avoids the ID checker

	newNode = (nodeT*)malloc(sizeof(nodeT));
	nodeAssignment(newNode, top, f);

	newNode->NEXT = *top;
	*top = newNode;
}

void addToDatabase(nodeT* top)
{
	nodeT* temp;
	nodeT* newNode;
	int f = 0;//triggers the ID checker

	newNode = (nodeT*)malloc(sizeof(nodeT));
	nodeAssignment(newNode, top, f);

	temp = top;

	while (temp->NEXT != NULL)
	{
		temp = temp->NEXT;
	}

	temp->NEXT = newNode;
	newNode->NEXT = NULL;
}

void displayList(nodeT* top)
{
	nodeT* temp;
	temp = top;

	while (temp != NULL)
	{
		displayText(temp);
		temp = temp->NEXT;
	}
}

void displaySpecific(nodeT* top)
{
	nodeT* temp;
	temp = top;
	int nodeID = 0;

	//user inputs an ID to search
	printf("Input the Accommodation ID to view its details\n");
	scanf("%d", &nodeID);
	while (temp != NULL)
	{
		if (temp->ID == nodeID)
		{
			displayText(temp);
		}
		temp = temp->NEXT;
	}
}

void updateData(nodeT* top)
{
	nodeT* temp;
	temp = top;
	int nodeID = 0;
	int f = 2;//avoids assigning the ID value

	//user inputs the ID of the node to update
	printf("Input the Accommodation ID to update its details\n");
	scanf("%d", &nodeID);

	while (temp != NULL)
	{
		if (temp->ID == nodeID)
		{
			nodeAssignment(temp, top, f);
		}
		temp = temp->NEXT;
	}
}

int findPos(nodeT* top, int pos)
{
	nodeT* temp;
	temp = top;
	int i = 0;

	while (temp != NULL)
	{
		if (temp->ID == pos)
		{
			pos = i;
		}
		i++;
		temp = temp->NEXT;
	}
	return pos;
}

void deleteStart(nodeT** top)
{
	nodeT* temp;

	temp = *top;
	*top = temp->NEXT;

	free(temp);
}

void deleteEnd(nodeT* top)
{
	nodeT* temp = top;
	nodeT* prevtemp;

	while (temp->NEXT != NULL)
	{
		prevtemp = temp;
		temp = temp->NEXT;
	}

	prevtemp->NEXT = NULL;
	free(temp);
}

void deleteAccommodation(nodeT* top, int pos)
{
	nodeT* temp = top;
	nodeT* prevtemp;
	int i = 1;
	while (i <= pos - 1)
	{
		i++;
		prevtemp = temp;
		temp = temp->NEXT;
	}

	prevtemp->NEXT = temp->NEXT;
	free(temp);
}

void statistics(nodeT* top, int i, FILE* db)
{
	nodeT* temp;
	temp = top;

	float stats[4] = {0, 0, 0, 0};
	int total = 0;

	float maxRent = 0;
	float minRent = 99999;

	printf("Enter a max rent range\n");
	scanf("%f", &maxRent);
	printf("Enter a min rent range\n");
	scanf("%f", &minRent);
	
	while (temp != NULL)
	{
		if (temp->rent >= minRent && temp->rent <= maxRent)//between max and min values
		{
			if (strcmp(temp->accomType, "Single_Room") == 0)
				stats[0]++;
			else if (strcmp(temp->accomType, "Apartment") == 0)
				stats[1]++;
			else if (strcmp(temp->accomType, "Bungalow") == 0)
				stats[2]++;
			else if (strcmp(temp->accomType, "Two_Story") == 0)
				stats[3]++;

			total++;
		}
		temp = temp->NEXT;
	}
	if (i == 0)
	{
		printf("%.2f%% are Single Room Accommodation\n", ((stats[0] / total) * 100));
		printf("%.2f%% are Apartment Accommodation\n", ((stats[1] / total) * 100));
		printf("%.2f%% are Bungalow Accommodation\n", ((stats[2] / total) * 100));
		printf("%.2f%% are Two Story Accommodation\n", ((stats[3] / total) * 100));
	}
	else if (i == 1)//function called via printReport function
	{
		fprintf(db, "%.2f%% are Single Room Accommodation\n", ((stats[0] / total) * 100));
		fprintf(db, "%.2f%% are Apartment Accommodation\n", ((stats[1] / total) * 100));
		fprintf(db, "%.2f%% are Bungalow Accommodation\n", ((stats[2] / total) * 100));
		fprintf(db, "%.2f%% are Two Story Accommodation\n", ((stats[3] / total) * 100));
	}
}

void printReport(nodeT* top, FILE* db, int i)//prints linked list to separate file 
{
	nodeT* temp;
	temp = top;

	while (temp != NULL)
	{
		if (i == 1)
		{
			fprintf(db, "Property ID: %d\n", temp->ID);
			fprintf(db, "First Name: %s\n", temp->firstName);
			fprintf(db, "Last Name: %s\n", temp->lastName);
			fprintf(db, "Year Built: %d\n", temp->year);
			fprintf(db, "Monthy Rental: %.2f\n", temp->rent);
			fprintf(db, "Email Address: %s\n", temp->email);
			fprintf(db, "Rental Accommodation Type Available: %s\n", temp->accomType);
			fprintf(db, "Bedrooms Available: %d\n", temp->bedrooms);
			fprintf(db, "Pets Allowed: %s\n", temp->pets);
			if (temp->staying == 1)
				fprintf(db, "Duration Of Stay: %d day\n\n", temp->staying);
			else
				fprintf(db, "Duration Of Stay: %d days\n\n", temp->staying);
		}
		else
		{
			fprintf(db, "%d\n", temp->ID);
			fprintf(db, "%s\n", temp->firstName);
			fprintf(db, "%s\n", temp->lastName);
			fprintf(db, "%d\n", temp->year);
			fprintf(db, "%f\n", temp->rent);
			fprintf(db, "%s\n", temp->email);
			fprintf(db, "%s\n", temp->accomType);
			fprintf(db, "%d\n", temp->bedrooms);
			fprintf(db, "%s\n", temp->pets);
			fprintf(db, "%d\n", temp->staying);
		}
		temp = temp->NEXT;
	}
	if(i == 1)
		statistics(top, i, db);

	fclose(db);
}

void orderedList(nodeT* top)//order by highest rent
{
	nodeT* temp;
	nodeT* orderedTemp;
	nodeT* displayTemp;
	temp = top;
	orderedTemp = top;

	int i = 0;
	int highest = 0;
	int nodeID = 0;

	while (temp != NULL)
	{
		while (orderedTemp != NULL)
		{
			if (orderedTemp->rent > highest)
			{
				displayTemp = orderedTemp;
				displayTemp->NEXT = NULL;
				nodeID = orderedTemp->ID;
				findPos(orderedTemp, nodeID);
			}
			orderedTemp = orderedTemp->NEXT;
		}
		highest = 0;
		/*
		if (nodeID <= 1)
		{
			deleteStart(&orderedTemp);
		}
		else if (nodeID >= listLength(orderedTemp))
		{
			deleteEnd(orderedTemp);
		}
		else
		{
			deleteAccommodation(orderedTemp, nodeID);
		}
		*/
		temp = temp->NEXT;
	}

	displayTemp = displayTemp;
	while (displayTemp != NULL)
	{
		i++;
		printf("\nAccommodation #%d", i);
		displayText(displayTemp);
		displayTemp = displayTemp->NEXT;
	}
}

int listLength(nodeT* top)
{
	nodeT* temp;
	int count = 0;

	temp = top;

	while (temp != NULL)
	{
		count++;
		temp = temp->NEXT;
	}
	return count;
}

void displayText(nodeT* temp)
{
	printf("\nProperty ID: %d\n", temp->ID);
	printf("First Name: %s\n", temp->firstName);
	printf("Last Name: %s\n", temp->lastName);
	printf("Year Built: %d\n", temp->year);
	printf("Monthy Rental: %.2f\n", temp->rent);
	printf("Email Address: %s\n", temp->email);
	printf("Rental Accommodation Type Available: %s\n", temp->accomType);
	printf("Bedrooms Available: %d\n", temp->bedrooms);
	printf("Pets Allowed: %s\n", temp->pets);
	if(temp->staying == 1)
		printf("Duration Of Stay: %d day\n", temp->staying);
	else
		printf("Duration Of Stay: %d days\n", temp->staying);
}

void nodeAssignment(nodeT* newNode, nodeT* top, int f)
{
	nodeT* tempSearch;

	int checker = 0;
	int ID = 0;
	int accType = 0;
	int emailCheck = 0;
	char* a;//a = @
	char* d;//d = dot

	if (f == 2)//function called from update function
		checker++;

	//user assigns ID
	//this is avoided if first node or updating a node
	while (checker == 0)
	{
		checker++;
		tempSearch = top;

		printf("Please enter the Property ID\n");
		scanf("%d", &ID);
		if (f == 0)
		{
			while (tempSearch != NULL)
			{
				if (tempSearch->ID == ID)
				{
					printf("ID already in use\n");
					checker--;
				}
				tempSearch = tempSearch->NEXT;
			}
		}
	}
	checker--;
	if(f != 2)//if function not called from update function
		newNode->ID = ID;
	printf("Please enter the Owner's First Name\n");
	scanf("%s", &newNode->firstName);
	printf("Please enter the Owner's Last Name\n");
	scanf("%s", &newNode->lastName);
	printf("Please enter the Year the Property was Built\n");
	scanf("%d", &newNode->year);
	printf("Please enter the Monthly Rental\n");
	scanf("%f", &newNode->rent);
	while (emailCheck == 0)//user assigns email
	{
		printf("Please enter the Owner's Email Address\n");
		scanf("%s", &newNode->email);

		a = strstr(newNode->email, "@");
		d = strstr(newNode->email, ".");
		if (a == NULL || d == NULL)
			printf("FORMATTING ERROR\n");
		else
			emailCheck++;
	}
	while (checker == 0)//user assigns accommodation type
	{
		checker++;
		printf("Please enter the Rental Accommodation Type\n");
		printf("1: Single Room\n");
		printf("2: Apartment\n");
		printf("3: Bungalow\n");
		printf("4: Two Story\n");
		scanf("%d", &accType);
		if (accType == 1)
		{
			strcpy(newNode->accomType, "Single_Room");
		}
		else if (accType == 2)
		{
			strcpy(newNode->accomType, "Apartment");
		}
		else if (accType == 3)
		{
			strcpy(newNode->accomType, "Bungalow");
		}
		else if (accType == 4)
		{
			strcpy(newNode->accomType, "Two_Story");
		}
		else
		{
			printf("ERROR: INVALID INPUT\n");
			checker--;
		}
	}
	checker--;
	while (checker < 1)
	{
		printf("Please enter the amount of Bedrooms Available\n");
		scanf("%d", &checker);
		if (checker < 1)
			printf("ERROR: INVALID INPUT\n");
	}
	newNode->bedrooms = checker;
	checker = 2;
	while (checker < 0 || checker > 1)
	{
		printf("Are Pets Allowed? (1 = Yes/0 = No)\n");
		scanf("%d", &checker);
		if (checker == 1)
			strcpy(newNode->pets, "Yes");
		else if (checker == 0)
			strcpy(newNode->pets, "No");
		else
			printf("ERROR: INVALID INPUT\n");
	}
	checker = 0;
	while (checker < 1)
	{
		printf("How Long is the stay duration?\n");
		scanf("%d", &checker);
		if (checker < 1)
			printf("ERROR INVALID INPUT\n");
	}
	newNode->staying = checker;
}

void loadPreviousFirst(nodeT** top, FILE* db)//assigns the first node of the file
{
	nodeT* newNode;
	
	newNode = (nodeT*)malloc(sizeof(nodeT));
	nodeAssignmentFile(newNode, top, db);

	newNode->NEXT = *top;
	*top = newNode;
}

void loadPrevious(nodeT* top, FILE* db)//assigns other nodes from file
{
	nodeT* temp;
	nodeT* newNode;

	newNode = (nodeT*)malloc(sizeof(nodeT));
	nodeAssignmentFile(newNode, top, db);

	temp = top;

	while (temp->NEXT != NULL)
	{
		temp = temp->NEXT;
	}

	temp->NEXT = newNode;
	newNode->NEXT = NULL;
}

void nodeAssignmentFile(nodeT* newNode, nodeT* top, FILE* db)
{
	fscanf(db, "%d", &newNode->ID);
	fscanf(db, "%s", &newNode->firstName);
	fscanf(db, "%s", &newNode->lastName);
	fscanf(db, "%d", &newNode->year);
	fscanf(db, "%f", &newNode->rent);
	fscanf(db, "%s", &newNode->email);
	fscanf(db, "%s", &newNode->accomType);
	fscanf(db, "%d", &newNode->bedrooms);
	fscanf(db, "%s", &newNode->pets);
	fscanf(db, "%d", &newNode->staying);
}