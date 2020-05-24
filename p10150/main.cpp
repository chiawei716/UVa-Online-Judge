#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;

#define MAX_SIZE 25500
#define MAX_LENGTH 16

string **dict;
int dict_size[MAX_LENGTH];

string first, last;

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
	int result = queue.element[queue.front];
	queue.front = (queue.front + 1) % size;
	if(queue.front == queue.rear)
	{
		queue.front = -1;
		queue.rear = -1;
	}
	return result;
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
	if(diff == 1) return true;
	else return false;
}

int main()
{

	// Initialize
	for(int i = 0; i < MAX_LENGTH; ++i)
	{
		dict_size[i] = 0;
	}
	dict = new string*[MAX_LENGTH];
	for(int i = 0; i < MAX_LENGTH; ++i)
		dict[i] = new string[MAX_SIZE];



	// Input data
	string input;
	while(getline(cin, input))
	{
		// If read an empty line, stop reading
		int len = input.length();
		if(len == 0) break;
		
		// Store data
		dict[len-1][dict_size[len-1]] = input;
		dict_size[len-1]++;
	}

	// Solving
	while(cin >> first >> last)
	{
		if(!cin)
		{
			for(int i = 0; i < MAX_LENGTH; ++i)
				delete [] dict[i];
			delete [] dict;
			return 0;
		}

		int parent = -1;

		// If not same length
		if(first.length() != last.length())
		{
			cout << "No solution.\n\n";
			continue;
		}

		int index = first.length() - 1;

		// Get adjacent matrix
		bool adjacentMatrix[MAX_SIZE][MAX_SIZE];
		for(int i = 0; i < dict_size[index]; ++i)
			for(int j = 0; j < dict_size[index]; ++j)
				adjacentMatrix[i][j] = false;

		for(int i = 0; i < dict_size[index]; ++i)
			for(int j = i + 1; j < dict_size[index]; ++j)
				adjacentMatrix[i][j] = isDoublet(dict[index][i], dict[index][j]);

			
			/*for(int i = 0; i < dict_size[index]; ++i)
			{
				for(int j = 0; j < dict_size[index]; ++j)
					cout << &(adjacentMatrix[index][i][j]) << " ";
				cout << "\n";
			}*/
			

		// If not solved yet, build BFS
		// Initialize		
		int distance[MAX_SIZE];
		int predecessor[MAX_SIZE];
		bool color[MAX_SIZE];
		struct Queue queue;
		for(int i = 0; i < dict_size[index]; ++i)
		{
			if(last.compare(dict[index][i]) == 0)
			{
				distance[i] = 0;
				queue.element[0] = i;
				color[i] = true;
			}
			else
			{
				distance[i] = MAX_SIZE;
				color[i] = false;
			}
			predecessor[i] = -1;
		}
		queue.front = 0;
		queue.rear = 0;	

		// Looping until front of queue is the last word
		bool found = false;
		while(!isEmpty(queue))
		{
			int current = dequeue(queue, dict_size[index]);	
			color[current] = true;
			for(int i = 0; i < dict_size[index]; ++i)
			{
				// Not traveled yet
				if(!color[i])
				{
					// Is doublet
					if(adjacentMatrix[current >= i ? i : current][current >= i ? current : i])
					{
						//cout << dict[index][current] << ":" << dict[index][i] << "\n";
						enqueue(queue, i, dict_size[index]);
						color[i] = true;
						predecessor[i] = current;
						distance[i] = distance[current] + 1;
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
			cout << "\n";
		}
		else
			cout << "No solution.\n\n";
		

	}
}
