#include<iostream>
#include<bits/stdc++.h>
using namespace std;


//program to find si,di,D of a control flow graph

struct node
{
  int val;
  int s;
  int g;
  int d;
  int D;
  int links;
  struct node *link[10];
};
typedef struct node* Node;
//node of the control flow graph which contains information required to check signature


void print(vector<vector<int> >matrix)
{
  for(int i=1;i<matrix.size();i++)
  {
    for(int j=1;j<matrix[0].size();j++)
    {
      cout<<matrix[i][j]<<" ";
    }
    cout<<"\n";
  }
}
int main()
{
  int i,j,n;
  //cout<<"Enter the number of connections";
  cin>>n;
  //to get the number of entries in the file

  vector< vector < int > >matrix(n+1,vector<int> (n+1,0));
//matrix to store the connected components

  //to get the input from the user in the form of matrix based on how the nodes are connected to each other
  while(n--) 
  {
    int node1,node2;
    cin>>node1>>node2;

    matrix[node1][node2]=1;
  }
  print(matrix);
  return 0;
}
