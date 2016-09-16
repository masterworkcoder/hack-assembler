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
  for(std::list<std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
	std::cout << *i << std::endl;
  Parser parse(&tokens);
  
  std::string out_file = in_file.substr(0,loc_ext);
  out_file += ".hack";
  Command cmd(out_file.c_str());
  while(parse.hasMoreCommands()) 
  {
	cmd.compile(parse.commandType(), parse.symbol(),parse.dest(),parse.comp(),parse.jump());
	parse.advance();
  }
  cmd.compile(parse.commandType(),parse.symbol(),parse.dest(),parse.comp(),parse.jump());
  std::cout << "compiler was successful" << std::endl;
  return 0;
}
