//O(nlg(n))=============inversion
#include <iostream>
#include <limits.h>
using namespace std;

int number = 0;
//This function will cost O(n) time
void m_merge(int A[],int p,int q,int r1)
{
	int n1 = q - p + 1;
	int n2 = r1 - q;
	int L[n1+1];
	int R[n2+1];
	for(int i = 0; i < n1; ++i)
	{
		L[i] = A[p+i];
	}
	for(int i = 0; i < n2; ++i)
	{
		R[i] = A[q+i+1];
	}
	L[n1] = INT_MAX;
	R[n2] = INT_MAX;
	int l = 0;
	int r = 0;
	for(int i = p; i <= r1; ++i)
	{
		if(L[l]<=R[r])
		{
			A[i] = L[l++];
		}	
		else
		{
			A[i] = R[r++];	
			number += n1 - l;
		}
	}
}

void merge_sort(int A[],int p,int r)
{
	if(p < r)
	{
		int q = (p+r)/2;
		merge_sort(A,p,q);
		merge_sort(A,q+1,r);
		m_merge(A,p,q,r);
	}
}

int main(int argc, char **argv) {
	int s[]={2, 3, 8, 6, 1};
	merge_sort(s,0,4);
	for(int i = 0; i < 5; ++i)
	{
		cout << s[i] << " ";
	}
	cout << number <<endl;
	return 0;
}
