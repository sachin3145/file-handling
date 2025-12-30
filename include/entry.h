#ifndef ENTRY_H
#define ENTRY_H

#include<stdlib.h>
#include<stdio.h>
#include "logger.h"


#define NAME_SIZE 32
#define DEFAULT_BUFFER_SIZE 64

typedef enum{
    BUFFER_SUCCESS, 
    BUFFER_FAILURE,
} BufferResult;

typedef enum{
    ENTRY_SUCCESS,
    ENTRY_NOT_FOUND,
    ENTRY_FAILURE
} EntryResult;

typedef enum {
    ENTRY_PRESENT, 
    ENTRY_DELETED
} EntryStatus;

typedef struct{
    int id;
    int marks;
    char name[NAME_SIZE];
    EntryStatus status; 
} Entry;


typedef struct{
    Entry *entries;
    size_t capacity;       // total allocated slots
    size_t currentSize;    // number of valid entries (non-deleted)
    size_t deletedCount;   // number of deleted entries present in buffer
    size_t nextInsertPos;  // index to insert next entry
} EntryBuffer;


/* FORWARD DECLARATIONS*/

BufferResult initBuffer(EntryBuffer *buff, int initialCapacity);
BufferResult freeBuffer(EntryBuffer *buff);
BufferResult compactBuffer(EntryBuffer *buff);
BufferResult expandBuffer(EntryBuffer *buff);
BufferResult sortBuffer(EntryBuffer *buff);

EntryResult findEntry(EntryBuffer *buff, int id, Entry **res);

EntryResult addEntry(EntryBuffer *buff, int id, int marks, const char name[NAME_SIZE]);
EntryResult deleteEntry(EntryBuffer *buff, int id);

EntryResult printEntry(EntryBuffer *buff, int id);
EntryResult printAllEntries(EntryBuffer *buff);

#endif // ENTRY_H

