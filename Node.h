#ifndef NODE_H
#define NODE_H

#include <iostream> 
#include <vector>
using namespace std;

class Node{
public:
	long label;	//class label
	long father;//father node
	vector<long> children;	//children nodes
	long dist;	//distance to father node
	long layer;	//layer No. from root
};



#endif
