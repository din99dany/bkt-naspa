#include <bits/stdc++.h> 
using namespace std; 

struct Item 
{ 
	float weight; 
	int value; 
}; 

struct BoundNode 
{ 
	int level, profit, bound; 
	float weight; 
}; 

bool cmp(Item a, Item b) 
{ 
	double r1 = (double)a.value / a.weight; 
	double r2 = (double)b.value / b.weight; 
	return r1 > r2; 
} 

int bound(BoundNode u, int n, int Weight, Item V[]) 
{ 
	if (u.weight >= Weight) 
    {
       return 0; 
    }

	int profit_bound = u.profit; 

	int j = u.level + 1; 
	int totweight = u.weight; 

	while ((j < n) && (totweight + V[j].weight <= Weight)) 
	{ 
		totweight += V[j].weight; 
		profit_bound += V[j].value; 
		j++; 
	} 

	if (j < n) 
    {
		profit_bound += (Weight - totweight) * V[j].value / V[j].weight; 
    }
    
	return profit_bound; 
} 

int knapsack(int Weight, Item V[], int n) 
{ 
	sort(V, V + n, cmp); 
    queue<BoundNode> Q; 
	
    BoundNode curr, next; 

	curr.level = -1; 
	curr.profit = curr.weight = 0; 
	Q.push(curr); 

	int maxProfit = 0; 
	while( !Q.empty() ) 
	{ 
		curr = Q.front(); 
		Q.pop(); 

		if (curr.level == -1) 
			next.level = 0; 

		if (curr.level == n-1) 
			continue; 

		next.level = curr.level + 1; 

		next.weight = curr.weight + V[next.level].weight; 
		next.profit = curr.profit + V[next.level].value; 

		if (next.weight <= Weight && next.profit > maxProfit) 
			maxProfit = next.profit; 

		next.bound = bound(next, n, Weight, V); 

		if (next.bound > maxProfit) 
			Q.push(next); 

		next.weight = curr.weight; 
		next.profit = curr.profit; 
		next.bound = bound(next, n, Weight, V); 

		if (next.bound > maxProfit) 
			Q.push(next); 
	} 

	return maxProfit; 
} 

int main() 
{   
    freopen( "datek.in", "r",stdin );

	int wieght, n;
    std:: cin >> wieght >> n;
    Item V[ n + 1];
 
    for ( int i = 0; i < n; ++i )
    {
        std::cin >> V[ i ].weight >> V[ i ].value;   
    }

	cout << knapsack(wieght, V, n) << "\n"; 

	return 0; 
} 
