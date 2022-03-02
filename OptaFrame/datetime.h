#ifndef DATETIME_H
#define DATETIME_H

#define DATE_DELIM "/"
#define DATETIME_DELIM " "
#define RELTIME_DELIM ":"

#define RELTIME_SIZE sizeof(RelTime)
#define DATETIME_SIZE sizeof(DateTime)
#define DATE_SIZE sizeof(Date)

#include <time.h>
#include <stdbool.h>
#include "utilities.h"

//Datetime and Reltime definitions
typedef struct{
    String value;       //value should be given in HH:MM format
    struct tm;          //time structure containing all time components
    int hrs;            //it should be equal to tm.tm_hours
    int min;            //it should be equal to tm.tm_min
}RelTime;

typedef struct{
    String value;       //value should be given in DDMMMYYYY HH:MM format
    struct tm;          //time structure containing all time components
    String weekday;
    int day;
    String month;
    int year;
    int hrs;
    int min;
}DateTime;

//This data type will be removed
typedef struct{
    String value;
    struct tm;
    int day;
    int month;
    int year;
}Date;

bool boolInit(String);
Date dateInit(String);
RelTime reltimeInit(String);
DateTime dateTimeInit(String);

void printDate(Date);
void printDateTime(DateTime);
void printRelTime(RelTime);


#endif // DATETIME_H
