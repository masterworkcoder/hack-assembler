/*Copyright (c) 2016 Aaron McKinney 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>
#include "symbol.hpp"

enum COMMAND_TYPE{INVALID_COMMAND,A_COMMAND,C_COMMAND,L_COMMAND};

class Parser
{
public:
	Parser(std::list<std::string>* list, Symbol* symbol_table): l_list(list), l_iter(l_list->begin()), l_symbol_table(symbol_table), rom_mem(0) {}
	
	bool hasMoreCommands() 
	{
		 if(l_iter != l_list->end()) return true; else  return false;
	 }
	bool advance()
	{
		if(hasMoreCommands())
		{
			l_command_type = 0;
			l_symbol = "";
			l_dest = "";
			l_comp = "";
			l_jump = "";	
			std::string buffer = *l_iter;
			if(buffer.at(0) == '@')
			{
				l_command_type = A_COMMAND;
				l_symbol = "";
				for(std::string::iterator i = buffer.begin()+1; i != buffer.end(); ++i)
					l_symbol += *i;
				++rom_mem;
			}
			else if(buffer.at(0) == 'D' || buffer.at(0) == 'A' || buffer.at(0) == 'M' || buffer.at(0) == '0' || buffer.at(0) == '1' || buffer.at(0) == '2' || buffer.at(0) == '3' || buffer.at(0) == '4' || buffer.at(0) == '5' || buffer.at(0) == '6' || buffer.at(0) == '7' || buffer.at(0) == '8' || buffer.at(0) =='9')
			{
				l_command_type = C_COMMAND;
				size_t located = buffer.find("=");
				if(located != std::string::npos)
				{
					l_dest = buffer.substr(0,located);
					l_comp = buffer.substr(located+1,buffer.size());
				}
				located = buffer.find(";");
				if(located != std::string::npos)
				{
					l_dest = "";
					l_comp = buffer.substr(0,located);
					l_jump = buffer.substr(located+1,buffer.size());
				}
				++rom_mem;	
			}
			else if(buffer.at(0) == '(')
			{
				l_command_type = L_COMMAND;
				if(buffer.at(1) >= 'a' && buffer.at(1) <= 'z' || buffer.at(1) >= 'A' && buffer.at(1) <= 'Z')
				{
					size_t loc = buffer.find(")");
					if(loc != std::string::npos)
					{
						if(!l_symbol_table->contains(buffer.substr(1,loc-1)))
							l_symbol_table->addEntry(buffer.substr(1,loc-1),rom_mem);
					}	
					else
					{
						error = "ERROR:Found \"(\" with no \")\"!";
						return false;
					}
				}
				else
				{
					error = "ERROR:Symbols must start with a letter!";
					return false;
				}
			//	std::cout << buffer << std::endl;	
			}
			else
			{
				l_command_type = INVALID_COMMAND;
				error = "Error: Invalid Command!";
			}
			++l_iter;
			return true;
		}
	}

	int commandType() { return l_command_type; }
	std::string symbol() { return l_symbol; }
	std::string dest() { return l_dest; }
	std::string comp() { return l_comp; }
	std::string jump() { return l_jump; }
	void reset()
	{
		l_iter = l_list->begin();
		advance();
		rom_mem = 0;
	}
private:
	Symbol* l_symbol_table;
	std::list<std::string>* l_list;
	std::list<std::string>::iterator l_iter;
	int l_command_type;
	std::string l_symbol;
	std::string l_dest;
	std::string l_comp;
	std::string l_jump;
	std::string error;
	int rom_mem;
};

#endif // PARSER_HPP
