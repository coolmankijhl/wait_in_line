/*
Michael Cartwright
1002167357

This project simulates students in queue for one of two advisors, it will read
students data from a file and store it into a singly linked list.

It will sort the linked list by advisor, split the linked list into two for both advisors, then print the line simulation and processing queue.

It will then print a visualization of students waiting in line for their advisor in text.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct studentNode
{
         int ID;
         int time;
         int advisor;
         char name[100];
         struct studentNode *next;
} studentNode;

studentNode* ReadCloseFile(FILE *fileHandle);
void printQueueLine(struct studentNode* headOne, struct studentNode* headTwo);
void printProcessingQueue(struct studentNode* headOne, struct studentNode* headTwo, int oneTime, int twoTime);
void destructor(struct studentNode* head);
studentNode* seperateLists(struct studentNode* head, struct studentNode* headTwo);
studentNode* organizeData(struct studentNode** head, int listSize);
studentNode* swapNodes(struct studentNode* n1, struct studentNode* n2);
int getListSize(struct studentNode* head);

int main()
{
        //prompts user for a file to open in read mode
	char file[100];
        printf("Enter filename: ");
        scanf("%s", file);
        FILE *fileHandle;
        fileHandle = fopen(file, "r");

        if(!fileHandle)
        {
                printf("\nFile not found. Exiting.\n");
                exit(0);
        }
	
	struct studentNode *head =  ReadCloseFile(fileHandle);
	
	organizeData(&head, getListSize(head));

	struct studentNode *headTwo = seperateLists(head, headTwo);
	
	printQueueLine(head, headTwo);

	printProcessingQueue(head, headTwo, 0, 0);

	destructor(head);
	destructor(headTwo);

}

//reads into studentNodes singly linked list then closes file
studentNode* ReadCloseFile(FILE* fileHandle)
{	
	struct studentNode *node = (struct studentNode*)calloc(1, sizeof(struct studentNode));
	struct studentNode *nodePrev = node;
	struct studentNode *head = node;

	char buffer[100];
	fgets(buffer, sizeof(buffer), fileHandle);

	while(!feof(fileHandle)) 
	{
		nodePrev = node;
		fscanf(fileHandle, "%d %d %d %s ", &node->ID, &node->advisor, &node->time, node->name);
		
		node = (struct studentNode*)calloc(1, sizeof(struct studentNode));			
                nodePrev->next = node;

	}
	
	nodePrev->next = NULL;
	free(node);	
	fclose(fileHandle);

	return head;

}

//organizes linked list by advisor
studentNode* organizeData(struct studentNode** head, int listSize)
{
	struct studentNode** h;
	int swapped;

	for(int i = 0; i <= listSize; i++)
	{
		h = head;
		swapped = 0;

		for(int j = 0; j < listSize-i-1; j++)
		{
			struct studentNode* p1 = *h;
			struct studentNode* p2 = p1->next;

			if(p1->advisor > p2->advisor)
			{
				
				*h = swapNodes(p1, p2);
				swapped = 1;
			}

			h = &(*h)->next;
		}

		if(swapped == 0)
			break;
	}
}

//swaps two adjacent nodes for organizeData()
studentNode* swapNodes(struct studentNode* n1, struct studentNode* n2)
{
	struct studentNode* temp = n2->next;
	n2->next = n1;
	n1->next = temp;

	return n2;
}

void printQueueLine(struct studentNode* headOne, struct studentNode* headTwo)
{
	printf("\n Line at Desk 1\t\t| Line at Desk 2\n Time  ID Name\t\t| Time  ID Name\n");

	while(headOne != NULL || headTwo != NULL)
	{
		if(headOne != NULL)
		{
			printf(" %4d %3d %-14s|", headOne->time, headOne->ID, headOne->name);
			headOne = headOne->next;
		}
		else
			printf("\t\t\t|");
		if(headTwo != NULL)
		{
			printf(" %4d %3d %s", headTwo->time, headTwo->ID, headTwo->name);
			headTwo = headTwo->next;
		}
		printf("\n");
	}
}

void printProcessingQueue(struct studentNode* headOne, struct studentNode* headTwo, int oneTime, int twoTime)
{
	int processRequestsDeskOne = 0;
	int processRequestsDeskTwo = 0;
	
	printf("\nProcessing Queues:\nleave  ID  Name\n");

	while(headOne != NULL || headTwo != NULL)
	{
		if(headOne == NULL || (headTwo != NULL && twoTime + headTwo->time < oneTime + headOne->time))
		{
			printf("%5d%4d %s\n", twoTime+headTwo->time, headTwo->ID, headTwo->name);
			twoTime += headTwo->time;
			headTwo = headTwo->next;
			processRequestsDeskTwo++;
		}
		else
		{
			printf("%5d%4d %s\n", oneTime+headOne->time, headOne->ID, headOne->name);
			oneTime += headOne->time;
			headOne = headOne->next;
			processRequestsDeskOne++;
		}
	}

	printf("\nTotal requests processed at Desk 1: %d\nTotal requests proccessed at Desk 2: %d\n", processRequestsDeskOne, processRequestsDeskTwo);
}

//frees memory allocated to singly linked lists
void destructor(struct studentNode* head)
{
	if(head->next == NULL)
	{
		free(head);
		return;
	}
	struct studentNode* ptr = head;
	while(head != NULL)
	{
		head = head->next;
		free(ptr);
		ptr = head;
	}
	free(head);
}

//seperates head into two linked lists defined by their advisor number
studentNode* seperateLists(struct studentNode* head, struct studentNode* headTwo)
{
	struct studentNode *ptr = head;

	while(ptr->next != NULL)
	{
		if(ptr->next->advisor == 2)
		{
			headTwo = ptr->next;
			ptr->next = NULL;
			return headTwo;
		}
		ptr = ptr->next;
	}
}

int getListSize(struct studentNode* head)
{
	int i = 0;
	while(head->next != NULL)
	{
		head = head->next;
		i++;
	}
	return i+1;	
}
