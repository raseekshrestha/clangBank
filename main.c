#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#ifdef __WIN32
#include <conio.h>
#endif


void login();
void autheticate(char username[],char password[],char role);
void userDashboard(char username[]);
void adminDashboard(char username[]);
void clear();
int askForNumber(int min,int max);
long int accountNumber();
int isFolder(char dirName[]); //returns 1 or 0 and creates folder if not exists
void registerUser();

struct customers
{
    char firstname[20],lastname[20];
    char gender[10];
    char DOB[15];
    int age;
}customer;


int main(){
	clear();
	// login();
	registerUser();
	return 0;
}


// functions
void login(){
	char role,username[50],password[50];
	printf("Login to Continue\n");
	printf("If you are not register contact administrator\n");
	printf("1. Admin Login\n2. User Login\n");
	int userChoice = askForNumber(1,2);
	clear();
	if (userChoice==1){
		role='a';//admin
		printf("Admin Login\n");
	}else{
		role = 'u';//user
		printf("User Login\n");
	}	
	printf("Username : ");
	scanf("%s",username);
	printf("Password : ");
	#ifdef __WIN32
	int i=0;
	do{
		password[i] = getch();
		if (password[i]!=13){
			printf("*");
		}
		i++;
	}while(password[i-1]!=13);
	password[i-1] = '\0';
	#endif
	#ifdef linux
	scanf("%s",password);
	#endif
	autheticate(username,password,role);

}

void autheticate(char username[],char password[],char role){
	// take username,password and role, validate credentials and redirect to corresponding dashboard//error
	char filename[20];
	int usrFound = 0;
	char user[50],pass[50];
	if (role == 'a'){
		strcpy(filename,"login/admin.txt");
	}
	else{
		strcpy(filename,"login/users.txt");
	}
	FILE *fp;
	fp = fopen(filename,"r");
	while (!feof(fp)){
		fscanf(fp,"%s %s",user,pass);
		if (strcmp(username,user)==0 && strcmp(password,pass)==0){
			clear();
			if (role=='a'){
				adminDashboard(username);
			}
			else{
				userDashboard(username);
			}
			usrFound = 1;
		}
	}
	if (!usrFound){
		printf("Incorrect username or password.\n");
	}
}

void userDashboard(char username[]){
	
	printf("welcome to user dashboard, %s\n",username );
	printf("currently incomplete,will be completed soon\n");

}
void adminDashboard(char username[]){
	
	printf("welcome to admin dashboard, %s\n",username );
	printf("currently incomplete,will be completed soon\n");	
}



int askForNumber(int min,int max){
	int n;
	while (1){
		printf("Choose (%d ... %d) : ",min,max);
		scanf(" %d",&n);
		if (n>=min && n<=max){
			return n;
		}
		else{
			printf("Choose a number between %d and %d\n", min,max);
			fflush(stdin);
		}
	}
}


long int accountNumber(){
	return time(0);
}

int isFolder(char dirName[]){
	DIR *dir = opendir(dirName);
    if (dir){
    	closedir(dir);
    	return 1;
    }
    else if (ENOENT == errno){
    	char cmd[20];
    	sprintf(cmd,"mkdir %s",dirName);
    	system(cmd);	
    	return 0;
    }

}

void clear(){
	#ifdef __WIN32
	system("cls");
	#endif
	#ifdef linux
	system("clear");
	#endif
}

void registerUser()
{
    FILE *fp;
    // creates details folder if not exists
    isFolder("details");
    fp=fopen("details/customerdetails.txt","a");
    printf("first name : ");
    scanf("%s",customer.firstname);
    printf("last name : ");
    scanf("%s",customer.lastname);
    printf("gender : ");
    scanf("%s",customer.gender);
    printf("age : ");
    scanf("%d",&customer.age);
    printf("date of birth : ");
    scanf("%s",customer.DOB);
    long int ac = accountNumber();    
    fprintf(fp,"%ld %s %s %s %d %s\n",ac,customer.firstname,customer.lastname,customer.gender,customer.age,customer.DOB);
 	fclose(fp);
 	// creating a separate file for indiviudal user
 	// FILE *fp1;
 	// char filename[50];
 	// sprintf(filename,"users/%ld.txt",ac);
 	// fp1 = fopen(filename,"w");
 	// fprintf(fp1, "Name: %s %s\nAccount Number: %ld\nGender: %s\nAge: %d\nDOB: %s",customer.firstname,customer.lastname,ac,customer.gender,customer.age );
 	// fclose(fp1);

 	// adding to the balance file
 	FILE *balance;
 	isFolder("balance");
 	balance = fopen("balance/allbalances.txt","a");
 	fprintf(balance, "%ld\t%f\n",ac,0.0);
 	fclose(balance);
 	printf("User is registered Successfully\n");

 
 }