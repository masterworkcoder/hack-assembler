/*Copyright (c) 2016 Aaron McKinney 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <list>
#include <fstream>
#include <iostream>

class Lexer
{
public:
	Lexer(std::list<std::string>* list, const char* filename): l_list(list)
	{
		l_source.open(filename, std::ios::in);
		if(l_source.is_open())
			l_good=true;
		else
			l_good=false;
	}

	~Lexer()
	{
		l_good=false;
		l_source.close();
	}	

	bool load_source(const char* filename)
	{
		if(l_good)
		{
			l_source.close();
		}
		l_source.open(filename, std::ios::in);
		if(l_source.is_open())
			l_good=true;
		else
			l_good=false;
	}

	bool lex()
	{
		if(l_good)
		{
			std::string buffer;
			while(std::getline(l_source,buffer))
			{
				if(buffer.at(0) == '/')
					continue;
				bool comment = false;	
				std::string buf = "";
				for(int i = 0; i < buffer.size(); ++i)
				{
					char c = buffer.at(i);
					switch(c)
					{
					case '/': comment = true; continue;
					case ' ': case '\t': case '\r':
					case '\n': continue;
					default: if(comment == false)buf += c; 
					}
				}
				if(buf != "")	
					l_list->push_back(buf);
				comment = false; 
			}
		}
		return false;	
	}
	void close_source()
	{
		l_source.close();
		l_good=false;
	}

	bool good()
	{
		if(l_source.is_open())
			l_good=true;
		else
			l_good=false;
		return l_good;
	}	
private:
	std::list<std::string>* l_list;
	std::ifstream l_source;
	bool l_good;
};
#endif // LEXER_HPP
