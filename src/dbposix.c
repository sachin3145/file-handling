#include "dbposix.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "logger.h"


#define LINE_BUFF_SIZE 64

static ssize_t readLine(int fd, char *buff, size_t maxLen){
    size_t i = 0;
    char c;

    while(i < maxLen - 1){
        ssize_t n = read(fd, &c, 1);

        if(n==1){
            buff[i++] = c;
            if(c=='\n'){
                break;
            }
        }
        else if(n==0){ //EOF
            break;
        }
        else{
            logSysError("read failed");
            return -1; // error
        }
    }
    buff[i] = '\0';
    return i;
}


DbResult posixPopulateBufferFromFile(EntryBuffer *buff){

    CHECK_NULL(buff, DB_FAILURE);
    CHECK_NULL(buff->entries, DB_FAILURE);

    int fd = open(FILE_NAME, O_RDONLY | O_CREAT,  0644);

    if(fd < 0){
        logSysError("open failed");
        return DB_FAILURE;
    }


    int ret, id=0, marks=0;
    char name[NAME_SIZE]={0}, lineBuffer[LINE_BUFF_SIZE];
    while(1){

        ssize_t n = readLine(fd, lineBuffer, LINE_BUFF_SIZE);


        if (n < 0) {
            logError("readLine failed");
            close(fd);
            return DB_FAILURE;
        }
        
        if(n==0){ 
            break;
        }

        if(n && sscanf(lineBuffer,"%d,%d,%[^\n]", &id, &marks, name) != 3){
            logError("malformed appdata\n");
            close(fd);
            return DB_FAILURE;

        }

        if(addEntry(buff, id, marks, name) == ENTRY_FAILURE){
            logError("addEntry failed");
            close(fd);
            return DB_FAILURE;
        }

    }

    close(fd);

    return DB_SUCCESS;
}


DbResult posixWriteBufferToFile(EntryBuffer *buff){

    CHECK_NULL(buff, DB_FAILURE);
    CHECK_NULL(buff->entries, DB_FAILURE);

    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(fd < 0){
        logSysError("open failed");
        return DB_FAILURE;
    }
    int len;
    char lineBuffer[LINE_BUFF_SIZE];
    for(size_t i = 0; i < buff->nextInsertPos; ++i){
        if(buff->entries[i].status == ENTRY_PRESENT){
            len =  sprintf(lineBuffer, "%d,%d,%s\n", 
                    buff->entries[i].id,
                    buff->entries[i].marks,
                    buff->entries[i].name);
            if (len < 0){
                logError("sprintf failed");
                close(fd);
                return DB_FAILURE;
            }
            if(write(fd, lineBuffer, len) != len){
                logSysError("write failed");
                close(fd);
                return DB_FAILURE;
            }
        }

    }

    close(fd);

    return DB_SUCCESS;
}
