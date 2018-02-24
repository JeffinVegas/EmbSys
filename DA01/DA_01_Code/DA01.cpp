#include <iostream>
#include <cmath>
using namespace std;

int main()
{
  int rem, fivecount = 0, notfivecount = 0;
  int fivesum = 0, notfivesum = 0;
  int array[300];

  //stores 300 numbers
  array[0] = 36; //FIRST ELEMENT (0x22 + 0x02)
  for(int i = 1 ; i < 300 ; i++)
    {
      if(array[i-1] == 254)
	  array[i] = 0;
      else
	array[i] = array[i-1] + 2;
    }

  //check if divisible by 5
  for(int j = 0 ; j < 300 ; j++) //TRAVERSE
    {
      rem = array[j] % 5;
      if(rem == 0 && array[j] != 0) //IF DIVISIBLE BY 5
	{
	  fivesum += array[j];
	  fivecount++;
	}
      else //IF NOT DIVISIBLE BY 5
	{
	  notfivesum += array[j];
	  notfivecount++;
	}
    }

  //print results
  cout << "#s divisible by 5: " << fivecount << endl;
  cout << "The result for numbers div. by 5: ";
  cout << fivesum << endl;

  cout << "#s not divisible by 5: " << notfivecount << endl;
  cout << "The result for numbers not div. by 5: ";
  cout << notfivesum <<endl;
}
