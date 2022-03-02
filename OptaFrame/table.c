#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "table.h"

int tableLoadV2(const char *table_name){

    Table MyTable;
    MyTable.name = table_name;

    //Build table path for opening file
    char table_path[50] = "";
    strcat(table_path, table_dir);
    strcat(table_path, table_name);

    printf("Filename %s\n", table_path);
    FILE *input = fopen(table_path,"r");

    //If there is an error
    if(input == NULL){
        perror("Error opening file"); // print error
        return(-1);
    }

    //Getting content line by line
    char*** table_content=(char***)malloc(MAX_TABLE_NUM_ROWS*sizeof(char**));
    int num_rows=0, num_columns=0, row_size=0, data_sizes[MAX_TABLE_NUM_COLUMNS]={0};
    String *data_types;
    String *headers;


    while (!feof(input)){
        char line[MAX_TABLE_ROW_EXTENSION] = "";
        fgets(line, MAX_TABLE_ROW_EXTENSION, input);
        printf("%s",line);

        //Convert CSV row to array of strings
        char **table_row = NULL;
        char **table_row_converted = NULL;
        const char *start_ptr = NULL, *end_ptr = NULL, *iter = NULL;
        int tokens_found = 0, line_size;
        int start_idx[MAX_TABLE_NUM_COLUMNS]={0},
            end_idx[MAX_TABLE_NUM_COLUMNS]={0};     //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6
        start_ptr = line;                           //Start visiting input. We will distinguish tokens in a single pass, for good performance.
                                                    //Then we are allocating one unified memory region & doing one memory copy.

        if (table_row != NULL)     return -4;  //This SHOULD be NULL: Not Already Allocated
        if (strlen(line)<1)       break;       //Empty line indicates file is over as well

        line_size = splitStringByDelimiter(line, DELIMITER, &tokens_found, start_idx, end_idx);
        num_columns = tokens_found;

        //It reserves a contiguous space for both (char**) pointers AND string region.
        int size_ptr_region = num_columns*sizeof(char*);                        //The size to store pointers to c-strings + 1 (*NULL).
        table_row = (char**)malloc(size_ptr_region + line_size + 1);

        for (int i=0; i<num_columns; i++)                                      //Assign addresses first part of the allocated memory pointers that point to
            table_row[i] = (char*)table_row + size_ptr_region + start_idx[i];  //the second part of the memory, reserved for Data.
        //table_row[num_columns] = (char*) NULL;                                 //[ ptr1, ptr2, ... , ptrN, (char*) NULL, ... ]: We just added the (char*) NULL.

        //Now assign the Data: c-strings. (\0 terminated strings):
        char *str_region = (char*) table_row + size_ptr_region;           //Region inside allocated memory which contains the String Data.
        memcpy(str_region, line, line_size );    //Copy input with delimiter characters: They will be replaced with \0.

        //Now we should replace: "arg1||arg2||arg3" with "arg1\0|arg2\0|arg3". Don't worry for characters after '\0'
        //They are not used in standard c lbraries.
        for(int i=0; i<num_columns; i++) str_region[end_idx[i]] = '\0';

        // *table_row memory should now contain (example data):
        //[ ptr1, ptr2,...,ptrN, "token1\0", "token2\0",...,"tokenN\0"]
        //   |_______________________^           ^              ^
        //          |____________________________|              |
        //                   |__________________________________|

        //Here we append the row contents to data types and headers sections
        //In the case of the data types row, we calculate the exact amount of memory required for each row (based on max size for each row)
        if (num_rows == DATA_TYPE_ROW){
            data_types = table_row;
            //Based on the data types array, we calculate the memory space for each piece of data
            for (int i=0; i<num_columns; i++){
                int data_type_key = (int)data_types[i][0];
                switch (data_type_key){
                    case 73:  row_size += sizeof(int);      data_sizes[i] = sizeof(int);                    break; //Int
                    case 83:  row_size += MAX_STRING_SIZE;  data_sizes[i] = MAX_STRING_SIZE*sizeof(char);   break; //String
                    case 82:  row_size += sizeof(RelTime);  data_sizes[i] = sizeof(RelTime);                break; //RelTime
                    case 68:  row_size += sizeof(DateTime); data_sizes[i] = sizeof(DateTime);               break; //DateTime
                    case 66:  row_size += sizeof(bool);     data_sizes[i] = sizeof(bool);                   break; //Bool
                    case 70:  row_size += sizeof(double);   data_sizes[i] = sizeof(double);                 break; //Float
                }
            }
        }
        else if (num_rows == HEADER_ROW){
            headers = table_row;
        }
        //For each row, data is converted to the corresponding type and stored in a contiguous memory space
        else{
            //It reserves a contiguous space for both void pointers AND data region
            int size_ptr_region = num_columns*sizeof(void*);
            table_row_converted = malloc(size_ptr_region + row_size);
            table_row_converted[0] = (void*)table_row_converted + size_ptr_region;

            //Assign addresses to the allocated memory pointers that point to the second part of the memory, reserved for Data.
            for (int i=1; i<num_columns; i++){
                void* table_row_ptr = table_row_converted[i-1] + data_sizes[i-1];
                table_row_converted[i] = table_row_ptr;
            }
            //table_row_converted[num_columns] = (void*) NULL;

            for (int i=0; i<num_columns; i++){
                int data_type_key = (int)data_types[i][0];
                switch (data_type_key){
                    case 73:   *((int*)(table_row_converted[i]))      = atoi(table_row[i]);         break; //Int
                    case 83:   table_row_converted[i]                 = strdup(table_row[i]);       break; //String
                    case 82:   *((RelTime*)(table_row_converted[i]))  = reltimeInit(table_row[i]);  break; //RelTime
                    case 68:   *((DateTime*)(table_row_converted[i])) = dateTimeInit(table_row[i]); break; //DateTime
                    case 66:   *((bool*)(table_row_converted[i]))     = boolInit(table_row[i]);     break; //Bool
                    case 70:   *((double*)(table_row_converted[i]))   = atof(table_row[i]);         break; //Float
                }
            }
        }
        table_content[num_rows] = table_row_converted; num_rows++;
    }

    MyTable.num_columns = num_columns;
    MyTable.num_rows = num_rows;
    MyTable.data_types = data_types;
    MyTable.headers = headers;
    MyTable.TableRows = table_content;

    for (int i = 0; i<num_rows; i++) {
        printf("Line[%d]:", i);
        for (int j=0; j<num_columns; j++){
            printf(" [%d]=%s", j, table_content[i][j] );
        }
        //printf("\n");
    }

    fclose(input); // close file

    return 0;
}


int tableLoad(const char *table_name){

    //Build table path for opening file
    char table_path[50] = "";
    strcat(table_path, table_dir);
    strcat(table_path, table_name);

    printf("Filename %s\n", table_path);
    FILE *input = fopen(table_path,"r");

    //If there is an error
    if(input == NULL){
        perror("Error opening file"); // print error
        return(-1);
    }

    //Getting content line by line
    char*** table_content=(char***)malloc(MAX_TABLE_NUM_ROWS*sizeof(char**));
    int num_rows=0, num_columns=0;
    while (!feof(input)){
        char line[MAX_TABLE_ROW_EXTENSION] = "";
        fgets(line, MAX_TABLE_ROW_EXTENSION, input);
        printf("%s",line);

        //Convert CSV row to array of strings
        char **table_row = NULL;
        const char *start_ptr = NULL, *end_ptr = NULL, *iter = NULL;
        int tokens_found = 0, line_size;
        int start_idx[100]={0}, end_idx[100]={0};       //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6
        start_ptr = line;                       //Start visiting input. We will distinguish tokens in a single pass, for good performance.
                                                //Then we are allocating one unified memory region & doing one memory copy.

        if (table_row != NULL)     return -4;  //This SHOULD be NULL: Not Already Allocated
        if (strlen(line)<1)       break;      //Empty line indicates file is over as well

        line_size = splitStringByDelimiter(line,DELIMITER,&tokens_found,start_idx,end_idx);
        num_columns = tokens_found;

        //It reserves a contiguous space for both (char**) pointers AND string region. 5 Bytes for "Out of Range" tests.
        int size_ptr_region = (1 + tokens_found)*sizeof(char*);   //The size to store pointers to c-strings + 1 (*NULL).
        table_row = (char**)malloc(size_ptr_region + (line_size + 1) + 5);

        //"Out of Range" TEST. Verify that the extra reserved characters will not be changed. Assign Some Values.
        //char *extra_chars = (char*) table_row + size_ptr_region + ( line_size + 1 );
        //extra_chars[0] = 1; extra_chars[1] = 2; extra_chars[2] = 3; extra_chars[3] = 4; extra_chars[4] = 5;

        for (int i=0; i<tokens_found; i++)                                      //Assign adresses first part of the allocated memory pointers that point to
            table_row[i] = (char*)table_row + size_ptr_region + start_idx[i];   //the second part of the memory, reserved for Data.
        table_row[tokens_found] = (char*) NULL;                                 //[ ptr1, ptr2, ... , ptrN, (char*) NULL, ... ]: We just added the (char*) NULL.

        //Now assign the Data: c-strings. (\0 terminated strings):
        char *str_region = (char*) table_row + size_ptr_region;           //Region inside allocated memory which contains the String Data.
        memcpy(str_region, line, line_size );    //Copy input with delimiter characters: They will be replaced with \0.

        //Now we should replace: "arg1||arg2||arg3" with "arg1\0|arg2\0|arg3". Don't worry for characters after '\0'
        //They are not used in standard c lbraries.
        for(int i=0; i<tokens_found; i++) str_region[end_idx[i]] = '\0';


        //"Out of Range" TEST. Wait until Assigned Values are Printed back.
        //for ( int i=0; i < 5; i++ ) printf("c=%x ", extra_chars[i] ); printf("\n");

        // *table_row memory should now contain (example data):
        //[ ptr1, ptr2,...,ptrN, (char*) NULL, "token1\0", "token2\0",...,"tokenN\0", 5 bytes for tests ]
        //   |__________________________________^           ^              ^             ^
        //          |_______________________________________|              |             |
        //                   |_____________________________________________|      These 5 Bytes should be intact.

        //Here we append the row contents to the whole table
        table_content[num_rows] = table_row; num_rows++;
    }

    for (int i = 0; i<num_rows; i++) {
        printf("Line[%d]:", i);
        for (int j=0; j<num_columns; j++){
            printf(" [%d]=%s", j, table_content[i][j] );
        }
        //printf("\n");
    }

    fclose(input); // close file

    return 0;
}


