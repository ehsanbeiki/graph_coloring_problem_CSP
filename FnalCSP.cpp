#include <iostream>
#include <vector>
#include <list>
#include <bits/stdc++.h>
#include <queue>
#include <iomanip> 

//		Ehsan Beiki,M.Sadegh Daghigh
//		*Hint : For "Complete Information" about this Code please visit "Documentation" pdf file in the archive ... tnx!
//
using namespace std; 

struct arc
{
	int xi;          // an arc_type structure with data components Xi and Xj for the arc Xi--->Xj 
	int xj;
};                    

int n,c;                        //two variables to store the number of variables (cities or provinces) and the number of colors
vector<string> Color;           //a vector to store the names of the colors 
vector<bool> Selection;         //a vector to store the selection status of variables
vector<int>ValuesLimit;         //a vector to store the remaining number of values from the domain of variables


bool RecursiveBacktracking(int assignment[],bool **W,vector < list <int> > Domain);  //RecursiveBacktracking function
int  SelectUnassignedVariable(int assignment[],bool **);  //Select from UnassignedVariables 
bool RemoveInconsistent(arc x,vector < list <int> > Domain); //remove inconsistent values from the domain of variables
void ArcConsistency(bool **W,vector < list <int> > OriginalDomain); // arc consistency check function
void MRVChecking(bool signal,int var,bool **W); //  calculate values for MRV heuristic  function
bool Consistent(int var,int value,int assignment[],bool **W); //checking the consistency of the value given to a variable with other variables
bool BacktrackingSearch(bool **W,int Solotion[],vector < list <int> > Domain); //BacktrackingSearch function
int  MinRV(); //MRV returns min value for heuristic implementation 



bool RemoveInconsistent(arc x,vector < list <int> > Domain)
{
	bool removed=false,condition;
	
	for (list<int>::iterator xit=Domain[x.xi].begin() ; xit!=Domain[x.xi].end() ; xit++)
	{	
		for (list<int>::iterator xjt=Domain[x.xj].begin() ; xjt!=Domain[x.xj].end() ; xjt++)
			if (*xit!=*xjt)
			{
				condition=0;
				break;	
			}
			else                                         // This function removes inconsistent values from the domain of variables
			{
				condition=1;	
			}
			
		if(condition)
		{
			    Domain[x.xi].remove(*xit);	
				removed=true;	
		}
	}

	return removed;	
}

void ArcConsistency(bool **W,vector < list <int> > OriginalDomain)
{	
	queue<arc> arcs_queue;
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)	
		if(W[i][j])
		{	arc XIJ;                                                    // This function checks arc consistency 
			XIJ.xi=i;
			XIJ.xj=j;
			arcs_queue.push(XIJ);	
		}	

	vector < list <int> > Domain;
	Domain.resize(n);	
	
	for(int i=0;i<n;i++)
		for (list<int>::iterator it=OriginalDomain[i].begin() ; it!=OriginalDomain[i].end() ; it++)
			Domain[i].push_back(*it) ;		
			
	while(arcs_queue.size()>0)
	{
		arc x=arcs_queue.front();
		arcs_queue.pop();
		if(RemoveInconsistent(x,Domain))
		{
			for(int k=0;k<n;k++)
			{
				if(W[k][x.xi])	
				{
					arc NewX ;
					NewX.xi=k;
					NewX.xj=x.xi;	
					arcs_queue.push(NewX);	
				}	
			}
		}		
	}
	
}
void MRVChecking(bool signal,int var,bool **W)
{
	if(!signal)
	{
	for(int j=0;j<n;j++)
	{
		if (W[var][j]==1)
		{
			--ValuesLimit[j];
		}	
		if(ValuesLimit[j]<0)
			ValuesLimit[j]=0;		                         // This function calculates values for MRV heuristic 
	}
	
	}
	if(signal)
	{
	for(int j=0;j<n;j++)
	{
		if (W[var][j]==1)
		{
			++ValuesLimit[j];
		}			
	}
	}
}


bool Consistent(int var,int value,int assignment[],bool **W)
{
	for (int i=0;i<n;i++)
	{
		if(W[var][i]==1 && assignment[i]==value)        // This function checks the consistency of the value given to -
														//								a variable with other variables
			return false;		
	}

return true;		
}                                                                   

bool BacktrackingSearch(bool **W,int Solotion[],vector < list <int> > Domain)
{	

		for(int i=0;i<n;i++)
	{
		for(int j = 0; j <c; j++)
			Domain[i].push_back(j) ;		
	}
	
	ArcConsistency(W,Domain);                     //BacktrackingSearch function
                                                           
	int assignment[n];
	for (int i=0;i<n;i++)
		assignment[i]=99;
	bool result=false;		
	result = RecursiveBacktracking(assignment,W,Domain);
	for (int i=0;i<n;i++)
		Solotion[i]=assignment[i];
		
	return result;
}	

	
bool RecursiveBacktracking(int assignment[],bool **W,vector < list <int> > Domain)
{
		//	ArcConsistency(W,Domain);

	bool Complete=1;
	for(int i=0;i<n;i++)
	{                                                      // RecursiveBacktracking function
	if(Selection[i]==0)
	{
		Complete=0;
		break;		
	}

	}
	if (Complete==1)
		return true;
	int var = SelectUnassignedVariable(assignment,W);	

	int result=true;
	for (list<int>::iterator it=Domain[var].begin() ; it!=Domain[var].end() ; it++)
	{

		int value= *it;

		
		if (Consistent(var,value,assignment,W))
		{

			assignment[var]=value;
			MRVChecking(0,var,W);
			

			result =RecursiveBacktracking(assignment,W,Domain);
			
			if (result != false) 
				return result;				
			else
			{
			MRVChecking(1,var,W);
			for(int i=var+1;i<n;i++)
				Selection[i]=0;
			}	
		}
	}	
	
return false;	
}

int MinRV()
{
	int min=0;
	for(int i=0;i<n;i++)
		{
			if(ValuesLimit[i]<ValuesLimit[min])           // This function returns minimum value for MRV heuristic 
				min=i;
		}
	return min;
}

int SelectUnassignedVariable(int assignment[],bool **W)
{	
	int min;

	for(int i=0;i<n;i++)
	{
		min=MinRV();
	if(Selection[min]==0)
		{bool Condition=false;
			for(int c=0;c<n;c++)
			{
			if(ValuesLimit[min]==ValuesLimit[c])                // This function Selects a variable from UnassignedVariables 
				{
				Condition=true;	
				break;				
				}	
			}

			if(Condition)	
			{
				int degree[n];
				for(int j=0;j<n;j++)
				{ 
				degree[j]=0;
					for(int k=0;k<n;k++)
						if (W[j][k]==1)
							++degree[j]; 		
			    }								//check for implementation of Degree heuristic
			    int max=0;
				for(int d=0;d<n;d++)
				{
					if(degree[max]<degree[d])
					max=d;
				}
				if(Selection[max]==0)
				{
				Selection[max]=1;
				return max;					
				}
			}
		Selection[min]=1;
		return min;  				
		}		
		else if( Selection[i]==0)		//MRV heuristic (returns the element with minimum Remaining)
		{	

			Selection[i]=1;
			return i;  				
		}
	}
}

int main() 
{
	vector < list <int> > Domain;	// a vector of linked list data type to store the values of the domain of variables

	cout <<"Plz Enter The Number of Color: ";
	cin >> c;	                                // Get the number of colors
	
	Color.resize(c);	
	cout <<"Plz Enter The Name of Colors: ";
	for(int i = 0; i <c; i++)
		cin >> Color[i];						// Get the names of the colors

	
	cout <<"Plz Enter The Number of Variables: ";     // Get the number of variables
	cin >> n;
	Domain.resize(n);
	ValuesLimit.reserve(n);
	for(int i=0;i<n;i++)
		ValuesLimit[i]=c;                         // Initialize all ValuesLimit elements with the number of colors

	Selection.resize(n);
	for(int i=0;i<n;i++)
		Selection[i]=0;                          // Initialize all Selection elements with 0
	
		
	bool **W;
	W = new bool *[n];
	for(int i = 0; i <n; i++)                   // A two-dimensional array as a csp problem
		W[i] = new bool[n];	                    
	
	for(int i=0;i<n;i++)
		for (int j=0;j<n;j++)                  // Initialize all elements of array w wirh 0
			W[i][j]=0;		
	
	string Variables[n];
	cout <<"\nPlz Enter The Name Of Variables: ";
	for (int i=0;i<n;i++)                             // Get variable names
		cin >> Variables[i];                             
	
	cout <<"\nThe Variables Are: \n";
	for (int i=0;i<n;i++)
		cout << Variables[i]<<"="<<i<<"    ";         // Display variables
	cout <<"\n\n";	
	for(int i=0;i<n;i++)
	{
		cout<<"\nPlz Enter The Neighbors Of Variable \"" << Variables[i] <<"(="<<i<<")"<< "\"(Zero Or one) :";	
		for (int j=0;j<n;j++)
			{
				cin >> W[i][j];		            // Get neighbors of all variables
			}
	}
			
	for(int i=0;i<n;i++)
	{
		cout<<"\nThe Neighbors Of Variable \"" << Variables[i] <<"\"(="<<i<<")"<< "\""<<setw(5)<<"\tis :"<<setw(10);	
		for (int j=0;j<n;j++)
		{
			if(W[i][j])	
			cout << Variables[j]<<setw(5);            // Display neighbors of all variables
		}	
	}
	
	int Solotion[n];
			
	if(BacktrackingSearch(W,Solotion,Domain))
	{
		cout <<"\n\n************************************************\n";
		cout <<"\n\nThe Solution is : \n";
		for (int i=0;i<n;i++)
		cout <<"\n"<< Variables[i]<<":"<<Color[Solotion[i]]<<"    ";			 // Display the final solution
	}
	else
		cout <<"\n\nNo Solution Found!\n";
		
	return 0;
}
