#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <map>
#include <iostream>

class Symbol
{
public:
	Symbol()
	{
		// setup the standard symbol table
		l_map["SP"]=0;
		l_map["LCL"]=1;
		l_map["ARG"]=2;
		l_map["THIS"]=3;
		l_map["THAT"]=4;
		l_map["R0"]=0;
		l_map["R1"]=1;
		l_map["R2"]=2;
		l_map["R3"]=3;
		l_map["R4"]=4;
		l_map["R5"]=5;
		l_map["R6"]=6;
		l_map["R7"]=7;
		l_map["R8"]=8;
		l_map["R9"]=9;
		l_map["R10"]=10;
		l_map["R11"]=11;
		l_map["R12"]=12;
		l_map["R13"]=13;
		l_map["R14"]=14;
		l_map["R15"]=15;
		l_map["SCREEN"]=16384;
		l_map["KBD"]=24576;
	}
	
	bool addEntry(std::string symbol,int location)
	{
		if(!contains(symbol))
		{	
			l_map[symbol]=location;
			return true;
		}
		return false;
	}
	
	bool contains(std::string symbol)
	{
		for(std::map<std::string,int>::iterator itr = l_map.begin(); itr != l_map.end(); ++itr)
		{
			if(symbol.compare(itr->first) == 0)
				return true;
		}
		return false;
	}

	int getAddress(std::string symbol)
	{
		if(contains(symbol))
		{
			for(std::map<std::string,int>::iterator itr = l_map.begin(); itr != l_map.end(); ++itr)
			{
				if(symbol.compare(itr->first) == 0)
					return itr->second;
			}
		}	
		
		return -1;
	}	
private:
	std::map<std::string,int> l_map;
};
#endif // SYMBOL_HPP
