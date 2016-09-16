/*Copyright (c) <year> <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include <iostream>
#include <string>
#include "Lexer.hpp"
#include "parser.hpp"
#include "command.hpp"

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
      std::cerr << "Usage: hasm <source.asm>" << std::endl;
      return 0;
  }
  
  std::string in_file(argv[1]);
  size_t loc_ext = in_file.find(".asm");
  if(loc_ext == std::string::npos)
  {
	std::cerr << "Invalid file type! hasm takes \".asm\" files." << std::endl;
	return 0;
  }

  std::list<std::string> tokens;
  Lexer lexer(&tokens,argv[1]);
  lexer.lex();
  Symbol symbol_table;
  Parser parse(&tokens,&symbol_table);
  
  std::string out_file = in_file.substr(0,loc_ext);
  out_file += ".hack";
  Command cmd(out_file.c_str());

  while(parse.hasMoreCommands())
  {
	parse.advance();
  }
  parse.reset();
  while(parse.hasMoreCommands()) 
  {
	cmd.compile(parse.commandType(), parse.symbol(),parse.dest(),parse.comp(),parse.jump(), &symbol_table);
	parse.advance();
  }
  cmd.compile(parse.commandType(),parse.symbol(),parse.dest(),parse.comp(),parse.jump(), &symbol_table);
  std::cout << "compiler was successful" << std::endl;
  return 0;
}
