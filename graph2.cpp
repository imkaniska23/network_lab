#include <bits/stdc++.h>
using namespace std;
void dfs(int i,int visited[], vector <int> lis[], int &count)
{
	visited[i]=1;
	for(int j=0;j<lis[i].size();j++)
	{
		if(visited[lis[i][j]]!=0)
			count++;
		if(visited[lis[i][j]]==0)
			dfs(lis[i][j],visited,lis,count);
	}
	visited[i]=2;
}
void printPathUtil(int s, int d, vector <int> lis[], int visited[], int path[],int &path_index)
{
	vector <int>::iterator i;
	visited[s]++;
	path[path_index]=s;
	path_index++;
	if(s==d)
	{
		for(int i=0;i<path_index;i++)
			cout<<path[i]<<" ";
		cout<<endl;
	}
	else
	{
		for(i=lis[s].begin();i!=lis[s].end();i++)
		{
			if(visited[*i]<=1)
				printPathUtil(*i,d,lis,visited,path,path_index);
		}
	}
	visited[s]=0;
	path_index--;
}
void printPath(int s, int d, int v, vector <int> lis[])
{
	int *visited = new int[v];
	int *path = new int[v];
	int path_index=0;
	for(int i=0;i<v;i++)
		visited[i]=0;
	printPathUtil(s,d,lis,visited,path,path_index);
}
int main()
{
	int v,e;
	cout<<"Enter number of nodes and edges:";
	cin>>v>>e;
	vector <int> lis[v];
	cout<<"Enter the edges:"<<endl;
	for(int i=0;i<e;i++)
	{
		int p,q;
		cin>>p>>q;
		lis[p].push_back(q);
	}
	cout<<"The adjacency list is:"<<endl;
	for(int i=0;i<v;i++)
	{	cout<<i;
		for(int j=0;j<lis[i].size();j++)
			cout<<"->"<<lis[i][j];
		cout<<endl;
	}
	int prc=0;
	cout<<"The predicate nodes are:"<<endl;
	for(int i=0;i<v;i++)
		if(lis[i].size()>=2)
		{
			cout<<i<<" ";
			prc++;
		}
	cout<<endl;
	cout<<"Total number of predicate nodes is "<<prc<<endl;
	int s;
	cout<<"All the paths"<<endl;
	printPath(0,(v-1),v,lis);
	cout<<"Number of bounded regions "<<endl;
	int visited[v]={0},count=1;
	for(int i=0;i<v;i++)
	{
		if(visited[i]==0 && lis[i].size())
		{
			dfs(i,visited,lis,count);
		}
	}
	cout<<count<<endl;
}