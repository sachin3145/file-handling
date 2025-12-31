#ifndef DBPOSIX_H
#define DBPOSIX_H

#include "dbiface.h"

/* FORWARD DECLARATION */

DbResult posixPopulateBufferFromFile(EntryBuffer *buff);
DbResult posixWriteBufferToFile(EntryBuffer *buff);

#endif // DBPOSIX_H

