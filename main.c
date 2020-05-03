#include<stdio.h>
int patientId();
void createRecord(int);
void retrieveRecord();
void deleteRecord();

//Structure to store date of birth of Patients
typedef struct{
    int year;
    int month;
    int day;
}date;

//Stuct to store other info about Patients
typedef struct{
    //every Patient has an ID that identifies their records
    //this is used when retrieving and deleting particular records
    int id;
    char firstName[20];
    char lastName[20];
    char address[20];
    char cellNum[11];
    date dob;
}Patient;

int main(){
    int ans;
    Patient pat;

    //the main loop where the whole program runs
    do{
        //clears the screen
        system("clear");
        printf("*************Welcome to Patient Record Management system**************\n");
        printf("1. Add a new medical record\n");
        printf("2. Search for your medical record\n");
        printf("3. Delete your medical record\n");
        printf("Enter the respective number for the task you want to do\nOR enter 0 to quit:\n");
        scanf("%d", &ans);
        system("clear");

        if(ans==0){
            break;
        }
        else if(ans==1){
            createRecord(patientId());
        }
        else if(ans==2){
            retrieveRecord();
        }
        else if (ans==3){
            deleteRecord();
        }

        printf("Enter 0 to quit or 1 to go to the front page: ");
        scanf("%d", &ans);
    }while(ans!=0);

}

//function that returns a int number that is used as the ID for a Patient record
//it loops through every record in the records.txt file and returns the last record's ID
//this function basically makes sure that each record has a unique ID
int patientId(){
    Patient patient;
    FILE* buff;
    buff=fopen("records.txt", "r");

    //checks if the records.txt file exists.
    //if it does not exist then returns ID value as 0.
    if(buff==NULL){
        return 0;
    }

    //goes through every record and gets to the last record
    while(!feof(buff)){
        fread(&patient, sizeof(Patient), 1, buff);
    }
    fclose(buff);
    //the last patient ID in the records is returned
    return (patient.id);
}

void createRecord(int id){
    int ans=0;
    Patient pat;
    FILE* buff;
    //the ID returned by patientId() function is assigned
    pat.id=id;

    do{
        system("clear");
        //patient ID for the new record is incremented by 1 so that the IDs never repeat
        pat.id++;
        //Info of the new record is taken from the user
        printf("Enter your full name: \n");
        scanf("%s %s", pat.firstName, pat.lastName);
        printf("Enter your address: \n");
        scanf("%s", pat.address);
        printf("Enter your phone number: \n");
        scanf("%s", pat.cellNum);
        printf("Enter your date of birth(dd/mm/yyyy): \n");
        scanf("%d/%d/%d", &pat.dob.day, &pat.dob.month, &pat.dob.year);

        //the obtained info of the Patient is saved in records.txt file
        buff=fopen("records.txt", "a");
        fwrite(&pat, sizeof(Patient), 1, buff);
        printf("Your info has been recorded.\nYour Patient ID is %d. Please do not forget this ID.\n", pat.id);
        fclose(buff);

        printf("Do you want to add more records?\n1 = Yes / 0 = No ");
        scanf("%d", &ans);
    }while(ans!=0);
}

void retrieveRecord(){
    int ans;
    //this variable checks if the record being searched exists or not
    int exists;
    Patient pat;
    int patient_id;
    FILE* buff;

    do{
        ans=0;
        exists=0;
        //asks for the patient id of the record they want to retrieve
        printf("Enter your Patient ID: ");
        scanf("%d", &patient_id);

        buff=fopen("records.txt", "r");
        //checks if the records.txt file exists or not
        if(buff==NULL){
            printf("Sorry! Currently there are no existing Patient Records.\n");
            return;
        }

        //the patient ID entered by the user is now searched in the records.txt file
        while(!feof(buff)){
            fread(&pat, sizeof(Patient), 1, buff);
            //if the patient id matches, the record is printed to the screen
            if(pat.id==patient_id){
                printf("Patient ID: %d\n", pat.id);
                printf("Name : %s %s\n", pat.firstName, pat.lastName);
                printf("Address : %s\n", pat.address);
                printf("Cell number : %s\n", pat.cellNum);
                printf("Date of Birth : %d/%d/%d\n", pat.dob.day, pat.dob.month, pat.dob.year);
                //the exists is given a value of 1 indicating that the record is found
                //if the patient id never matches then the value remains 0 indicating patient ID does not exist
                exists=1;
                break;
            }
        }
        fclose(buff);

        //checks if the patient id was not found and displays a not found message
        if(exists==0){
            system("clear");
            //ans is changed to 1 so that the loop does not end because of the condition
            ans=1;
            printf("Sorry! There are no records with this Patient ID. \nTry entering a different ID\n");
            continue;
        }

        printf("Do you want to search for more records?\n1 = Yes / 0 = No ");
        scanf("%d", &ans);
        system("clear");
    }while(ans!=0);
}

void deleteRecord(){
    //variable to check if record exists
    int exists;
    int ans;
    Patient pat;
    int patient_id;
    FILE* buff;
    //temporary file to store the records that should not be deleted
    FILE* temp;

    do{
        ans=0;
        exists=0;
        printf("Enter the Patient ID of the record you want to delete : ");
        scanf("%d", &patient_id);

        //opens the original records file in read mode and temporary file in write mode
        buff=fopen("records.txt", "r");
        temp=fopen("temp.txt", "w");

        //copies all the records from original file to the temporary file
        //except for the record that needs to be deleted
        while(!feof(buff)){
            fread(&pat, sizeof(Patient), 1, buff);
            //checks for the record that needs to be deleted and
            //skips the write function for that record
            if(patient_id==pat.id){
                //indicates that the file exists
                exists=1;
                //continues to next iteration of the loops without
                //running the write command for the record
                continue;
            }
            fwrite(&pat, sizeof(Patient), 1, temp);
        }
        fclose(buff);
        fclose(temp);

        //checks to see if the record does not exist and prints the error message
        if(exists==0){
            ans=1;
            system("clear");
            printf("Couldn't find the record under that Patient ID.\nPlease try a different one.\n");
            continue;
        }

        //checking if temp is empty by seeking to the end of the file
        //and checking the size of temp
        temp=fopen("temp.txt", "r");
        fseek(temp, 0L, SEEK_END);
        //if temp is empty, then that means there is only one record in the
        //records file which means the records file can be deleted
        //this needs to be done because the method of copying to
        //a temporary file and rewriting the original file does not
        //work when there is only one record in the file
        if(ftell(temp)==0){
            fclose(temp);
            remove("records.txt");
            remove("temp.txt");
            return;
        }else{
            //if temp is not empty then the file is closed and
            //opened again so that the cursor is at the begining of the temp file
            fclose(temp);
            temp=fopen("temp.txt", "r");
        }

        //now that temp has only the records that should not be deleted,
        //writing the contents of temp to records.txt
        buff=fopen("records.txt", "w");

        while(!feof(temp)){
            fread(&pat, sizeof(Patient), 1, temp);
            fwrite(&pat, sizeof(Patient), 1, buff);
        }
        fclose(temp);
        fclose(buff);

        printf("Do you want  to delete more records?\n1= Yes / 0=No ");
        scanf("%d", &ans);
        system("clear");
    }while(ans!=0);
    //temp is deleted
    remove("temp.txt");
}
