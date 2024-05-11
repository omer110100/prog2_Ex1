#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "prog2_ex1.h"

//bla2 
bool InputFileIsOpend = false, OutputFileIsOpend = false;
void ReadFromFile(FILE **FileInput, int FileLocasionInargv, char *argv[], char *Buffer);
void ExitFile(FILE *FileInput, FILE *FileOtput);
void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOtput);
void MyOpenInputFile(char *FileName, char Mode, FILE **FileInput);
int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOtput);
void DetectCommands(char *Buffer);

typedef struct Worker
{
        char name[50];
        int id;
        float hourly_wage;
        char role[50];
        int number_of_shifts; // 1 <= num <= 7
        Shift shifts[7];
} Worker;

typedef struct Shift
{
        shift_type shift; 
} Shift;

Worker workers[MAX_WORKERS];


int main(int argc, char *argv[])
{
        FILE *FileInput = stdin;
        FILE *FileOtput = stdout;
        int FileLocasionInargv = 0;
        char Buffer[MAX_LEN];
        FileLocasionInargv = ValidInputFile(argc, argv, &FileInput, &FileOtput);
        ExitFile(FileInput, FileOtput);
        ReadFromFile(&FileInput, FileLocasionInargv, argv, Buffer);
}
int ValidInputFile(int argc, char *argv[], FILE **FileInput, FILE **FileOtput)
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
                                MyOpenOutputFile(argv[i + 1], 'r', FileOtput);
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
void MyOpenOutputFile(char *FileName, char Mode, FILE **FileOtput)
{
        *FileOtput = fopen(FileName, &Mode);
        if (*FileOtput == NULL)
        {
                prog2_report_error_message(CANNOT_OPEN_FILE);
        }
        else
        {
                OutputFileIsOpend = true;
                // printf("file Is open : FileOtput\n");
        }
}
void ExitFile(FILE *FileInput, FILE *FileOtput)
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
                        fclose(FileOtput);
                        // printf("file Close :FileOtput\n");
                }
        }
}

void ReadFromFile(FILE **FileInput, int FileLocasionInargv, char *argv[], char *Buffer)
{
        MyOpenInputFile(argv[FileLocasionInargv], 'r', FileInput);
        while (fgets(Buffer, MAX_LEN, *FileInput) != NULL)
        {
                // Buffer represents a single row
                if (Buffer[0] != '\n' && Buffer[0] != '#')
                {
                        DetectCommands(Buffer);
                }
                else
                {
                        // printf("The Line Countine onley : /n \n");
                        // continue;
                }
        }
}

void DetectCommands(char *Buffer)
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
                                addWorker(Buffer);
                        }
                        else if (strcmp(word, "Shift") == 0)
                        {
                                addShift(Buffer);
                        }
                }
                else if (strcmp(word, "Report") == 0)
                {
                        // report command
                        word = strtok(NULL, " ");
                        if (strcmp(word, "Shift") == 0)
                        {
                                reportShiftDetails(Buffer);
                        }
                        else if (strcmp(word, "Workers") == 0)
                        {
                                reportWorkers(Buffer);
                        }
                        else if (strcmp(word, "Shifts") == 0)
                        {
                                reportShifts(Buffer);
                        }
                }
                else if (strcmp(word, "Remove") == 0)
                {
                        // renive command
                        removeWorker(Buffer);
                }

                word = strtok(NULL, " ");
        }
}