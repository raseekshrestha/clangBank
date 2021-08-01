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
void autheticate(char username[],char password[],char role); // role => 'a' for admin 'u' for user
void userDashboard(char username[]);
void adminDashboard(char username[]);
void clear();
int askForNumber(int min,int max);//allows to choose number betn min and max
long int accountNumber();
int isFolder(char dirName[]); //returns 1 or 0 and creates folder if not exists
void registerUser();
float checkBalance(long int ac);
int checkOs();
void colorize(char msg[],char colorName[]);
char *color(char colorName[]);
void cleanStdin();
int generateCharacter(int min,int max);
char *generateRandomPassword();




struct customers
{
    char firstname[20],lastname[20];
    char number[15];
    char gender[10];
    char DOB[15];
    int age;
}customer;


int main(){
	clear();
	char pass[5];
	strcpy(pass,generateRandomPassword());
	printf("%s",pass);
	colorize("red\n","red");
	colorize("blue\n","blue");
	colorize("green\n","green");
	colorize("magenta\n","magenta");
	colorize("yellow\n","yellow");
	colorize("cyan\n","cyan");
	colorize("white\n","white");
	colorize("okaay","green");
	login();
	return 0;
}


// functions
void login(){
	char role,username[50],password[50];
	printf("Login to Continue\n");
	colorize("If you are not register contact administrator\n","yellow");
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
	char user[50],pass[50],fname[30];
	if (role == 'a'){
		strcpy(filename,"login/admin.txt");
	}
	else{
		strcpy(filename,"login/users.txt");
	}
	FILE *fp;
	fp = fopen(filename,"r");
	while (!feof(fp)){
		fscanf(fp,"%s %s %s",user,pass,fname);
		if (strcmp(username,user)==0 && strcmp(password,pass)==0){
			clear();
			if (role=='a'){
				adminDashboard(username);
			}
			else{

				userDashboard(fname);
			}
			usrFound = 1;
		}
	}
	if (!usrFound){
		colorize("\nIncorrect username or password.\n","red");
	}
}

void userDashboard(char username[]){

	printf("welcome to user dashboard,");
	strcpy(username,strcat(username,"\n"));
	colorize(username,"cyan");
	printf("currently incomplete,will be completed soon\n");

}
void adminDashboard(char username[]){
	
	printf("welcome to admin dashboard, %s\n",username );
	printf("currently incomplete,will be completed soon\n");	
}



int askForNumber(int min,int max){
	int n;
	char errMsg[200];
	while (1){
		printf("Choose (%d ... %d) : ",min,max);
		scanf(" %d",&n);
		if (n>=min && n<=max){
			return n;
		}
		else{
			sprintf(errMsg,"Invalid Input: Choose a number between %d and %d\n", min,max);	
			colorize(errMsg,"red");				
			cleanStdin();
		}

	}
}

void cleanStdin(){
	if (checkOs()){
	    int c;
	    do {
	        c = getchar();
	    } while (c != '\n' && c != EOF);
	}
	else{
    	fflush(stdin);
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
    printf("Number : ");
    scanf("%s",customer.number);
    printf("gender : ");
    scanf("%s",customer.gender);
    printf("age : ");
    scanf("%d",&customer.age);
    printf("date of birth : ");
    scanf("%s",customer.DOB);
    long int ac = accountNumber();    
    fprintf(fp,"%ld %s %s %s %s %d %s\n",ac,customer.firstname,customer.lastname,customer.number,customer.gender,customer.age,customer.DOB);
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
 	fprintf(balance, "%ld %s %f\n",ac,customer.number,0.0);
 	fclose(balance);

 	// generating temporary password and saving login/users.txt
	// format -> number password firstname
	char tempPass[6],tempPass2[6];
	strcpy(tempPass,generateRandomPassword());
	// strcpy(tempPass,tempPass2);
	FILE *userFile;
	userFile = fopen("login/users.txt","a");
	fprintf(userFile, "%s %s %s\n",customer.number,tempPass,customer.firstname );
	fclose(userFile);
 	colorize("User is registered Successfully\n","green");

 
 }

float checkBalance(long int ac){
 	FILE *fp;
 	fp = fopen("balance/allbalances.txt","r");
 	char line[30];
 	long int acFromFile;
 	float userBalance;
 	while (!feof(fp)){
 		// fscanf(fp,"%ld %f",&acFromFile,&userBalance);
 		if( fgets (line, 60, fp)!=NULL ) {
	      /* writing content to stdout */
	      sscanf(line,"%ld %f",&acFromFile,&userBalance);
	      if (ac==acFromFile){
	      	printf("%ld\t%f\n",acFromFile,userBalance );
	      }

	   }

 		// printf("%ld\t%f\n",acFromFile,userBalance );

 	}
 }

 int checkOs(){
 	#ifdef __WIN32
 	return 0;
 	#endif
 	#ifdef linux
 	return 1;
 	#endif
 }

 char *color(char colorName[]){
	if (strcmp(colorName,"red")==0){
		return "\033[1;31m";
	}
	else if (strcmp(colorName,"green")==0){
		return "\033[1;32m";
	}
	else if (strcmp(colorName,"yellow")==0){
		return "\033[1;33m";
	}
	else if (strcmp(colorName,"blue")==0){
		return "\033[1;34m";
	}
	else if (strcmp(colorName,"magenta")==0){
		return "\033[1;35m";
	}
	else if (strcmp(colorName,"cyan")==0){
		return "\033[1;36m";
	}
	else if (strcmp(colorName,"white")==0){
		return "\033[1;37m";
	}
	else if (strcmp(colorName,"reset")==0){
		return "\033[1;m";
	}
}

void colorize(char msg[],char colorName[]){
	printf("%s%s%s",color(colorName),msg,color("reset"));
}

int generateCharacter(int min,int max){	
	return (rand() % (max-min+1))+min;
	
}
	
char *generateRandomPassword(){
	static char pass[5];
	int charType; //0->lowercase 1-->uppercase 2->number
	for (int i=0;i<5;i++){
		charType = rand()%3;
		if (charType==0){
			pass[i] = generateCharacter(97,122);
		}
		else if (charType == 1){
			pass[i] = generateCharacter(65,90);
		}
		else{
			pass[i] = generateCharacter(49,57);
			// 0 is exclude as user might get confused with letter O

		}
	}
	return pass;
}