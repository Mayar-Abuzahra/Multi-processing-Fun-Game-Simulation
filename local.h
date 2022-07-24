#ifndef __LOCAL_H_
#define __LOCAL_H_

/*
 * Common header file
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <wait.h>
#include <sys/mman.h>
#include <math.h>

#define NUM_COLLECTERS 2
#define NUM_ATTACKERS 4
#define NUM_DEFENDERS 2
#define SHM_SIZE  5

#define MAX 4
#define PROCESS_NUM 8
#define RANGE_MAX 50 
#define RANGE_MIN 15 
#define DISTANCE_MIN 20
#define DISTANCE_MAX 40 
#define NUM_OF_PLAYERS 8
#define MAX_BAG_RATIO 0.1
#define MIN_BAG_RATIO 0.05

// Keys to create shared memories
#define KEY 0x4000
#define KEY_TANK 0x5000
#define KEY_LOCATION 0x8000
#define KEY_BAG 0x6000

struct tank{
 float size;
};


/*Colors*/
#define ORIGINAL "\x1B[0m"
#define RED_COLOR "\x1B[31m"
#define GREEN_COLOR "\x1B[32m"
#define YELLOW_COLOR "\x1B[33m"
#define BLUE_COLOR "\x1B[34m"
#define PURPLE_COLOR "\x1B[35m"
#define CYAN_COLOR "\x1B[36m"
#define WHITE_COLOR "\x1B[37m"

#endif

