#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "../data/calendar.txt"

struct Event
{
    int day;
    int month;
    int year;

    int hour;
    int minute;

    char name[50];
    char place[100];

    char description[1000];
};

struct Node
{
    struct Event event;
    struct Node* left;
    struct Node* right;
};

struct Node* append(struct Node*, struct Event);
struct Node* removeByName(struct Node*);
struct Node* clearDay(struct Node*);

int readFromFile(struct Node*);
int writeToFile(struct Node*);

struct Event addEvent();

void printList(struct Node*);
void printCalendar(struct Node*);

int countList(struct Node*);
int compareEvents(struct Event, struct Event);
void freeList(struct Node*);

int main()
{
    struct Node* head;

    while(1)
    {
        int choice;

        printf("Welcome to Planix!\n\n");
        printf("What would you like to do?\n");
        printf("1 - Add an event\n");
        printf("2 - Remove an event\n");
        printf("3 - Load calendar from file\n");
        printf("4 - Print events in calendar form\n");
        printf("5 - Print events in list form\n");
        printf("6 - Save calendar to file\n");
        printf("7 - Clear a day\n");
        printf("8 - Exit\n\n");

        scanf("%d", &choice);
        while(getchar() != '\n');

        switch(choice)
        {
            case 1:
                head = append(head, addEvent());
                break;
            case 2:
                head = removeByName(head);
                break;
            case 3:
                if(!readFromFile(head))
                {
                    printf("Successfully loaded calendar from file.\n");
                }
                else 
                {
                    printf("Failed to load calendar from file.\n");
                }
                break;
            case 4: 
                printCalendar(head);
                break;
            case 5:
                printList(head);
                break;
            case 6:
                writeToFile(head);
                break;
            case 7:
                clearDay(head);
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    return 0;
}

struct Node* append(struct Node* head, struct Event event)
{
    struct Node* node = malloc(sizeof(struct Node));

    node->event = event;
    node->left = NULL;
    node->right = NULL;

    if(head == NULL)
    {
        return node;
    }

    struct Node* current = head;

    while(current != NULL)
    {
        if(compareEvents(current->event, event) == 2)
        {
            if(current->right == NULL)
            {
                current->right = node;
                return head;
            }

            current = current->right;
        }
        else
        {
            if(current->left == NULL)
            {
                current->left = node;
                return head;
            }

            current = current->left;
        }
    }

    return head;
}