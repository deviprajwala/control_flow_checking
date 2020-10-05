#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<cstdlib>
using namespace std;

//to generate the accurate value of signatures for each of the nodes so as to check the control flow of the graphs
//program to find si,di,D for nodes of a control flow graph

struct node
{
  int val;
  int s;
  int g;
  int d;
  int D;
  int links;
  //number of child nodes
  struct node *link[10];
  //link to the child nodes
  struct node *parent;
  //link to the parent node
};
//node of the control flow graph which contains information required to check signature

stack<struct node*> s;
//to traverse the control flow graph in depth first search fashion

map<int,int>mp;
//to store parent and child

vector<pair<int,int> >diamond;
//to track nodes in which D should be changed


struct node* getnode(int num,struct node* cur);
//function to get a new node whose information is set as per the requirement
void print(vector<vector<int> >matrix);
//function to print the elements of the matrix
int count(int num,vector<vector<int> > matrix,struct node* cur);
//to count the number of child nodes and if there is branch fan in node then it is stored as pair in vector
void generate_graph(vector<vector<int> >matrix);
//to generates the control flow graph
void diagraph(struct node *root);
//to print the commands to a text file to generate a graph 
struct node* update_d(int val1,struct node* root);
//to update the value of D wherever necesssary 
struct node* compute_signature(struct node *root);
//to compute the values of s and d for each of the node 


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
//to count the number of child nodes and if there is branch fan in node then it is stored as pair in vector
{
   int cnt=0;
   for(int i=1;i<matrix[0].size();i++)
   {
      if(matrix[num][i]==1)
      {
        struct node* x=getnode(i,cur);
        //if there is a connection between node parent and its value is updated
        if(mp.find(i)!=mp.end())
        //if two nodes have same child, it is noted in the map named diamond
        {
          diamond.push_back(make_pair(mp[i],cur->val));
        }
        mp[i]=cur->val;
        //map value is added
        cur->link[cnt]=x;
        //link of the node is set to its child
        cnt++;
        //value of cnt is incremented
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
   //number of child nodes is stored in a variable
   s.pop();
   while(link_count!=0)
   {
     struct node* x=cur->link[idx];
     s.push(x);
     //child nodes are pushed into the stack

     x->d=x->val;
     x->links=count(x->val,matrix,x);
     //child nodes of x is calculated and it is updated with its parent

     link_count-=1;
     //link count is decremented by one

     idx++;
     //index value is incremented by one
   }
  }
  return;
}
void diagraph(struct node *root)
//to print the commands to a text file to generate a graph 
{
  stack <struct node* >s;
  s.push(root);
  map<int,int>check;
  cout<<"digraph G{\n";
  while(!s.empty())
  {
   struct node* x=s.top();
   cout<<x->val<<"[ shape = record label=\""<<x->val<<"|s="<<x->s<<"|d="<<x->d<<"|g="<<x->g;
  //to print the record values of a node
   cout<<"|D="<<x->D<<"\"]\n";
   s.pop();
   int id=0;
   while(x->links!=0)
   {
     s.push(x->link[id]);
     //child nodes are pushed into stack
     if(check.find(x->val)==check.end()||check[x->val]!=x->link[id]->val)
     //to avoid the multiple copies of same node relations with the help of map
     {
       check[x->val]=x->link[id]->val;
       //values are updated in the map
       cout<<x->val<<"->"<<x->link[id]->val<<"\n";
     }
     x->links-=1;
     //link value decremented by one
     id++;
     //id value incremented by one

   }
  }  
  cout<<"}";
}

struct node* update_d(int val1,struct node* root)
//to update the value of D wherever necesssary,while having one node present in diamond pattern 
{
  int value;
  for(int i=0;i<diamond.size();i++)
  {
    if(diamond[i].second==val1)   
    {
      value=diamond[i].first;
      //value of the other node is found
    }
  }
 //to find the node with value val

 s.push(root);
 //seach the node whose val is value
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
  // the node with value is not found then null is returned
  return NULL; 
}

struct node* compute_signature(struct node *root)
//to compute the values of s and d for each of the node 
{
  stack<struct node*>s;
  vector<int>v(100);
  s.push(root);
  while(!s.empty())
  //depth first traversal is used to visit each of the node and calculate the signature
  {
    struct node* x=s.top();
    int lcount=x->links;
    v.push_back(x->val);
    //visited node is noted

    s.pop();
    if(x->s==0)
    {
      x->g=x->parent->g^x->d;
      //gi=g(prev)^di
      x->s=x->g;
      //si=gi
    }
    int idx=lcount-1;
    while(lcount!=0)
    {
      if(find(v.begin(),v.end(),(x->link[idx]->val))!=v.end())
      {
        //if a visited node is pushed to stack again then there must be a cycle
        struct node*y=update_d(x->val,root);
        //node is obtained in which D has to be updated
        if(y!=NULL)
        {
         y->D=0;
         //y D=0000;

         x->D=x->s^y->s;
         //x D=s(prev1)^s(prev2)

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
    //nodes which are linked are read

    if(node1==node2)continue;
    //if self loop is found 
 
    if(node1>node2) continue;
    //if back edges are found

    matrix[node1][node2]=1;
    //matrix is generated based on connected components
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
  //function is called to generate a graph

  root=compute_signature(root);
  //signature for each of the node is calculated

  diagraph(root); 
  //function is called to print the commands needed to generate graph

  return 0;
}
