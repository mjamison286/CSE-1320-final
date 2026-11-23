#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "./data/calendar.txt"

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

struct Node* readFromFile(struct Node*);
int writeToFile(struct Node*);

struct Event addEvent();

void printList(struct Node*);
void printCalendar(struct Node*);

int countList(struct Node*);
int compareEvents(struct Event, struct Event);
void printTraversal(struct Node*);
void saveTraversal(struct Node*, FILE*);
void countTraversal(struct Node*, int*);
void timeTraversal(struct Node*, struct Event[], int*, int, int);
void fillArrTraversal(struct Node*, struct Event[], int*);
struct Node* freeList(struct Node*);

int main()
{
    struct Node* head = NULL;

    while(1)
    {
        int choice;

        do
        {
            while(getchar() != '\n');

            printf("Welcome to Planix!\n\n");
            printf("What would you like to do?\n");
            printf("1 - Add an event\n");
            printf("2 - Remove an event\n");
            printf("3 - Load calendar from file\n");
            printf("4 - Print events for a specific month\n");
            printf("5 - Print events in list form\n");
            printf("6 - Save calendar to file\n");
            printf("7 - Clear a day\n");
            printf("8 - Exit\n");
        }
        while(scanf("%d", &choice) != 1);

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
                head = readFromFile(head);
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
                head = clearDay(head);
                break;
            case 8:
                head = freeList(head);
                return 0;
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

    if(node == NULL)
    {
        printf("Failed to allocate memory for new node.\n");
        exit(1);
    }

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
            else
            {
                current = current->right;
            }
        }
        else
        {
            if(current->left == NULL)
            {
                current->left = node;
                return head;
            }
            else
            {
                current = current->left;
            }
        }
    }

    return head;
}

struct Node* removeByName(struct Node* head)
{
    if(head == NULL)
    {
        printf("There is no event in your calendar to remove.\n");
        return NULL;
    }

    char nameToRemove[50];

    printf("Enter the name of the event you want to remove: ");
    fgets(nameToRemove, sizeof(nameToRemove), stdin);
    nameToRemove[strcspn(nameToRemove, "\n")] = '\0';

    struct Event eventArr[countList(head)];

    int count = 0;

    fillArrTraversal(head, eventArr, &count);

    head = freeList(head);

    for(int i = 0; i < count; i++)
    {
        if(strcmp(nameToRemove, eventArr[i].name) != 0)
        {
            head = append(head, eventArr[i]);
        }
    }

    return head;
}

struct Node* clearDay(struct Node* head)
{
    if(head == NULL)
    {
        printf("There are no events in your calender to remove.\n");
        return NULL;
    }

    int day;
    int month;
    int year;
    char c;

    do
    {
        while(getchar() != '\n');

        printf("Enter the date of the day you want to clear (MM/DD/YYYY): ");
    }
    while(scanf("%d%c%d%c%d", &month, &c, &day, &c, &year) != 5);

    struct Event eventArr[countList(head)];

    int count = 0;

    fillArrTraversal(head, eventArr, &count);

    head = freeList(head);

    for(int i = 0; i < count; i++)
    {
        if(!(eventArr[i].day == day || eventArr[i].month == month || eventArr[i].year == year))
        {
            head = append(head, eventArr[i]);
        }
    }

    return head;
}

struct Node* readFromFile(struct Node* head)
{
    FILE* fptr;
    fptr = fopen(PATH, "r");

    if(fptr == NULL)
    {
        printf("Failed to open the file for reading. Nothing was read.\n");
        return NULL;
    }

    char* readLine = NULL;
    size_t allocated = 0;

    while(getline(&readLine, &allocated, fptr) != -1)
    {
        readLine[strcspn(readLine, "\n")] = '\0';

        char* strArr[8];
        int count = 1;
        char* tok;

        tok = strtok(readLine, "|");

        strArr[0] = tok;

        while(tok != 0)
        {
            tok = strtok(0, "|");

            strArr[count] = tok;

            count++;
        }

        struct Event e;

        e.month = atoi(strArr[0]);
        e.day = atoi(strArr[1]);
        e.year = atoi(strArr[2]);
        e.hour = atoi(strArr[3]);
        e.minute = atoi(strArr[4]);
        strcpy(e.name, strArr[5]);
        strcpy(e.place, strArr[6]);
        strcpy(e.description, strArr[7]);

        head = append(head, e);
    }

    free(readLine);
    fclose(fptr);
    return head;
}

int writeToFile(struct Node* head)
{
    if(head == NULL)
    {
        int choice;

        do
        {
            printf("There are no events to save, are you sure you want to clear every event?\n");
            printf("1 - yes\n");
            printf("2 - no\n");
        }
        while(scanf("%d", &choice) != 1);

        if(choice == 1)
        {
            FILE* f = fopen(PATH, "w");
            fclose(f);
        }
        else if(choice == 2)
        {
            printf("Nothing was cleared.\n");
        }
        else
        {
            printf("Incorrect input. Nothing was cleared.\n");
        }

        return -1;
    }

    FILE* fptr;
    fptr = fopen(PATH, "w");

    if(fptr == NULL)
    {
        printf("Failed to open the file for writing.\n");
        return -1;
    }

    saveTraversal(head, fptr);

    fclose(fptr);
    return 0;
}

struct Event addEvent()
{
    struct Event event;
    
    char c;

    do
    {
        while(getchar() != '\n');

        printf("Enter the date of the event (MM/DD/YYYY): ");
    }
    while(scanf("%d%c%d%c%d", &(event.month), &c, &(event.day), &c, &(event.year)) != 5);

    do
    {
        while(getchar() != '\n');

        printf("Enter the time of the event (HH:MM): ");
    }
    while(scanf("%d%c%d", &(event.hour), &c, &(event.minute)) != 3);

    while(getchar() != '\n');

    printf("Enter the name of the event: ");
    fgets(event.name, sizeof(event.name), stdin);
    event.name[strcspn(event.name, "\n")] = '\0';

    printf("Enter the address or the location of the event: ");
    fgets(event.place, sizeof(event.place), stdin);
    event.place[strcspn(event.place, "\n")] = '\0';

    printf("Enter a short description of the event: ");
    fgets(event.description, sizeof(event.description), stdin);
    event.description[strcspn(event.description, "\n")] = '\0';

    return event;
}

void printList(struct Node* head)
{
    if(head == NULL)
    {
        printf("There are no events in your calendar.\n");
        return;
    }

    printTraversal(head);    
}

void printCalendar(struct Node* head)
{
    if(head == NULL)
    {
        printf("There are no events in your calender.\n");
        return;
    }

    struct Event eventArr[countList(head)];

    int count = 0;
    int month;
    int year;
    char c;

    do 
    {
        while(getchar() != '\n');

        printf("Enter the month you want to view (MM/YYYY): ");
    }
    while (scanf("%d%c%d", &month, &c, &year) != 3);

    timeTraversal(head, eventArr, &count, month, year);

    for(int i = 0; i < count; i++)
    {
        struct Event e;

        e = eventArr[i];

        printf("%d/%d/%d at %d:%0.2d - %s, located at %s.\n", e.month, e.day, e.year, e.hour, e.minute, e.name, e.place);
    }
}

int countList(struct Node* head)
{   
    if(head == NULL)
    {
        return 0;
    }

    int count = 0;

    countTraversal(head, &count);

    return count;
}

int compareEvents(struct Event e1, struct Event e2)
{
    long long e1minutes = e1.minute + (e1.hour * 60) + (e1.day * 24 * 60) + ((int)(e1.month * 30.44 * 24 * 60)) + (e1.year * 365 * 24 * 60);
    long long e2minutes = e2.minute + (e2.hour * 60) + (e2.day * 24 * 60) + ((int)(e2.month * 30.44 * 24 * 60)) + (e2.year * 365 * 24 * 60);

    if(e1minutes > e2minutes)
    {
        return 1;
    }
    else if(e2minutes > e1minutes)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

void printTraversal(struct Node* node)
{
    if(node->left != NULL)
    {
        printTraversal(node->left);
    }
    printf("%d/%d/%d at %d:%0.2d - %s, located at %s.\n", node->event.month, node->event.day, node->event.year, node->event.hour, node->event.minute, node->event.name, node->event.place);
    if(node->right != NULL)
    {
        printTraversal(node->right);
    }
}

void saveTraversal(struct Node* node, FILE* fptr)
{
    if(fptr == NULL)
    {
        printf("Something went wrong with the file pointer when saving. Nothing saved.\n");
        return;
    }

    if(node->left != NULL)
    {
        saveTraversal(node->left, fptr);
    }
    fprintf(fptr, "%d|%d|%d|%d|%d|%s|%s|%s\n", node->event.month, node->event.day, node->event.year, node->event.hour, node->event.minute, node->event.name, node->event.place, node->event.description);
    if(node->right != NULL)
    {
        saveTraversal(node->right, fptr);
    }
}

void countTraversal(struct Node* node, int* cptr)
{
    if(node->left != NULL)
    {
        countTraversal(node->left, cptr);
    }
    *cptr = *cptr + 1;
    if(node->right != NULL)
    {
        countTraversal(node->right, cptr);
    }
}

void timeTraversal(struct Node* node, struct Event arr[], int* count, int month, int year)
{
    if(node->left != NULL)
    {
        timeTraversal(node->left, arr, count, month, year);
    }

    int currMonth = node->event.month;
    int currYear = node ->event.year;

    if(currMonth == month && currYear == year)
    {
        arr[*count] = node->event;

        *count = *count + 1;
    }

    if(node->right != NULL)
    {
        timeTraversal(node->right, arr, count, month, year);
    }
}

void fillArrTraversal(struct Node* node, struct Event* arr, int* cptr)
{
    if(node->left != NULL)
    {
        fillArrTraversal(node->left, arr, cptr);
    }

    arr[*cptr] = node->event;
    *cptr = *cptr + 1;

    if(node->right != NULL)
    {
        fillArrTraversal(node->right, arr, cptr);
    }   
}

void freeTraversal(struct Node* node)
{
    if(node->left != NULL)
    {
        freeTraversal(node->left);
    }
    if(node->right != NULL)
    {
        freeTraversal(node->right);
    }

    free(node);
}

struct Node* freeList(struct Node* head)
{
    if(head != NULL)
    {
        freeTraversal(head);
    }

    return NULL;
}