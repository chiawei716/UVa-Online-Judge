#include <iostream>

int main()
{
	// Input two numbers
	int i, j;
	while(std::cin >> i >> j){
		
		int max_count = 0;
		int min, max, process, count, temp;

		// Decide min & max
		if(i > j)
		{
			min = j;
			max = i;
		}
		else
		{
			min = i;
			max = j;
		}
		
		// Run all numbers in the interval
		for(process = min; process <= max; ++process)
		{
			temp = process;
			count = 0;
			while(true)
			{
				count++;
			
				if(temp == 1)
				{
					if(count > max_count) max_count = count;
					break;
				}
				else
				{
					if((temp & 1) == 1)
						temp = 3 * temp + 1;
					else
						temp >>= 1;
				}
			}
		}

		std::cout << i << " " << j << " " << max_count << "\n";
	}

	return 0;
}
