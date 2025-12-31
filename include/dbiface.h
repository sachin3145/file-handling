#ifndef DBIFACE_H
#define DBIFACE_H

#include "entry.h"

typedef enum{
    DB_BACKEND_STDIO,
    DB_BACKEND_POSIX
} DbBackend;

typedef enum{
    DB_SUCCESS,
    DB_FAILURE
} DbResult;


DbResult initDbIface(DbBackend backend);


extern DbResult (*populateBufferFromFile)(EntryBuffer *buff);
extern DbResult (*writeBufferToFile)(EntryBuffer *buff);


#endif // DBIFACE_H
