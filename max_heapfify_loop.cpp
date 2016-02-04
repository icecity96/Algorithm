#include <iostream>

#define LEFT(i)		(2*(i)+1)
#define RIGHT(i)	(2*(i)+2)

using namespace std;

//var n means the size of array(heap)
void max_heapfify_loop(int A[],int i,int n)
{
	while((LEFT(i)<n&&A[i]<A[LEFT(i)])||(RIGHT(i)<n&&A[i]<A[RIGHT(i)]))
	{
		int largest=i;
		if(A[i]<A[LEFT(i)])
		{
			largest = LEFT(i);	
		}	
		if(A[largest]<A[RIGHT(i)])
		{
			largest = RIGHT(i);
		}
		swap(A[i],A[largest]);
		i = largest;
	}
}

int main(int argc, char **argv) {
	int A[14]={27, 17, 3, 16, 13, 10,1,5,7,12,4,8,9,0};
	max_heapfify_loop(A,2,14);
	for(int i = 0; i < 14; ++i)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	return 0;
}
