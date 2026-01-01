#include<stdio.h>
#include<stdlib.h>

#include "logger.h"
#include "entry.h"
#include "dbiface.h"

typedef enum {
    MENU_CONTINUE,
    MENU_EXIT_REQUESTED,
    MENU_EXIT_FAILURE
} MenuResult;

typedef enum {
    INPUT_OK,
    INPUT_EOF,
    INPUT_EMPTY,
    INPUT_INVALID,
} InputResult;

static void printMenu();
static MenuResult handleUserChoice(int choice, EntryBuffer *buff);

static InputResult readInt(int *out);
static InputResult readString(char *buff, size_t buffSize);


int main(int argc, char **argv)
{
    EntryBuffer buff;
    int exitCode = EXIT_SUCCESS;

    if(initBuffer(&buff, 2) == BUFFER_FAILURE){
        logError("entry buffer initialization failed");
        return EXIT_FAILURE;
    }

    
    if(initDbIface(DB_BACKEND_STDIO) == DB_FAILURE){
        logError("dbiface initialization failed");
        exitCode = EXIT_FAILURE;
        goto cleanup;
    }

    if(populateBufferFromFile(&buff) == DB_FAILURE){
        logError("populateBufferFromFile failed");
        exitCode = EXIT_FAILURE;
        goto cleanup;
    }
        
    int choice;
    MenuResult ret;
    while(1){
        printMenu();
        if(readInt(&choice) != INPUT_OK){
            break;
        }
        ret = handleUserChoice(choice, &buff);
        if(ret == MENU_EXIT_REQUESTED){
            break;
        }
        else if(ret == MENU_EXIT_FAILURE){
            exitCode = EXIT_FAILURE;
            break;
        }
    }

    if(writeBufferToFile(&buff) == DB_FAILURE){
        logError("writeBufferFromFile failed");
        exitCode = EXIT_FAILURE;
        goto cleanup;
    }

    cleanup:
    if(freeBuffer(&buff) == BUFFER_FAILURE){
        logError("freeBuffer failed");
        return EXIT_FAILURE;
    }

    return exitCode;
}


static void printMenu()
{
    printf("\n------------------------------------- ENTRY MANAGER ----------------------------------\n");
    printf("0. Exit\t1. Add Entry\t2. Delete Entry\t3. Display Entry By ID\t4. Display All Entries\n");
    printf("Enter choice: ");
}


static MenuResult handleUserChoice(int choice, EntryBuffer *buff)
{
    switch(choice){
        case 0:
            return MENU_EXIT_REQUESTED;
        case 1:
            {
                int id, marks;
                char name[NAME_SIZE];
                printf("Enter Id: "); 
                if(readInt(&id) != INPUT_OK){
                    return MENU_EXIT_FAILURE;
                }
                printf("Enter Name: ");
                if(readString(name, NAME_SIZE) != INPUT_OK){
                    return MENU_EXIT_FAILURE;
                }
                printf("Enter Marks: ");
                if(readInt(&marks) != INPUT_OK){
                    return MENU_EXIT_FAILURE;
                }
                if(addEntry(buff, id, marks, name) == ENTRY_FAILURE){
                    return MENU_EXIT_FAILURE;
                }
                break;
            }
        case 2:
            {
                int id;
                printf("Enter id of entry to be delted:  ");
                if(readInt(&id) != INPUT_OK){
                    return MENU_EXIT_FAILURE;
                }
                if(deleteEntry(buff, id) == ENTRY_FAILURE){
                    return MENU_EXIT_FAILURE;
                }
                break;
            }
        case 3:
            {
                int id;
                printf("Enter id of entry to fetch:  ");
                if(readInt(&id) != INPUT_OK){
                    return MENU_EXIT_FAILURE;
                }
                if(printEntry(buff, id) == ENTRY_FAILURE){
                    return MENU_EXIT_FAILURE;
                }
                break;
            }
        case 4:
            {
                if(printAllEntries(buff) == ENTRY_FAILURE){
                    return MENU_EXIT_FAILURE;
                }
                break;
            }
        default:
            printf("please provide a integer input b/w 0 & 4\n");
    } 

    return MENU_CONTINUE;
}


static InputResult readInt(int *out)
{
    int ret = scanf("%d", out);
    if(ret == 1){
        int c;
        while((c = getchar()) != '\n' && c != EOF); // flush newline

        return INPUT_OK;
    }
    else if(ret == EOF){
        logError("[EOF] input stream closed");
        return INPUT_EOF;
    }

    // invalid input clear buffer
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    logError("[INVALID INPUT] readInt failed");
    return INPUT_INVALID;
}


static InputResult readString(char *buff, size_t buffSize)
{

    /*
     * Maximum usable input length is (buffSize - 2).
     *
     * Reason:
     *  - fgets() reads at most (buffSize - 1) characters and always appends '\0'
     *  - For interactive input, the newline '\n' is included when the user presses Enter
     *  - So the buffer layout becomes:
     *        [usable characters][ '\n' ][ '\0' ]
     *
     * Therefore, at most (buffSize - 2) visible characters can be safely accepted.
     */


    if(!fgets(buff, buffSize, stdin)){ 
        logError("[EOF] input stream closed");
        return INPUT_EOF;
    }

    size_t len = strlen(buff);
    if(len > 0 && buff[len-1] == '\n'){
        buff[len-1] = '\0';
        return INPUT_OK;
    }

    if(buff[0]=='\0'){
        logError("[EMPTY INPUT] no input provided");
        return INPUT_EMPTY;
    }
    
    // invalid input : input too long - flush remaining input
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    logError("[INVALID INPUT] input too long");

    return INPUT_INVALID;
}

