// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void branchAndBound(knapsack &k, int t);
void branchRecursive(knapsack &bestSol, stack<knapsack> &branches);
boolean isFathomed(knapsack &k, knapsack &bestSol);


int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   
   // Read the name of the file from the keyboard or
   // hard code it here for testing.
   
   // fileName = "knapsack16.input";

   cout << "Enter filename" << endl;
   cin >> fileName;
   
   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      cout << "Reading knapsack instance" << endl;
      knapsack k(fin);

      branchAndBound(k, 600);

      cout << endl << "Best solution" << endl;
      k.printSolution();
      
   }    

   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
}

void branchAndBound(knapsack &k, int t) {
	knapsack bestSol = knapsack(k);
	stack<knapsack> branches;

	branches.push(k);
	branchRecursive(k, bestSol, branches);

}

void branchRecursive(knapsack &bestSol, stack<knapsack> &branches) {
	for (int 0; i <= 1; i++) {
		knapsack temp = knapsack(branches.top());
		temp.setLevel = branches.top().getLevel() + 1;
		if (i == 1) {
			temp.select(temp.getLevel());
		}
		branches.push(temp);
		if (!isFathomed(branches.top(), bestSol)) {
			branchRecursive(bestSol, branches);
		}
		else {
			branches.pop();
		}
	}
	branches.pop();
	return;
}

boolean isFathomed(knapsack &k, knapsack &bestSol) {
	if (k.getCost() > k.getCostLimit()) {
		return true;
	}
	else if (k.getCost() == k.getCostLimit()) {
		if (k.getValue() > bestSol.getValue()) {
			bestSol = knapsack(k);
		}
		return true;
	}
	else if (k.bound() <= bestSol.getValue()) {
		return true;
	}
	else if (level == k.getNumObjects()) {
		if (k.getValue() > bestSol.getValue()) {
			bestSol = knapsack(k);
		}
		return true;
	}
	return false;
}