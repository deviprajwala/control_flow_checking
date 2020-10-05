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

map<int,int>mp;
//to store parent and child

vector<pair<int,int> >diamond;
//to track nodes in which D should be changed

struct node* getnode(int num,struct node* cur)
//function to get a new node whose information is set as per the requirement
{
 struct node* x=new struct node();
 x->val=num;
 x->s=0;
 x->parent=cur;
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
        if(mp.find(i)!=mp.end())
        {
          diamond.push_back(make_pair(mp[i],cur->val));
        }
        mp[i]=cur->val;
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
struct node* update_d(int val1,struct node* root)
{
  int value;
  for(int i=0;i<diamond.size();i++)
  {
    if(diamond[i].second==val1)
    {
      value=diamond[i].first;
    }
  }
 //to find the node with value val
 s.push(root);
 while(!s.empty())
  {
   struct node* x=s.top();
   if(x->val==value)
   {
    return x;
   }
   int lcount=x->links;
   s.pop();
   int id=0;
   while(lcount!=0)
   {
     s.push(x->link[id]);
     lcount-=1;
     id++;
   }
  } 
  return NULL; 
}
struct node* compute_signature(struct node *root)
{
  stack<struct node*>s;
  vector<int>v(100);
  s.push(root);
  while(!s.empty())
  {
    struct node* x=s.top();
    int lcount=x->links;
    v.push_back(x->val);
    s.pop();
    if(x->s==0)
    {
      x->g=x->parent->g^x->d;
      x->s=x->g;
    }
    int idx=lcount-1;
    while(lcount!=0)
    {
      if(find(v.begin(),v.end(),(x->link[idx]->val))!=v.end())
      {
        //if a visited node is pushed to stack again then there must be a cycle
        struct node*y=update_d(x->val,root);
        if(y!=NULL)
        {
         y->D=0;
         x->D=x->s^y->s;
         x->link[idx]->g=x->link[idx]->g^y->D;
        }
      }
      s.push(x->link[idx]);
      idx--;
      lcount--;
    }
  }
  return root;
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
    if(node1==node2)continue;
    if(node1>node2) continue;
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
  root=compute_signature(root);
  diagraph(root); 
 
  return 0;
}
