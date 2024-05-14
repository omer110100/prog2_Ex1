#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "prog2_ex1.h"
#include <math.h>
#define MaxName 20
#define MaxDays 7
#define MaxRole 6
#define Max_Week_Shift 7 

typedef struct 
{
        char name[MaxName];
        int id;
        float hourly_wage;
        worker_role role;
        int number_of_shifts; // 1 <= num <= 7
        shift_type shift_type[MaxDays];
} Worker;

Worker workers[MAX_WORKERS];

shift_type ShiftTypeConvertor(char *word);
worker_role WorkerRoleConvertor(char *word);

bool InputFileIsOpend = false, OutputFileIsOpend = false;

void ReadFromFile(FILE **FileInput,  FILE **FileOutput, int FileLocasionInargv, char *argv[], char *Buffer);
void ExitFile(FILE *FileInput, FILE *FileOutput);

void MyOpenInputFile(char *FileName, char Mode, FILE **FileInput);
void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOutput);
int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOutput);

void DetectCommands(char *Buffer,  FILE **FileOutput);

void AddWorker(char *Buffer);
void AddShift(char *Buffer);

void RemoveWorker(char *Buffer);

void ReportShifts(char *Buffer,  FILE **FileOutput);
void ReportWorkers(char *Buffer,  FILE **FileOutput);
void ReportShiftDetails(char *Buffer,  FILE **FileOutput);

void SortWorkers(Worker workers[]);
float TotalPayment(int workerID);

int main(int argc, char *argv[])
{
        FILE *FileInput = stdin;
        FILE *FileOutput = stdout;
        int FileLocasionInargv = 0;
        char Buffer[MAX_LEN];
        FileLocasionInargv = ValidInputFile(argc, argv, &FileInput, &FileOutput);
        ExitFile(FileInput, FileOutput);
        ReadFromFile(&FileInput, &FileOutput, FileLocasionInargv, argv, Buffer);

        return 0;
}

int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOutput)
{
        int FileLocasionInargv = -1;
        for (int i = 1; i < argc; i++)
        {
                if (strcmp(argv[i], "-i") == 0)
                {
                        if (argv[i + 1] != NULL && strcmp(argv[i + 1], "-o") != 0)
                        {
                                MyOpenInputFile(argv[i + 1], 'r', FileInput);
                                FileLocasionInargv = i + 1;
                        }
                        else
                                prog2_report_error_message(INVALID_ARGUMENTS);
                }

                if (strcmp(argv[i], "-o") == 0)
                {
                        if (argv[i + 1] != NULL && strcmp(argv[i + 1], "-i") != 0)
                        {
                                MyOpenOutputFile(argv[i + 1], 'r', FileOutput);
                        }
                        else
                                prog2_report_error_message(INVALID_ARGUMENTS);
                }
        }

        return FileLocasionInargv;
}

void MyOpenInputFile(char *FileName, char Mode, FILE **FileInput)
{
        *FileInput = fopen(FileName, &Mode);
        if (*FileInput == NULL)
                prog2_report_error_message(CANNOT_OPEN_FILE);
        else
                InputFileIsOpend = true;
        
}

void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOutput)
{
        *FileOutput = fopen(FileName, &Mode);
        if (*FileOutput == NULL)
                prog2_report_error_message(CANNOT_OPEN_FILE);
        else
                OutputFileIsOpend = true;
}

void ExitFile(FILE *FileInput, FILE *FileOutput)
{
        if (FileInput == stdin)
                exit(1);
        else
        {
                if (InputFileIsOpend == true)
                        fclose(FileInput);
                if (OutputFileIsOpend == true)                
                        fclose(FileOutput);
        }
}

void ReadFromFile(FILE **FileInput, FILE **FileOutput, int FileLocasionInargv, char *argv[], char *Buffer)
{
        MyOpenInputFile(argv[FileLocasionInargv], 'r', FileInput); 

        while (fgets(Buffer, MAX_LEN, *FileInput) != NULL)
        {
                // Buffer represents a single row
                if (Buffer[0] != '\n' && Buffer[0] != '#')
                        DetectCommands(Buffer, FileOutput);
        }
}

void DetectCommands(char *Buffer, FILE **FileOutput)
{
        char row[MAX_LEN];
        strcpy(row, Buffer);
        char *word = strtok(row, " \t\r\n");   

        while (word != NULL)
        {
                if (strcmp(word, "Add") == 0)
                {
                        word = strtok(NULL, " \t\r\n");

                        if (strcmp(word, "Worker") == 0)
                                AddWorker(Buffer);
                        else if (strcmp(word, "Shift") == 0)
                                AddShift(Buffer);
                }
                else if (strcmp(word, "Report") == 0)
                {                        
                        word = strtok(NULL, " \t\n\r");

                        if (strcmp(word, "Shift") == 0)
                                ReportShiftDetails(Buffer, FileOutput);
                        else if (strcmp(word, "Workers") == 0)
                                ReportWorkers(Buffer, FileOutput);
                        else if (strcmp(word, "Shifts") == 0)
                                ReportShifts(Buffer, FileOutput);
                }
                else if (strcmp(word, "Remove") == 0)
                        RemoveWorker(Buffer);

                word = strtok(NULL, " \t\r\n");
        }
}

void AddWorker(char *Buffer)
{
        int WorkerPlace;
        bool Worker__OVERFLOW=true;

        for(WorkerPlace=0;WorkerPlace<MAX_WORKERS;WorkerPlace++)
        {
                if(workers[WorkerPlace].id==0)
                {
                        Worker__OVERFLOW=false;
                        break;
                }
        }

        if(Worker__OVERFLOW==true)
        {
                prog2_report_error_message(WORKERS_OVERFLOW);
                return;
        }

        int i=0;
        char *word = strtok(Buffer, " \t\r\n");
        float Current_Hourly_wage;
        int Worker_Week_Shift;
        worker_role role_t;

        while(word!=NULL)
        {
                switch (i)
                {
                        case 2:
                                strcpy(workers[WorkerPlace].name,word);
                                break;
                        case 3:
                                bool ValidId=true;
                                int CurrentId=atoi(word);

                                for(int Id_Place=0;Id_Place<MAX_WORKERS;Id_Place++)
                                {
                                        if(CurrentId==workers[Id_Place].id)
                                        {
                                                ValidId=true;
                                                prog2_report_error_message(WORKER_ALREADY_EXISTS);
                                                workers[WorkerPlace].id = 0;
                                                return;
                                        }
                                }

                                if(CurrentId>0&&ValidId==true)
                                        workers[WorkerPlace].id=CurrentId;
                                else
                                {
                                        prog2_report_error_message(INVALID_WORKER_ID);
                                        workers[WorkerPlace].id = 0;
                                        return;
                                }                                        
                                break;
                        case 4:
                                Current_Hourly_wage = atof(word);
                                sprintf(word, "%.2f", Current_Hourly_wage);
                                Current_Hourly_wage = atof(word);
                                if (Current_Hourly_wage>0)
                                        workers[WorkerPlace].hourly_wage=Current_Hourly_wage;
                                else
                                {
                                        prog2_report_error_message(INVALID_WAGE);
                                        workers[WorkerPlace].id = 0;
                                        return;
                                }
                                        
                                break;
                        case 5:                              
                                role_t = WorkerRoleConvertor(word);

                                if(role_t == -1)
                                {
                                        prog2_report_error_message(INVALID_ROLE);
                                        workers[WorkerPlace].id = 0;
                                        return;
                                }
                                else
                                        workers[WorkerPlace].role = role_t;

                                break;
                        case 6:               
                                Worker_Week_Shift=atoi(word);
                                if(Worker_Week_Shift<=Max_Week_Shift&&Worker_Week_Shift>=0)
                                        workers[WorkerPlace].number_of_shifts=Worker_Week_Shift;
                                else
                                {
                                        prog2_report_error_message(INVALID_NUM_OF_SHIFTS);
                                        workers[WorkerPlace].id = 0;
                                        return;
                                }
                                break;    
                        default:
                                break;
                }

                i++;

                word = strtok(NULL, " \t\r\n");  
        }
}

void AddShift(char *Buffer)
{
        char *word = strtok(Buffer, " \n\t\r");   
        int i=0;
        int Id_Place;
        int Shift_Day;
        int CounterShift=0;

        while (word != NULL)
        {
                switch (i)
                {
                        case 2:
                                bool ValidId=false;
                                int CurrentId=atoi(word);

                                if(CurrentId<=0)
                                {
                                        prog2_report_error_message(INVALID_WORKER_ID);
                                        return;    
                                }

                                for(int i =0;i<MAX_WORKERS;i++)
                                {
                                        if(CurrentId==workers[i].id)
                                        {
                                                Id_Place = i;
                                                ValidId=true;
                                                break;
                                        }
                                }

                                if(ValidId!=true)
                                {
                                        prog2_report_error_message(WORKER_DOESNT_EXIST);
                                        return;                       
                                }     

                                break;
                        case 3:
                                Shift_Day=atoi(word);

                                if(!(Shift_Day > 0 && Shift_Day <= Max_Week_Shift))
                                {
                                        prog2_report_error_message(INVALID_SHIFT_DAY);
                                        return;
                                }

                                break;
                        case 4:
                                if(ShiftTypeConvertor(word)==-1)
                                {
                                        prog2_report_error_message(INVALID_SHIFT_TYPE);
                                        return;
                                }

                                if (Shift_Day && workers[Id_Place].shift_type[Shift_Day - 1]!=0)
                                {
                                        prog2_report_error_message(SHIFT_ALREADY_EXISTS);
                                        return;
                                }

                                for(int ShiftPlace=0; ShiftPlace<MaxDays;ShiftPlace++)
                                {
                                        if(workers[Id_Place].shift_type[ShiftPlace]!=0)
                                                CounterShift++;                                        
                                }

                                if(CounterShift>=workers[Id_Place].number_of_shifts)
                                {
                                        prog2_report_error_message(SHIFTS_OVERFLOW);
                                        return;
                                }

                                workers[Id_Place].shift_type[Shift_Day - 1] = ShiftTypeConvertor(word);
                                break;
                }
                
                word = strtok(NULL, " \n\t\r");
                i++;
        }
        
}

void RemoveWorker(char *Buffer)
{       
        char *word = strtok(Buffer, " \n\r");
        int i=0;

        while (word != NULL)
        {
                if(i==2)
                {
                        int CurrentId=atoi(word);
                        int WorkerPlace;
                        bool Worker__Exist=true;

                        for(WorkerPlace=0;WorkerPlace<MAX_WORKERS;WorkerPlace++)
                        {
                                if(CurrentId<=0)                                
                                        prog2_report_error_message(INVALID_WORKER_ID);

                                if(workers[WorkerPlace].id==CurrentId)
                                {
                                        Worker__Exist=false;
                                        workers[WorkerPlace].id=0;
                                        strcpy(workers[WorkerPlace].name, "");
                                        workers[WorkerPlace].number_of_shifts=0;      
                                        workers[WorkerPlace].hourly_wage=0;    
                                        workers[WorkerPlace].role = 0;
                                        for (int x = 0; x < MaxDays; x++)
                                        {
                                                workers[WorkerPlace].shift_type[x] = 0;
                                        }                             
                                        break;
                                }
                        }

                        if(Worker__Exist==true)
                        {
                                prog2_report_error_message(WORKER_DOESNT_EXIST);
                                return;
                        }
                }

                i++;
                word = strtok(NULL, " \t\r\n");
        }
}

void ReportShiftDetails(char *Buffer, FILE **FileOutput)
{
        char *word = strtok(Buffer, " \t\r\n");
        shift_type type;

        while (word != NULL)
        {
                if (strcmp(word, "Details") == 0)
                {
                        int passValid = 1;

                        word = strtok(NULL, " \t\r\n");
                        int day = atoi(word);
                        
                        if( day < 1 || day > 7 )
                        {
                                prog2_report_error_message(INVALID_SHIFT_DAY);
                                passValid = 0;
                        }
                        else{
                                shift_day day = day;
                        }

                        word = strtok(NULL, " \t\r\n");

                        if(ShiftTypeConvertor(word) == -1){
                                prog2_report_error_message(INVALID_SHIFT_TYPE);
                                passValid = 0;                                
                        }
                        else
                                type = ShiftTypeConvertor(word);
                        
                        if(passValid)
                        {
                                int num_of_workers = 0;
                                float total_payment = 0;
                                
                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        if(workers[i].id){                                                
                                                if(workers[i].shift_type[day - 1] == type)
                                                {
                                                        num_of_workers++;
                                                        total_payment += HOURS_PER_SHIFT * workers[i].hourly_wage;
                                                }    
                                        }
                                }

                                if(!num_of_workers)
                                        prog2_report_error_message(NO_WORKERS);
                                
                                
                                prog2_report_shift_details(stdout, day, type, num_of_workers, total_payment);
                        }
                }

                word = strtok(NULL, " \t\r\n");
        }
}

void ReportShifts(char *Buffer, FILE **FileOutput)
{
        char *word = strtok(Buffer, " \t\r\n"); 

        while (word != NULL)
        {
                if (strcmp(word, "Shifts") == 0)
                {
                        word = strtok(NULL, " \n\t\r");
                        int id = atoi(word);

                        if(id  > 0)
                        {                                
                                int employeeExist  = 0;
                                float total_payment = 0;
                                shift_day day;
                                shift_type type;

                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        if(workers[i].id == id){
                                                employeeExist = 1;
                                                total_payment = TotalPayment(workers[i].id);
                                                prog2_report_worker(stdout, workers[i].id, workers[i].name, workers[i].hourly_wage, workers[i].role, total_payment);

                                                if(workers[i].number_of_shifts)
                                                {
                                                        for (int j = 0; j < Max_Week_Shift; j++)
                                                        {
                                                                type = workers[i].shift_type[j];
                                                                if(type)
                                                                {
                                                                        day = j + 1;
                                                                        prog2_report_shift(stdout, day, type);
                                                                }
                                                        }
                                                        
                                                }
                                                else
                                                        prog2_report_error_message(NO_SHIFTS);
                                        }
                                }

                                if(!employeeExist)
                                        prog2_report_error_message(WORKER_DOESNT_EXIST);
                        }
                        else
                                prog2_report_error_message(INVALID_WORKER_ID);
                }

                word = strtok(NULL, " \t\r\n");
        }
}

void ReportWorkers(char *Buffer, FILE **FileOutput)
{
        int workers_num = 0;
        char *word = strtok(Buffer, " \n\t\r");   
        float total_payment = 0;   
        worker_role role_t;

        while (word != NULL)
        {                
                if (strcmp(word, "Workers") == 0)
                {
                        word = strtok(NULL, " \n\t\r");
                        SortWorkers(workers);

                        if(word)
                        {
                                role_t = WorkerRoleConvertor(word);

                                if(role_t == -1)
                                        prog2_report_error_message(INVALID_ROLE);
                                
                        }

                        for (size_t i = 0; i < MAX_WORKERS; i++)
                        {
                                if(workers[i].id)
                                {
                                        workers_num++;

                                        if (word)
                                        {
                                                if (workers[i].role == role_t)
                                                {
                                                        total_payment = TotalPayment(workers[i].id);
                                                        prog2_report_worker(stdout, workers[i].id, workers[i].name, workers[i].hourly_wage, workers[i].role, total_payment);
                                                }                                                
                                        }
                                        else
                                        {
                                                total_payment = TotalPayment(workers[i].id);
                                                prog2_report_worker(stdout, workers[i].id, workers[i].name, workers[i].hourly_wage, workers[i].role, total_payment);
                                        }
                                }
                        }


                }

                word = strtok(NULL, " \n\t\r");
        }

        if(!workers_num)
                prog2_report_error_message(NO_WORKERS);
}

void SortWorkers(Worker workers[])
{
        Worker temp;

        for (size_t i = 0; i < MAX_WORKERS-1; i++)
        {
                for (size_t j = i+1; j < MAX_WORKERS; j++)
                {
                        if(workers[i].id > workers[j].id)
                        {
                                temp = workers[i];
                                workers[i] = workers[j];
                                workers[j] = temp;
                        }
                }
        }
        
}

shift_type ShiftTypeConvertor(char *word){
        if (strcmp(word, "Morning") == 0)
                return MORNING;
        if (strcmp(word, "Afternoon") == 0)               
                return AFTERNOON;
        if (strcmp(word, "Evening") == 0)
                return EVENING;
        if (strcmp(word, "Night") == 0)
                return NIGHT;
        return -1;
}

worker_role WorkerRoleConvertor(char *word){
        if (strcmp(word, "Bartender") == 0)
                return BARTENDER;
        if (strcmp(word, "Waiter") == 0)
                return WAITER;
        if (strcmp(word, "Manager") == 0)
                return MANAGER;
        if (strcmp(word, "Cashier") == 0)
                return CASHIER;
        if (strcmp(word, "Chef") == 0)
                return CHEF;
        if (strcmp(word, "Dishwasher") == 0)
                return DISHWASHER;
        return -1;
}

float TotalPayment(int workerID)
{
        shift_type type;
        float total_payment = 0;
        for (size_t i = 0; i < MAX_WORKERS; i++)
        {
                if(workers[i].id == workerID){
                        for (int j = 0; j < Max_Week_Shift; j++)
                        {
                                type = workers[i].shift_type[j];
                                if(type)
                                        total_payment += HOURS_PER_SHIFT * workers[i].hourly_wage;
                                
                        }
                }
        }

        return total_payment;
}