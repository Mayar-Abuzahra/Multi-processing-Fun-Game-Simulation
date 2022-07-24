#include "local.h"
#include "attackers_local.h"

void main(int argc, char *argv[])
{
    float step_range;
    collector_x_loc=atoi(argv[1]);
    collector_y_loc=atoi(argv[2]);
    attacker_player.effiency_percentage=1.0;
    attacker_player.backbag_size=atof(argv[3]);
    attacker_player.water_cup_size=attacker_player.backbag_size * 0.1;
    attacker_speed=atof(argv[5]);
    attacker_player.speed=attacker_speed;
    team_type_num=atoi(argv[6]);
    attacker_index_number=atoi(argv[7]); 
    
    
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
	  if(team_type_num==1){
	   if(attacker_index_number==0){
	  	  attackers_location[0] =loc_shm[4];
	  	  attackers_location[1] =loc_shm[5];
	  	  
	  }
	  else if(attacker_index_number==1){
	  	  attackers_location[0] =loc_shm[6];
	  	  attackers_location[1] =loc_shm[7];
	  }
	  }
	  else{
	    if(attacker_index_number==2){
	  	  attackers_location[0] =loc_shm[8];
	  	  attackers_location[1] =loc_shm[9];
	  }
	  else if(attacker_index_number==3){
	  	  attackers_location[0] =loc_shm[10];
	  	  attackers_location[1] =loc_shm[11];
	  }
	  }
        if (shmdt(loc_shm) == -1)
       {
         fprintf(stderr, "shmdt\n");
       }
    }
    sem_post(p);
    
    /* semaphore for bag collector*/
    if ((u = sem_open("bag_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
    sem_init(u,1,1);

    if ((b = sem_open("tank_semaphore_2", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
    sem_init(b,1,1);
    
    if ((m = sem_open("tank_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
    sem_init(m,1,1);	
    
    kill(getpid(), SIGSTOP);	
    signal(SIGUSR1, sig_fill_bag); 
    printf("%s", ORIGINAL); 
    printf("%s", CYAN_COLOR);
    printf("\u263A :");
    printf("Attacker Id -> %d \n",getpid());
    printf("%s", ORIGINAL); 
    
    step_range=atof(argv[4]);
    target_steps=(ceil(distance(collector_x_loc,attackers_location[0],collector_y_loc,attackers_location[1]) / step_range))-2;   
    ideal_time=ceil(step_range / attacker_speed);
    signal(14, sig_attacker_task); 
    alarm(ideal_time);
     
    while(1);                                             
}

void sig_attacker_task(int signum){
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
	 if(team_type_num==1){		
			if(attacker_index_number==0)
			{
			 loc_shm[4]=collector_x_loc;
			 loc_shm[5]=collector_y_loc;
			}
			else
			{
			 loc_shm[6]=collector_x_loc;
			 loc_shm[7]=collector_y_loc;
			}
	  }
	  else{
			if(attacker_index_number==2)
			{
			 loc_shm[8]=collector_x_loc;
		   loc_shm[9]=collector_y_loc;
			}
			else
			{
				loc_shm[10]=collector_x_loc;
		    loc_shm[11]=collector_y_loc;
			}
			
	  }
    }
    if (shmdt(loc_shm) == -1)
    {
      fprintf(stderr, "shmdt\n");
    }
    sem_post(p);
    counter_steps=0;
    sleep(2);
  }
  
  
  if(team_type_num==1)
  {
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
   	if (loc_shm = shmat(locations_shmid2, NULL, 0));
   	
	if (loc_shm == (void *)-1)
	  {
		fprintf(stderr, "Shared memory attach error !\n");
	  }

	  if(attacker_index_number==0)
	  {
	   loc_shm[4]+=ideal_time;
	   distance_col_attack=distance(loc_shm[2], loc_shm[4], loc_shm[3], loc_shm[5]);
	  }
	  else
	  {
	  	loc_shm[6]+=ideal_time;
	    distance_col_attack=distance(loc_shm[2], loc_shm[6], loc_shm[3], loc_shm[7]);
	  }
        if (shmdt(loc_shm) == -1)
       {
         fprintf(stderr, "shmdt\n");
       }
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
	  if(attacker_index_number==0)
	  {
	  	 loc_shm[8]+=ideal_time;
	     distance_col_attack=distance_col_attack=distance(loc_shm[0], loc_shm[8], loc_shm[1], loc_shm[9]);
	  }
	  else
	  {
	  	 loc_shm[10]+=ideal_time;
	     distance_col_attack=distance_col_attack=distance(loc_shm[0], loc_shm[10], loc_shm[1], loc_shm[11]);
	  }
        if (shmdt(loc_shm) == -1)
       {
         fprintf(stderr, "shmdt\n");
       }
    }
    sem_post(p);
  }
    if((sand_put_times > 2) && (flag_sand_put_time == 1)){
    
    	int sand = sand_put_times-2;
    	float x = exp(-(sand));
    	
    	attacker_player.effiency_percentage -= x;
        if(attacker_player.effiency_percentage < 0)
        {
           attacker_player.effiency_percentage = 0.05;
        }
    	flag_sand_put_time = 0;
    }
    probability_distance = (rand() % (RANGE_DIS_MAX - RANGE_DIS_MIN + 1)) + RANGE_DIS_MIN;
    if(distance_col_attack <= probability_distance)
    {
      if(attacker_player.effiency_percentage > 0.5)
      {   
      /*
       * If stole process succeed
       */
      
      /*modify bag size*/ 
		  sem_wait(u);
		  if ((bag_shmid3 = shmget((int)KEY_BAG,0, 0)) != -1)
		  {
				if ((shmptr3 = (char *)shmat(bag_shmid3, (char *)0, 0)) == (char *)-1)
				{
					perror("shmat -- passenger -- attach");
					exit(1);
				}
				bag_col_player = (struct bag_col*)shmptr3;
				
				if(team_type_num == 1)
		    {
		       (bag_col_player->bag_team_two)-=attacker_player.water_cup_size;
		       fflush(stdout);
		       printf("%s",ORIGINAL);
		       printf("%s",BLUE_COLOR);
		       printf("\n ~> Attacker From Team 1 stole water from the Collector in Team [2]\n");
		       printf("%s",ORIGINAL);
		    }
		    else
		    {
		       (bag_col_player->bag_team_one)-=attacker_player.water_cup_size;
		       fflush(stdout);
		       printf("%s",ORIGINAL);
		       printf("%s",BLUE_COLOR);
		       printf("\n ~> Attacker From Team 2 stole water from the Collector in Team [1]\n");
		       printf("%s",ORIGINAL);
		    }
		  }

		 if (shmdt(bag_col_player) == -1)
		  { 
			fprintf(stderr, "shmdt\n");
		  }
		sem_post(u); 
    sleep(2);
    
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
   	if (loc_shm = shmat(locations_shmid2, NULL, 0));
   	
		if (loc_shm == (void *)-1)
			{
			fprintf(stderr, "Shared memory attach error !\n");
			}
	 if(team_type_num==1){		
			if(attacker_index_number==0)
			{
			 loc_shm[4]=collector_x_loc;
			 loc_shm[5]=collector_y_loc;
			}
			else
			{
			 loc_shm[6]=collector_x_loc;
			 loc_shm[7]=collector_y_loc;
			}
	  }
	  else{
			if(attacker_index_number==2)
			{
			 loc_shm[8]=collector_x_loc;
		   loc_shm[9]=collector_y_loc;
			}
			else
			{
				loc_shm[10]=collector_x_loc;
		    loc_shm[11]=collector_y_loc;
			}
			
	  }
        if (shmdt(loc_shm) == -1)
       {
         fprintf(stderr, "shmdt\n");
       }
    }
    sem_post(p);
           
           if(team_type_num == 1)
           {
           /*modify tank size*/ 
           sem_wait(m);
						if ((tank_shmid1 = shmget((int)KEY,0, 0)) != -1)
						{
						 if ((shmptr = (char *)shmat(tank_shmid1, (char *)0, 0)) == (char *)-1)
						 {
								perror("shmat -- passenger -- attach");
								exit(1);
						 }
						 tank = (struct tank*)shmptr;
						 tank->size+=attacker_player.water_cup_size;				 
						}
						if (shmdt(tank) == -1)
						 {
							 fprintf(stderr, "shmdt\n");
						 }
						 sem_post(m); 
					 }
						else{
						
						/*modify tank size*/ 
						sem_wait(b);
						if ((tank_shmid2 = shmget((int)KEY_TANK,0, 0)) != -1)
						{
						if ((shmptr_2 = (char *)shmat(tank_shmid2, (char *)0, 0)) == (char *)-1)
					 {
						  perror("shmat -- passenger -- attach");
						  exit(1);
					 }
					 tank_two = (struct tank*)shmptr_2;
					  tank_two->size+=attacker_player.water_cup_size;				 
					}
					if (shmdt(tank_two) == -1)
					 {
						 fprintf(stderr, "shmdt\n");
					 }
					 sem_post(b); 

						}
           sleep(2);   
     
      }
    }
  alarm(ideal_time);
}

void sig_fill_bag(int signum){
	sand_put_times++;
	flag_sand_put_time=1;	
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

