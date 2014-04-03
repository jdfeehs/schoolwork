#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <set>
#include <iostream>
using namespace std;
int main()
{
  map<string,string> map;
  set<string> set;
  string parent,child;
  parent = "1";
  int children_count = 0;
  while(parent.compare("0101"))
  {
    //cout << "Input a parent, then a child." << endl;
    cin >> parent;
    cin >> child;
    children_count ++;
    if(parent.compare("0101"))
    {
      //cout << "The parent is: " << parent << endl;
      //cout << "The child is: " << child << endl;
      map.insert(pair<string,string>(child,parent));
      if(set.count(child) == 0)
      {
        set.insert(child);
      }
      else
      {
        cout << "CYCLECYCLECYCLECYCLE!!!!!" << endl;
      }
    }
  }
  int counter = 0;
  for(multimap<string,string>::iterator it = map.begin(); it!=map.end(); ++ it)
  {
    counter = 0;
    while(parent != "Object" && counter < children_count)
	{
	  child = parent;
	  parent = map[child];
	  counter ++;  
	}
    if(counter == children_count && parent != "Object")
    {
	//this means there was a cycle
	cout << "CYCLE FROM LOOP!" << endl;
    }

  }


  for(multimap<string,string>::iterator it = map.begin(); it!=map.end(); ++ it)
  {
    cout << (*it).second << " is the parent of: " << (*it).first << endl;
  }

  return 0;
}
