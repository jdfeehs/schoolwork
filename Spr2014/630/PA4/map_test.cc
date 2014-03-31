#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <set>
#include <iostream>
using namespace std;
int main()
{
  multimap<string,string> map;
  set<string> set;
  string parent,child;
  parent = "1";
  while(parent.compare("0101"))
  {
    //cout << "Input a parent, then a child." << endl;
    cin >> parent;
    cin >> child;
    if(parent.compare("0101"))
    {
      //cout << "The parent is: " << parent << endl;
      //cout << "The child is: " << child << endl;
      map.insert(pair<string,string>(parent,child));
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
  for(multimap<string,string>::iterator it = map.begin(); it!=map.end(); ++ it)
  {
    cout << (*it).first << " is the parent of: " << (*it).second << endl;
  }

  return 0;
}
