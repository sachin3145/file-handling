#include "dbstdio.h"

DbResult stdioPopulateBufferFromFile(EntryBuffer *buff){

    CHECK_NULL(buff, DB_FAILURE);
    CHECK_NULL(buff->entries, DB_FAILURE);

    FILE *fp = NULL;
    fp = fopen(FILE_NAME, "a+");

    if(fp == NULL){
        logSysError("fopen failed");
        return DB_FAILURE;
    }

    fseek(fp, 0, SEEK_SET);

    int ret, id=0, marks=0;
    char name[NAME_SIZE]={0};
    while(1){
        ret = fscanf(fp,"%d,%d,%31[^\n]", &id, &marks, name);

        if(ret == 3){
            if(addEntry(buff, id, marks, name) == ENTRY_FAILURE){
                logError("addEntry failed");
                fclose(fp);
                return DB_FAILURE;
            }
        }
        else if(ret == EOF || feof(fp)){
            break;
        }
        else{
            logError("malformed appdata\n");
                fclose(fp);
                return DB_FAILURE;
        }
    }

    fclose(fp);

    return DB_SUCCESS;
}

DbResult stdioWriteBufferToFile(EntryBuffer *buff){

    CHECK_NULL(buff, DB_FAILURE);
    CHECK_NULL(buff->entries, DB_FAILURE);

    FILE *fp = NULL;
    fp = fopen(FILE_NAME, "w");

    if(fp == NULL){
        logSysError("fopen failed");
        return DB_FAILURE;
    }

    for(size_t i = 0; i < buff->nextInsertPos; ++i){
        if(buff->entries[i].status == ENTRY_PRESENT){
            if ( fprintf(fp, "%d,%d,%s\n", 
                        buff->entries[i].id,
                        buff->entries[i].marks,
                        buff->entries[i].name) < 0){
                logError("fprintf failed");
            }
        }
    }

    fclose(fp);

    return DB_SUCCESS;
}
