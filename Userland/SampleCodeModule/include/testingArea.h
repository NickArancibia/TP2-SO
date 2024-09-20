#ifndef _TESTING_AREA_H_
#define _TESTING_AREA_H_


#define SELECTED_MODE       0

#define IDLE_MODE           1
#define HELP                2
#define TEST_MM             3
#define EXIT                4

static const char* commands[] = {
    "","","help","testMM","q"
};

static char* helpTextTestingArea[] = { "Tests command information is displayed below\n",
"Disclaimer: each test loops forever so in order to come back you have to reset the terminal\n\n",
"HELP                ->      Shows a description on each available command.\n",
"TestMM              ->      Test memory manager.\n",
"q                   ->      Return to shell.\n",
"end" 
};


void initTestingArea();


#endif