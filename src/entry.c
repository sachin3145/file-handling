#include "entry.h"

#include<stdlib.h>
#include<stdio.h>
#include "logger.h"




BufferResult initBuffer(EntryBuffer *buff, int initialCapacity)
{
    CHECK_NULL(buff, BUFFER_FAILURE);
    if(initialCapacity <= 0){
        initialCapacity = 64;
        logWarn("invalid initial capacity provided using default capacity " 
                XSTR(DEFAULT_BUFFER_SIZE));        
    }


    buff->entries = (Entry *)(calloc(initialCapacity, sizeof(Entry)));
    CHECK_NULL(buff->entries, BUFFER_FAILURE);

    buff->capacity = initialCapacity;
    buff->currentSize = 0;
    buff->deletedCount = 0;
    buff->nextInsertPos = 0;

    return BUFFER_SUCCESS;
}


BufferResult freeBuffer(EntryBuffer *buff)
{
    CHECK_NULL(buff, BUFFER_FAILURE);
    CHECK_NULL(buff->entries, BUFFER_FAILURE);
    free(buff->entries);
    buff->entries = NULL;
    buff->capacity = 0;
    buff->currentSize = 0;
    buff->deletedCount = 0;
    buff->nextInsertPos = 0;

    return BUFFER_SUCCESS;   
}


BufferResult compactBuffer(EntryBuffer *buff)
{
    CHECK_NULL(buff, BUFFER_FAILURE);
    CHECK_NULL(buff->entries, BUFFER_FAILURE);
    size_t ni = 0;
    for(size_t i = 0; i < buff->nextInsertPos; ++i){
        if(buff->entries[i].status == ENTRY_PRESENT){
            buff->entries[ni] = buff->entries[i];
            ni += 1;
        }
    }
    buff->nextInsertPos = ni;
    buff->deletedCount = 0;

    return BUFFER_SUCCESS;
}


BufferResult expandBuffer(EntryBuffer *buff)
{
    CHECK_NULL(buff, BUFFER_FAILURE);
    CHECK_NULL(buff->entries, BUFFER_FAILURE);
    int newCapacity = buff->capacity * 2;
    Entry *newEntries = (Entry *)realloc(buff->entries, newCapacity * sizeof(Entry));
    if(newEntries == NULL){
        logError("Realloc Failed: cannot expand entry buffer");
        return BUFFER_FAILURE;
    }
    buff->entries = newEntries;
    buff->capacity = newCapacity;

    return BUFFER_SUCCESS ;
}

static int compareEntryById(const void *a, const void *b){
    const Entry *ea = (Entry *)a;
    const Entry *eb = (Entry *)b;

    if(ea->id < ea->id) return -1;
    if(ea->id > ea->id) return 1;
    return 1;
}

BufferResult sortBuffer(EntryBuffer *buff)
{
    CHECK_NULL(buff, BUFFER_FAILURE);
    CHECK_NULL(buff->entries, BUFFER_FAILURE);
    if(buff->nextInsertPos <= 1){
        return BUFFER_SUCCESS;
    }
    qsort(buff->entries, buff->nextInsertPos, sizeof(Entry), compareEntryById);

    return BUFFER_SUCCESS;
}


EntryResult findEntry(EntryBuffer *buff, int id, Entry **res)
{
    CHECK_NULL(buff, ENTRY_FAILURE);
    CHECK_NULL(buff->entries, ENTRY_FAILURE);

/*

// not using binary search as 
// maintaining sorted while insertion [shifting elements] defeatsthe purpose of lazy deletion

// while implemnting binary search only return populate if entry.status == ENTRY_PRESENT
    Entry key = { .id = id };
    res = bsearch(&key, buff->entries, buff->nextInsertPos, sizeof(Entry), compareEntryById); 
    if(res == NULL){
        return ENTRY_NOT_FOUND;
    }
*/

    for(size_t i = 0; i < buff->nextInsertPos; ++i){
        if(buff->entries[i].id == id && buff->entries[i].status == ENTRY_PRESENT){
            *res = &(buff->entries[i]);
            return ENTRY_SUCCESS;
        }
    }

    return ENTRY_NOT_FOUND;
}


EntryResult addEntry(EntryBuffer *buff, int id, int marks, const char name[NAME_SIZE])
{
    CHECK_NULL(buff, ENTRY_FAILURE);
    CHECK_NULL(buff->entries, ENTRY_FAILURE);
    if(buff->nextInsertPos == buff->capacity){
        if(buff->deletedCount > 0) compactBuffer(buff);
        else expandBuffer(buff);
    }

    Entry *entry = NULL;
    EntryResult searchRes = findEntry(buff, id, &entry);

    switch(searchRes)
    {
        case ENTRY_SUCCESS:
            {
                logError("duplicate entry not allowed");
                return ENTRY_FAILURE;
            }
        case ENTRY_FAILURE:
            {
                logError("findEntry failed");
                return ENTRY_FAILURE;
            }
    }

    int ret = snprintf(buff->entries[buff->nextInsertPos].name, NAME_SIZE, name);
    if(ret < 0){
        logError("snprintf failed");
        return ENTRY_FAILURE;
    }
    else if(ret > NAME_SIZE){
        logError("name larger than max allowable size");
        buff->entries[buff->nextInsertPos].name[0] = '\0';
        return ENTRY_FAILURE;
    }   
    buff->entries[buff->nextInsertPos].id = id;
    buff->entries[buff->nextInsertPos].marks = marks;
    buff->entries[buff->nextInsertPos].status = ENTRY_PRESENT;

    buff->currentSize += 1;
    buff->nextInsertPos += 1;

    return ENTRY_SUCCESS;
}

EntryResult deleteEntry(EntryBuffer *buff, int id)
{
    CHECK_NULL(buff, ENTRY_FAILURE);
    CHECK_NULL(buff->entries, ENTRY_FAILURE);
    Entry *entryP = NULL;
    EntryResult searchRes = findEntry(buff, id, &entryP);

    switch(searchRes)
    {
        case ENTRY_NOT_FOUND:
            {
                logWarn("entry not found");
                return ENTRY_NOT_FOUND;
            }
        case ENTRY_FAILURE:
            {
                logError("findEntry failed");
                return ENTRY_FAILURE;
            }
    }

    entryP->status = ENTRY_DELETED;
    buff->currentSize -= 1;
    buff->deletedCount += 1;

    return ENTRY_SUCCESS;
}

EntryResult printEntry(EntryBuffer *buff, int id)
{
    CHECK_NULL(buff, ENTRY_FAILURE);
    CHECK_NULL(buff->entries, ENTRY_FAILURE);
    Entry *entryP = NULL;
    EntryResult searchRes = findEntry(buff, id, &entryP);

    switch(searchRes)
    {
        case ENTRY_NOT_FOUND:
            {
                logWarn("entry not found");
                return ENTRY_NOT_FOUND;
            }
        case ENTRY_FAILURE:
            {
                logError("findEntry failed");
                return ENTRY_FAILURE;
            }
    }

    printf("id:%d, name:%s, marks:%d\n",
            entryP->id,
            entryP->name,
            entryP->marks
          );

    return ENTRY_SUCCESS;
}

EntryResult printAllEntries(EntryBuffer *buff)
{
    CHECK_NULL(buff, ENTRY_FAILURE);
    CHECK_NULL(buff->entries, ENTRY_FAILURE);
    if(buff->currentSize == 0){
        printf("No entries present\n");
        return ENTRY_SUCCESS;
    }

    for(size_t i = 0; i < buff->nextInsertPos; ++i){
        if(buff->entries[i].status == ENTRY_PRESENT){
            printf("id:%d, name:%s, marks:%d\n",
                    buff->entries[i].id,
                    buff->entries[i].name,
                    buff->entries[i].marks
                  );
        }
    }

    return ENTRY_SUCCESS;
}

