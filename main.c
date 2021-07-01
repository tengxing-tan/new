#include<stdio.h>
 
void draw(int pos[3][3])
{
	for(int i=0; i<3; ++i) 
	{
		printf("\n");
        
		for(int j=0; j<3; ++j)
		{
			if(pos[i][j]==1)
			{ 
				printf(" O "); // p1 = O
			}else if (pos[i][j]==2)
			{
				printf(" X "); // p2 = X
			}else 	printf("   ");

			if(j!=2) printf("|");
		}
		if(i!=2) printf("\n---+---+---");
	}
}

void get_position(int pos[3][3], int playerNo)
{
	int x, y;

	do {
		printf("\nPlayer %d, select a tile to play (1 1) : ", playerNo);
		scanf("%d %d", &x, &y);
	} while(x>3 || y>3 || pos[--x][--y] > 0);

	pos[x][y] = playerNo;
}
	
int check_winner(int pos[3][3])
{
    int j;
    for(int i=0; i<3; ++i)
    {
        // horizontal 
        if((pos[i][0] == pos[i][1]) && (pos[i][0] == pos[i][2]))
            return pos[i][0];
        
        // vertical
        if((pos[0][i] == pos[1][i]) && (pos[0][i] == pos[2][i]))
            return pos[0][i];
    }
    // diagonal
    if(pos[1][1] != 0)
    {
        if(pos[0][0] == pos[2][2])
            return pos[0][0];
        else if(pos[0][2] == pos[2][0])
            return pos[0][2];
    }
    return 0;
}

int main()
{
	int position[3][3] = {0,0,0,0,0,0,0,0,0};
	int player=0; // 1 or 2
	int turn=0, win=0;

	// while loop until count = 9 OR get winner
	do {
		++turn;
		player = (turn % 2 == 0) ? 2 : 1;
	// ask player position
		get_position(position, player);
	// check winner
	// print board function
		draw(position);
		if(turn > 3)
        {
	            win = check_winner(position);
        }
	} while(turn <= 9 && win == 0);

    if(win == 0)
    {
        printf("\nNo one win :(");
    }else{
        printf("\nPlayer %d has win!", win);
    }

	return 0;
}
