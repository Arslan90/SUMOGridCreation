//============================================================================
// Name        : SUMOFile.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <set>

using namespace std;

bool isASinkOrSourceNode (std::set<string> sinkSourceNode, string node){

	bool result = false;

	std::set<string>::iterator it2 = sinkSourceNode.find(node);

	if ( it2 !=sinkSourceNode.end()){
		result = true;
	}

	return result;
}

bool isANodeToDelete (std::set<string> nodeTodelete, string node){

	bool result = false;

	std::size_t found;
	std::size_t discard;

	for (std::set<string>::iterator it2 = nodeTodelete.begin(); it2 != nodeTodelete.end(); ++it2){
		string tmp = *it2;
		found = node.find(tmp);
		discard = node.find("0/");
		if (found != string::npos){
			if ((tmp.compare("0/") == 0) && (discard != 0 )){
				if ((node.compare("10/0") == 0)||(node.compare("10/11") == 0)){
					result = true;
				}else {
					result = false;
				}
			}else {
				result = true;
			}
		}
	}

	return result;
}

int main() {

	enum nodeType{
		normal,
		todelete,
		sink_or_source,
	};

	nodeType currentNodeType;
	nodeType neighborNodeType;

	cout << "Fix the number of Nodes for one axis" << endl;
	int nbrXNode = 0;
	cin >> nbrXNode;
	cout << "actual number of Nodes for one axis = " << nbrXNode << endl;

	cout << "generating Nodes of the net grid " << endl;

	string matrixNodes[nbrXNode][nbrXNode];

	for (int j = 0; j < nbrXNode; ++j)	{
		for (int i = 0; i < nbrXNode ; ++i)	{

			ostringstream convertX, convertY;
			convertX << i;
			convertY << (nbrXNode-j-1);

			matrixNodes[i][j] = convertX.str()+"/"+convertY.str();

			cout << matrixNodes[i][j] << "\t";
		}
		cout << endl;
	}

	std::set<string> sinkSourceNode;
	sinkSourceNode.insert("0/2");
	sinkSourceNode.insert("0/5");
	sinkSourceNode.insert("0/7");
	sinkSourceNode.insert("0/9");
	sinkSourceNode.insert("2/0");
	sinkSourceNode.insert("5/0");
	sinkSourceNode.insert("7/0");
	sinkSourceNode.insert("9/0");

	sinkSourceNode.insert("3/11");
	sinkSourceNode.insert("5/11");
	sinkSourceNode.insert("7/11");
	sinkSourceNode.insert("9/11");
	sinkSourceNode.insert("11/3");
	sinkSourceNode.insert("11/5");
	sinkSourceNode.insert("11/7");
	sinkSourceNode.insert("11/9");






	for (std::set<string>::iterator it = sinkSourceNode.begin(); it != sinkSourceNode.end(); it++){
		string tmp = *it;
		cout << "sink node is " << tmp << endl;
	}

//	std::set<string> sourceNode;
//	sourceNode.insert("5/0");
//	sourceNode.insert("7/0");
//	sourceNode.insert("9/0");
//	sourceNode.insert("11/2");
//	sourceNode.insert("11/4");
//	sourceNode.insert("11/6");
//
//	for (std::set<string>::iterator it = sourceNode.begin(); it != sourceNode.end(); it++){
//		string tmp = *it;
//		cout << "source node is " << tmp << endl;
//	}

	cout << "generating edges to keeps and those to delete " << endl;

	cout << "nodes to delete are those with this form X/0, X/11, 0/Y, 11/Y  execpts sources and sink nodes" << endl;

	std::set<string> nodeTodelete;
	nodeTodelete.insert("/0");
	nodeTodelete.insert("/11");
	nodeTodelete.insert("0/");
	nodeTodelete.insert("11/");

	std::set<string> edgeToKeep;
	std::set<string> edgeToDelete;

	std::set<string> neighborhoodNodes;

	for (int i = 0; i < nbrXNode; ++i) {

		neighborhoodNodes.clear();

		for (int j = 0; j < nbrXNode; ++j) {
			cout << "current node is " << matrixNodes[i][j];

			if (isANodeToDelete(nodeTodelete,matrixNodes[i][j])){
				if (isASinkOrSourceNode(sinkSourceNode,matrixNodes[i][j])){
					cout << " (sink_or_source)";
					currentNodeType = sink_or_source;
				}else {
					cout << " (toDelete)";
					currentNodeType = todelete;
				}
			}else {
				currentNodeType = normal;
			}

			int x, y;
			string currentNeighboor ="";

			for (int k = 1; k <= 4; ++k) {

				switch (k) {
					case 1:
						x = i-1;
						y = j;
						break;
					case 2:
						x = i+1;
						y = j;
						break;
					case 3:
						x = i;
						y = j-1;
						break;
					case 4:
						x = i;
						y = j+1;
						break;
				}

				if (((0 <= x)&&(x < nbrXNode)) && ((0 <= y)&&(y < nbrXNode))){

					currentNeighboor = matrixNodes[x][y];

					string edge;
					edge = matrixNodes[i][j]+"to"+matrixNodes[x][y];

					if (isANodeToDelete(nodeTodelete,matrixNodes[x][y])){
						if (isASinkOrSourceNode(sinkSourceNode,matrixNodes[x][y])){
							neighborNodeType = sink_or_source;
						}else {
							neighborNodeType = todelete;
						}
					}else {
						neighborNodeType = normal;
					}

//					if (currentNodeType == normal){
//						if ((neighborNodeType == normal)||(neighborNodeType == sink)){
//							edgeToKeep.insert(edge);
//						}else {
//							edgeToDelete.insert(edge);
//						}
//					}else if (currentNodeType == source){
//						if (neighborNodeType == normal){
//							edgeToKeep.insert(edge);
//						}else {
//							edgeToDelete.insert(edge);
//						}
//					}else {
//						edgeToDelete.insert(edge);
//					}

					if (currentNodeType != todelete){
						if (neighborNodeType != todelete){
							edgeToKeep.insert(edge);
						}else {
							edgeToDelete.insert(edge);
						}
					}else {
						edgeToDelete.insert(edge);
					}


				}

			}

			cout << endl;

		}
	}

	cout << "listing edges to delete " << endl;

	for (std::set<string>::iterator it = edgeToDelete.begin(); it != edgeToDelete.end(); ++it) {
		string tmp = *it;
		cout << tmp << ",";
	}

	return 0;
}
