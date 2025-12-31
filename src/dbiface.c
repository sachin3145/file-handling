#include "dbiface.h"

#include "logger.h"
#include "dbstdio.h"
#include "dbposix.h"


DbResult (*populateBufferFromFile)(EntryBuffer *buff) = NULL;
DbResult (*writeBufferToFile)(EntryBuffer *buff) = NULL;

DbResult initDbIface(DbBackend backend){
    switch(backend){
        case DB_BACKEND_STDIO:
        {
            populateBufferFromFile = stdioPopulateBufferFromFile;
            writeBufferToFile = stdioWriteBufferToFile;
            break;
        }
        case DB_BACKEND_POSIX:
        {
            populateBufferFromFile = posixPopulateBufferFromFile;
            writeBufferToFile = posixWriteBufferToFile;
            break;
        }
        default:
            {
                logError("invalid backend type provided");
                return DB_FAILURE;
            }
    }

    return DB_SUCCESS;
}
