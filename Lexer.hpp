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
