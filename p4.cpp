// Project 4: Branch and bound algorithm
// Created by Patrick Hanbury and Cassandra Smith
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <deque>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void branchAndBound(knapsack &k, int t);
void branchRecursive(knapsack &bestSol, deque<knapsack> &branches, vector<int> items, clock_t startT, int t);
bool isFathomed(knapsack &k, knapsack &bestSol, vector<int> items);
void orderKnapsack(knapsack &k, vector<int> &items);

int main()
{
   char x;
   ifstream fin;
   //stack <int> moves;
   string fileName;
   
   // Read the name of the file from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack28.input";

   //cout << "Enter filename" << endl;
   //cin >> fileName;
   
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
	  
	  //system("pause");

      branchAndBound(k, 600);

      cout << endl << "Best solution" << endl;
      k.printSolution("knapsack28.output");
	  system("pause");

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

//Branch and bound algorithm for solving knapsack
void branchAndBound(knapsack &k, int t) {
	clock_t startTime = clock();
	deque<knapsack> branches;
	knapsack bestSol = knapsack(k);
	vector<int> items;

	items.resize(k.getNumObjects());
	for (int i = 0; i < k.getNumObjects(); i++) {
		items[i] = i;
	}
	orderKnapsack(k, items);

	branches.push_back(k);
	branchRecursive(bestSol, branches, items, startTime, t);

	k = knapsack(bestSol);
}

//Creates and expands branches recursively
void branchRecursive(knapsack &bestSol, deque<knapsack> &branches, vector<int> items, clock_t startT, int t) {
	for (int i = 0; i <= 1; i++) {
		if (((float)(clock() - startT) / CLOCKS_PER_SEC) >= t) {
			return;
		}

		knapsack temp = knapsack(branches.back());
		temp.setLevel(branches.back().getLevel() + 1);

		if (i == 1) {
			temp.select(items[temp.getLevel() - 1]);
		}
		branches.push_back(temp);

		if (!isFathomed(branches.back(), bestSol, items)) {
			branchRecursive(bestSol, branches, items, startT, t);
		}
		else {
			branches.pop_back();		
		}
	}
	branches.pop_back();
	return;
}

//Function to check if a branch is fathomed or not
bool isFathomed(knapsack &k, knapsack &bestSol, vector<int> items) {
	if (k.getCost() > k.getCostLimit()) {
		return true;
	}
	else if (k.getCost() == k.getCostLimit()) {
		if (k.getValue() > bestSol.getValue()) {
			bestSol = knapsack(k);
		}
		return true;
	}
	else if (k.getLevel() == (k.getNumObjects())) {
		if (k.getValue() > bestSol.getValue()) {
			bestSol = knapsack(k);
		}
		return true;
	}
	else if (k.bound(items) <= bestSol.getValue()) {
		return true;
	}
	return false;
}

//Orders the items in a knapsack by increasing ratio value
void orderKnapsack(knapsack &k, vector<int> &items) {
	int temp, j;

	for (int i = 1; i < k.getNumObjects(); i++) {
		temp = items[i];
		j = i - 1;

		while (j >= 0 && k.getRatio(items[j]) < k.getRatio(temp)) {
			items[j + 1] = items[j];
			j = j - 1;
		}
		items[j + 1] = temp;
	}
}