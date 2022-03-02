#ifndef TABLE_H
#define TABLE_H

#define DELIMITER ","
#define MAX_TABLE_ROW_EXTENSION 1000
#define MAX_TABLE_NUM_ROWS 20000
#define MAX_STRING_SIZE 100
#define MAX_TABLE_NUM_COLUMNS 100
#define HEADER_ROW 1
#define DATA_TYPE_ROW 0
#include "utilities.h"
#include "datetime.h"

//TODO: Create settings structure for this
const char *table_dir = "tables/";

typedef struct{
    //This is the data structure for the table, all the memory
    //should be allocated in a continuous memory block for the data,
    //a continuous memory block for void addresses pointing to the data
    //and a continuous array of void pointer to point to those addresses
    String *name;
    int num_columns;
    int num_rows;
    String *data_types;
    String *headers;
    void *TableRows;
}Table;

int tableLoadV2(const char *);
int tableLoad(const char *);

#endif // TABLE_H
