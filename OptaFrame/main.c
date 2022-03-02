#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "datetime.h"

void dateTypesTest1(){

    char test[] = "01/01/2021";

    char *token = strtok(test, "/");
    while (token != NULL){
        printf("%s\n",token);
        token = strtok(NULL,"/");
    }

    char day[3];
    char month[3];
    char year[5];
    memcpy(day,&test[0],2);     day[2] = '\0';
    memcpy(month,&test[3],2);   month[2] = '\0';
    memcpy(year,&test[6],4);    year[4] = '\0';

    printf("%s\n",test);
    printf("%s\n",&day);
    printf("%s\n",&month);
    printf("%s\n",&year);

    printf("%s\n",test);
    printf("%d\n",atoi(day));
    printf("%d\n",atoi(month));
    printf("%d\n",atoi(year));

    char test2[] = "01/01/2021 08:15";
    char hour[3];
    char min[3];
    memcpy(day,&test2[0],2);     day[2] = '\0';
    memcpy(month,&test2[3],2);   month[2] = '\0';
    memcpy(year,&test2[6],4);    year[4] = '\0';
    memcpy(hour,&test2[11],2);   hour[2] = '\0';
    memcpy(min,&test2[14],2);    min[2] = '\0';

    printf("%s\n",test2);
    printf("%s\n",&day);
    printf("%s\n",&month);
    printf("%s\n",&year);
    printf("%s\n",&hour);
    printf("%s\n",&min);

    printf("%s\n",test2);
    printf("%d\n",atoi(day));
    printf("%d\n",atoi(month));
    printf("%d\n",atoi(year));
    printf("%d\n",atoi(hour));
    printf("%d\n",atoi(min));

    Date day_of_origin = dateInit("06/03/1988");
    String flight_designator = "Y4 000465 ";
    RelTime leg_block_time = reltimeInit("1:30");
    DateTime schedule_departure = dateTimeInit("01/01/2021 8:15");
}

void dateTypesTest2(){

    Date day_of_origin = dateInit("06/03/1988");
    printDate(day_of_origin);

    String flight_designator = "Y4 000465 ";
    printf("%s\n", flight_designator);

    RelTime leg_block_time = reltimeInit("1:30");
    printRelTime(leg_block_time);

    DateTime schedule_departure = dateTimeInit("01/01/2021 8:15");
    printDateTime(schedule_departure);
}

int main()
{
    const char *table_name  = "aircraft_flight_duty_202001.csv";

    //dateTypesTest1();
    dateTypesTest2();

    //tableLoadV2(table_name);

    return 0;
}
