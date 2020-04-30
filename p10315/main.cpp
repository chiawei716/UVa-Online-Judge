#include <iostream>
#include <stdio.h>
#include <string.h>
using std::cout;
using std::cin;
using std::endl;

int getValue(char value)
{
	switch(value)
	{
		case 'A': return 14;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'T': return 10;
		case 'J': return 11;
		case 'Q': return 12;
		case 'K': return 13;
		default: return 0;
	}
}

// Insertion sort
void sort(char **hand)
{
	int min_value, min_index;
	for(int i = 0; i < 5; ++i)
	{
		min_index = i;
		for(int j = i; j < 5; ++j)
			if(getValue(hand[min_index][0]) > getValue(hand[j][0])) min_index = j;
		
		// swap
		char temp[2];
		strcpy(temp, hand[i]);
		strcpy(hand[i], hand[min_index]);
		strcpy(hand[min_index], temp);
	}
}

// Return largest card of a pair
char *pair(char **hand)
{
	int count = 0;
	char *result = new char[4];
	result[0] = '0';
	int i = 4;
	for(; i >= 1; --i)
		if(hand[i][0] == hand[i - 1][0])
		{
			result[0] = hand[i][0];
			break;
		}
	for(int j = 4; j >= 0; --j)
		if(j != i && j != i - 1)
			result[++count] = hand[j][0];
	return result;
}

// Return largest card of two pairs
char *two_pairs(char **hand)
{
	char *result = new char[3];
	result[0] = result[1] = result[2] = '0';
	int count = 0;
	for(int i = 0; i <= 4; ++i)
	{
		if(i == 4)
		{
			result[2] = hand[4][0];
			break;
		}

		if(hand[i][0] == hand[i + 1][0])
		{
			result[1 - count] = hand[i][0];
			count++;
			i++;
		}
		else
			result[2] = hand[i][0];
	}
	return result;
}

// Return largest card of three of a kind
char *three_of_a_kind(char **hand)
{
	char *result = new char[1];
	result[0] = '0';
	for(int i = 4; i >= 2; --i)
		if(hand[i][0] == hand[i - 1][0] && hand[i - 1][0] == hand[i - 2][0]) result[0] = hand[i][0];
	return result;
}

// Return largest card of four of a kind
char *four_of_a_kind(char **hand)
{
	char *result = new char[1];
	result[0] = '0';
	for(int i = 4; i >= 3; --i)
		if(hand[i][0] == hand[i - 1][0] && hand[i - 1][0] == hand[i - 2][0] && hand[i - 2][0] == hand[i - 3][0]) result[0] = hand[i][0];
	return result;
}

char *full_house(char **hand)
{
	char *result = new char[1];
	result[0] = '0';

	char *temp_three = three_of_a_kind(hand), *temp_two = two_pairs(hand);
	if(temp_three[0] != '0' && temp_two[0] != '0')
		result[0] = temp_three[0];
	delete[] temp_three;
	delete[] temp_two;
	return result;
}

char *straight(char **hand)
{
	char *result = new char[1];
	result[0] = '0';

	int top = getValue(hand[0][0]);
	int next;
	for(int i = 1; i <= 4; ++i)
	{
		next = getValue(hand[i][0]);
		if(top + 1 != next/* && !(i == 4 && top == 5 && next == 14)*/) return result;
		top = next;
	}
	result[0] = hand[4][0];
	return result;
}

char *flush(char **hand)
{
	char *result = new char[5];
	strcpy(result, "00000");

	for(int i = 0; i < 4; ++i)
		if(hand[i][1] != hand[i + 1][1]) return result;
	for(int i = 0; i < 5; ++i)
		result[i] = hand[4 - i][0];
	return result;
}

char *straight_flush(char **hand)
{
	char *result = new char[1];
	result[0] = '0';
	char *temp_straight = straight(hand), *temp_flush = flush(hand);
	if(temp_straight[0] != '0' && temp_flush[0] != '0') result[0] = temp_straight[0];
	delete[] temp_straight;
	delete[] temp_flush;
	return result;
}


int judge(char** hand1, char** hand2)
{
	char** player1_result = new char*[8];
	char* player2_result = NULL;
	int result = 0, even = 0;
	int i = 0;
	for(; i < 9; ++i)
	{
		//cout << i << endl;
		if(i == 8) break;
		switch(i)
		{
			case 0: player1_result[i] = straight_flush(hand1); break;
			case 1: player1_result[i] = four_of_a_kind(hand1); break;
			case 2: player1_result[i] = full_house(hand1); break;
			case 3: player1_result[i] = flush(hand1); break;
			case 4: player1_result[i] = straight(hand1); break;
			case 5: player1_result[i] = three_of_a_kind(hand1); break;
			case 6: player1_result[i] = two_pairs(hand1); break;
			case 7: player1_result[i] = pair(hand1); break;
			default: ;
		}
		if(player1_result[i][0] != '0') break;
	}

	for(; i >= 0; --i)
	{
		if(i == 8)
		{
			for(int j = 4; j >= 0; --j)
			{
				if(getValue(hand1[j][0]) > getValue(hand2[j][0])) { result = 1; break; }
				else if(getValue(hand1[j][0]) < getValue(hand2[j][0])) { result = -1; break; }
			}

			if(result == -1) break;
			else if(result != 1) even = 1;
		}
		else
		{
			if(player2_result)
			{
				delete[] player2_result;
 				player2_result = NULL;
			}
			switch(i)
			{
				case 0: player2_result = straight_flush(hand2); break;
				case 1: player2_result = four_of_a_kind(hand2); break;
				case 2: player2_result = full_house(hand2); break;
				case 3: player2_result = flush(hand2); break;
				case 4: player2_result = straight(hand2); break;
				case 5: player2_result = three_of_a_kind(hand2); break;
				case 6: player2_result = two_pairs(hand2); break;
				case 7: player2_result = pair(hand2); break;
				default: ;
			}	
			if(player1_result[i][0] != '0')
			{
				for(int j = 0; j < strlen(player1_result[i]); ++j)
				{	
					//cout << player1_result[i][j] << ":" << player2_result[j] << endl;
					if(getValue(player1_result[i][j]) < getValue(player2_result[j])) { result = -1; break; }
					else if(getValue(player1_result[i][j]) > getValue(player2_result[j])) { result = 1; break; }
				}
				
				if(result == -1) break;
				else if(result != 1) even = 1;
			}
			else
			{
				if(player2_result[0] != '0') { result = -1; break; }
			}
		}
		if(i == 0)
		{
			if(even == 1) result = 0;
			else result = 1;
		}	
	}
/*
	for(int j = 0; j < 8; ++j)
		if(player1_result[j])
			delete[] player1_result[j];*/
	delete[] player1_result;
	return result;
}


int main()
{
	// Input
	char*** hands = new char**[2];
	hands[0] = new char*[5];
	hands[1] = new char*[5];
	
	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 5; ++j)
			hands[i][j] = new char[2];

	while(1)
	{
		for(int i = 0; i < 2; ++i)
			for(int j = 0; j < 5; ++j)
				cin >> hands[i][j];	
		if(!cin) break;

		// Sort cards by value
		sort(hands[0]);
		sort(hands[1]);

		// Judge
		int game_result = judge(hands[0], hands[1]);
		switch(game_result)
		{
			/*
			case 1: cout << "Black wins.\n"; break;
			case -1: cout << "White wins.\n"; break;
			case 0: cout << "Tie.\n"; break;
			*/
			case 1: puts("Black wins."); break;
			case -1: puts("White wins."); break;
			case 0: puts("Tie."); break;

			default: ;
		}
	}
	
	// Release memory
	for(int i = 0; i < 5; ++i)
	{
		delete[] hands[0][i];
		delete[] hands[1][i];
	}
	delete[] hands[0];
	delete[] hands[1];

	delete[] hands;
}

