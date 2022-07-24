#define RANGE_DIS_MAX 3
#define RANGE_DIS_MIN 1

struct defender{
  float speed
       ,sand_cup_size
       ,effiency_percentage;
};
sem_t *p;
int team_type,defender_location[2],locations_shmid2,tank_x_loc,tank_y_loc;
long pid_attacker_one,pid_attacker_two;
static int *loc_shm; 
float ideal_time, target_steps,counter_steps;
struct defender defender_player;

/*Functions prototypes*/
int distance(int, int , int, int);
void sig_defender_task(int);


