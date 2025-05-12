
#include <iostream>
#include <VectorClass.h>


using namespace std;

int main()
{
  TVector<int> A(4);
  A[0] = 1;
  A[1] = 123;
  A[2] = 76;
  A[3] = 3;
  A.SortQuick();

  cout << A[0] << " " << A[1] << " " << A[2]<< " " << A[3] << endl;
  cout << A.FirstNorm();
  return 0;
}