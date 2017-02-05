#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

	srand((unsigned int)time(NULL));//random number generator
	int first = 1;//used to determined which version of sentence to be printed out
	int countP = 0;//player's winning 
	int countA = 0;//AI's winning
	 
	printf("Welcome to Rock, Paper, Scissors\n");
	
	while (1)
	{
		char answer[2];//char answer;//used to remember user's choice
		if (first == 1)//first time player opens the game
		{
			printf("\nDo you want to play?(Y/N)");
			
			
		}
		else if (first != 1)//not the first time
		{
			printf("\nDo you want to play again?(Y/N)");
			
		}
		
		scanf("%s",answer);
		
		
		if (strcmp(answer, "Y")==0 || strcmp(answer, "y")==0)//player wants to play
		{
			countA = 0;
			countP = 0;
			first = 100;//change the sentence to be print out in next possible rounds
			while ( countA < 3 && countP < 3)//no one gets three wins so far 
			{
				char choice[2];//player's choice for paper rock scissors
				printf("\nWhat is your choice?(r for rock;p for paper and s for scissors)");
				scanf("%s",choice);
				
				if (strcmp(choice,"r") != 0 && strcmp(choice,"R") != 0 && strcmp(choice,"p") != 0 && strcmp(choice,"P") != 0 && strcmp(choice,"s") != 0 && strcmp(choice,"S") != 0)//incorrect input
				{
					printf("Invaild character, please try again.");
					continue;
				}
				
				int pick = 0;//AI is ready to randomly picks his choice
				//1 is paper, 2 is rock, 3 is scissors. When get o we reroll
				
				do 
				{
					pick = rand() % 4;
				}while(pick == 0);//just in case pick is zero
		
				
				if (pick == 1 && (strcmp(choice,"r") == 0 || strcmp(choice, "R") == 0))//AI has a paper while player has a rock
				{
					printf("AI has a paper while you have rock, you lose!");
					countA ++;
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					printf("Keep fighting!!\n");
					continue;
				}
				else if (pick == 1 && (strcmp(choice,"p") == 0 || strcmp(choice, "P") == 0))//its a tie
				{
					printf("You guys both have paper. Tie! Keep going!");
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 1 && (strcmp(choice,"s") == 0 || strcmp(choice, "S") == 0))//player has a scissor and AI has a paper
				{
					printf("You have scissors while AI has a paper. You win man.");
					countP ++;
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 2 && (strcmp(choice,"r") == 0 || strcmp(choice, "R") == 0))//AI has a rock while player has a rock
				{
					printf("You guys both have rocks. Tie! Keep going!");
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 2 && (strcmp(choice,"p") == 0 || strcmp(choice, "P") == 0))//AI has a rock while player has a paper
				{
					printf("You have a paper while AI has a rock. You win!");
					countP ++;
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 2 && (strcmp(choice,"s") == 0 || strcmp(choice, "S") == 0))//AI has a rock while player has a scissor
				{
					printf("You have a scissor while AI has a rock. You lose!");
					countA ++;
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 3 && (strcmp(choice,"r") == 0 || strcmp(choice, "R") == 0))//AI has a scissor while player has a rock
				{
					printf("You have a rock while AI has a scissor. You win!");
					countP ++;
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 3 && (strcmp(choice,"p") == 0 || strcmp(choice, "P") == 0))//AI has a scissor while player has a paper
				{
					printf("You have a paper while AI has a scissor. You lose!");
					countA ++;
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				else if (pick == 3 && (strcmp(choice,"s") == 0 || strcmp(choice, "S") == 0))//both get scissors, tie
				{
					printf("You guys both have scissors. Tie! Keep going!");
					printf("\nYour wins: %d. AI wins: %d.",countP,countA);
					continue;
				}
				
				
			}//end of while loop
			
			
			if (countA == 3 || countP == 3)//ready to print out the final result
			{
				if (countA == 3)//AI wins
				{
					printf("\nAI has winned: %d times and you winned %d times. AI wins the round.",countA,countP);
					continue;
				}
				else if (countP == 3)//player wins
				{
					printf("\nAI has winned: %d times and you winned %d times. You win the round.",countA,countP);
					continue;
				}
			}
			else if (countA > 3 || countP > 3)//logically impossible case if user types in correct characters.If someone type in some crazy stuff, warning will occur
			{
				printf("\nYou typed in some crazy stuff. Please follow the instruction and try again.");
				continue;
			}
			
			
			
		}//end of yes if statment
		
		else if (strcmp(answer, "N")==0 || strcmp(answer, "n")==0)//player does not want to play
		{
		
			break;
		}
		else if (strcmp(answer,"N")!=0 && strcmp(answer, "n")!=0 && strcmp(answer, "Y")!=0 && strcmp(answer, "y")!=0)//player does not type in the required character
		{
			
			printf("That is not a vaild character. Please try again\n");
			continue;
		}
		
		
		
	}//end of while loop
	
	
	printf("\nThanks for playing. Have a nice day. >_<");
	return (0);
	
}//end of main 