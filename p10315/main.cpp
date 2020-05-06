#include <iostream>
#include <stdio.h>
#include <string.h>
using std::cout;
using std::cin;
using std::endl;

// Mapping value from char to integer
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

// Selection sort by value
void sort(char **hand)
{
	int min_value, min_index;
	for(int i = 0; i < 5; ++i)
	{
		// find smallest one
		min_index = i;
		for(int j = i; j < 5; ++j)
			if(getValue(hand[min_index][0]) > getValue(hand[j][0])) min_index = j;
		
		// swap
		char temp[2];
		strcpy(temp, hand[i]);
		strcpy(hand[i], hand[min_index]);
		strcpy(hand[min_index], temp);
	}
	return;
}

// Return values for comparing high cards - { single_1st, single_2nd, single_3rd, single_4th, single_5th }
char *high_card(char **hand)
{
	char *result = new char[5];
	for(int i = 0; i <= 4; ++i)
		result[4 - i] = hand[i][0];
	return result;
}

// Return values for comparing pairs - { pair, single_1st, single_2nd, single_3rd }
char *pair(char **hand)
{
	int count = 0;
	char *result = new char[4];
	result[0] = result[1] = result[2] = result[3] = '0';

	// Find backward in order to get the larger one
	int i = 4;
	for(; i >= 1; --i)
	{
		if(hand[i][0] == hand[i - 1][0]) { result[0] = hand[i][0]; break; }
		if(i == 1) return result;
	}

	// If pair exist, put other single card to array
	for(int j = 4; j >= 0; --j)
		if(j != i && j != i - 1)
			result[++count] = hand[j][0];
	return result;
}

// Return values for comparing two pairs - { pair_larger, pair_smaller, single }
char *two_pairs(char **hand)
{
	char *result = new char[3];
	result[0] = result[1] = result[2] = '0';
	int count_down = 1;

	// Find forward to make sure result[1] has value iff two pairs
	for(int i = 0; i <= 4; ++i)
	{
		if(i == 4) { result[2] = hand[i][0]; break; }	// Single left
		if(hand[i][0] == hand[i + 1][0])
			result[count_down--] = hand[i++][0];
	}
	return result;
}

// Return value for comparing three of a kind - { three_of_a_kind }
char *three_of_a_kind(char **hand)
{
	char *result = new char[1];
	result[0] = '0';
	for(int i = 0; i <= 2; ++i)
		if(hand[i][0] == hand[i + 1][0] && hand[i + 1][0] == hand[i + 2][0]) result[0] = hand[i][0];
	return result;
}

// Return value for comparing four of a kind - { four_of_a_kind }
char *four_of_a_kind(char **hand)
{
	char *result = new char[1];
	result[0] = '0';
	for(int i = 0; i <= 1; ++i)
		if(hand[i][0] == hand[i + 1][0] && hand[i + 1][0] == hand[i + 2][0] && hand[i + 2][0] == hand[i + 3][0]) result[0] = hand[i][0];
	return result;
}

// Return value for comparing full house - { three_of_a_kind }
char *full_house(char **hand)
{
	char *result = new char[1];
	result[0] = '0';

	// Check result of three of a kind & two pairs, use the value of three of a kind if both true
	char *result_three = three_of_a_kind(hand);
	char *result_two = two_pairs(hand);
	if(result_three[0] != '0' && result_two[0] != '0') 
		result[0] = result_three[0];
	delete[] result_three;
	delete[] result_two;
	return result;
}

// Return value for comparing straight - { largest }
char *straight(char **hand)
{
	char *result = new char[1];
	result[0] = '0';

	// If not consecutive, return empty result
	int check = getValue(hand[0][0]) + 1;
	for(int i = 1; i <= 4; ++i)
		if(check++ != getValue(hand[i][0])) return result;

	// If consecutive, return the largest value
	result[0] = hand[4][0];
	return result;
}

// Return values for comparing flush - { single_1st, single_2nd, single_3rd, single_4th, single_5th }
char *flush(char **hand)
{
	char *result = new char[5];
	strcpy(result, "00000");

	// Checking & filling
	for(int i = 0; i <= 4; ++i)
	{	
		if(i == 4) { result[4 - i] = hand[i][0]; break; }
		if(hand[i][1] == hand[i + 1][1])
			result[4 - i] = hand[i][0];
		else break;
	}
	return result;
}

// Return values for comparing straight flush - { largest }
char *straight_flush(char **hand)
{
	char *result = new char[1];
	result[0] = '0';
	char *result_straight = straight(hand);
	char *result_flush = flush(hand);
	if(result_straight[0] != '0' && result_flush[0] != '0') 
		result[0] = result_straight[0];
	delete[] result_straight;
	delete[] result_flush;
	return result;
}

// Return result of comparing two hands - { 1: player1 wins, 0: tie, -1: player1 lose } 
int compare(char** hand1, char** hand2)
{
	// Get results of player1
	char* player1_result = nullptr;
	int player1_level = 0;
	for(; player1_level < 9; ++player1_level)
	{	
		switch(player1_level)
		{
			case 0: player1_result = straight_flush(hand1); 	break;
			case 1: player1_result = four_of_a_kind(hand1); 	break;
			case 2: player1_result = full_house(hand1); 		break;
			case 3: player1_result = flush(hand1); 				break;
			case 4: player1_result = straight(hand1); 			break;
			case 5: player1_result = three_of_a_kind(hand1);	break;
			case 6: player1_result = two_pairs(hand1); 			break;
			case 7: player1_result = pair(hand1); 				break;
			case 8: player1_result = high_card(hand1); 			break;
			default: ;
		}
		if(player1_result[0] == '0') { memset(player1_result, '\0', strlen(player1_result)); delete [] player1_result; player1_result = nullptr; }	
		else break;
	}

	// According to result of player1, check if player2 has even or larger type
	int result = 1;
	char* player2_result = nullptr;
	int player2_level = player1_level;
	for(; player2_level >= 0; --player2_level)
	{
		if(player2_result) { memset(player2_result, '\0', strlen(player2_result)); delete[] player2_result; player2_result = nullptr; }	
		switch(player2_level)
		{
			case 0: player2_result = straight_flush(hand2); 	break;
			case 1: player2_result = four_of_a_kind(hand2); 	break;
			case 2: player2_result = full_house(hand2); 		break;
			case 3: player2_result = flush(hand2); 				break;
			case 4: player2_result = straight(hand2); 			break;
			case 5: player2_result = three_of_a_kind(hand2); 	break;
			case 6: player2_result = two_pairs(hand2); 			break;
			case 7: player2_result = pair(hand2); 				break;
			case 8: player2_result = high_card(hand2); 			break;
			default: ;
		}
		// Same type
		if(player1_level == player2_level)
		{
			for(int i = 0; i < strlen(player1_result); ++i)
			{
				int val1 = getValue(player1_result[i]), val2 = getValue(player2_result[i]);
				if(val1 > val2) { result = 1; break; }
				else if(val1 < val2) { result = -1; break; }
				else if(val1 == val2) { result = 0; }
			}
			// lose already
			if(result == -1) break;
		}

		/* 
		 * Now result = 1 if player1 got lead with same type.
		 *     result = 0 if tie with same type.
		 *
		 * But we still need to check if player2 got larger type.
		 */

		// Check if player2 has larger type
		else
		{
			if(player2_result[0] != '0') { result = -1; break; }
		}
	}

	if(player1_result) delete[] player1_result;
	if(player2_result) delete[] player2_result;
	return result;
}


int main()
{		
	// Memory alloc
	char*** hands = new char**[2];
	hands[0] = new char*[5];
	hands[1] = new char*[5];
	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 5; ++j)
			hands[i][j] = new char[2];

	while(1)
	{
		// Input
		for(int i = 0; i < 2; ++i)
			for(int j = 0; j < 5; ++j)
			{
				cin >> hands[i][j];

				// Terminate when end of input
				if(!cin)
				{
					// Release memory
					for(int p = 0; p < 2; ++p)
					{
						for(int k = 0; k < 5; ++k)
							delete[] hands[p][k];
						delete [] hands[p];
					}
					delete[] hands;
					return 0;
				}
			}

		// Sort cards by value
		sort(hands[0]);
		sort(hands[1]);
	
		// Compare
		int game_result = compare(hands[0], hands[1]);
		switch(game_result)
		{
			case 1: 	cout << "Black wins.\n"; break;
			case -1: 	cout << "White wins.\n"; break;
			case 0: 	cout << "Tie.\n"; 		break;
			default: ;
		}
	}
}

