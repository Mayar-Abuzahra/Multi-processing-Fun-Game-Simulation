gcc fungame.c -o fungame -lpthread -lm
gcc attackers.c -o attackers -lpthread -lm
gcc collectors.c -o collectors -lpthread -lm
gcc defenders.c -o defenders -lpthread -lm
gcc referee.c -o referee -lpthread -lm
./fungame variables.txt


