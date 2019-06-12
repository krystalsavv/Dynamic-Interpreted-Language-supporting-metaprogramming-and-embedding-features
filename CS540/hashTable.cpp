#include "hashTable.h"
#include <string>

std::string Anonymous_func_name(int fcount)
{
    int counter = 0, tmp = fcount;
	while(fcount>0){
		counter++;
		fcount=fcount - fcount%10;
		fcount=fcount/10;
	}
	std::string name = "_f";
	std::string c;
	while(counter>0){
		int x=tmp%10;
		if(x==0) c="0";
		else if(x==1) c="1";
		else if(x==2) c="2";
		else if(x==3) c="3";
		else if(x==4) c="4";
		else if(x==5) c="5";
		else if(x==6) c="6";
		else if(x==7) c="7";
		else if(x==8) c="8";
		else if(x==9) c="9";
		else c = "";
		tmp=tmp-tmp%10;
		tmp=tmp/10;
		name += c;
		counter--;
	}
	return name;
}


