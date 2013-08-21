#include <iostream>
#include <queue>
#include <set>
#include "Node.h"
#include "Edge.h"
#include <fstream>
#include <stdio.h>
#include <set>
#define INF 999999999

using namespace std;

long totalNode;
long totalEdge;
set<long> rootSet;	//record all the root

Node * allNode;
priority_queue<Edge> edgeQueue;	//edge queue;

void Kruskal();
long Find(long);
void Union(long, long, long);
void Ending(){
	delete []allNode;
}

int main(){
	FILE *fp;
	fp = fopen("facebook-hour.txt", "r");
	fscanf(fp, "%ld%ld", &totalNode, &totalEdge);
	
	allNode = new Node[totalNode+1];	//please pay attention to +1 here
	for(long i=1; i<totalNode+1; i++){
		allNode[i].label = i;
		allNode[i].father = i;
		allNode[i].dist = INF;
		rootSet.insert(i);
	}

	Edge e;
	for(long i=0; i<totalEdge; i++){
		fscanf(fp, "%ld%ld%ld", &e.v1, &e.v2, &e.dist);
		edgeQueue.push(e);
	}

	Kruskal();
	
	for(long i=1; i<totalNode+1; i++){
		cout <<i<<" --- "<<allNode[i].father<<endl;
	}

	


	
	Ending();
	return 0;
}


void Kruskal(){
	Edge e;
	while(!edgeQueue.empty()){
		e = edgeQueue.top();
		edgeQueue.pop();

		if(Find(e.v1)!=Find(e.v2))
			Union(e.v1, e.v2, e.dist);
	}
}

long Find(long n){
	long current = n;

	while(current != allNode[current].label)	//find the class label of input n
		current = allNode[current].label;

	long tmp;
	while(current != allNode[n].label){			//update the class label of the nodes in chain from n to current.
		tmp = allNode[n].label;
		allNode[n].label = current;
		n = tmp;
	}

	return current;
}

void Union(long a, long b, long d){
	allNode[a].father = b;	
	allNode[b].children.push_back(a);
	allNode[a].dist = d;
	rootSet.erase(a);

	long rootA = Find(a);
	long rootB = Find(b);
	if(rootA != rootB){
		allNode[rootA].label = rootB;	//B become the father, and A is the child
	}
}
