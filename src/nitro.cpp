#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

#include "AST/ASTNodeConstant.hpp"
#include "AST/ASTNodeNil.hpp"
#include "AST/ASTNodeBinary.hpp"
#include "AST/ASTNodeUnary.hpp"
#include "AST/ASTPrettyPrinter.hpp"

using namespace Nitro;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [script]" << std::endl;
		return -10;
	}
	std::filesystem::path script_path{ argv[1] };

	std::cout << "Compiling " << script_path.string() << std::endl;

	std::ifstream infile(script_path);
	if (!infile) {
		std::cerr << "File does not exist. Now exiting." << std::endl;
		return -20;
	}
	std::string source(
		(std::istreambuf_iterator<char>(infile)),
		(std::istreambuf_iterator<char>())
			);

	// Test the lexer
	Lexer lexer(source);


	Token tk;
	while (true) {
		tk = lexer.next();
		if (tk.lexeme.find('\t') != std::string_view::npos) {
			tk.lexeme = "\\t";
		} else if (tk.lexeme.find('\n') != std::string_view::npos) {
			tk.lexeme = "\\n";
		} else if (tk.lexeme.find('\0') != std::string_view::npos) {
			tk.lexeme = "\\0";
		}
		std::cout << "[Type: " << static_cast<int>(tk.type) << ", \"" << tk.lexeme << "\", " << tk.line << ":" << tk.col << "]\n";

		if (tk.type == Token::Type::Eof ||
		    tk.type == Token::Type::Error) {
			break;
		}
	}

	Lexer lexer2(source);
	Parser parser(lexer2);

	auto ast = parser.parse();
	
	if (!ast) {
		std::cerr << "Did not compile" << std::endl;
		std::exit(-10);
	}

	ASTPrettyPrinter printer(std::cout);
	ast->visit(printer);

	return 0;
}

