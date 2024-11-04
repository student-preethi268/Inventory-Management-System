#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Item structure
typedef struct Item {
    int item_id;
    char item_name[50];
    int quantity;
    char expiry_date[20];
    int threshold_quantity;
    struct Item* next;
} Item;

// Aisle structure
typedef struct Aisle {
    int aisle_number;
    Item* items;
    struct Aisle* next;
} Aisle;

Aisle* aisles[5] = { NULL };

// Function prototypes
void addOrUpdateItem(int aisle_number, int item_id, const char* item_name, int quantity, const char* expiry_date, int threshold_quantity);
void deleteItem(int aisle_number, int item_id);
void addOrUpdateAisle(int aisle_number);
void deleteAisle(int aisle_number);
int checkAvailability(int item_id, int quantity, const char* expiry_date);
void generateThresholdMessage(FILE* fp);
void generateExpiryMessage(FILE* fp);
void generateReadyToEatMessage(FILE* fp);
void mergeAisles(int aisle1_number, int aisle2_number);
void itemsBoughtTogether(FILE* fp, int item_id);

// Function to add or update an item in the linked list
void addOrUpdateItem(int aisle_number, int item_id, const char* item_name, int quantity, const char* expiry_date, int threshold_quantity) 
{   
    // Allocate memory for a new Item structure and initialize its fields
    Item* new_item = malloc(sizeof(Item)); 
    new_item->item_id = item_id;
    strcpy(new_item->item_name, item_name);  //strcpy-Copy to one memory location to another
    new_item->quantity = quantity;
    strcpy(new_item->expiry_date, expiry_date);
    new_item->threshold_quantity = threshold_quantity;
    new_item->next = NULL;

    // Check if the specified aisle number already exists in the inventory system
    Aisle* current = aisles[aisle_number - 1];
    if (current == NULL) 
    {   
        // If the aisle does not exist, allocate memory for a new Aisle structure and initialize its fields
        aisles[aisle_number - 1] = malloc(sizeof(Aisle));
        aisles[aisle_number - 1]->aisle_number = aisle_number;
        aisles[aisle_number - 1]->items = new_item;
        aisles[aisle_number - 1]->next = NULL;
    } 
    else 
    {   
        // If the aisle exists, traverse the linked list of Aisle pointers until the last Aisle is found
        while (current->next != NULL) 
        {
            current = current->next;
        }
        // Allocate memory for a new Aisle structure and initialize its fields
        current->next = malloc(sizeof(Aisle));
        current->next->aisle_number = aisle_number;
        current->next->items = new_item;
        current->next->next = NULL;
    }
}

// Function to delete an item from the store's inventory
void deleteItem(int aisle_number, int item_id) 
{   
    //Initialization
    Aisle* current = aisles[aisle_number - 1];
    //Traversal
    while (current != NULL) 
    {   
        //initialization the 2 ptrs
        Item* prev = NULL;
        Item* item = current->items;
        //Iterates
        while (item != NULL) 
        {   
            //We check the item_id matches to the target item_id to be deleted.
            if (item->item_id == item_id) 
            {   
                //deletion process
                if (prev == NULL) 
                {
                    current->items = item->next;
                } 
                else //not deleted 
                {
                    prev->next = item->next;
                }
                free(item); //free memory allocated
                return;
            }
            prev = item;
            item = item->next;
        }
        current = current->next; //traversal update
    }
}

// Function to add or update an aisle in the linked list
void addOrUpdateAisle(int aisle_number) 
{   
    // Get the current aisle pointer based on the aisle_number
    Aisle* current = aisles[aisle_number - 1];
    // Check if the aisle is currently NULL
    if (current == NULL) 
    {   
        // If the aisle does not exist, allocate memory for a new aisle
        aisles[aisle_number - 1] = malloc(sizeof(Aisle));
        // Initialize the new aisle's attributes
        aisles[aisle_number - 1]->aisle_number = aisle_number;
        aisles[aisle_number - 1]->items = NULL; // Initialize items to NULL
        aisles[aisle_number - 1]->next = NULL;  // Initialize next to NULL
    }
}

// Function to delete an aisle from the linked list
void deleteAisle(int aisle_number) 
{   
    //Initialization aisle ptr.
    Aisle* current = aisles[aisle_number - 1];
    if (current != NULL)//checking the aisle existence 
    {   
        //deletion process of aisle.
        aisles[aisle_number - 1] = current->next;
        free(current);  //free memory allocation
    }
}

// Function to check the availability of a particular item
int checkAvailability(int item_id, int quantity, const char* expiry_date) 
{  
    // Obtain the pointer to the first aisle in the aisles array corresponding to the given item_id
    Aisle* current = aisles[item_id - 1];
    //traverse throught the linked list
    while (current != NULL) 
    {   
        //get the ptr to the first item in the current aisle
        Item* item = current->items;
        while (item != NULL) 
        {   
            //current item matches the provided item_id &satisfies quantity and expiry_date.
            if (item->item_id == item_id && item->quantity >= quantity && strcmp(item->expiry_date, expiry_date) > 0) {
                return 1; // Item is available
            }
            //move to the nextitem in the linked list.
            item = item->next;
        }
        //move to the next aisle in the linked list.
        current = current->next;
    }
    return 0; // Item is not available
}

// Function to generate a message when an item goes beyond its threshold quantity
void generateThresholdMessage(FILE* fp) 
{  
    //Iterate over ecah aisle
    for (int i = 0; i < 5; i++) 
    {   
        //ptr to current aisle
        Aisle* current = aisles[i];
        //traverse
        while (current != NULL) 
        {   
            //ptr to the 1st item int he current aisle
            Item* item = current->items;
            //traverse
            while (item != NULL) 
            {
                //check if the quantity of the item exceeds the thershold quantity %d. order soon!\n
                if (item->quantity > item->threshold_quantity) 
                {
                    //generat a msg & wrt it to the file pointed by fp
                    fprintf(fp, "Item %s in aisle %d has quantity %d, which exceeds the threshold quantity %d. Order soon!\n",
                        item->item_name, current->aisle_number, item->quantity, item->threshold_quantity);
                }
                item = item->next;//move to the next item in ll
            }
            current = current->next; //move to the next aisle in ll
        }
    }
}

// Function to generate a message for ready-to-eat items with expiry within one week
void generateExpiryMessage(FILE* fp) 
{   
    //setting the current time
    time_t now;
    time(&now);
    struct tm *current_time = localtime(&now);

    current_time->tm_mday += 7; // Add 7 days
    mktime(current_time);
    //iterate over each side
    for(int i = 0; i < 5; i++)
    {   
        //get the ptrs to the current aisle
        Aisle* current = aisles[i];
        //traverse
        while (current != NULL) 
        {
            //get the ptrs to the first item in the current aisle
            Item* item = current->items;
            //traverse 
            while (item != NULL) 
            {
                //convert the expiry date str to a time structure.
                struct tm expiry_tm;
                strptime(item->expiry_date, "%Y-%m-%d", &expiry_tm);
                //cpr the expiry date with the current time.
                if (difftime(mktime(&expiry_tm), mktime(current_time)) < 0) 
                {
                    fprintf(fp, "Item %s in aisle %d with expiry date %s is approaching within one week.\n",
                    item->item_name, current->aisle_number, item->expiry_date);
                }
                item = item->next;//move to the next item in ll
            }
            current = current->next;//move to the next aisle in ll
        }
    }
}

// Function to generate a message for dairy products likely to expire within one day
void generateReadyToEatMessage(FILE* fp) 
{
    //obtain the current time.
    time_t now;
    time(&now);
    //convert current time to local time 
    struct tm* current_time = localtime(&now);
    //add one day to the current time
    current_time->tm_mday += 1; // Add 1 day
    //convert modified current time back to time_t format
    mktime(current_time);
    //iterate
    for (int i = 0; i < 5; i++) 
    {   
       //get the ptr to the current aisle
        Aisle* current = aisles[i];
        //traverse
        while (current != NULL) 
        {   
            //get the ptr to the first item to the current aisle
            Item* item = current->items;
            //traverse
            while (item != NULL) 
            {  
                //convert expiry date str to the struct tm
                struct tm expiry_tm;
                strptime(item->expiry_date, "%Y-%m-%d", &expiry_tm);
                //calculate the diff in sec btw expiry date and modified current time
                if (difftime(mktime(&expiry_tm), mktime(current_time)) < 0) 
                {
                    fprintf(fp, "Dairy product %s in aisle %d with expiry date %s is likely to expire within one day.\n",
                    item->item_name, current->aisle_number, item->expiry_date);
                }
                item = item->next;//move to the next item in ll
            }
            current = current->next;//move to the next aisle in ll
        }
    }
}

// Function to merge two aisles into a single aisle
void mergeAisles(int aisle1_number, int aisle2_number) 
{
    //get the ptr to the aisle 1 & aisle 2
    Aisle* aisle1 = aisles[aisle1_number - 1];
    Aisle* aisle2 = aisles[aisle2_number - 1];
    //check if either aisle doesnot exist
    if (aisle1 == NULL || aisle2 == NULL) 
    {
        printf("One or both of the aisles does not exist.\n");
        return;
    }
    // Find the last node of aisle1
    while (aisle1->next != NULL) 
    {
        aisle1 = aisle1->next;
    }
    // Attach aisle2 to aisle1
    aisle1->next = aisle2;
    aisles[aisle2_number - 1] = NULL; // Remove aisle2
}

// Function to write items bought together with the given item_id to a file
void itemsBoughtTogether(FILE* fp, int item_id) 
{
    // Implementation to find items bought together is required based on transaction data.
    fprintf(fp, "Items often bought with item_id %d are:\n", item_id);
    fprintf(fp, "1. Milk\n");
    fprintf(fp, "2. Bread\n");
    fprintf(fp, "3. Eggs\n");
}

int main() 
{
    //open the ip file for reading.
    FILE* input_file = fopen("file.txt", "r");
    if (input_file == NULL) 
    {
        printf("Error opening input file.\n");
        return 1;
    }

    // Read input from file and build the aisles and items
    int aisle_number, item_id, quantity, threshold_quantity;
    char item_name[50], expiry_date[20];
    //read data from the ip file & populate the data structures
    while (fscanf(input_file, "%d %d %s %d %s %d", &aisle_number, &item_id, item_name, &quantity, expiry_date, &threshold_quantity) != EOF) 
    {  
        //add/update the aislewith the aisle number
        addOrUpdateAisle(aisle_number);
        //add/update the item with provided details
        addOrUpdateItem(aisle_number, item_id, item_name, quantity, expiry_date, threshold_quantity);
    }
    //close the ip file
    fclose(input_file);

    //open the output file for wrting
    FILE* output_file = fopen("output.txt", "w");
    if (output_file == NULL) 
    {
        printf("Error opening output file.\n");
        return 1;
    }
    //generate thershold,expiry,and ready-to-eat msgs and wrt them to the output file
    generateThresholdMessage(output_file);
    generateExpiryMessage(output_file);
    generateReadyToEatMessage(output_file);
    //close the output file
    fclose(output_file);
    return 0;
}
