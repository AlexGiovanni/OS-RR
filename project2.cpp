//Alexis Santiago project 2
/*************************************************************
*This programm simulates a round robin scheduling system
*it reads in times from a file and stores them in a vector
*of Process objects.
*
**************************************************************/
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <utility>
using namespace std;
//process class holds information about each process
class Process
{
	public:
		int pid;//process id
		double arrivalT;//arrival Time
		double serviceT;//service time
		double exitT;//exit Time
		double startExecT;//time it started executing
		double rst; //remaining service time
		double waitTime;//start executing - arrival time
		double turnAround;//exit time - arrived time
};

/*input: vector of Process objects
**output: arrivalT and serviceT for each process*/
void displayfile(vector<Process> list){
  //prints out all objects times
  cout << "---plist---"<<endl;
  for (int i=0; i<list.size(); i++){
	cout << list[i].arrivalT<<"	"
	<<list[i].serviceT<<endl;
  }
}

/*input: vector of Process objects
**output: arrivalT and serviceT for each process*/
void displayrunningP(vector<Process> list){
  //prints out all objects times
  cout << "---runningP---"<<endl;
  for (int i=0; i<list.size(); i++){
	cout << list[i].arrivalT<<"	"
	<<list[i].serviceT<<endl;
  }
}

/*
*runningP holds currently runing proceses
*doneP holds finished processes
*plist holds all Process objects read from file
*/

int main(){

cout << "-------Project 2--------"<<endl;
//create the queues to hold running processes, and finished processes
vector<Process> runningP;
vector<Process> doneP;
vector<Process> plist;//array of process objects
vector<pair<int, int> > gc;//gant chart 

//set the contexts switch time and time quantum
double cst = 0.025;
double timeq = 0.500;
double time = 0;
int pcount=1; //used to assign pid
double idleTime=0;
//opening the file and read in values
  ifstream f;
  Process p;
  f.open("times.txt");
	if (!f) {
	  cout << "Cant open times.txt" << endl;
	}
	while (f >> p.arrivalT  >> p.serviceT ){
		p.pid=-1;p.startExecT=-1;p.rst=p.serviceT;p.exitT=-1;p.waitTime=-1;p.turnAround=-1;
		plist.push_back(p);
	}
	f.close();

//add the first processes
//loop trough the vector of ojects and search all processes that
//have an arrival time less than or equal to the current time
for (int i=0; i< plist.size();i++ ){
	if (plist[i].arrivalT>time){break;}
	//cout << plist[i].arrivalT<<" "<<plist[i].serviceT <<endl;
	plist[i].pid=pcount;//set pid
	pcount++;
	runningP.push_back(plist[i]);//add to the queue
	plist.erase(plist.begin());//remove from the vector
	i--;//decrease i because the first element was removed, start over from begining
	
	 
}
//displayrunningP(runningP);


//cout << "starting while loop"<<endl;
//cout << runningP.size()<<endl;
//while ready&&job queue not empty
while(runningP.size()>0 || plist.size()>0){
//cout << "n runningP.size() "<<runningP.size()<<endl
//	<<"plist.size() "<<plist.size()<<endl;

//check if runningP is empty this is idle time
if (runningP.size()<=0){
	time+=timeq;
	gc.push_back(make_pair(0, time));//add to gantt chart
	idleTime+=timeq;
 //does a new process arrive in the time intervall
  //loop trough the vector of ojects and search all processes that
  //have an arrival time less than or equal to the current time
  for (int i=0; i< plist.size();i++ ){
	if (plist[i].arrivalT>time){break;}
//	cout <<"Time: "<<time<<" "<< plist[i].arrivalT<<" "<<plist[i].serviceT <<endl;
	plist[i].pid=pcount;//set pid
	pcount++;
	runningP.push_back(plist[i]);//add to the queue
	plist.erase(plist.begin());//remove from the vector
	i--;
  }
  time+=cst;//adds the context switch time 
  gc.push_back(make_pair(0, time));//add the cst
	continue;
}

Process temp = runningP.front();//stores the first element in running queue
runningP.erase(runningP.begin());//pops the first element in running queue

//cout << "temp.arrivalT: "<<temp.arrivalT
//	<<" temp.rst: "<<temp.rst<<endl;

//if temp finishes before it reaches the timeq
if (temp.rst<timeq){
 // cout << "finishes before it reaches tq"<<endl;
  time+=temp.rst; //calculate new time 
  gc.push_back(make_pair(temp.pid, time));//add to gantt chart
  //check for new processes and adds them
  for (int i=0; i< plist.size();i++ ){
	if (plist[i].arrivalT>time){break;}
//	cout << plist[i].arrivalT<<" "<<plist[i].serviceT <<endl;
	plist[i].pid=pcount;//set pid
	pcount++;
	runningP.push_back(plist[i]);//add to the queue
	plist.erase(plist.begin());//remove from the vector
	i--;
  }
  
  //check if its the first time it ran 
  if(temp.serviceT==temp.rst){
 // 	cout <<"++++first time it ran "<<endl;
  	temp.startExecT=time-temp.rst;
  }
  //set it rst to 0 because it finished befor it got to the next tq
  temp.rst=0;
  
  temp.exitT=time;//set exitT
  temp.waitTime = temp.startExecT-temp.arrivalT;//find wait time
  temp.turnAround = temp.exitT-temp.arrivalT;//find turn around time
  doneP.push_back(temp);//add to done queue
  time+=cst;//adds the context switch time 
  gc.push_back(make_pair(0, time));//add the cst 


}else{//if temp runs the whole timeq
	//cout << "runs whole timeq"<<endl;
  time+=timeq; //calculate new time 
  gc.push_back(make_pair(temp.pid, time));//add to gantt chart
  //look for new processes and add them
  for (int i=0; i< plist.size();i++ ){
	if (plist[i].arrivalT>time){break;}
//	cout <<"Time: "<<time<<" "<< plist[i].arrivalT<<" "<<plist[i].serviceT <<endl;
	plist[i].pid=pcount;//set pid
	pcount++;
	runningP.push_back(plist[i]);//add to the queue
	plist.erase(plist.begin());//remove from the vector
	i--;
  }
  //subtract timeq from running process(temp) rst
  temp.rst=temp.rst-timeq;
  //check if its the first time it ran
  if (temp.rst == (temp.serviceT - timeq)){
 // 	cout << "first time it ran"<<endl;
  	temp.startExecT = time - timeq;//set its startExecT
 // 	cout<<"temp.startExecT: "<<temp.startExecT<<endl;

  }
  
  //check if it finished running
  if (temp.rst > 0){
    //add running process to queue only if its rst is > 0
 //   cout<<"adding back to runningP"<<endl;
    runningP.push_back(temp);
  }
  else{//Process finished running
 //	cout<<"Process finished running"<<endl;
  	temp.exitT=time;//set exitT
  	temp.waitTime=temp.startExecT-temp.arrivalT;//find wait time
  	temp.turnAround=temp.exitT-temp.arrivalT;//find turn around time
    doneP.push_back(temp);//add to done queue
  }
  time+=cst;//adds the context switch time 
  gc.push_back(make_pair(0, time));
}


}


//displayfile(plist);

//find wait time, and turnaroud time average
double wtAverage=0;double taAverage=0;
for (int i=0; i<doneP.size(); i++){
  wtAverage+=doneP[i].waitTime;
  taAverage+=doneP[i].turnAround;
}
wtAverage=wtAverage/doneP.size();
taAverage=taAverage/doneP.size();





/*

//print the done queue
cout << "--------done processes-----------"<<endl;
while (doneP.size()>0){
//print information
Process d = doneP.front();
cout << "pid: "<<d.pid<<endl
     << "arrivalT: "<<d.arrivalT<<endl
     << "serviceT: "<<d.serviceT<<endl
     << "exitT: "<<d.exitT<<endl
     << "startExecT: "<<d.startExecT<<endl
     << "rst: "<<d.rst<<endl
     << "waitTime: "<<d.waitTime<<endl
     << "turnAround: "<<d.turnAround<<endl<<endl;

//pop process
doneP.erase(doneP.begin());

}




//print gantt chart
cout <<"-----gantt chart-----"<<endl<<"|";
for (int i=0; i< gc.size()-1; i++){
	cout << "P"<<gc[i].first << ", "
         << gc[i].second << "|"; 
}
*/

cout <<"time quantum: "<<timeq<<endl
	 <<"context switch time: "<<cst<<endl;

//print the average wait time and turn around time
cout << "\nwait time: "<<wtAverage<<endl
	 << "turn around time: "<<taAverage<<endl
	 << "cpu time: "<<time-idleTime<<endl
	 << "idle time: "<<idleTime<<endl;



return 0;
}
