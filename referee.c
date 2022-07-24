#include "local.h"
#include "referee_local.h"

void main(int argc, char *argv[])
{ 
  int counter;  
  pid_parent=atoi(argv[1]);
  time_game=atof(argv[2]);

  if ((m = sem_open("tank_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("semaphore initilization");
    exit(1);
  }
  sem_init(m,1,1);
  
  if ((b = sem_open("tank_semaphore_2", O_CREAT, 0644, 1)) == SEM_FAILED) 
    {
      perror("semaphore initilization");
      exit(1);
    }
  sem_init(b,1,1);
  
  /* <<<<<<  Start the game Till all the palyers Ready to start >>>>>> */ 
  kill(getpid(), SIGSTOP);
  printf("%s", ORIGINAL); 
  printf("\u263A :");
  printf("%s", PURPLE_COLOR );
  printf("Referee Id -> %d \n",getpid());
  printf("%s", ORIGINAL); 
  
  /* <<<<<< ftok to generate unique key >>>>>> */
  key = ftok("Random", 65);
  msgid = msgget(key, 0666 | IPC_CREAT);

  time_game*=60; //convert time from minutes to seconds
  signal(SIGALRM,sig_size_tank_check); 
  alarm(1);
  while(1);                                             
}

/*
* description : The referee will end the round when the Tank A | Tank C is empty OR the Time is over 
*/
void sig_size_tank_check(int signum){
	float tank_A_size, tank_B_size, tank_C_size, tank_D_size;
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<< attach to shared memory >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  /*Check tank size A and C*/ 
 	 sem_wait(m);
		if ((tank_shmid1 = shmget((int)KEY,0, 0)) != -1)
		{
		 if ((shmptr = (char *)shmat(tank_shmid1, (char *)0, 0)) == (char *)-1)
		 {
		      perror("shmat -- passenger -- attach");
		      exit(1);
		 }
		 tank = (struct tank*)shmptr;
		 tank_A_size = tank->size;
		}
      if (shmdt(tank) == -1)
     {
       fprintf(stderr, "shmdt\n");
     }
   sem_post(m);
  
   sem_wait(b);
   if ((tank_shmid2 = shmget((int)KEY_TANK,0, 0)) != -1)
   {
			if ((shmptr_2 = (char *)shmat(tank_shmid2, (char *)0, 0)) == (char *)-1)
		 {
			  perror("shmat -- passenger -- attach");
			  exit(1);
		 }
		 tank_two = (struct tank*)shmptr_2;
		 tank_C_size = tank_two->size;
   }
   if (shmdt(tank_two) == -1)
   {
     fprintf(stderr, "shmdt\n");
   }
   sem_post(b); 
  counter++;

  
  /* check the stop conditions of the round  */
  if((counter == (int)(time_game)) || (tank_A_size == 0) || (tank_C_size == 0)){
  
    while ((msgrcv(msgid, (void *) &message, sizeof(message.tank_size), 1, MSG_NOERROR | IPC_NOWAIT))  > 0 ) 
    {    
         tank_size_team_one=message.tank_size;
    }
    while ((msgrcv(msgid, (void *) &message, sizeof(message.tank_size), 2, MSG_NOERROR | IPC_NOWAIT))  > 0 ) 
    {     
         tank_size_team_two=message.tank_size;
    }
    tank_B_size = tank_size_team_one;
    printf("\n Referee Check \t Tank B size = %f \n",tank_B_size);
    tank_D_size = tank_size_team_two;
    printf("\n Referee Check \t Tank D size  = %f \n",tank_D_size);
    if(tank_B_size > tank_D_size)
    {
     	kill(pid_parent, SIGUSR1);
     	if (msgctl(msgid, IPC_RMID, NULL) == -1) 
     	{
		fprintf(stderr, "Message queue could not be deleted.\n");
		exit(EXIT_FAILURE);
		}
    }
    else if(tank_B_size < tank_D_size)
    {
      kill(pid_parent, SIGUSR2);
      if (msgctl(msgid, IPC_RMID, NULL) == -1) 
      {
		fprintf(stderr, "Message queue could not be deleted.\n");
		exit(EXIT_FAILURE);
	  }
    }
	else
    {
    	if (msgctl(msgid, IPC_RMID, NULL) == -1) 
        {
			fprintf(stderr, "Message queue could not be deleted.\n");
			exit(EXIT_FAILURE);
	    }
        kill(pid_parent, SIGQUIT);
    }

  }
  alarm(1);
}
