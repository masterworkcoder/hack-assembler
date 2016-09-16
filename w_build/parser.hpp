/*Copyright (c) 2016 Aaron McKinney 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>

enum COMMAND_TYPE{INVALID_COMMAND,A_COMMAND,C_COMMAND,L_COMMAND};

class Parser
{
public:
	Parser(std::list<std::string>* list): l_list(list), l_iter(l_list->begin()) {}
	
	bool hasMoreCommands() { if(l_iter != l_list->end()) return true; return false; }
	void advance()
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
			}
			else if(buffer.at(0) == 'D' || buffer.at(0) == 'A' || buffer.at(0) == 'M' || buffer.at(0) >= '0' || buffer.at(0) <='9')
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
			}
			else
			{
				l_command_type = INVALID_COMMAND;
			}	
			++l_iter;
		}
	}

	int commandType() { return l_command_type; }
	std::string symbol() { return l_symbol; }
	std::string dest() { return l_dest; }
	std::string comp() { return l_comp; }
	std::string jump() { return l_jump; } 
private:
	std::list<std::string>* l_list;
	std::list<std::string>::iterator l_iter;
	int l_command_type;
	std::string l_symbol;
	std::string l_dest;
	std::string l_comp;
	std::string l_jump;
};

#endif // PARSER_HPP
