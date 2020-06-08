#include <iostream>
using std::cin;
using std::cout;

// Selection sort
void sort(int* arr, int size)
{
	for(int i = 0; i < size; ++i)
	{
		// Find minimum
		int min = i;
		for(int j = i + 1; j < size; ++j){ if(arr[j] < arr[min]) min = j; }

		// Swap
		int temp = arr[min];
		arr[min] = arr[i];
		arr[i] = temp;
	}
}

int main()
{
	// Read number of test cases
	int t_num;
	cin >> t_num;

	for(int i = 0; i < t_num; ++i)
	{
		// Read number of relatives
		int r_num;
		cin >> r_num;

		// Read street numbers
		int *st = new int[r_num];
		for(int j = 0; j < r_num; ++j) cin >> st[j];

		// Sort
		sort(st, r_num);

		// Get sum
		int mid = st[r_num / 2];
		int sum = 0;
		for(int j = 0; j < r_num; ++j)
			sum += ( mid < st[j] ? st[j] - mid : mid - st[j]);

		// Print result
		cout << sum << "\n";
		delete[] st;
	}
	return 0;
}
