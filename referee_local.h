static struct tank *tank, *tank_two; 
int tank_shmid1,tank_shmid2, pid_parent;
char *shmptr,*shmptr_2 ;
time_t start;
float time_game, tank_size_team_one, tank_size_team_two;
int counter=0;
sem_t   *m, *b;

/*Create message queue*/
    key_t key;
	int msgid; 
  
struct mesg_buffer {
	long mesg_type;
	float tank_size;
} message;

void sig_size_tank_check(int);
