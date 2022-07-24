#include "local.h"
#include "fungame_local.h"

/*
*  Prepare the environment required to start the fun game
*/

void main(int argc, char *argv[])
{
	if (argc != 2) 
	{
      fprintf(stderr, "Usage: %s filename\n", argv[0]);
      exit(1);
    }
    strcpy(config_file,argv[1]);
	int round_num=1;
	char input_char;
	
	// -----------------------------------------------------------------
	//print tank 
	printf("%s",ORIGINAL);
	printf("%s",GREEN_COLOR);
	printf("\n \t \t Welcome to Telematch Game \n");
	printf("%s",ORIGINAL);
	for(int i=0;i<5;i++)
	{
		if(i==4){
			printf( "|");   
			printf("_______|");
			printf("\t\t\t\t\t|");
			printf("_______|\n");
			printf(" Tank A \t\t\t\t\t Tank B\n");
		}
		else{
		printf("|");
		printf("%s", BLUE_COLOR);
		printf("~~~~~~");
		printf("%s",ORIGINAL);
		printf("\t|");
		printf("\t\t\t\t\t|");
		printf("------");
		printf("\t|\n");
		  }  
    }
        
	for(int j=0;j<5;j++)
	{
		if(j==4)
		{
			printf( "|");   
			printf("_______|");
			printf("\t\t\t\t\t|");
			printf("_______|\n");
			printf(" Tank C \t\t\t\t\t Tank D\n");
		}
		else
		{
		printf("|");
		printf("%s", BLUE_COLOR);
		printf("~~~~~~");
		printf("%s",ORIGINAL);
		printf("\t|");
		printf("\t\t\t\t\t|");
		printf("------");
		printf("\t|\n");
		}   
    }
	// -----------------------------------------------------------------
    printf( "\n Click y to start The Fun Game Simulation :\n");
    scanf("%c", &input_char);
    if(input_char == 'y' || input_char == 'Y'){
			do{			
			fflush(stdout);
			printf("\n <<<<<<<<<<<<<<<<<<<<<<<<<< | Round Number %d | >>>>>>>>>>>>>>>>>>>>>>>>>>\n",round_num);
			
			/* Redefine signals */
			signal(SIGUSR1, sig_team1_win);
			signal(SIGUSR2, sig_team2_win);
			signal(SIGQUIT,sig_tie);
				  
			unsigned int tank_shmid1, locations_shmid,tank_shmid2;
			
			static struct tank *tank, *tank_team_two;
			
			static int *loc_shm;
			sem_t   *p;
			
			unsigned int x_1, y_1, x_2,y_2, distance, location_x_y[2*NUM_OF_PLAYERS], num_of_obstacles , pid_parent;   
			float obstacles_percentage = 0.25, bag_ratio, bag_size; 
			
			//_____________Define previous variables as string to send them through execl method _____________
			char loc_x[5], loc_y[5],num_obs[10], pid_parentt[8], bag_size_char[6], tank_distance[3], loc_y_two[5],
				   i_char[5],attackers_team_two[10],attackers_team_one[10],*team_num;
			
			pid_parent=getpid();
			srand(time(NULL)); 
			
			//_____________Call Config paser to Read variable from file _____________
			config_parser();
			
			if(size_tank == 0)
			{
				   perror("Invalid size tank");
				   kill(getpid(),SIGINT);
			}
			
			// _____________Assign random numbers for tank locations_____________
			x_1 = (rand() % (RANGE_MAX - RANGE_MIN + 1)) + RANGE_MIN;
			y_1 = (rand() % (RANGE_MAX - RANGE_MIN + 1)) + RANGE_MIN;
			distance = (rand() % (DISTANCE_MAX - DISTANCE_MIN + 1)) + DISTANCE_MIN;
			x_2 = x_1 + distance;
			y_2 = y_1 + (distance/2);
			random_numbers(NUM_OF_PLAYERS, location_x_y,x_1, x_2, y_1);
			sprintf(tank_distance, "%d", distance);
			sprintf(loc_y_two, "%d", y_2);
				
			//_____________calculate number of obstacles_____________ 
			num_of_obstacles = (int)(distance * obstacles_percentage);
			
			unsigned int obstacles_lacations_y_1[num_of_obstacles];
			random_obstacales(num_of_obstacles, obstacles_lacations_y_1, x_1,x_2,y_1);
			sprintf(loc_x, "%d", x_1);
			sprintf(loc_y, "%d", y_1);
			sprintf(num_obs ,"%d", num_of_obstacles);
			printf("\n X_axis Tank A = %d \t  Y_axis Tank A = %d \n", x_1, y_1);
			printf("\n X_axis Tank C = %d \t  Y_axis Tank C = %d \n", x_1, y_2);
			printf("\n");
			
			//_____________create an array for players locations and obstacles locations_____________
			int number_locations = ((2* NUM_OF_PLAYERS) + (2* num_of_obstacles));
			int locations[number_locations];
			locations[0]=x_1;
			locations[1]=y_1;
			locations[2]=x_1;
			locations[3]=y_2;
		  for(int u=4;u<number_locations;u++)
			{
				if(u < ((2*NUM_OF_PLAYERS))){
			    	locations[u] = location_x_y[u];
				}else{
				locations[u] = obstacles_lacations_y_1[u-((2*NUM_OF_PLAYERS))];
				}
			}
				
			//_____________ semaphore for player's locations _____________
			
			if ((p = sem_open("player_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
				{
				  perror("semaphore initilization");
				  exit(1);
				}
				sem_init(p,1,1);
				

			
			//_____________ create tank 'A' _____________
			
			tank_shmid1 = shmget(KEY, sizeof(struct tank), 0644 | IPC_CREAT);
				/* Initialize tank data value */
			tank = shmat(tank_shmid1, NULL, 0);
			if (tank == (void *)-1)
			{
				fprintf(stderr, "Shared memory attach error !\n");
			}
			tank->size = size_tank;
			if (shmdt(tank) == -1)
			{
				 fprintf(stderr, "shmdt\n");
			}
			
			//_____________create tank 'C'_____________
			
			tank_shmid2 = shmget(KEY_TANK, sizeof(struct tank), 0644 | IPC_CREAT);
			/* Initialize tank data value */
			tank_team_two = shmat(tank_shmid2, NULL, 0);
			if (tank_team_two == (void *)-1)
			{
				fprintf(stderr, "Shared memory attach error !\n");
			}
			tank_team_two->size = size_tank;
			if (shmdt(tank_team_two) == -1)
			{
				 fprintf(stderr, "shmdt\n");
			}
			
			// _____________create shared memory for players and obstacles locations_____________
			sem_wait(p);
			locations_shmid = shmget(KEY_LOCATION, sizeof(location_x_y), 0644 | IPC_CREAT);
			loc_shm = shmat(locations_shmid, NULL, 0);
			if (loc_shm == (void *)-1)
			{ 
				fprintf(stderr, "Shared 2memory attach error !\n");
			}
			for(int u=0; u < number_locations ;u++)
			{
				loc_shm[u] = locations[u];
			}
			
			if (shmdt(loc_shm) == -1)
			{
				 fprintf(stderr, "shmdt\n");
			}
			sem_post(p);
			
			/* _____________ Make the bag ratio to Tank size _____________*/
			bag_ratio=(((double)rand())/(((double)RAND_MAX))*(MAX_BAG_RATIO-MIN_BAG_RATIO)+MIN_BAG_RATIO);
			bag_size = size_tank * bag_ratio;
			gcvt(bag_size, 6, bag_size_char);
			
			/* print the game rules */
			printf("%s",ORIGINAL);
			printf("%s",CYAN_COLOR);
			printf("\n*************************** |  GAME RULES  | ***************************\n");
			printf("\n\t\t <> Number Of Obstacles = %d\n",num_of_obstacles);
			printf("\n\t\t <> Number Of Players For Each Team = %d\n",(NUM_OF_PLAYERS/2));
			printf("\n\t\t <> Distance Between The 2 Tanks Will Range From  [ %d - %d ] Meters\n",DISTANCE_MIN, DISTANCE_MAX);
			printf("\n\t\t <> Tank Size = %f\n",size_tank);
			printf("\n\t\t <> Bag Size For Each Collector = %f\n",bag_size);
			printf("\n*************************************************************************\n");
			printf("%s",ORIGINAL);

			//__________________________ << START FORKING >> __________________________ 

			for (int i = 0; i < NUM_COLLECTERS ; i++)
			{
				// P11 , P21 << Collecters >>
				pid_collecter[i] = fork();
				if(pid_collecter[i] == 0)
				 {
				  if(i==0)
				  {
				  	team_num="1";
						if (execl("./collectors", "collectors", loc_x, loc_y, team_num, bag_size_char, step_range, player_speed, tank_distance,num_obs, NULL) == -1)
						{
						 	perror("EXECL: collectors");
						 	exit(1);
						}
				  }
				  else
				  {
				  	team_num="2";
						if (execl("./collectors", "collectors", loc_x, loc_y_two, team_num, bag_size_char, step_range, player_speed, tank_distance,num_obs, NULL) == -1)
						{
						 	perror("EXECL: collectors");
						 	exit(1);
						}
				  }
				  
				 }    
		 }
			
			for (int i = 0; i < NUM_ATTACKERS ; i++)
			{
				// P12, P13 , P22, P23 << Attackers >>
				pid_attacker[i] = fork();
				if(pid_attacker[i]== 0)
				{
				  if(i<2)
				  {
				    team_num="1";
				    sprintf(i_char, "%d", i);
				    if (execl("./attackers", "attackers", loc_x, loc_y_two, bag_size_char, step_range, player_speed, team_num, i_char,NULL) == -1)
				    {
				     	perror("EXECL: attackers");
				     	exit(2);
				    }  
				  }
				  else
				  {
				    team_num="2";
				    sprintf(i_char, "%d", i);
				    if (execl("./attackers", "attackers", loc_x, loc_y, bag_size_char, step_range, player_speed, team_num, i_char, NULL) == -1)
				    {
				     	perror("EXECL: attackers");
				     	exit(2);
				    } 
				  }
				 }  
			}
				  
			for (int i = 0; i < NUM_DEFENDERS  ; i++)
			{
				// P14, P24 << Defenders >>
				pid_defender[i] = fork();
				if(pid_defender[i]== 0)
				{ 
					if(i==0)
					{
						team_num="1";
						snprintf(attackers_team_two, sizeof attackers_team_two, "%d;%d",pid_attacker[2],pid_attacker[3]);
						if (execl("./defenders", "defenders",loc_x,loc_y, step_range, player_speed,team_num,attackers_team_two, NULL) == -1)
						{
							 perror("EXECL: defenders");
							 exit(3);
						}
					}
					else 
					{
						team_num="2";
						snprintf(attackers_team_one, sizeof attackers_team_one, "%d;%d",pid_attacker[0],pid_attacker[1]);
						if (execl("./defenders", "defenders",loc_x,loc_y_two, step_range, player_speed,team_num,attackers_team_one,NULL) == -1)
						{
							 perror("EXECL: defenders");
							 exit(3);
						}
					}
				}    
			}
		 
			 // << Referee >> 
			 pid_referee = fork();
			 sprintf(pid_parentt, "%d", pid_parent);
			 if(pid_referee == 0)
			 {
				  if (execl("./referee", "referee", pid_parentt, time_game, NULL) == -1)
				  {
				      perror("EXECL: referee");
				      exit(4);
				  }
			 } 
				/* To ensure that we have forked all needed processes we will alert a SIGCONT to start the game */   
				sleep(2);
				for(int j = 0; j < NUM_COLLECTERS ; j++)
				{
				    kill(pid_collecter[j],SIGCONT);
				}
				for(int j = 0; j < NUM_ATTACKERS ; j++)
				{
				    kill(pid_attacker[j],SIGCONT);
				}
				for (int j = 0; j < NUM_DEFENDERS  ; j++)
				{
				    kill(pid_defender[j],SIGCONT);
				}
				kill(pid_referee,SIGCONT);
			
			/* Wait for all the children to be killed*/	
			for(int i = 0 ; i< 9 ; i++)
			{
				wait(NULL);
			}
						
			round_num++;
			/* Declare the winner of the round*/
			printf("%s", ORIGINAL); 
			printf("%s",PURPLE_COLOR);
			printf("\n================================================\n"); 
			printf("| Team1 Count Wins = %d \t Team2 Count Wins = %d |\n",count_wins_team1,count_wins_team2);
			printf("================================================\n"); 
			printf("%s", ORIGINAL); 
			
			
			if(count_wins_team2 > count_wins_team1)
			{
			    printf("%s", ORIGINAL); 
				printf("%s",GREEN_COLOR);
				printf("\n ~~ THE WINNER OF THIS ROUND \t TEAM 2 ~~\n");
				printf("%s",ORIGINAL);
			}
			else if(count_wins_team1 > count_wins_team2)
			{
				printf("%s", ORIGINAL);
				printf("%s", RED_COLOR);
				printf("\n=============================================\n"); 
				printf("| ~~ THE WINNER OF THIS ROUND \t TEAM 1 ~~|\n");
				printf("=============================================\n"); 
				printf("%s", ORIGINAL); 
			}
			else{
				printf("%s", ORIGINAL); 
				printf("%s", BLUE_COLOR);
				printf("\n=============================================\n"); 
				printf("| ~~ TIE IN THIS ROUND ~~ |\n");
				printf("=============================================\n"); 
				printf("%s", ORIGINAL); 
			}
			/* Announce the Winner of the game */
			if(count_wins_team1 == 3 || count_wins_team2 == 3)
			{
				finish_game = 1;
				if(count_wins_team1 == 3)
					printf("\n THE WINNER OF THE GAME IS TEAM 1 \n");
				else 
					printf("\n THE WINNER OF THE GAME IS TEAM 2 \n");
					
				// unlink semaphore and clean shared memories
				sem_unlink("player_semaphore");
				shmctl(tank_shmid1, IPC_RMID, NULL);
				shmctl(tank_shmid2, IPC_RMID, NULL);
				shmctl(locations_shmid, IPC_RMID, NULL);
			}
			} while(finish_game != 1);
			}
			else
			{
				printf("%s", RED_COLOR);
				printf("TRY AGAIN \n");
				printf("%s", ORIGINAL);
			}
}

/*
  Function name : random_numbers
  Args : NUM_OF_PLAYERS, location_x_y , x_1 , x_2, y_1
  Returns : None
  Description : Function generate random array of ( x and y ) locations for the players
*/
void random_numbers(int number, int array[], int x_1, int x_2, int y_1){
  int current, check, r, get;
  current = 0;
  while(current < (number*2))
  {
    if(current < number)
    {
       get = (rand() % (x_2 - x_1 + 1)) + x_1; // generate random number.
    }
    else
    {
       get = (rand() % (y_1) + (y_1/2));
    }  
    for(r=0; r <= current; r++) //After generating that number check if it is already in array.
    {
      if(array[r] == get)
       {
           check= 0;
           break;
       }
       else if(array[r] != get)
        {
           check= 1;
        }
      }
      if(check== 1)
      {
          array[current] = get;
          current++;
      }
   }
}

/*
  Function name : random_obstacales
  Args : num_of_obstacles, obstacles_lacations_y_1 , x_1 ,x_2 ,y_1
  Returns : None
  Description : Function generate array of random y locations for the obstacales
*/
void random_obstacales(int number, int array[], int x_1, int x_2, int y_1){
  int current, check, r, get;
  current = 0;
  while(current < (number*2))
  {
    if(current < number)
    {
       get = (rand() % (x_2 - x_1 + 1)) + x_1; // generate random number.
    }
    else
    {
     get =(int)(((double)rand())/(((double)RAND_MAX))*((y_1+(atof(step_range)*8))-(y_1-(atof(step_range)*8)))+(y_1-(atof(step_range)*8)));
    }  
    for(r=0; r <= current; r++) //After generating that number check if it is already in array.
    {
      if(array[r] == get)
       {
           check= 0;
           break;
       }
       else if(array[r] != get)
        {
           check= 1;
        }
      }
      if(check== 1)
      {
          array[current] = get;
          current++;
      }
   }
}



/*
  Function name : config_parser
  Args : None
  Returns : None
  Description : Function read the config file and update the corresponding parameters.
*/
void config_parser(void)
{
  FILE *fp;
  char chunk[30];
  int i;
  i = 0;
  memset(chunk, '\0', 30);
  fp = fopen(config_file, "r");

  if (fp == NULL)
  {
	fprintf(stderr, "Unable to open file\n");
	exit(5);
  }

  while (fgets(chunk, sizeof(chunk), fp) != NULL)
  {
    if (i == 0)
      strcpy(time_game , (&chunk[strlen("GAME_TIME=")]));
    else if (i == 1)
      size_tank = atof(&chunk[strlen("SIZE_TANK=")]);
    else if (i == 2)
      strcpy(player_speed , (&chunk[strlen("INITIAL_SPEED_COLLECTOR=")]));
    else if (i == 3)
      strcpy(step_range, (&chunk[strlen("STEP_RANGE=")]));
      
    memset(chunk, '\0', 30);
    i++;
   }
   fclose(fp);
}

void sig_team1_win(int signum)
{
		 count_wins_team1++;
	   count_wins_team2 = 0;
		for(int j = 0; j < NUM_COLLECTERS ; j++)
		{
		    kill(pid_collecter[j],SIGINT);
		}
		for(int j = 0; j < NUM_ATTACKERS ; j++)
		{
		    kill(pid_attacker[j],SIGINT);
		}
		for (int j = 0; j < NUM_DEFENDERS  ; j++)
		{
		    kill(pid_defender[j],SIGINT);
		}
		kill(pid_referee,SIGINT);
}

void sig_team2_win(int signum)
{
		count_wins_team2++;
	  count_wins_team1 = 0;
	    for(int j = 0; j < NUM_COLLECTERS ; j++)
		{
		    kill(pid_collecter[j],SIGINT);
		}
		for(int j = 0; j < NUM_ATTACKERS ; j++)
		{
		    kill(pid_attacker[j],SIGINT);
		}
		for (int j = 0; j < NUM_DEFENDERS  ; j++)
		{
		    kill(pid_defender[j],SIGINT);
		}
		kill(pid_referee,SIGINT);    
}

void sig_tie(int signum)
{
  		for(int j = 0; j < NUM_COLLECTERS ; j++)
		{
		    kill(pid_collecter[j],SIGINT);
		}
		for(int j = 0; j < NUM_ATTACKERS ; j++)
		{
		    kill(pid_attacker[j],SIGINT);
		}
		for (int j = 0; j < NUM_DEFENDERS  ; j++)
		{
		    kill(pid_defender[j],SIGINT);
		}
		kill(pid_referee,SIGINT);
}


