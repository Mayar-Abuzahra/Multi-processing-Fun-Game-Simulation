#define RANGE_DIS_MAX 3
#define RANGE_DIS_MIN 1 
struct attacker{
  float speed
       ,water_cup_size
       ,backbag_size
       ,effiency_percentage;
};

struct bag_col {
	float bag_team_one
	      ,bag_team_two;
};
    
sem_t   *m, *u, *p, *b;
static struct tank *tank, *tank_two;
struct attacker attacker_player;
static struct bag_col *bag_col_player;
int team_type_num, attacker_index_number,locations_shmid2, distance_col_attack, attackers_location[2], probability_distance, bag_shmid3, tank_shmid1, tank_shmid2,collector_x_loc, collector_y_loc, sand_put_times, flag_sand_put_time;
static int *loc_shm; 
char *shmptr3, *shmptr, *shmptr_2;
float target_steps, attacker_speed, ideal_time,counter_steps;


/*Functions prototypes*/
int distance(int, int , int, int);
void sig_attacker_task(int);
void sig_fill_bag(int);
