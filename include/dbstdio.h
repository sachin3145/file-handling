#ifndef DBSTDIO_H
#define DBSTDIO_H

#include "dbiface.h"

/* FORWARD DECLARATION */

DbResult stdioPopulateBufferFromFile(EntryBuffer *buff);
DbResult stdioWriteBufferToFile(EntryBuffer *buff);

#endif // DBSTDIO_H
