#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "prog2_ex1.h"

//bla2 bka hello
//bla2 
//asdd
bool InputFileIsOpend = false, OutputFileIsOpend = false;
void ReadFromFile(FILE **FileInput,  FILE **FileOutput, int FileLocasionInargv, char *argv[], char *Buffer);
void ExitFile(FILE *FileInput, FILE *FileOutput);
void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOutput);
void MyOpenInputFile(char *FileName, char Mode, FILE **FileInput);
int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOutput);
void DetectCommands(char *Buffer,  FILE **FileOutput);
// void AddWorker(char *Buffer);
// void AddShift(char *Buffer);
//void ReportShifts(char *Buffer,  FILE **FileOutput);
void ReportShiftDetails(char *Buffer,  FILE **FileOutput);
// void ReportWorkers(char *Buffer,  FILE **FileOutput);
// void RemoveWorker(char *Buffer);

typedef struct Worker
{
        char name[50];
        int id;
        float hourly_wage;
        worker_role role;
        int number_of_shifts; // 1 <= num <= 7
        shift_type shift_type[7];
} Worker;

// typedef struct Shift
// {
//         shift_type shift_type; 
//         shift_day shift_day; 
// } Shift;

Worker workers[MAX_WORKERS];


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
        char *word = strtok(Buffer, " \n\r");

        while (word != NULL)
        {
                if (strcmp(word, "Add") == 0)
                {
                        // add command
                        word = strtok(NULL, " ");
                        if (strcmp(word, "Worker") == 0)
                        {
                                //AddWorker(Buffer);
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
                                ReportShiftDetails(Buffer, FileOutput);
                        }
                        else if (strcmp(word, "Workers") == 0)
                        {
                                //ReportWorkers(Buffer);
                        }
                        else if (strcmp(word, "Shifts") == 0)
                        {
                                //ReportShifts(Buffer);
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
        char *word = strtok(Buffer, " \n\r");
        shift_type type;
        
        while (word != NULL)
        {
                
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

                        word = strtok(NULL, " ");

                        if (strcmp(word, "Morning") == 0)
                        {
                                type = MORNING;
                        }
                        else if (strcmp(word, "Afternoon") == 0)
                        {
                                type = AFTERNOON;
                        }
                        else if (strcmp(word, "Evening") == 0)
                        {
                                type = EVENING;
                        }
                        else if (strcmp(word, "Night") == 0)
                        {
                                type = NIGHT;
                        }
                        else{
                                prog2_report_error_message(INVALID_SHIFT_TYPE);
                                passValid = 0;
                        }

                        if(passValid)
                        {
                                int num_of_workers = 0;
                                int total_payment = 0;
                                
                                for (size_t i = 0; i < MAX_WORKERS; i++)
                                {
                                        if(workers[i].id){
                                                //עובד רלוונטי לבדיקה
                                                if(workers[i].shift_type[day] == type)
                                                {
                                                        num_of_workers++;
                                                        total_payment += workers[i].hourly_wage;
                                                }    
                                        }
                                }
                                
                                // void prog2_report_shift_details(FILE *out, shift_day day, shift_type type, int num_of_workers, float total_payment);
                                prog2_report_shift_details(*FileOutput, day, type, num_of_workers, total_payment);
                        }


                }
                word = strtok(NULL, " ");
        }
}

