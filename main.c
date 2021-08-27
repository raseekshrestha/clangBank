#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#ifdef __WIN32
#include <conio.h>
#endif



void login();
int autheticate(char username[],char password[],char role); // role => 'a' for admin 'u' for user
void userDashboard();
void adminDashboard();
void clear();
int askForNumber(int min,int max);//allows to choose number betn min and max
long int accountNumber();
int isFolder(char dirName[]); //returns 1 or 0 and creates folder if not exists
void registerUser();
float checkBalance(char ac[]);
int checkOs();
void colorize(char msg[],char colorName[]);
char *colorizeReturn(char msg[],char colorName[]);
char *color(char colorName[]);
void cleanStdin();
int generateCharacter(int min,int max);
char *generateRandomPassword();
void listUsers();
void firstTimeLogin();
int countLinesInFile(char filename[]);
int mobileNumberExists(char mobileNumber[]);
void addBalance(char mobileNumber[],float amount);
int depositMoney(char mobileNumber[],float amount);
int transferMoney(char toMobile[],float amount); //login to user acc before transferring as sender mobile is global variable set in authenticate()
int sendNotification(char msg[],char number[]); 
int removeAndRename(char tempFile[],char originalFile[]);
void showNotifications();
void superNotification(char msg[]);


// global variables
char currentUser[20],currentUserMobile[15]="987654321",currentUserAc[15];
int firstLogin;




struct customers
{
	long int ac;
    char firstname[20],lastname[20];
    char number[15];
    char gender[10];
    char DOB[15];
    int age;
}customer;


int main(){
	srand(time(0));
	clear();
	
	// int lines= countLinesInFile("login/users.txt");
	// printf("no of lines : %d",lines);
	// login();
	// int num = mobileNumberExists("123456789");
	// superNotification("this is super notification sent by admin");
	// superNotification("just a fuckcing thing");
	int res = depositMoney("987654321",10000);
	if (res == 1){
		printf("sending Notification to you\n");
		sendNotification("bank deposit success rs. 10000","987654321");
	}
	else{
		printf("failed to deposit money");
	}
	// login();
	int tx = transferMoney("1234567890",100);
	if (tx==1){
		printf("transaction successful send notication now\n");
		
	}


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
		if (password[i]!=13 && password[i]!=8){
			printf("*");
		}
		if (password[i] == 8){
			i--;
		}else{
			i++;
		}
		
	}while(password[i-1]!=13);
	password[i-1] = '\0';
	#endif
	#ifdef linux
	scanf("%s",password);
	#endif
	autheticate(username,password,role);

}

int autheticate(char username[],char password[],char role){
	// take username,password and role, validate credentials and redirect to corresponding dashboard//error
	char filename[20];
	int usrFound =0,pin;
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
		fscanf(fp,"%s %s %s %d %d",user,pass,fname,&firstLogin,&pin);
		if (strcmp(username,user)==0 && strcmp(password,pass)==0){
			
			clear();
			if (role=='a'){
				fclose(fp);
				strcpy(currentUser,username); //setting up value for global variable "currentUser"
				adminDashboard();
				
			}
			else{
				fclose(fp);
				strcpy(currentUser,fname); // third entry is fname in users.txt
				strcpy(currentUserMobile,user); // first entry is ac num " "
				// strcpy(currentUserMobile,)
				userDashboard();
				
			}
			usrFound = 1;
			return 0;			
		}
	}
	if (!usrFound){
		colorize("\nIncorrect username or password.\n","red");
		colorize("Forgot Password? (y/n) : ","white");
		char ch;
		cleanStdin();
		scanf("%c",&ch);
		if(ch=='Y' || ch=='y'){
			printf("Relax and Try to remember...;)");
		}
	}
	
}

void userDashboard(){

	printf("welcome to user dashboard, %s\n",colorizeReturn(currentUser,"cyan"));
	// strcpy(username,strcat(username,"\n"));
	// colorize(username,"cyan");
	if (firstLogin==1){
		printf("Looks like you are a new user\n");
		printf("Secure you account by changing password and transaction pin\n");
		firstTimeLogin();
	}
	printf("...");

}
void adminDashboard(){
	
	printf("welcome to admin dashboard, %s\n",currentUser );
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
    	return 2;
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
	fprintf(userFile, "%s %s %s 1 9999\n",customer.number,tempPass,customer.firstname );
	fclose(userFile);
 	colorize("User is registered Successfully\n","green");
 	printf("Account No.\t: %ld\n",ac);
 	printf("Username \t: %s\n",customer.number);
 	printf("Password \t: %s\n",tempPass);
 	printf("\nChange password and set transation pin on first login\n");

 
 }

float checkBalance(char number[]){
 	FILE *fp;
 	fp = fopen("balance/allbalances.txt","r");
 	char line[30];
 	char acFromFile[15];
 	float userBalance;
	char userNumber[15];
 	while (!feof(fp)){
 		// fscanf(fp,"%ld %f",&acFromFile,&userBalance);
 		if( fgets (line, 60, fp)!=NULL ) {
	      /* writing content to stdout */
	      sscanf(line,"%s %s %f",acFromFile,userNumber,&userBalance);
	      if (strcmp(userNumber,number)==0){
	      	// printf("%s\t%s\t%f\n",acFromFile,userNumber,userBalance );
	      	fclose(fp);
			return userBalance;
	      }

	   }
 	}
 	fclose(fp);
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
	else if (strcmp(colorName,"blueUnderline")==0){
		return "\033[1;34;4m";
	}
}

void colorize(char msg[],char colorName[]){
	printf("%s%s%s",color(colorName),msg,color("reset"));
}

char * colorizeReturn(char msg[],char colorName[]){
	static char tempString[200];
	sprintf(tempString,"%s%s%s",color(colorName),msg,color("reset"));
	return tempString;
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

void listUsers(){
	FILE *fp;
	char firstname[20],lastname[20],number[20],gender[20],DOB[20],ac[20],fullname[50];
	int age;

	char line[200];
	int counter = countLinesInFile("details/customerdetails.txt");
	fp = fopen("details/customerdetails.txt","r");
	printf("%s",colorizeReturn("Acc no.\t\tName\t\t\tNumber\t\tGender\tAge\tDOB\n","blueUnderline"));
	// rewind(fp); // take pointer to beginning of the file
	// printf("%d",counter);
	for (int i=1;i<=counter;i++){
		fscanf(fp,"%s %s %s %s %s %d %s",ac,firstname,lastname,number,gender,&age,DOB);
		sprintf(fullname,"%s %s",firstname,lastname);
		printf("%s\t%s\t\t%s\t%s\t%d\t%s\n",ac,fullname,number,gender,age,DOB);
		
	}
	fclose(fp);
}

void firstTimeLogin(){
	char newPass[30],confirmPass[30];
	int pin,linesInUsersTxt;
	cleanStdin();
	while (1){
		printf("Enter New Password : ");
		gets(newPass);
		printf("Enter Confirm Pass : ");
		gets(confirmPass);
		if (strcmp(newPass,confirmPass)==0){
			break;
		}
		else{
			colorize("New password and Confirm Password doesn't match\nTry again\n","red");
		}
	}
	printf("enter 4 digit pin code. ");
	char mobile[20],pass[20],fname[20];
	int fLogin,eachUserPin;
	pin = askForNumber(1000,9999);
	FILE *usersFile;
	usersFile = fopen("login/users.txt","r");
	FILE *tempFile;
	tempFile  = fopen("login/temp.txt","w");
	linesInUsersTxt = countLinesInFile("login/users.txt");
	for(int i=1;i<=linesInUsersTxt;i++){
		fscanf(usersFile,"%s %s %s %d %d",mobile,pass,fname,&fLogin,&eachUserPin);
		if (strcmp(currentUserMobile,mobile)==0){
			fprintf(tempFile,"%s %s %s %d %d\n",mobile,newPass,fname,0,pin);
		}
		else{
			fprintf(tempFile,"%s %s %s %d %d\n",mobile,pass,fname,fLogin,eachUserPin);
		}
	}
	fclose(usersFile);
	fclose(tempFile);
	if (remove("login/users.txt") == 0 && rename("login/temp.txt","login/users.txt") ==0){
		colorize("New Password and Transaction Pin Udated Successfully\n","green");
	}
	else{
		colorize("unable to remove the file\n","red"); 
	}
}

int countLinesInFile(char filename[]){
	int lines = 0;
	FILE *fp = fopen(filename,"r");
	while(!feof(fp)){
		if (fgetc(fp)=='\n'){
			lines ++;
		}
	}
	fclose(fp);
	return lines;

}


int mobileNumberExists(char mobileNumber[]){
	char filename[30] = "details/customerdetails.txt";
	int lines = countLinesInFile(filename);
	char ac[10],fname[20],lname[20],num[15],gender[10],dob[15];
	int age;
	FILE *fp = fopen(filename,"r");
	for (int i=1;i<=lines;i++){
		fscanf(fp,"%s %s %s %s %s %d %s",ac,fname,lname,num,gender,&age,dob);
		if (strcmp(mobileNumber,num)==0){
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

void addBalance(char mobileNumber[],float amount){
	char filename[30] = "balance/allbalances.txt";
	int lines = countLinesInFile(filename);
	char ac[15],num[15];
	float balance;
	FILE *fp = fopen(filename,"r");
	FILE *tempFile = fopen("balance/temp.txt","w");
	for (int i=1;i<=lines;i++){
		fscanf(fp,"%s %s %f",ac,num,&balance);
		if (strcmp(mobileNumber,num)==0){
			fprintf(tempFile,"%s %s %f\n",ac,num,balance+amount);
		}
		else{
			fprintf(tempFile,"%s %s %f\n",ac,num,balance);
		}
	}
	fclose(fp);
	fclose(tempFile);
	remove(filename);
	rename("balance/temp.txt",filename);

}

int depositMoney(char mobileNumber[],float amount){
	if (mobileNumberExists(mobileNumber)){
		addBalance(mobileNumber,amount);
		
		return 1;
	}
	else{
		colorize("Given Mobile number doesn't exist in our database\n","red");
		return 0;
	}
}

int transferMoney(char toMobile[],float amount){
	
	if (mobileNumberExists(toMobile)){
		// printf("\n\n\nmobile number %s exists now transferring money",toMobile);
		float balance = checkBalance(currentUserMobile);
		char message[100],filename[30];
		if (balance >= amount){
			if (depositMoney(toMobile,amount) && depositMoney(currentUserMobile,-1*amount)){
				// disp success message after function returns 
				sprintf(message,"transferred Rs.%.2f to %s",amount,toMobile);
				colorize(message,"green");
				printf("\n");
				
				//sending notification to both sender and receiver
				
				// sprintf(filename,"%s.txt",currentUserMobile);
				sendNotification(message,currentUserMobile); //sender

				// sprintf(filename,"%s.txt",toMobile);
				sprintf(message,"Received Rs.%.2f from %s",amount,currentUserMobile);
				sendNotification(message,toMobile); //receiver


				return 1;
			}
		}
		else{
			colorize("Insufficient balance\n","red");
		}
	}
	else{
		colorize("given Mobile number doesn't exists in out database\n","red");
	}
	return 0;
}


int sendNotification(char msg[],char number[]){
	char filename[40];
	sprintf(filename,"%s.txt",number);
	char path[15] = "notifications",eachLine[200],originalFile[20],eachLine1[200];
	isFolder(path);
	sprintf(originalFile,"%s/%s",path,filename);
	// printf("filename : %s\n",originalFile);
	if( access( originalFile, F_OK ) != 0 ){
		// if file do not exists directly write the notification
    	FILE *notify = fopen(originalFile,"w");
    	fprintf(notify,"%s | %s\n",msg,__DATE__);
    	fclose(notify);
	}
	else {
		char ch;
	    // if file exists write the notification to first line and copy rest from the originalFile and perform removeAndRename
		FILE *notify = fopen(originalFile,"r");
		FILE *tempFile = fopen("notifications/temp.txt","w");
		fprintf(tempFile,"%s | %s\n",msg,__DATE__);
		int linesInOriginalFile = countLinesInFile(originalFile);
		for (int i=1;i<=linesInOriginalFile;i++){
			while (1){
				ch = fgetc(notify);
				if (ch !='\n'){
					fputc(ch,tempFile);
				}
				else{
					fputc('\n',tempFile);
					break;
				}
			}
		}
		fclose(notify);
		fclose(tempFile);
		removeAndRename("notifications/temp.txt",originalFile);
	}
}

int removeAndRename(char tempFile[],char originalFile[]){
	if (remove(originalFile) == 0 && rename(tempFile,originalFile) ==0){
		return 1;
	}
	else{
		return 0;
	}
}

void showNotifications(){
	char filename[30],eachLine[200];
	char ch;
	sprintf(filename,"notifications/%s.txt",currentUserMobile);
	// printf("filename: %s\n",filename);
	int lines = countLinesInFile(filename);
	FILE *fp = fopen(filename,"r");
	for (int i=1;i<=lines;i++){
		while (1){
			ch = fgetc(fp);
			printf("%c",ch);
			if (ch=='\n'){
				break;
			}
		}

	}
}


void superNotification(char msg[]){
	char customersFile[30] = "details/customerdetails.txt";
	int lines = countLinesInFile(customersFile);
	char ac[10],fname[20],lname[20],num[15],gender[10],dob[15];
	int age;
	// char notificationFileName[30];

	FILE *fp = fopen(customersFile,"r");
	for (int i=1;i<=lines;i++){
		fscanf(fp,"%s %s %s %s %s %d %s",ac,fname,lname,num,gender,&age,dob);
		// sprintf(notificationFileName,"%s.txt",num);
		sendNotification(msg,num);
	}
}