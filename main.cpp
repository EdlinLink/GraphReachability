#include <iostream>
#include <queue>
#include <set>
#include "Node.h"
#include "Edge.h"
#include "Query.h"
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <set>
#include <cstring>

using namespace std;

long totalNode;
long totalEdge;
long totalQuery;
set<long> rootSet;	//record all the root

Node * allNode;
//priority_queue<Edge> edgeQueue;	//edge queue
queue<Edge> edgeQueue;	//edge queue
vector<Edge> edgeVec;
Query * allQuery;
FILE *fp;

void Kruskal(char *);
long Find(long);
void Union(long, long, long);
void LabelNode();

void DoQuery(char *);
void RealQuery(char *);
void OutputResult(char *);

void MST();

void Ending(){
	delete []allNode;
	delete []allQuery;
}

int main(int argc, char ** argv){

	if(argc!=5){
		printf("### Error! The program usage is like:\n### WCR.exe facebook-hour.txt facebook-query-1.txt result.txt time.txt\n");
		return 0;
	}
	fp = fopen(argv[1], "r");
	fscanf(fp, "%ld%ld", &totalNode, &totalEdge);

	allNode = new Node[totalNode+1];	//please pay attention to +1 here
	for(long i=1; i<totalNode+1; i++){
		allNode[i].label = i;
		allNode[i].father = i;
		allNode[i].dist = 0;
	}
	
	Edge e;
	for(long i=0; i<totalEdge; i++){
		fscanf(fp, "%ld%ld%ld", &e.v1, &e.v2, &e.dist);
		edgeVec.push_back(e);
	}

	sort(edgeVec.begin(), edgeVec.end());
	for(long i=totalEdge-1; i>=0; i--){
		edgeQueue.push(edgeVec[i]);
	}


	Kruskal(argv[4]);
	
	DoQuery(argv[2]);
	RealQuery(argv[4]);
	OutputResult(argv[3]);

	Ending();
	return 0;
}



void Kruskal(char * timeFile){
	clock_t start, end;
	start = clock();
	Edge e;
	while(!edgeQueue.empty()){
		e = edgeQueue.front();
		edgeQueue.pop();
		if(Find(e.v1)!=Find(e.v2)){
			Union(e.v1, e.v2, e.dist);
		}
	}

	long theLabel;
	for(long i=1; i<totalNode+1; i++){
		theLabel = Find(i);		//final update the class label
		if(theLabel == i){		//if label equal the node number
			rootSet.insert(i);	//it is a root 
		}
	}

	MST();	//really construct the tree  
	end = clock();
	
	fp = fopen(timeFile, "w");
	double ConstructTime = (double)(end-start)/CLOCKS_PER_SEC;
	fprintf(fp, "MST Construct Time: %lf s\n", ConstructTime);
	fprintf(fp, "MST Size: %ld kb\n", sizeof(Node)*totalNode/1024);
	fclose(fp);
}


void MST(){
	vector<long> v1, v2;
	v1.clear();
	v2.clear();

	for(set<long>::iterator i = rootSet.begin(); i!=rootSet.end(); i++){
		allNode[*i].father = *i;
		allNode[*i].dist = 0;
		allNode[*i].layer = 1;
		v1.push_back(*i);
	}	

	while(!v1.empty()){
		v2.clear();
		for(vector<long>::iterator i=v1.begin(); i!=v1.end(); i++){
			long fa = *i;
			for(vector<NodeDist>::iterator son=allNode[fa].children.begin(); son!=allNode[fa].children.end(); son++){
				for(vector<NodeDist>::iterator sonson=allNode[(*son).name].children.begin(); sonson!=allNode[(*son).name].children.end(); sonson++){
					if((*sonson).name == fa){
						allNode[(*son).name].children.erase(sonson);
						break;
					}
				}
			}
			for(long j = allNode[fa].children.size()-1; j>=0; j--){
				long son = allNode[fa].children[j].name;
				v2.push_back(son);
				allNode[son].father = fa;
				allNode[son].dist = allNode[fa].children[j].dist;
				allNode[son].layer = allNode[fa].layer+1;
			}
		}
		v1.clear();
		v1.assign(v2.begin(), v2.end());
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
	NodeDist tmp;
	
	tmp.dist = d;
	tmp.name = b;
	allNode[a].children.push_back(tmp);
	tmp.name = a;
	allNode[b].children.push_back(tmp);

	long rootA = Find(a);
	long rootB = Find(b);
	if(rootA != rootB){
		allNode[rootA].label = rootB;	//B become the father, and A is the child
	}
}

void DoQuery(char *queryFile){
	fp = fopen(queryFile, "r");
	fscanf(fp, "%ld", &totalQuery);

	allQuery = new Query[totalQuery];
	for(long i=0; i<totalQuery; i++){
		fscanf(fp, "%ld%ld%ld", &allQuery[i].a, &allQuery[i].b, &allQuery[i].limit);
	}

	fclose(fp);
}

void RealQuery(char * timeFile){
	clock_t start, end;
	start = clock();

	long a, b, limit;
	int canReach;
	for(long i=0; i<totalQuery; i++){
		
		a = allQuery[i].a;
		b = allQuery[i].b;
		limit = allQuery[i].limit;

		if(allNode[a].label==allNode[b].label){
			canReach = 1;
		}
		else{
			canReach = 0;
		}


		//Get to the same layer
		if(canReach == 1){
			while(allNode[a].layer > allNode[b].layer){
				if(allNode[a].dist > limit){
					canReach = 2;
					break;
				}
				else{
					a = allNode[a].father;
				}
			}

			while(allNode[a].layer < allNode[b].layer){
				if(allNode[b].dist > limit){
					canReach = 3;
					break;
				}
				else{
					b = allNode[b].father;
				}
			}
		}
		
		
		//At here, they are in the same layer and still ok.
		if(canReach == 1){
			while(a != b){
				if(allNode[a].dist > limit){
					canReach = 2;
					break;
				}
				if(allNode[b].dist > limit){
					canReach = 3;
					break;
				}
				a = allNode[a].father;
				b = allNode[b].father;
			}
		}
		
		if(canReach==1)
			allQuery[i].label = 1;
		else
			allQuery[i].label = 0;

	}
	end = clock();
	double QueryTime = (double)(end-start)/CLOCKS_PER_SEC;

	fp = fopen(timeFile, "a+");
	fprintf(fp, "Query Processing Time: %lf s\n", QueryTime);
	fclose(fp);
}

void OutputResult(char * resultFile){
	fp = fopen(resultFile, "w");

	for(long i=0; i<totalQuery; i++){
		if(allQuery[i].label == 1)
			fprintf(fp, "Yes\n");
		else
			fprintf(fp, "No\n");
	}

	fclose(fp);
}
