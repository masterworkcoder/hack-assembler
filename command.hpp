/*Copyright (c) 2016 Aaron McKinney 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <list>
#include <fstream>
#include "parser.hpp"

// there are 26 here
const std::string COMP[] =
{
	"0:0101010",
	"1:0111111",
	"-1:0111010",
	"D:0001100",
	"A:0110000",
	"M:1110000",
	"!D:0001101",
	"!A:0110001",
	"!M:1110001",
	"-D:0001111",
	"-A:0110011",
	"-M:1110011",
	"D+1:0011111",
	"A+1:0110111",
	"M+1:1110111",
	"D-1:0001110",
	"A-1:0110010",
	"M-1:1110010",
	"D+A:0000010",
	"D+M:1000010",
	"D-A:0010011",
	"D-M:1010011",
	"A-D:0000111",
	"M-D:1000111",	
	"D&A:0000000",
	"D&M:1000000",
	"D|A:0010101",
	"D|M:1010101",
};

const std::string DEST[] =
{
	"M:001",
	"D:010",
	"MD:011",
	"A:100",
	"AM:101",
	"AD:110",
	"AMD:111",
};

const std::string JMP[] = 
{
	"JGT:001",
	"JEQ:010",
	"JGE:011",
	"JLT:100",
	"JNE:101",
	"JLE:110",
	"JMP:111",
};

class Command
{
public:
	Command(const char* filename): ram_loc(16)
	{
		l_binary.open(filename, std::ios::out);
		if(l_binary.is_open())
			l_good = true;
		else
			l_good = false;	
	}
	~Command()
	{
		if(good())
			l_binary.close();
	}
	
	bool compile(int command_type,std::string symbol,std::string dest, std::string comp, std::string jump, Symbol* symbol_table)
	{
		if(command_type == A_COMMAND)
		{	
			int val = 0;
			if(symbol_table->contains(symbol))
			{
				val = symbol_table->getAddress(symbol);
			}
			else if(symbol.at(0) == '0' || symbol.at(0) == '1' || symbol.at(0) == '2' || symbol.at(0) == '3' || symbol.at(0) == '4' || symbol.at(0) == '5' || symbol.at(0) == '6' || symbol.at(0) == '7' || symbol.at(0) == '8' || symbol.at(0) == '9')
			{
				val = atoi(symbol.c_str());
			}
			else
			{
				val = atoi(symbol.c_str());
				symbol_table->addEntry(symbol,ram_loc);
				val = ram_loc;
				++ram_loc;
			}	
			std::list<int> values;
			values.push_back(val);
			while(val != 1 && val != 0)
			{
				val = val / 2;
				values.push_front(val);
			}
			std::string binary = "";
			for(std::list<int>::iterator i = values.begin(); i != values.end(); ++i)
			{
				if(*i % 2 == 0)
					binary += "0";
				else
					binary += "1";
			}
			values.clear();
			for(int i = binary.size()-1; i < 15; ++i)
			{
				std::string temp(binary);
				binary = "";
				binary += "0" + temp;
			}
			l_binary << binary << "\n";
			return true;
		}
		else if(command_type == C_COMMAND)
		{
			std::string binary = "111";
			if(comp != "")
			{
				for(int i = 0; i < 28; ++i)
				{
					size_t loc = COMP[i].find(":");
					if(comp.compare(COMP[i].substr(0,loc)) == 0)
					{
						binary += COMP[i].substr(loc+1,COMP[i].size());
						break;
					}
				}
			}
			else
			{
				binary += "0000000";
			}		
			
			if(dest != "")
			{
				for(int i = 0; i < 7; ++i)
				{
					size_t loc = DEST[i].find(":");
					if(dest.compare(DEST[i].substr(0,loc)) == 0)
					{
						binary += DEST[i].substr(loc+1,DEST[i].size());
						break;
					}
				}						
			}
			else
			{
				binary += "000";
			}
			
			if(jump != "")
			{
				for(int i = 0; i < 7; ++i)
				{
					size_t loc = JMP[i].find(":");
					if(jump.compare(JMP[i].substr(0,loc)) == 0)
					{
						binary += JMP[i].substr(loc+1,JMP[i].size());
						break;
					}
				}
			}
			else
			{
				binary += "000";
			}
			l_binary << binary << "\n";
		}
		else if(command_type == L_COMMAND)
		{
			return true;	
		}
		else
		{
			return false;	
		}
		return true;
	}	
	
	bool good() { if(l_binary.is_open()) { l_good = true; } else { l_good = false; } return l_good; }
private:
	std::ofstream l_binary;
	bool l_good;
	int ram_loc;
};

#endif // COMMAND_HPP
