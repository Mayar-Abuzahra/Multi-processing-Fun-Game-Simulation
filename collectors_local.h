struct collector{
  float speed
       ,backbag_size
       ,effiency_percentage;
};

struct mesg_buffer {
	long mesg_type;
	float tank_size;
} message;

struct bag_col {
	float bag_team_one
	      ,bag_team_two;
};

sem_t   *m, *b, *u, *p;
static struct tank *tank, *tank_two;
static struct bag_col *bag_col_player;
static int *loc_shm; 
int tank_shmid1, num_obs, location_x, tmp, round_collector,y_1, x_1, obstacles_check,msgid,locations_shmid2, tank_shmid2,bag_shmid3; 
float drop_water_amount;
char *shmptr, *shmptr_2, *shmptr3;
float ideal_time, target_steps, counter_steps, collector_speed, percentage_fall, bag_size_collector, change_bag_size;
struct collector c;
int *players_loc, *obs_loc, *player_obstacles_distances; 
long type_team;
key_t key;
    
/*function's prototypes*/  
int cmpfunc(const void *, const void *) ;
void sig_fill_tank(int);
int distance(int, int , int);




