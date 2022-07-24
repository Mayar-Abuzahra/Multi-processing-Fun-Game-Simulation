#include "local.h"
#include "defenders_local.h"

void main(int argc, char *argv[])
{	
	float step_range;
	int defender_speed;	
	char attackers_pid[10],attacker_one_pid_char[5],attacker_two_pid_char[5];
	
	tank_x_loc=atoi(argv[1]);
    tank_y_loc=atoi(argv[2]);
    step_range=atof(argv[3]);
    defender_speed=atof(argv[4]);
    team_type=atoi(argv[5]);
    strcpy(attackers_pid,argv[6]);
    
    memcpy(attacker_one_pid_char,&attackers_pid[0],4);
    memcpy(attacker_two_pid_char,&attackers_pid[5],4);
    pid_attacker_one=atoi(attacker_one_pid_char);
    pid_attacker_two=atoi(attacker_two_pid_char);
	
    /*Inialize defender player */
    defender_player.speed=defender_speed;
    defender_player.sand_cup_size=1.0;
    defender_player.effiency_percentage=1.0;

   /* pause defender work till the parent send them signal to continue */    
    kill(getpid(), SIGSTOP);
    
    printf("%s", ORIGINAL); 
    printf("\u263A :");
    printf("%s", RED_COLOR);
    printf("Defender Id -> %d \n",getpid());
    printf("%s", ORIGINAL); 
    
    /*Get the location of defender */
    /* semaphore for player's locations*/
    if ((p = sem_open("player_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
    sem_init(p,1,1);
    
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
		{
			 	if (loc_shm = shmat(locations_shmid2, NULL, 0));
			 	
				if (loc_shm == (void *)-1)
				{
					fprintf(stderr, "Shared memory attach error !\n");
				}
				if(team_type==1)
				{
					defender_location[0] =loc_shm[12];
					defender_location[1] =loc_shm[13];
				}
				else
				{ 
					defender_location[0] =loc_shm[14];
					defender_location[1] =loc_shm[15];
				}
		}
    if (shmdt(loc_shm) == -1)
    {
    	fprintf(stderr, "shmdt\n");
    }
    sem_post(p);
    
    /*Compute step rang of defenders*/
    target_steps=(ceil(distance(tank_x_loc,defender_location[0],tank_y_loc,defender_location[1]) / step_range))-4;   
    ideal_time=ceil(step_range / defender_speed);
    signal(14, sig_defender_task); 
    alarm(ideal_time);


    while(1);                       
}

/*
  Function name : sig_defender_task
  Args : int signum
  Returns : none
  Function Description : 
*/
void sig_defender_task(int signum){
	int distance_defender_attack1,distance_defender_attack2;
	float threshold;
// if team_type 1 check the distance between defender and attackers from second team 
// if distance < threashold kill(pid,siguser1)
// siguser in attacker just inc backbag size 

  counter_steps+=ideal_time;

  if(counter_steps==target_steps)
  {
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
    	if (loc_shm = shmat(locations_shmid2, NULL, 0));
   	
			if (loc_shm == (void *)-1)
			{
				fprintf(stderr, "Shared memory attach error !\n");
			}
	 		if(team_type==1)
	 		{		
			  loc_shm[12]=tank_x_loc;
			  loc_shm[13]=tank_y_loc;
			}
	    else
	    {
			  loc_shm[14]=tank_x_loc;
			  loc_shm[15]=tank_y_loc;	
	    }
    }
    if (shmdt(loc_shm) == -1)
    {
       fprintf(stderr, "shmdt\n");
    }
    sem_post(p);
    counter_steps=0;
    sleep(3);
  }

  if(team_type==1)
  {
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
		 	if (loc_shm = shmat(locations_shmid2, NULL, 0));
		 	
			if (loc_shm == (void *)-1)
			{
				fprintf(stderr, "Shared memory attach error !\n");
			}
			loc_shm[12]+=ideal_time;	
			distance_defender_attack1=distance(loc_shm[4], loc_shm[12], loc_shm[5], loc_shm[13]);
			distance_defender_attack2=distance(loc_shm[6], loc_shm[12], loc_shm[7], loc_shm[13]);
    }
    if (shmdt(loc_shm) == -1)
		{
			fprintf(stderr, "shmdt\n");
		}
    sem_post(p);
  }
  else
  {
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
		 	if (loc_shm = shmat(locations_shmid2, NULL, 0));
		 	
			if (loc_shm == (void *)-1)
			{
			fprintf(stderr, "Shared memory attach error !\n");
			}
			loc_shm[14]+=ideal_time;
			distance_defender_attack1=distance(loc_shm[8], loc_shm[14], loc_shm[9], loc_shm[15]);
			distance_defender_attack2=distance(loc_shm[10], loc_shm[14], loc_shm[11], loc_shm[15]);
    }
    if (shmdt(loc_shm) == -1)
		{
			fprintf(stderr, "shmdt\n");
		}
    sem_post(p);
  }
  threshold = (((distance_defender_attack1+distance_defender_attack2)/2.0)-(ceil(abs(distance_defender_attack1-distance_defender_attack2)/4.0)));
	 // Attacker 1 nearest to the defender
	 if(distance_defender_attack1 <  distance_defender_attack2)
	 {
	 	if((defender_player.sand_cup_size ==1) && (distance_defender_attack1 <= threshold))
	 	{
	 		kill(pid_attacker_one,SIGUSR1);
	 		if(team_type ==1)
	 		{
			  printf("%s", ORIGINAL); 
			  printf("%s", RED_COLOR );  
	 			printf("\n ~> Defender from Team 1 put sand in Attacker [1] bag \n");
	 			printf("%s", ORIGINAL); 
	 	  }
	 		else
	 		{
	 			printf("%s", ORIGINAL); 
			  printf("%s", RED_COLOR );
	 			printf("\n ~> Defender from Team 2 put sand in Attacker [1] bag \n");
	 			printf("%s", ORIGINAL); 

	 	    }
	 	    fflush(stdout);	
	 		sleep(distance_defender_attack1);
	 		if(distance_defender_attack2 < (threshold + distance_defender_attack1))
	 		{
	 	  		// attacker 2 will push the defender 
	 			sleep(3);
	 			if(team_type ==1)
	 			{
	 				printf("%s",ORIGINAL);
	 				printf("%s",CYAN_COLOR);
	 				printf("\n ~> Attacker [2] from Team 2 pushed the Defender from Team [1]\n");
	 				printf("%s",ORIGINAL);
	 			}
	 			else
	 			{
	 				printf("%s",ORIGINAL);
	 				printf("%s",CYAN_COLOR);
	 				printf("\n ~> Attacker [2] from Team 1 pushed the Defender from Team [2]\n");
	 				printf("%s",ORIGINAL);
	 			}
	 		}
	 		else
	 		{
	 			// refill the sand cup size 
	 		 defender_player.sand_cup_size = 0;
	 		 sleep(2);
	 	   defender_player.sand_cup_size = 1;
			}
	 	}
	 
	 }
		// Attacker 2 nearest to the defender
	 else
	 {
	 	 if((defender_player.sand_cup_size ==1) && (distance_defender_attack2 <= threshold))
		 {
		 	kill(pid_attacker_two,SIGUSR1);
		 	if(team_type ==1)
		 	{
		 		printf("%s",ORIGINAL);
		 		printf("%s",RED_COLOR);
	 			printf("\n ~> Defender from Team 1 put sand in Attacker [2] bag \n");
	 			printf("%s",ORIGINAL);
	 		}
	 		else
	 		{
	 			printf("%s",ORIGINAL);
		 		printf("%s",RED_COLOR);
	 			printf("\n ~> Defender from Team 2 put sand in Attacker [2] bag \n");
	 			printf("%s",ORIGINAL);
	 		}
	 			
		 	if(distance_defender_attack1 < (threshold + distance_defender_attack2))
	 		{
	 		  // attacker 1 will push the defender  
	 			sleep(3);
	 			if(team_type ==1)
	 			{
	 				printf("%s",ORIGINAL);
	 				printf("%s",CYAN_COLOR);
	 				printf("\n ~> Attacker [1] from Team 2 pushed the Defender from Team [1]\n");
	 				printf("%s",ORIGINAL);
	 			}
	 			else
	 			{
	 				printf("%s",ORIGINAL);
	 				printf("%s",CYAN_COLOR);
	 				printf("\n ~> Attacker [1] from Team 1 pushed the Defender from Team [2]\n");
	 				printf("%s",ORIGINAL);
	 		    }
	 		}
	 		else
	 		{
		 	  defender_player.sand_cup_size = 0;
	 	    sleep(2);
	 	    defender_player.sand_cup_size = 1;
	 	    }
		 }
	 
	 }
  alarm(ideal_time);
}


/*
  Function name : distance
  Args : int x_1, int x_2 , int y_1, int y_2
  Returns : dis
  Function Description : distance calculation equation function
*/
int distance(int x_1, int x_2 , int y_1, int y_2)
{
    int dis=(int)sqrt(pow((x_2-x_1),2)+pow((y_2-y_1),2));
    return dis;
}

/*
  Function name : cmpfunc
  Args : none
  Returns : *(int*)a - *(int*)b
  Description : This is the function that compares two elements. 
*/
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

