//build_d (d stands for dummy)

//modified on 29 Dec 2004 to include multiple files capability
//this program is created for the soUl purpose of fUn

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_FILES 10

int MaxFiles;
int DaisyChain = 0;
FILE * files[MAX_FILES];
FILE * CurrentFile;

int CheckEOF (void)
{
	int i;

	for (i = 0;i < MaxFiles;i++)
	{
		if (files[i]!=NULL && !feof(files[i]))
			return 1;
	}

	return 0;
}

void AssignIfEOF (void)
{
	int i;

	if (files[DaisyChain]!=NULL && feof(files[DaisyChain]))
	{
		for (i = 0;i < MaxFiles;i++)
		{
			DaisyChain = (DaisyChain + 1) % MaxFiles;
			if (files[DaisyChain] != NULL && !feof(files[DaisyChain]))
				break;
		}
	}

	CurrentFile = files[DaisyChain];
}

void OpenMultiple (void)
{
	FILE * ListFile;
	char FileName[257];
	int i = 0;
	int j = 0;

	ListFile = fopen ("FileList.txt","r");

	if (ListFile == NULL)
		exit(0);

	fscanf(ListFile,"%d",&MaxFiles);

	if (MaxFiles < 1)
		exit(0);

	for (i = 0;i < MaxFiles;i++)
	{
		fscanf(ListFile,"%s",FileName);
		files[j] = fopen(FileName,"r");

		if (files[j] != NULL)
			j++;
	}

	MaxFiles = j;

	if (MaxFiles == 0)
		exit(0);
}

void CloseMultiple (void)
{
	int i;

	for (i = 0;i < MaxFiles;i++)
	{
		if (files[i] != NULL)
			fclose(files[i]);
	}
}

void ChangeFile (void)
{
	DaisyChain = (DaisyChain + 1) % MaxFiles;
	CurrentFile = files[DaisyChain];
}

int main (int argc, char ** argv)
{
    char ch;
    int slept = 0;
    int forever = 0;
    
    if (argc == 2)
    {
        if ((strcmp(argv[1],"-f") == 0) || (strcmp(argv[1],"-F") == 0))
            forever = 1;
    }

    do
    {
		OpenMultiple();

        while (CheckEOF())
        {
			AssignIfEOF();

			ch = fgetc(CurrentFile);
        
			if (ch == 10 || ch == 13)
			{
				if (slept == 0)
				{
					sleep((rand() % ((rand() % 1030) + 1)) + 23);
					slept = 1;
				}
				else
				{
					slept = 0;
				}

				if ((ch == 10) && ((rand() % 500) > 250))
				{
					ChangeFile();
				}
			}
        
			printf("%c",ch);
        }
		CloseMultiple();

    } while (forever);
}
