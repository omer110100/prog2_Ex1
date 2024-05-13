#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "prog2_ex1.h"
#define MaxName 20
#define MaxDays 7

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

bool InputFileIsOpend = false, OutputFileIsOpend = false;
void ReadFromFile(FILE **FileInput,  FILE **FileOutput, int FileLocasionInargv, char *argv[], char *Buffer);
void ExitFile(FILE *FileInput, FILE *FileOutput);
void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOutput);
void MyOpenInputFile(char *FileName, char Mode, FILE **FileInput);
int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOutput);
void DetectCommands(char *Buffer,  FILE **FileOutput);
void AddWorker(char *Buffer);
// void AddShift(char *Buffer);
void ReportShifts(char *Buffer,  FILE **FileOutput);
void ReportShiftDetails(char *Buffer,  FILE **FileOutput);
void ReportWorkers(char *Buffer,  FILE **FileOutput);
// void RemoveWorker(char *Buffer);
void SortWorkers(Worker workers[]);
shift_type ShiftTypeConvertor(char *word);
worker_role WorkerRoleConvertor(char *word);



// typedef struct Shift
// {
//         shift_type shift_type; 
//         shift_day shift_day; 
// } Shift;




int main(int argc, char *argv[])
{
        FILE *FileInput = stdin;
        FILE *FileOutput = stdout;
        int FileLocasionInargv = 0;
        char Buffer[MAX_LEN];
        FileLocasionInargv = ValidInputFile(argc, argv, &FileInput, &FileOutput);
        ExitFile(FileInput, FileOutput);
        ReadFromFile(&FileInput, &FileOutput, FileLocasionInargv, argv, Buffer);
}
int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOutput)
{
        int FileLocasionInargv = -1;
        for (int i = 1; i < argc; i++)
        {
                // printf("%s", argv[i]);
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
        {
                prog2_report_error_message(CANNOT_OPEN_FILE);
        }
        else
        {
                // printf("file Is open : FileInput\n");
                InputFileIsOpend = true;
        }
}
void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOutput)
{
        *FileOutput = fopen(FileName, &Mode);
        if (*FileOutput == NULL)
        {
                prog2_report_error_message(CANNOT_OPEN_FILE);
        }
        else
        {
                OutputFileIsOpend = true;
                // printf("file Is open : FileOutput\n");
        }
}
void ExitFile(FILE *FileInput, FILE *FileOutput)
{
        if (FileInput == stdin)
        {
                exit(1);
                printf("Exit Program\n");
        }
        else
        {
                if (InputFileIsOpend == true)
                {
                        fclose(FileInput);
                        // printf("file Close : FileInput\n");
                }
                if (OutputFileIsOpend == true)
                {
                        fclose(FileOutput);
                        // printf("file Close :FileOutput\n");
                }
        }
}

void ReadFromFile(FILE **FileInput, FILE **FileOutput, int FileLocasionInargv, char *argv[], char *Buffer)
{
        MyOpenInputFile(argv[FileLocasionInargv], 'r', FileInput);
        while (fgets(Buffer, MAX_LEN, *FileInput) != NULL)
        {
                
                // Buffer represents a single row
                if (Buffer[0] != '\n' && Buffer[0] != '#')
                {
                        
                        DetectCommands(Buffer, FileOutput);
                }
                else
                {
                        // printf("The Line Countine onley : /n \n");
                        // continue;
                }
        }
}

void DetectCommands(char *Buffer, FILE **FileOutput)
{
        
        char row[MAX_LEN];
        strcpy(row, Buffer);
        char *word = strtok(row, " ");       
        while (word != NULL)
        {
                if (strcmp(word, "Add") == 0)
                {
                        // add command
                        word = strtok(NULL, " ");
                        if (strcmp(word, "Worker") == 0)
                        {
                                AddWorker(Buffer);
                        }
                        else if (strcmp(word, "Shift") == 0)
                        {
                                //AddShift(Buffer);
                        }
                }
                else if (strcmp(word, "Report") == 0)
                {
                        // report command
                        word = strtok(NULL, " ");
                        if (strcmp(word, "Shift") == 0)
                        {
                                //printf("\n%s\n", Buffer);
                                ReportShiftDetails(Buffer, FileOutput);
                        }
                        else if (strcmp(word, "Workers") == 0)
                        {
                                ReportWorkers(Buffer, FileOutput);
                        }
                        else if (strcmp(word, "Shifts") == 0)
                        {
                                ReportShifts(Buffer, FileOutput);
                        }
                }
                else if (strcmp(word, "Remove") == 0)
                {
                        // renive command
                        //RemoveWorker(Buffer);
                }

                word = strtok(NULL, " ");
        }
}
void ReportShiftDetails(char *Buffer, FILE **FileOutput)
{
        char *word = strtok(Buffer, " ");
        shift_type type;
        while (word != NULL)
        {
                // printf("\n%s\n", word);
                if (strcmp(word, "Details") == 0)
                {
                        
                        int passValid = 1;

                        word = strtok(NULL, " ");
                        int day = atoi(word);
                        
                        if( day < 1 || day > 7 )
                        {
                                prog2_report_error_message(INVALID_SHIFT_DAY);
                                passValid = 0;
                        }
                        else{
                                shift_day day = day;
                        }

                        word = strtok(NULL, " \t\r");
                        

                        if(ShiftTypeConvertor(word) == -1){
                                prog2_report_error_message(INVALID_SHIFT_TYPE);
                                passValid = 0;                                
                        }
                        else{
                                type = ShiftTypeConvertor(word);
                        }
                        
                        if(passValid)
                        {
                                int num_of_workers = 0;
                                float total_payment = 0;
                                
                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        if(workers[i].id){
                                                //עובד רלוונטי לבדיקה
                                                if(workers[i].shift_type[day] == type)
                                                {
                                                        num_of_workers++;
                                                        total_payment += workers[i].hourly_wage * HOURS_PER_SHIFT;
                                                }    
                                        }
                                }

                                if(!num_of_workers)
                                {
                                        prog2_report_error_message(NO_WORKERS);
                                }
                                
                                // void prog2_report_shift_details(FILE *out, shift_day day, shift_type type, int num_of_workers, float total_payment);
                                prog2_report_shift_details(*FileOutput, day, type, num_of_workers, total_payment);
                        }


                }
                word = strtok(NULL, " ");
        }
}

void ReportShifts(char *Buffer, FILE **FileOutput)
{
        char *word = strtok(Buffer, " ");        
        while (word != NULL)
        {
                // printf("\n%s\n", word);
                if (strcmp(word, "Shifts") == 0)
                {
                        

                        word = strtok(NULL, " \n\t\r");
                        int idlen = strlen(word);
                        if(idlen  > 0)
                        {
                                int id = atoi(word);
                                int employeeExist  = 0;
                                float total_payment = 0;
                                shift_day day;
                                shift_type type;
                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        if(workers[i].id == id){
                                                //עובד רלוונטי לבדיקה
                                                employeeExist = 1;
                                                total_payment = workers[i].number_of_shifts * HOURS_PER_SHIFT * workers[i].hourly_wage;
                                                prog2_report_worker(*FileOutput, workers[i].id, workers[i].name, workers[i].hourly_wage, workers[i].role, total_payment);
                                                if(workers[i].number_of_shifts)
                                                {
                                                        for (size_t j = 0; j < workers[i].number_of_shifts; i++)
                                                        {
                                                                type = workers[i].shift_type[j];
                                                                day = j + 1;
                                                                prog2_report_shift(*FileOutput, type, day);
                                                        }
                                                        
                                                }
                                                else{
                                                        prog2_report_error_message(NO_SHIFTS);
                                                }
                                        }
                                }
                                if(!employeeExist)
                                {
                                        prog2_report_error_message(WORKER_DOESNT_EXIST);
                                }
                        }
                        else{
                                
                                prog2_report_error_message(INVALID_WORKER_ID);
                        }


                }
                word = strtok(NULL, " ");
        }
}

void ReportWorkers(char *Buffer, FILE **FileOutput)
{
        char *word = strtok(Buffer, " ");     
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
                                // print specific role
                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        if(workers[i].role == role_t)
                                        {
                                        total_payment = workers[i].number_of_shifts * HOURS_PER_SHIFT * workers[i].hourly_wage;
                                        prog2_report_worker(*FileOutput, workers[i].id, workers[i].name, workers[i].hourly_wage, workers[i].role, total_payment);
                                        }
                                }
                        }
                        else{                                
                                // print all workers
                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        total_payment = workers[i].number_of_shifts * HOURS_PER_SHIFT * workers[i].hourly_wage;
                                        prog2_report_worker(*FileOutput, workers[i].id, workers[i].name, workers[i].hourly_wage, workers[i].role, total_payment);
                                }
                        }


                }
                word = strtok(NULL, " ");
        }
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
void AddWorker(char *Buffer)
{
        // printf("Add Worker\n");
        // int i=0;
        // char *wordd = strtok(Buffer, " ");
        // while(wordd!=NULL)
        // {
        //          printf("Add Worker\n");
        //          wordd = strtok(NULL, " ");
        // }
        //         printf("%d\n",i);
        //         switch (i)
        //         {
        //         case 2:
        //         printf("%d\n",i);
        //                 break;
        //         case 3:
        //         printf("%d\n",i);
        //                 break;
        //         case 4:
        //         printf("%d\n",i);

        //                 break;
        //         case 5:
        //         printf("%d\n",i);

        //                 break;
        //         case 6:
        //         printf("%d\n",i);
        //                 break;
                
        //         default:
        //                 break;
        //         }
        //         i++;
       // word = strtok(NULL, " ");
        

}
void RemoveWorker()
{
        printf("RemoveWorker\n");

}
void AddShift()
{
        printf("AddShift\n");
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