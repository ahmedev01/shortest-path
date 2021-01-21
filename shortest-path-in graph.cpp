#include<iostream>
#include<utility>
#include<sstream>
#include<vector>
#include<fstream>
#include<cstring>
#include<cstdlib>
using namespace std;

//*** Used Structures ***//
struct neighbors;
struct relation{
	char node;
	vector<neighbors> myNeighbors;
}rel;
struct neighbors{
	char neighbor;
	int distance;
};
struct path{
	char node;
	char predecessor;
	int distanceMin;
	int finalStatus;
};

//*** Used Vectors ***//
vector<relation> graph;
vector<neighbors> theNeighbors;
vector<path> route;

//*** Global Variables ***//
char src, dest;
string content;
char fileName[100];

//*** Functions ***//
int initialChoice();
int endNeighborEntry();
int endGraphEntry();
bool neighborExist(char neighbor, vector<neighbors> theNeighbors);
bool nodeExist(char node, vector<relation> graph);
void graphEntry();
void viewGraph();
void importGraph();
void initialisePath(char src, vector<relation> graph);
bool findPath(char src, char dest, vector<relation> graph);
void viewPath(char src, char dest);
void calculatePath();

//*** Main Function ***//
int main()
{
	//*** Begin the program and ask for user entry***//
    int choice = initialChoice();

	switch(choice) {
		case '1':
			graphEntry();
			if(!graph.empty()){
				main();
			}
			break;
		case '2':
			importGraph();
			break;
		case '3':
			viewGraph();
			break;
		case '4':
			calculatePath();
			break;
		case '5':
			cout<< "Exit the program...\n";
			exit(0);
		default:
		cout<< "Invalid input, Please try again\n";
		main();
	}
	return 0;
}

//*** initialChoice: First menu and first user choice ***//
int initialChoice(){
	char choice;
    cout<<"1 : Create Graph. \n";
    cout<<"2 : Import a Graph from file (must be in the same folder). \n";
    cout<<"3 : View Graph. \n";
    cout<<"4 : Calculate the shortest path from one point to an another. \n";
	cout<<"5 : Quit. \n";
    cin>>choice;
	return (int)choice;
}

//************* Create Graph Option **************//

//*** graphEntry: process the graph entry by the user when create graph option is selected ***//
void graphEntry(){
	char responsen, responsev, responsee, node, neighbor;
	int distance, endGraphEntry = 0, endNeighborEntry = 0, ncount = 0, vcount = 0;
	//Verify that the user didn't ask to abort the graph entry.
	while (endGraphEntry == 0){
		cout<<"Add node ? YES 'y': \n";
		cin>>responsen;
		if ((responsen == 'y')||(reponsen =='Y')){
			endNeighborEntry = 0;
			theNeighbors.clear();
			cout<<"Node name : \n";
			cin>>node;
			//Add node only if it doesn't exist
			if (nodeExist(node, graph) == false){
				ncount++;
				//We detect when neighbors entry is done.
				while (endNeighborEntry == 0){
					cout<<"Add neighbor ? YES 'y': \n";
					cin>>responsev;
					if ((responsev == 'y')||(responsev =='Y')){
						cout<<"Neighbor name : \n";
						cin>>neighbor;
						if (neighborExist(neighbor, theNeighbors) == false){
							cout<<"Distance : \n";
							cin>>distance;
							struct neighbors theNeighbor = {neighbor, distance};
							theNeighbors.push_back(theNeighbor);
							cout<<" "<<theNeighbors.empty();
							vcount++;
						}else{
							cout<<"Error: Neighbor already exist.\n";
								endNeighborEntry = endNeighborEntry();
						}
					}else{
						if ((vcount == 0) && (ncount == 1)){
							cout<<"Error: The node has no neighbors. The graph is empty. exit? YES: 'y' \n";
							cin>>responsee;
							if ((responsee == 'y')||(responsee =='Y')){
								graph.clear();
								ncount = 0;
								main();
							}
						}else if ((vcount == 0) && (ncount > 1)){
							cout<<"Error: The node has no neighbors. Terminate graph input? YES: 'y' \n";
							cin>>responsee;
							if ((responsee == 'y')||(responsee =='Y')){
								endGraphEntry = 1;
								main();
							}
						}else{
							endNeighborEntry = endNeighborEntry();
						}
					}
				}
				if (vcount != 0){
					graph.push_back({node, theNeighbors});
					int s = graph.size();
					cout<<"Node: "<<graph[s-1].node<<" Neighbors: \n";
					vector<neighbors>vois = graph[s-1].myNeighbors;
					for (unsigned int it=0;it<vois.size();it++){
						cout<<"Neighbor "<<it+1<<" :"<<vois[it].neighbor<<" || ";
						cout<<"Distance :"<<vois[it].distance<<"\n";
					}
				}
				vcount = 0;
			}else{
				cout<<"Error: Node already exists.";
				endGraphEntry = endGraphEntry();
			}
		}else{
		    endGraphEntry = endGraphEntry();
		}
	}
	cout<<"Input complete. Back to main menu.\n";
	main();
}

//*** Check if the node exists or not ***//
bool nodeExist(char node, vector<relation> graph){
	bool exist = false;
	for (unsigned int it=0;it<graph.size();it++){
		if (graph[it].node == node){
			exist = true;
		}
	}
	return exist;
}

//*** Check if the neighbor exists or not ***//
bool neighborExist(char neighbor, vector<neighbors> theNeighbors){
	bool exist = false;
		for (unsigned int it1=0;it1<theNeighbors.size();it1++){
			if (theNeighbors[it1].neighbor == neighbor){
				exist = true;
			}
		}
	return exist;
}

//*** Add more nodes or end the graph entry ***//
int endGraphEntry(){
	char responsen;
	bool result;
	cout<<"Terminate graph input? YES 'y'\n";
	cin>>responsen;
	if ((responsen == 'y')||(responsen =='Y')){
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//*** Add more neighbors or not***//
int endNeighborEntry(){
	char responsev;
	bool result;
	cout<<"Terminate neighbors input? YES 'y'\n";
	cin>>responsev;
	if ((responsev == 'y')||(responsev =='Y')){
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//**************** Import Graph Option **************//

//*** importGraph: import the graph from a txt file ***//
void importGraph(){
	char node = '1';
	bool error = false;
	vector<neighbors> neighberhood;
	cout<<"Deleting graph from memory (if exist)\n";
	graph.clear();
	cout<<"File name to import (example: myfile.txt):\n";
	cin>>fileName;
	ifstream importFile(fileName);
	if (importFile.is_open()){
		cout<<"File open.\n Import in progress!\n";
		while(content != "STOP") {
            getline(importFile, content);
			if ((neighberhood.empty())&&(node=='1')){
				if ((content[0]=='N')||(content[0]=='n')){
					node = content[1];
					cout<<".";
				}else if((content[0]=='V')||(content[0]=='v')){
					cout<<"Error: Neighbors were defined without node.\n Import aborted!\n";
					error = true;
					main();
				}else if (content == "STOP"){
				    cout<<"Error: File empty.\n";
					error = true;
					main();
                }else{
					cout<<"Error: Import aborted because of file mis-structured!\n";
					error = true;
					main();
				}
			}else if ((neighberhood.empty())&&(node!='1')){
				if ((content[0]=='N')||(content[0]=='n')){
					graph.push_back({node, neighberhood});
					neighberhood.clear();
					node = content[1];
					cout<<".";
				}else if((content[0]=='V')||(content[0]=='v')){
					string dist = content.substr(3);
					std::stringstream ss(dist);
					int x;
					ss >> x;
					if (ss){
						neighberhood.push_back({content[1],x});
						cout<<".";
					}else{
						cout<<"Couldn't convert";
					}
				}else if (content == "STOP"){
				    graph.push_back({node, neighberhood});
					neighberhood.clear();
                }else{
					cout<<"Error: Import aborted because of file mis structure!\n";
					error = true;
					main();
				}
			}else if ((!neighberhood.empty())&& (node!='1')){
				if ((content[0]=='N')||(content[0]=='n')){
					rel = {node, neighberhood};
					graph.push_back(rel);
					neighberhood.clear();
					node = content[1];
					cout<<".";
				}else if((content[0]=='V')||(content[0]=='v')){
				    string dist = content.substr(3);
					std::stringstream ss(dist);
					int x;
					ss >> x;
					if (ss){
						neighberhood.push_back({content[1],x});
						cout<<".";
					}else{
						cout<<"Couldn't convert";
					}
				}else if (content == "STOP"){
				    graph.push_back({node, neighberhood});
					neighberhood.clear();
                }else if (content != "STOP"){
					cout<<"Error: Import aborted because of file mis structure!\n";
					error = true;
					main();
				}
			}else{
				cout<<"Error: Import aborted because of file mis structure!\n";
				cout<<"Try to view the result to find out the origin of the error.\n";
				error = true;
				main();
			}
		}
		if (error == false){
		    cout<<"\nImport successfully done.\n";
			importFile.close();
		    main();

		}
	}else{
		cout<<"Error: Can't open file.\n";
		importFile.close();
		main();
	}
}

//**************** View Graph Option *********************//
void viewGraph(){
	if (graph.empty()){
		cout<<"Graph empty.\n";
		main();
	}else{
		for (unsigned int it=0;it<graph.size();it++){
			cout<<"N"<<it+1<<": "<<graph[it].node<<"\n";
			cout<<"-------------------------\n";
			vector<neighbors>vois = graph[it].myNeighbors;
			for (unsigned int it1=0;it1<vois.size();it1++){
				cout<<"Neighbor "<<graph[it].node<<"-"<<it1+1<<" :"<<vois[it1].neighbor<<" || ";
				cout<<"Distance :"<<vois[it1].distance<<"\n";
			}
			cout<<"\n";
		}
		main();
	}
}

//*************** Calculate the shortest path option ******//
void calculatePath(){
	if (graph.empty()){
		cout<<"Error: Graph empty!\n Try to create or import one.\n";
		main();
	}else{
		//Prepare the needed variables
		cout<<"Please enter the start node \n";
		cin>>src;
		if (nodeExist(src, graph)){
			cout<<"Please enter the destination node \n";
			cin>>dest;
			if (nodeExist(dest, graph)){
				initialisePath(src, graph);
				if (route.empty()){
                    cout<<"Wrong Path\n";
				}else{
				bool finded = findPath(src, dest, graph);
				if(finded){
					viewPath(src,dest);
				}else{
					cout<<"Path doesn't exist.\nBack to main menu.\n";
					main();
				}
				}
			}else{
				cout<<"Destination node doesn't exist.\n Try again.\n";
				calculatePath();
			}
		}else{
			cout<<"Start node doesn't exist.\n Try again.\n";
			calculatePath();
		}
	}
}

void initialisePath(char src, vector<relation> graph){
	char node, pred;
	int distMin, statFin;
	for (unsigned int it=0; it<graph.size(); it++){
		if (graph[it].node == src){
			node=src;
			pred = '/';
			distMin = 0;
			statFin = 1;
		}else{
			node = graph[it].node;
			pred = '#';
			distMin = 999;
			statFin = 0;
		}
		route.push_back({node, pred, distMin, statFin});
		cout<<"N: "<<route[it].node<<" P: "<<route[it].predecessor<<" D: "<<route[it].distanceMin<<" FS: "<<route[it].statutFinal<<"\n";
	}
	cout<<"Path Initialised\n";
}

bool findPath(char src, char dest, vector<relation> graph){
	vector<neighbors> actualNeighbors;
	int remainingNode = route.size();
	char actualNode = src;
	int distAct = 0, calcDist, distMin, index;
	bool findIt;
    cout<<remainingNode;
	while((remainingNode!=1)&&(findIt == false)){
        actualNeighbors.clear();
		for (unsigned int it=0; it<graph.size(); it++){
			if (graph[it].node == actualNode){
				for (unsigned int itv=0; itv<graph[it].myNeighbors.size(); itv++){
				actualNeighbors.push_back({graph[it].myNeighbors[itv].neighbor, graph[it].myNeighbors[itv].distance});
				cout<<"Node "<<graph[it].myNeighbors[itv].neighbor<<" Distance: "<<graph[it].myNeighbors[itv].distance<<"\n";
				}
			}
		}
		cout<<"Neighbors are "<<actualNeighbors.size()<<"\n";
		if (actualNeighbors.size()>0){
			for (int it1=0; it1<actualNeighbors.size(); it1++){
				for (int it2=0; it2<route.size(); it2++){
					if ((actualNeighbors[it1].neighbor == route[it2].node) && (route[it2].finalStatus == 0)){
						calcDist = distAct + actualNeighbors[it1].distance;
						if(route[it2].distanceMin > calcDist){
							route[it2].distanceMin = calcDist;
							route[it2].predecessor = actualNode;
						}
						cout<<"_-_-_---_-_\n";
						cout<<"N: "<<route[it2].node<<" P: "<<route[it2].predecessor<<" D: "<<route[it2].distanceMin<<" FS: "<<route[it2].finalStatus<<"\n";
						cout<<"_-_-_---_-_\n";
					}
				}
			}
			for (unsigned int it3=0; it3<route.size(); it3++){
				if (route[it3].finalStatus == 0){
					if ((distMin == 0) || (distAct == 0) || (distMin > route[it3].distanceMin)){
						distMin = route[it3].distanceMin;
						actualNode = route[it3].node;
						distAct = distMin;
						index = it3;
						cout<<"Min Distance: "<<distMin<<"\n";
						cout<<"Current Node: "<<noeudAct<<"\n";
						cout<<"Current Distance: "<<distAct<<"\n";
						cout<<"Index: "<<index<<"\n";

					}
				}
			}
			route[index].finalStatus = 1;
			if (route[index].node == dest){
					findIt = true;
				}else{
				findIt = false;
				}

		}else{
			distAct = 0;
		}
		distMin=0;
		remainingNode = remainingNode - 1;
		cout<<"Found: "<<findIt<<"\n";
		cout<<"Remaining Nodes: "<<remainingNode<<"\n";
		cout<<"-------------------------------\n";
	}
	return findIt;
}

void viewPath(char src, char dest){
	char actualNode = dest;
	int distanceMin = 0;
	vector<char> path;
	cout<<"Shortest path found \n";
	while (actualNode != src){
		for (unsigned int it=0; it<route.size(); it++){
			if(route[it].node == actualNode){
				cout << actualNode;
				cout<<"--> ";
				actualNode = route[it].predecessor;
			}
			if(route[it].node == dest){
                distanceMin = route[it].distanceMin;
			}
		}
	}
	cout<<src;
	cout<<" with a distance of "<<distanceMin<<"\n";
}
