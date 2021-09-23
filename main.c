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
int sendNotification(char msg[],char number[],int isNew); //isNew is for setting up unseen notification (available values [-1,0,1])
int removeAndRename(char tempFile[],char originalFile[]);
void showNotifications();
void superNotification(char msg[]);
int changePasswordOrPin(char choice[]);
char * askPassword();
void toHtml(char title[],char cols[][50],int noOfCols);
int noOfUnseenNotification();
void setUnseenNotification(char number[20],int isNew); // isNew= 1 if account is new,0 if not new account,-1 if you want to set unseen notification counter to 0

// global variables
char currentUser[20],currentUserMobile[15]="9988",currentUserAc[15];
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
	login();
	// // transferMoney();
	// // superNotification("test of the super notification");
	// // printf("balance is %d\n",noOfUnseenNotification());
	// // login();
	// // depositMoney("9988",10001);
	// transferMoney("9861",101);
	// showNotifications();
	// printf("No of notification for %s is %d\n",currentUserMobile,noOfUnseenNotification());

	// listUsers();



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
	strcpy(password,askPassword());
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
	while (1){
		printf("Number : ");
		scanf("%s",customer.number);
		if (mobileNumberExists(customer.number)){
			colorize("Mobile number already exists\n","red");
		}
		else{
			break;
		}
	}
    printf("gender : ");
    scanf("%s",customer.gender);
    printf("age : ");
    scanf("%d",&customer.age);
    printf("date of birth : ");
    scanf("%s",customer.DOB);
    long int ac = accountNumber();    
    fprintf(fp,"%ld %s %s %s %s %d %s\n",ac,customer.firstname,customer.lastname,customer.number,customer.gender,customer.age,customer.DOB);
 	fclose(fp);
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
	FILE *userFile;
	userFile = fopen("login/users.txt","a");
	fprintf(userFile, "%s %s %s 1 9999\n",customer.number,tempPass,customer.firstname );
	fclose(userFile);
 	colorize("User is registered Successfully\n","green");
 	printf("Account No.\t: %ld\n",ac);
 	printf("Username \t: %s\n",customer.number);
 	printf("Password \t: %s\n",tempPass);
 	printf("\nChange password and set transation pin on first login\n");
	sendNotification("Account Registration Complete",customer.number,1);
	// setUnseenNotification(customer.number,1); // 1 new notification for new account
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
			pass[i] = generateCharacter(50,57);
			// 0 and 1 is exclude as user might get confused with letter O and l
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
	char extraSpace[3];
	for (int i=1;i<=counter;i++){
		fscanf(fp,"%s %s %s %s %s %d %s",ac,firstname,lastname,number,gender,&age,DOB);
		sprintf(fullname,"%s %s",firstname,lastname);
		strlen(number) < 8 ? strcpy(extraSpace,"\t") : strcpy(extraSpace,"");
		printf("%s\t%s\t\t%s\t%s%s\t%d\t%s\n",ac,fullname,number,extraSpace,gender,age,DOB);
	}
	fclose(fp);
}

void firstTimeLogin(){
	char newPass[30],confirmPass[30];
	int pin,linesInUsersTxt;
	cleanStdin();
	while (1){
		printf("Enter New Password : ");
		strcpy(newPass,askPassword());
		printf("\nEnter Confirm Pass : ");
		strcpy(confirmPass,askPassword());
		if (strcmp(newPass,confirmPass)==0){
			break;
		}
		else{
			colorize("\nNew password and Confirm Password doesn't match\nTry again\n","red");
		}
	}
	printf("\nenter 4 digit pin code. ");
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
		sendNotification("Password and pin Changed",currentUserMobile,0);
	}
	else{
		colorize("unable to remove the file\n","red");
		sendNotification('Failed to remove file',currentUserMobile,0);
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
				sendNotification(message,currentUserMobile,0); //sender
				sprintf(message,"Received Rs.%.2f from %s",amount,currentUserMobile);
				sendNotification(message,toMobile,0); //receiver
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


int sendNotification(char msg[],char number[],int isNew){
	char filename[40];
	sprintf(filename,"%s.txt",number);
	char path[15] = "notifications",eachLine[200],originalFile[100],eachLine1[200];
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
	setUnseenNotification(number,isNew);
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
	char filename[100],eachLine[200];
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
	setUnseenNotification(currentUserMobile,-1);
}


void superNotification(char msg[]){
	char customersFile[30] = "details/customerdetails.txt";
	int lines = countLinesInFile(customersFile);
	char ac[10],fname[20],lname[20],num[15],gender[10],dob[15];
	int age;
	FILE *fp = fopen(customersFile,"r");
	for (int i=1;i<=lines;i++){
		fscanf(fp,"%s %s %s %s %s %d %s",ac,fname,lname,num,gender,&age,dob);
		sendNotification(msg,num,0);
	}
}

int changePasswordOrPin(char choice[]){
	int lines = countLinesInFile("login/users.txt"),firstLogin,pin;
	char number[15],orgpass[30],fname[20];
	FILE *fp = fopen("login/users.txt","r");
	for (int i=1;i<=lines;i++){
		fscanf(fp,"%s %s %s %d %d",number,orgpass,fname,&firstLogin,&pin);
		if (strcmp(number,currentUserMobile)==0){
			break;
		}
	}
	char oldPass[30],newPass[30],confirmPass[30];
	int oldPin,newPin;
	while (1){
		if (strcmp(choice,"password")==0){
			printf("Old pass\t: ");
			strcpy(oldPass,askPassword());

			if (strcmp(oldPass,orgpass)==0){
				break;
			}
			else{
				colorize("\nIncorrect Old password\n","red");
			}	
		}
		else if (strcmp(choice,"pin")==0){
			printf("Old Pin\t:");
			oldPin = askForNumber(1000,9999);
			if (oldPin == pin){
				break;
			}
			else{
				colorize("\nIncorrect Old pin\n","red");
			}

		}
	}
	while (1){
		if (strcmp(choice,"password")==0){
			printf("\nNew pass\t: ");
			strcpy(newPass,askPassword());
			printf("\nConfirm pass\t: ");
			strcpy(confirmPass,askPassword());
			if (strcmp(newPass,confirmPass)==0){
				break;
			}
			else{
				colorize("\n\nNew Password and Confirm Password doesn't match\n","red");
			}	
		}
		else if (strcmp(choice,"pin")==0){
			printf("\nNew Pin\t: ");
			newPin = askForNumber(1000,9999);
			break;

		}
	}
	rewind(fp);
	FILE *temp = fopen("login/tempusers.txt","w");
	for (int i=1;i<=lines;i++){
		fscanf(fp,"%s %s %s %d %d",number,orgpass,fname,&firstLogin,&pin);
		if (strcmp(number,currentUserMobile)==0){
			if (strcmp(choice,"password")==0){
				fprintf(temp,"%s %s %s %d %d\n",number,newPass,fname,firstLogin,pin);
			}
			else if (strcmp(choice,"pin")==0){
				fprintf(temp,"%s %s %s %d %d\n",number,orgpass,fname,firstLogin,newPin);
			}
		}
		else{
			fprintf(temp,"%s %s %s %d %d\n",number,orgpass,fname,firstLogin,pin);
		}
	}
	fclose(fp);
	fclose(temp);
	char msg[100];
	if (removeAndRename("login/tempusers.txt","login/users.txt")){
		sprintf(msg,"\n%s changed Successfully\n",choice);
		colorize(msg,"green");
		sprintf(msg,"%s Changed Successfully, if you didn't request a new %s contact nearest branch immediately",choice,choice);
		sendNotification(msg,currentUserMobile,0);
	}
	else{
		colorize("\nError Occured\n","red");
	}
}

char * askPassword(){
	static char password[30];
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
	return password;
}

void toHtml(char title[],char cols[][50],int noOfCols){
	int counter = countLinesInFile("details/customerdetails.txt");
	FILE *fp;
	FILE *html;
	fp = fopen("details/customerdetails.txt","r"); // file to read and write can be passed as argument
	html = fopen("index.html","w");
	char row[noOfCols][20]; // will contain all values of row
	// writing html
	fprintf(html, "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t");
	fprintf(html,"<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x' crossorigin='anonymous'>");
	fprintf(html, "\n\t\t<title>%s</title>\n\t\t<style>\n\t\t\ttable{overflow:hidden;}tr{transition:0.3s;}tbody tr:hover{transform:scale(1.03);box-shadow: 4px 3px 8px 1px #969696;}</style></head>\n<body>\n",title);
	fprintf(html,"<div class='container'><h2 class='text-center'>%s</h2>",title);
	fprintf(html,"<table class='table table-striped text-center'>\n\t<thead>\n\t\t<tr id='thead'>");
	for (int i=0;i<noOfCols;i++){
		fprintf(html, "\n\t\t\t<th>%s</th>",cols[i]);// for table heading with custom col name
	}
	fprintf(html, "\n\t\t</tr>\n\t</thead>\n\t<tbody id='tbody'>\n\t");
	char name[50];
	for (int i=1; i<=counter;i++){
		fscanf(fp,"%s %s %s %s %s %s %s",row[0],row[1],row[2],row[3],row[4],row[5],row[6]);
		sprintf(name,"%s %s",row[1],row[2]);
		fprintf(html, "<tr>\n\t\t<td>%d</td>\n\t\t<td>%s</td>\n\t\t<td>%s</td>\n\t\t<td>%s</td>\n\t\t<td>%s</td>\n\t<td>%s</td><td>%s</td>\n\t\n\t</tr>\n\t",i,row[0],name,row[3],row[4],row[5],row[6]);
	}
	fprintf(html, "\n\t</tbody>\n</table>");
	fprintf(html,"<button onclick='getJson()' class='btn btn-primary'> get json</button>\n");
	fprintf(html,"<script src='https://cdnjs.cloudflare.com/ajax/libs/FileSaver.js/2.0.0/FileSaver.min.js' integrity='sha512-csNcFYJniKjJxRWRV1R7fvnXrycHP6qDR21mgz1ZP55xY5d+aHLfo9/FcGDQLfn2IfngbAHd8LdfsagcCqgTcQ==' crossorigin='anonymous' referrerpolicy='no-referrer'></script>");
	fprintf(html,"<script  src='script.js'></script>\n</body>\n</html>");
	fclose(fp);
	fclose(html);
	colorize("Successfully exported to index.html\n","green");
}

int noOfUnseenNotification(){
	FILE *notify = fopen("notifications/unseen_notifications.txt","r");
	int lines = countLinesInFile("notifications/unseen_notifications.txt");
	char number[15];
	int unseenNum;
	for (int i=1;i<=lines;i++){
		fscanf(notify,"%s %d",number,&unseenNum);
		if (strcmp(number,currentUserMobile)==0){
			fclose(notify);
			return unseenNum;
		}
	}
	fclose(notify);
	return 0;
}

void setUnseenNotification(char number[20],int isNew){ // if new is 1 set notification to 0 else +1
	if (!(isNew == -1 || isNew == 0 || isNew == 1)){
		colorize("Invalid Value for 'isNew' Available Values[-1,0,1] \n","red");
		colorize("if New Account set its value to 1\n","red");
		colorize("if old Account, and want to increase unseen notification number set its value to 0","red");
		colorize("if old Account, and want to set unseen notification to 0 set its vlaue to -1","red");
	}
	
	if (isNew == 1){
		FILE *fp = fopen("notifications/unseen_notifications.txt","r");	
		fprintf(fp,"%s %d\n",number,1); // 1 = new account so unseen notification is set to 1
		fclose(fp);
	}
	else{//increase number of unseen notification by 1 if isNew == 0 (not new account)
		FILE *fp = fopen("notifications/unseen_notifications.txt","r");	
		FILE *temp = fopen("notifications/temp.txt","w");
		int lines = countLinesInFile("notifications/unseen_notifications.txt");
		char mobile[20];
		int unseenNum;
		for (int i=1;i<=lines;i++){
			fscanf(fp,"%s %d\n",mobile,&unseenNum);
			if (strcmp(mobile,number)==0){
				if (isNew==0){
					fprintf(temp,"%s %d\n",mobile,unseenNum+1);
				}
				else{ // possible value here will be -1 so setting unseen Notification number to 0
					fprintf(temp,"%s %d\n",mobile,0);
				}				
			}
			else{
				fprintf(temp,"%s %d\n",mobile,unseenNum);
			}
		}
		fclose(temp);
		fclose(fp);
		removeAndRename("notifications/temp.txt","notifications/unseen_notifications.txt");
	}	
}
