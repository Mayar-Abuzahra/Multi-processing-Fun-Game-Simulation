#include "local.h"
#include "collectors_local.h"

void main(int argc, char *argv[])
{   
    float step_range, bag_size;
    static int *shmptr_locations;

    x_1 = atoi(argv[1]);
    location_x = x_1;
    y_1 = atoi(argv[2]); 
    num_obs = atoi(argv[8]);
    c.effiency_percentage=1.0;
    bag_size_collector=atof(argv[4]);
    type_team=atoi(argv[3]);
    c.backbag_size=bag_size_collector;   
    collector_speed=atof(argv[6]);
    c.speed=collector_speed; 
    
    /* Declare the amount of the dropped water according to the bag size */
    drop_water_amount = 0.009 * c.backbag_size;
      
    /* semaphore for collector bag */
    if ((u = sem_open("bag_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
    sem_init(u,1,1);
    
    /* semaphore for player's locations*/
    if ((p = sem_open("player_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
    sem_init(p,1,1);
    
    /*define collector's bags as shared memory*/
    bag_shmid3 = shmget(KEY_BAG, sizeof(struct bag_col), 0644 | IPC_CREAT);
    // Initialize bag data value.
    bag_col_player = shmat(bag_shmid3 , NULL, 0);
    if (bag_col_player == (void *)-1)
    {
     fprintf(stderr, "Shared memory attach error !\n");
    }
    bag_col_player->bag_team_one = bag_size_collector;
    bag_col_player->bag_team_two = bag_size_collector;
    if (shmdt(bag_col_player) == -1)
    {
      fprintf(stderr, "shmdt\n");
    } 
    
    if(type_team == 1)
    {
			if ((m = sem_open("tank_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) 
			{
				perror("semaphore initilization");
				exit(1);
			}
			sem_init(m,1,1);
    }
    else if(type_team == 2)
    {
		  if ((b = sem_open("tank_semaphore_2", O_CREAT, 0644, 1)) == SEM_FAILED) 
		  {
		    perror("semaphore initilization");
		    exit(1);
		  }
    sem_init(b,1,1);
    }
    
    /* Declare dynamic arrays to save the locations of players and the Obstacles*/
    players_loc = malloc(sizeof(int) * (2*NUM_OF_PLAYERS));
    obs_loc = malloc(sizeof(int) * (2*num_obs));
    player_obstacles_distances = malloc(sizeof(int) * (num_obs));
    
    /* Read the locations from the shared memory into the dynamic arrays */
    sem_wait(p);
    if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
		 	if (loc_shm = shmat(locations_shmid2, NULL, 0));
		 	
			if (loc_shm == (void *)-1)
			{
				fprintf(stderr, "Shared memory attach error !\n");
			}
			for(int u=0; u < ((2*NUM_OF_PLAYERS)+ (2*num_obs)) ;u++)
			{
				if(u < ((2*NUM_OF_PLAYERS)))
				{
					 players_loc[u]=loc_shm[u];
				}
		    else 
		    {
				  obs_loc[u-(2*NUM_OF_PLAYERS)]=loc_shm[u];
		    }
		  }
    }
    if (shmdt(loc_shm) == -1)
    {
       fprintf(stderr, "shmdt\n");
    }
    sem_post(p);
    
   /* pause collectors work till the parent send them signal to continue */    
    kill(getpid(), SIGSTOP);
    printf("%s", ORIGINAL);
    printf("%s", BLUE_COLOR);
    printf("\u263A :");
    printf("Collecter Id -> %d \n",getpid());
    fflush(stdout);
    printf("%s", ORIGINAL); 
    	
    /* Compute the step consume time */
    step_range=atof(argv[5]);
    target_steps=ceil(atoi(argv[7]) / step_range);
    ideal_time=ceil(step_range / collector_speed);
    signal(14, sig_fill_tank); 
    obstacles_check=ceil(atoi(argv[7]) / 4);
    tmp = obstacles_check;
    alarm(ideal_time);

    while(1);                                                  
}

/*
  Function name : sig_fill_tank
  Args : signum
  Returns : null 
  Description : Each round the collector try to fill the tank in the other side and it may face some struggles with obstacles and attckers
*/
void sig_fill_tank(int signum){
  float dropped_water;
  counter_steps+=ideal_time;
  x_1+=ideal_time;
  
  // <<<<<<<<<<<<<<<<<<<<<<< attach to shared memory >>>>>>>>>>>>>>>>>>>>>>>
  // no ipc-create: We just want to connect to it
  /* Update Collector location */
  sem_wait(p);
  if ((locations_shmid2 = shmget((int)KEY_LOCATION,0, 0)) != -1)
    {
		 if (loc_shm = shmat(locations_shmid2, NULL, 0));
		 	
		 if (loc_shm == (void *)-1)
		 {
			  fprintf(stderr, "Shared memory attach error !\n");
		 }
		 if(type_team==1)
		 {
			  loc_shm[0]=x_1;
		 }
		 else
		 {
			  loc_shm[2]=x_1;
		 }
    }
  if (shmdt(loc_shm) == -1)
    {
       fprintf(stderr, "shmdt\n");
    }
  sem_post(p);
  
 /* Reach to the tank in the oher side */ 
  if(counter_steps==target_steps){
  
   if(type_team == 1){
   
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
   
     change_bag_size = bag_size_collector - bag_col_player->bag_team_one;
    }
    if (shmdt(bag_col_player) == -1)
     {
       fprintf(stderr, "shmdt\n");
     }
     sem_post(u);     
   
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
     if(tank->size  < c.backbag_size)
     { 
       c.backbag_size=tank->size;
       tank->size=0;
     }
     else
     {
       tank->size -=  c.backbag_size;
     }
    }
    if (shmdt(tank) == -1)
     {
       fprintf(stderr, "shmdt\n");
     }
     sem_post(m); /* check if the tank size =0 then stop fill function  */
     }
     else
     {

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
      change_bag_size=bag_size_collector-bag_col_player->bag_team_two;
     }
     if (shmdt(bag_col_player) == -1)
     {
       fprintf(stderr, "shmdt\n");
     }
     sem_post(u); 
     
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
    	if(tank_two->size  < c.backbag_size)
		{ 
		  c.backbag_size=tank_two->size;
		  tank_two->size=0;
		}
	    else
	    {
		  tank_two->size -=  c.backbag_size;
	    }
    }
    if (shmdt(tank_two) == -1)
     {
       fprintf(stderr, "shmdt\n");
     }
     sem_post(b); /* check if the tank size =0 then stop fill function  */
     
     }
     
     /* Compute the lost water from the bag */
     dropped_water = drop_water_amount * dropped_water;
     dropped_water -= change_bag_size; 
     if(c.backbag_size > dropped_water)
     {
     	c.backbag_size-=dropped_water;
     }
     
     /*Create message queue -- ftok to generate unique key*/
     key = ftok("Random", 65);
     msgid = msgget(key, 0666 | IPC_CREAT);

     if( type_team ==1)
     {
       message.tank_size += c.backbag_size;
       message.mesg_type = type_team;
       printf("\n >> Tank B is filled \t its size = %f \n",message.tank_size );
     }
     else
     {
       message.tank_size += c.backbag_size;
       message.mesg_type = type_team;
       printf("\n >> Tank D is filled \t its size = %f \n",message.tank_size);
     }  
     msgsnd(msgid, &message, sizeof(message), 0); 
     
     obstacles_check = tmp;
     counter_steps = 0;
     round_collector+=1;
     c.backbag_size = 0;
     /*time needed to return to the tank*/
     sleep(2);
     x_1 = location_x;
     c.backbag_size=bag_size_collector;
     dropped_water =0;
  }

  if(round_collector > 1)
  {
  	if(c.effiency_percentage > (1.0 / exp(round_collector)))
  	{
       c.effiency_percentage-= 1.0 / exp(round_collector);
	}
    if(c.effiency_percentage < 0.5)
    {
       c.speed-=round_collector;
    }
  }
  
  /*check obstacles*/
  if(counter_steps == (float)obstacles_check)
  {
   int index=0, obs_affect_fall=0;
   for(int v=0;v<(2*num_obs);v+=2)
   {
     player_obstacles_distances[index]=distance(x_1,obs_loc[v],obs_loc[v+1]);
     index++;
   }
    obstacles_check+=obstacles_check;
    qsort(player_obstacles_distances, num_obs, sizeof(int), cmpfunc);
    for( int n = 0 ; n < (num_obs); n++ ) 
    { 
      if(player_obstacles_distances[n] <= 4)
      {
      	obs_affect_fall++;
      }
    }
    percentage_fall = (float)obs_affect_fall/ (float)num_obs;
    if(percentage_fall > 0.5)
    {
      c.backbag_size = 0;
      counter_steps=0;
      obstacles_check=tmp;
      sleep(1);    
      x_1 = location_x;
      c.backbag_size=bag_size_collector;
    }   
  }
 
  /* Drop water because changing in speed*/
  if(c.speed != collector_speed)
  { 
    dropped_water++;
  }  
  alarm(ideal_time);
}

/*
  Function name : distance
  Args : x_1, obs_loc[i], obs_loc[i+1]
  Returns : distance
  Function Description : distance calculation equation function
*/
int distance(int x_player, int x_obstacle , int y_obstacle)
{
    int dis=(int)sqrt(pow((x_player-x_obstacle),2)+pow((y_1-y_obstacle),2));
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
