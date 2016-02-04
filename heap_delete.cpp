#include "max_heapfify_loop.cpp"

//O(lgn)
void heap_delete(int A[],int i,int n)
{
	A[i] = A[n-1];
	max_heapfify_loop(A,i,n-1);
}

#ifdef TEST_
int main(int argc, char **argv) {
	int A[] = { 27,16,13,10,9,5,7,4,8,1,0};
	heap_delete(A,1,11);
	for(int i = 0; i<10;++i)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	return 0;
}
#endif
