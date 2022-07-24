float size_tank;
char time_game[10],player_speed[3], step_range[3],config_file[20];

void random_numbers(int , int array[], int , int , int);
void config_parser(void);
void random_obstacales(int , int array[], int, int, int);
void sig_team1_win(int);
void sig_team2_win(int);
void sig_tie(int);


int count_wins_team1, count_wins_team2 , current_team_win, previous_team_win, finish_game;

  /* Define needed variables**/
  int pid_collecter[NUM_COLLECTERS],
      pid_attacker[NUM_ATTACKERS],
      pid_defender[NUM_DEFENDERS],
      pid_referee;

