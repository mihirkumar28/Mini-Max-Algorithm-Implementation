#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

class move1
{
public:
	int r, c;
	move1(int i=-1, int j=-1)
	{
		r=i;	c=j;
	}
	move1(move1 &obj)
	{
		r=obj.r;
		c=obj.c;
	}
};

bool comp=1;
int minimax(char [3][3], int, bool);
int evaluate(char [3][3], bool);
bool isMoveLeft(char [3][3]);
bool mValidation(char [3][3], move1);

move1 findBestMove(char board[3][3])
{
	int bestVal=-10000;
	move1 bestMove;

	char piece;
	if(comp)
		piece='X';
	else piece='O';

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			int moveVal;
			if(board[i][j]!='_')
				continue;

			board[i][j]=piece;
			moveVal=minimax(board, 0, !comp);
			board[i][j]='_';

			if(moveVal>bestVal)
			{
				bestVal=moveVal;
				bestMove.r=i;
				bestMove.c=j;
			}
		}
	}
	return bestMove;
}

int minimax(char board[3][3], int depth, bool x)
{
	int score=evaluate(board, !x);
	if(score!=0)	return score;

	char piece;
	if(x)
	{
		piece='X';
	}
	else
	{
		piece='O';
	}

	if(!isMoveLeft(board))
	{
		return 0;
	}

	if(x==comp)
	{
		int bestVal=-10000;
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				int moveVal;
				if(board[i][j]!='_')
					continue;

				board[i][j]=piece;
				moveVal=minimax(board, depth+1, !x);
				board[i][j]='_';

				bestVal=max(bestVal, moveVal);
			}
		}
		return bestVal;
	}
	else
	{
		int bestVal=10000;
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				int moveVal;
				if(board[i][j]!='_')
					continue;

				board[i][j]=piece;
				moveVal=minimax(board, depth+1, !x);
				board[i][j]='_';

				bestVal=min(bestVal, moveVal);
			}
		}
		return bestVal;
	}
}

bool mValidation(char board[3][3], move1 M)
{
	if(M.r>=0 && M.r<3 && M.c>=0 && M.c<3 && board[M.r][M.c]=='_')
		return 1;
	else return 0;
}

void makemove(char board[][3], char piece, move1 m)
{
	board[m.r][m.c]=piece;
}

int evaluate(char board[3][3], bool x=1)
{
	char me, opp;
	if(comp && x)
	{
		me='X';
		opp='O';
	}
	else
	{
		me='O';
		opp='X';
	}

    // Checking for Rows for X or O victory.
    for (int row = 0; row<3; row++)
    {
        if (board[row][0]==board[row][1] &&
        		board[row][1]==board[row][2])
        {
            if (board[row][0]==me)
                return +10;
            else if (board[row][0]==opp)
                return -10;
        }
    }

	 // Checking for Columns
	for (int col = 0; col<3; col++)
	{
		if (board[0][col]==board[1][col] &&
				board[1][col]==board[2][col])
		{
			if (board[0][col]==me)
				return +10;

			else if (board[0][col]==opp)
				return -10;
		}
	}

	// Checking for Diagonals
	if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
	{
		if (board[0][0]==me)
			return +10;
		else if (board[0][0]==opp)
			return -10;
	}

	if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
	{
		if (board[0][2]==me)
			return +10;
		else if (board[0][2]==opp)
			return -10;
	}

	// Else if none of them have won then return 0
	return 0;
}

bool isMoveLeft(char board[3][3])
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]=='_')
                return true;
    return false;
}

void displayboard(char board[3][3])
{
	ofstream fout;
	fout.open("latest_game_record.txt", ios::app);

	for(int i=0;i<3;i++)
	{
		cout<<"|"<<board[i][0]<<"|"<<board[i][1]<<"|"<<board[i][2]<<"|"<<endl;
		fout<<"|"<<board[i][0]<<"|"<<board[i][1]<<"|"<<board[i][2]<<"|"<<endl;
	}
	cout<<endl;
	fout<<endl;
	fout.close();
}

void initBoard(char board[][3])
{
    for (int i = 0; i<3; i++)
    	for (int j = 0; j<3; j++)
    		board[i][j]='_';
}

void singleplayer(char board[][3])
{
	ofstream fout;
	fout.open("latest_game_record.txt");
	fout<<"Player VS Computer:\n";


	move1 Move;
	bool var;
	cout<<"Do you want to play first?";
	cin>>var;
	comp=!var;

	if(var)	fout<<"Player plays first:\n\n";
	else	fout<<"Computer plays first:\n\n";
	fout.close();

	while(isMoveLeft(board))
	{
		if(!var)
		{
			Move=findBestMove(board);
			makemove(board, comp?'X':'O', Move);
			displayboard(board);
			if(evaluate(board)==10)
			{
				fout.open("latest_game_record.txt", ios::app);
				fout<<"Computer wins!"<<endl;
				fout.close();
				cout<<"Computer wins!"<<endl;
				exit(1);
			}
		}

		if(isMoveLeft(board))
		{
			bool validFlag;
			do
			{
				displayboard(board);
				validFlag=1;
				cout<<"Enter input:";
				cin>>Move.r>>Move.c;
				if(!mValidation(board, Move))
				{
					cout<<"Invalid input!"<<endl;
					validFlag=0;
				}
			}while(!validFlag);

			makemove(board, comp?'O':'X', Move);
			displayboard(board);
			if(evaluate(board)==10)
			{
				fout.open("latest_game_record.txt", ios::app);
				fout<<"Player wins!"<<endl;
				fout.close();
				cout<<"You win!"<<endl;
				exit(1);
			}
		}
		var=0;
	}

	fout.open("latest_game_record.txt", ios::app);
	fout<<"It's a draw!"<<endl;
	fout.close();
	cout<<"It's a draw!"<<endl;
}

void twoplayer(char board[][3])
{
	ofstream fout;
	fout.open("latest_game_record.txt");
	fout<<"Player VS Player:\n";
	fout.close();

	move1 Move;
	displayboard(board);

	while(isMoveLeft(board))
	{
		bool validFlag;
		do
		{
			validFlag=1;
			cout<<"Enter input player 1:";
			cin>>Move.r>>Move.c;
			if(!mValidation(board, Move))
			{
				cout<<"Invalid input!"<<endl;
				validFlag=0;
			}
		}while(!validFlag);
		makemove(board, 'X', Move);
		displayboard(board);
		if(evaluate(board, 1)==10)
		{
			fout.open("latest_game_record.txt", ios::app);
			fout<<"Player 1 wins!\n";
			fout.close();

			cout<<"Player 1 wins!"<<endl;
			exit(1);
		}

		if(isMoveLeft(board))
		{
			do
			{
				validFlag=1;
				cout<<"Enter input player 2:";
				cin>>Move.r>>Move.c;
				if(!mValidation(board, Move))
				{
					cout<<"Invalid input!"<<endl;
					validFlag=0;
				}
			}while(!validFlag);
			makemove(board, 'O', Move);
			displayboard(board);
			if(evaluate(board, 0)==10)
			{
				fout.open("latest_game_record.txt", ios::app);
				fout<<"Player 2 wins!\n";
				fout.close();

				cout<<"Player 2 wins!"<<endl;
				exit(1);
			}
		}
	}

	fout.open("latest_game_record.txt", ios::app);
	fout<<"It's a draw!\n";
	fout.close();
	cout<<"It's a draw!"<<endl;
}

int main()
{
	char board[3][3];/*={
			{'X','X','O'},
			{'O','O','X'},
			{'X','X','O'}
	};*/
	initBoard(board);

	int choice;
	cout<<"Enter choice:\n1.Player VS Computer\n2.Player VS Player"<<endl;
	cin>>choice;

	switch(choice)
	{
		case 1:
		{
			singleplayer(board);
			break;
		}
		case 2:
		{
			twoplayer(board);
			break;
		}
		default:
		{
			cout<<"Invalid input!";
			break;
		}
	}



	return 0;
}

