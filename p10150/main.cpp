#include <iostream>
#include <string>
#include <vector>

#define MAX_SIZE 26000

using std::cin;
using std::cout;
using std::string;
using std::vector;

#define MAX_LENGTH 16

vector<string> *dict;

struct Queue
{
	int element[MAX_SIZE];
	int front;
	int rear;
};

// Tell if queue is empty
bool isEmpty(struct Queue &queue)
{
	if(queue.front == -1 && queue.rear == -1) return true;
	else return false;
}

// Enqueue
void enqueue(struct Queue &queue, int &index, int size)
{	
	if(isEmpty(queue))
	{
		queue.front = 0;
		queue.rear = 0;
		queue.element[queue.rear] = index;
	}
	else
	{
		queue.rear = (queue.rear + 1) % size;
		queue.element[queue.rear] = index;
	}
}

// Dequeue
int dequeue(struct Queue &queue, int size)
{
	bool empty = false;
	int result = queue.element[queue.front];
	if(queue.front == queue.rear) empty = true;
	queue.front = (queue.front + 1) % size;
	if(empty)
	{
		queue.front = -1;
		queue.rear = -1;
	}	return result;
}

// Doublet recognition
bool isDoublet(const string &s1, const string &s2)
{
	// If length of two aren't same, not doublet
	if(s1.length() != s2.length()) return false;

	// Compute amount of different words
	int diff = 0;
	for(int i = 0; i < s1.length(); ++i)
	{
		if(s1[i] != s2[i]) diff++;
		if(diff > 1) return false;
	}
	return diff == 1;
}

int main()
{

	// Initialize
	dict = new vector<string>[MAX_LENGTH];
	
	// Input data
	string input;
	while(getline(cin, input))
	{
		// If read an empty line, stop reading
		int len = input.length();
		if(len == 0) break;
		
		// Store data
		dict[len-1].push_back(input);
	}

	
	// Solving
	bool first_prob = true;
	string first, last;
	while(cin >> first >> last)
	{
		if(!first_prob) puts("");
		else first_prob = false;

		if(!cin)
		{
			delete [] dict;
			return 0;
		}

		int parent = -1;

		// If not same length
		if(first.length() != last.length())
		{
			puts("No solution.");
			continue;
		}

		int index = first.length() - 1;
		int size = dict[index].size();

		// Get adjacent matrix
		bool **adjacentMatrix = new bool*[size];
		for(int i = 0; i < size; ++i)
			adjacentMatrix[i] = new bool[size];

		for(int i = 0; i < size; ++i)
			for(int j = 0; j < size; ++j)
				adjacentMatrix[i][j] = false;

		for(int i = 0; i < size; ++i)
			for(int j = i + 1; j < size; ++j)
				adjacentMatrix[i][j] = isDoublet(dict[index][i], dict[index][j]);	

		// If not solved yet, build BFS
		// Initialize
		int *predecessor = new int[size];
		bool *color = new bool[size];
		struct Queue queue;
		queue.front = -1;
		queue.rear = -1;
		bool appeared = false;
		for(int i = 0; i < size; ++i)
		{
			if(last.compare(dict[index][i]) == 0 && !appeared)
			{	
				enqueue(queue, i, size);
				color[i] = true;
				appeared = true;
			}
			else
				color[i] = false;

			predecessor[i] = -1;
		}

		// Looping until front of queue is the last word
		bool found = false;
		while(!isEmpty(queue))
		{
			int current = dequeue(queue, size);	
			color[current] = true;
			for(int i = 0; i < size; ++i)
			{
				// Not traveled yet
				if(!color[i])
				{
					// Is doublet
					if(adjacentMatrix[current >= i ? i : current][current >= i ? current : i])
					{
						enqueue(queue, i, size);
						color[i] = true;
						predecessor[i] = current;
						if(first.compare(dict[index][i]) == 0)
						{
							parent = i;
							found = true;
							break;
						}
					}
				}
			}
			if(found) break;
		}

		if(found)
		{
			while(parent != -1)
			{
				cout << dict[index][parent] << "\n";
				parent = predecessor[parent];
			}
		}
		else
			puts("No solution.");
	
		delete[] predecessor;
		delete[] color;
	}
}
