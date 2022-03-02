#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "datetime.h"

bool boolInit(String boolean){
    if (boolean = "True"){
        return true;
    }
    else{
        return false;
    }
}

Date dateInit(String date){

    char *start_ptr = date;
    char *end_ptr = NULL;
    char *iter = NULL;
    //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6
    int tokens_found=0, start_idx[10]={0}, end_idx[10]={0};

    splitStringByDelimiter(date,DATE_DELIM,&tokens_found,start_idx,end_idx);

    //printf("%.*s\n",end_idx[0] - start_idx[0], &date[start_idx[0]]);
    //printf("%.*s\n",end_idx[1] - start_idx[1], &date[start_idx[1]]);
    //printf("%.*s\n",end_idx[2] - start_idx[2], &date[start_idx[2]]);

    char day[3]="";
    char month[3]="";
    char year[3]="";

    memcpy(day, &date[start_idx[0]], end_idx[0] - start_idx[0]);
    memcpy(month, &date[start_idx[1]], end_idx[1] - start_idx[1]);
    memcpy(year, &date[start_idx[2]], end_idx[2] - start_idx[2]);

    //printf("%s\n",day);
    //printf("%s\n",month);
    //printf("%s\n",year);

    Date mydate;
    mydate.day = atoi(day);
    mydate.month = atoi(month);
    mydate.year = atoi(year);
    mydate.value = date;

    return mydate;
}

RelTime reltimeInit(String time){

    char *start_ptr = time;
    char *end_ptr = NULL, *iter = NULL;
    int tokens_found=0, start_idx[10]={0}, end_idx[10]={0};       //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6

    splitStringByDelimiter(time,RELTIME_DELIM,&tokens_found,start_idx,end_idx);

    //printf("%.*s\n",end_idx[0] - start_idx[0], &time[start_idx[0]]);
    //printf("%.*s\n",end_idx[1] - start_idx[1], &time[start_idx[1]]);

    char hours[3]="";
    char mins[3]="";
    memcpy(hours, &time[start_idx[0]], end_idx[0] - start_idx[0]);
    memcpy(mins, &time[start_idx[1]], end_idx[1] - start_idx[1]);

    //printf("%s\n",hours);
    //printf("%s\n",mins);

    RelTime myRelTime;
    myRelTime.hrs = atoi(hours);
    myRelTime.min = atoi(mins);
    myRelTime.value = time;

    return myRelTime;
}

DateTime dateTimeInit(String datetime){

    char *start_ptr = datetime;
    char *end_ptr = NULL, *iter = NULL;
    int tokens_found=0, start_idx[10]={0}, end_idx[10]={0};       //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6

    splitStringByDelimiter(datetime,DATETIME_DELIM,&tokens_found,start_idx,end_idx);

    //printf("%.*s\n",end_idx[0] - start_idx[0], &datetime[start_idx[0]]);
    //printf("%.*s\n",end_idx[1] - start_idx[1], &datetime[start_idx[1]]);

    char date[10]="";
    char time[10]="";
    strncpy(date, datetime+start_idx[0], end_idx[0] - start_idx[0]);
    strncpy(time, datetime+start_idx[1], end_idx[1] - start_idx[1]);

    date[end_idx[0] - start_idx[0]] = '\0';
    time[end_idx[1] - start_idx[1]] = '\0';

    //printf("%s\n", date);
    //printf("%s\n", time);

    /*******************************************************************************************************************************/

    start_ptr = date;
    end_ptr = NULL, iter = NULL;
    tokens_found=0;
    int start_idx2[10]={0}, end_idx2[10]={0};       //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6

    splitStringByDelimiter(date,DATE_DELIM,&tokens_found,start_idx2,end_idx2);

    //printf("%.*s\n",end_idx2[0] - start_idx2[0], &date[start_idx2[0]]);
    //printf("%.*s\n",end_idx2[1] - start_idx2[1], &date[start_idx2[1]]);
    //printf("%.*s\n",end_idx2[2] - start_idx2[2], &date[start_idx2[2]]);

    char day[3]="";
    char month[3]="";
    char year[3]="";

    memcpy(day, &date[start_idx[0]], end_idx[0] - start_idx[0]);
    memcpy(month, &date[start_idx[1]], end_idx[1] - start_idx[1]);
    memcpy(year, &date[start_idx[2]], end_idx[2] - start_idx[2]);

    //printf("%s\n",day);
    //printf("%s\n",month);
    //printf("%s\n",year);

    /*******************************************************************************************************************************/

    start_ptr = time;
    end_ptr = NULL, iter = NULL;
    tokens_found=0;
    int start_idx3[10]={0}, end_idx3[10]={0};       //Store the indexes of tokens. Example "Power;": loc('P')=1, loc(';')=6

    splitStringByDelimiter(time,RELTIME_DELIM,&tokens_found,start_idx3,end_idx3);

    //printf("%.*s\n",end_idx3[0] - start_idx3[0], &time[start_idx3[0]]);
    //printf("%.*s\n",end_idx3[1] - start_idx3[1], &time[start_idx3[1]]);

    char hours[3]="";
    char mins[3]="";
    memcpy(hours, &time[start_idx[0]], end_idx[0] - start_idx[0]);
    memcpy(mins, &time[start_idx[1]], end_idx[1] - start_idx[1]);

    //printf("%s\n",hours);
    //printf("%s\n",mins);

    DateTime myDateTime;
    return myDateTime;
}

void printDate(Date myDate){
    printf("%s\n", myDate.value);
}

void printDateTime(DateTime myDateTime){
    printf("%s\n", myDateTime.value);
}

void printRelTime(RelTime myRelTime){
    printf("%s\n", myRelTime.value);
}

