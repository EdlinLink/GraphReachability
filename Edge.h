#ifndef EDGE_H
#define EDGE_H
#include <iostream>
using namespace std;

class Edge{
public:
	long v1;
	long v2;
	long dist;
	bool operator<(const Edge &a) const{
		return a.dist < dist;
	}
};

#endif
