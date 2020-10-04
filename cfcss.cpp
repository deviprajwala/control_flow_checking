#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<cstdlib>
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
  struct node *parent;
};
//node of the control flow graph which contains information required to check signature

stack<struct node*> s;
//to traverse the control flow graph in depth first search fashion

struct node* getnode(int num,struct node* par)
//function to get a new node whose information is set as per the requirement
{
 struct node* x=new struct node();
 x->val=num;
 x->parent=par;
 x->s=0;
 x->d=num;
 x->D=0;
}
void print(vector<vector<int> >matrix)
//function to print the elements of the matrix
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
int count(int num,vector<vector<int> > matrix,struct node* cur)
{
   int cnt=0;
   for(int i=1;i<matrix[0].size();i++)
   {
      if(matrix[num][i]==1)
      {
        struct node* x=getnode(i,cur);
        cur->link[cnt]=x;
        cnt++;
      }
   }
   return cnt;
}
void generate_graph(vector<vector<int> >matrix)
//to generates the control flow graph
{
  while(!s.empty())
  {
   struct node* cur=s.top();
   int idx=0,link_count=cur->links;
   s.pop();
   while(link_count!=0)
   {
     struct node* x=cur->link[idx];
     s.push(x);
     x->d=x->val;
     x->links=count(x->val,matrix,x);
     link_count-=1;
     idx++;
   }
  }
  return;
}
void diagraph(struct node *root)
{
  stack <struct node* >s;
  s.push(root);
  
  cout<<"digraph G{\n";
  while(!s.empty())
  {
   struct node* x=s.top();
   cout<<x->val<<"[ shape = record label=\""<<x->val<<"|s="<<x->s<<"|d="<<x->d<<"|g="<<x->g;
   cout<<"|D="<<x->D<<"\"]\n";
   s.pop();
   int id=0;
   while(x->links!=0)
   {
     s.push(x->link[id]);
     cout<<x->val<<"->"<<x->link[id]->val<<"\n";
     x->links-=1;
     id++;
   }
  }  
  cout<<"}";
}
void compute_signature(struct node *root)
{
  stack<struct node*>s;
  s.push(root);
  while(!s.empty())
  {
    struct node* x=s.top();
    int lcount=x->links;
    s.pop();
    if(x->s==0)
    {
      x->g=x->parent->g^x->d;
      x->s=x->g;
    }
    int idx=0;
    while(lcount!=0)
    {
      s.push(x->link[idx]);
      idx++;
      lcount--;
    }
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
 // print(matrix);
 //root node is set with the initial values required for the computation of conrol flow graph
  struct node* root= new struct node();
  root->val=1;
  root->s=1;
  root->d=1;
  root->g=1;
  root->D=0;
  root->parent=NULL;
  root->links=count(root->val,matrix,root);
 
  s.push(root);
  generate_graph(matrix);
/*  cout<<root->val<<"\n";
  cout<<root->link[0]->val<<" "<<root->link[1]->val<<"\n";
  cout<<root->link[0]->link[0]->val<<" "<<root->link[0]->link[1]->val<<root->link[1]->link[0]->val<<"\n";
  cout<<root->link[0]->link[0]->link[0]->val;*/
  compute_signature(root);
  diagraph(root); 
  
  return 0;
}
