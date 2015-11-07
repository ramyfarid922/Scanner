#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
#include <fstream>

using namespace std;

enum char_type {white_space, letter, digit, symbol, left_brace, right_brace, colon, new_line, tab};

enum token_type {special_symbol, number, identifier, reserved_word, other, none};

char_type get_type(char x) {

	if((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')) return letter;
	else if(x >= '0' && x <= '9') return digit;
	else if(x == ' ') return white_space;
	else if(x == '\n') return new_line;
	else if(x == '\t') return tab;
	else if(x == '+' || x == '-' || x == '*' || x == '/' || x == '=' || x == '<' || x == ';') {return symbol;}
	else if(x == ':') return colon;
	else if(x == '{') return left_brace;
	else if(x == '}') return right_brace;
	
}

string stringify_enum(token_type x)
{
	switch(x){
		case special_symbol: return "special symbol"; break;
		case number: return "number"; break;
		case identifier: return "identifier"; break;
		case reserved_word: return "reserved word"; break;
		case other: return "other"; break;
		case none: return "none"; break;
	}
}

class Token {
public:
	token_type category;
	string lexeme;

	void assign_cat(){

		if(lexeme == "if" || lexeme == "then" || lexeme == "else" || lexeme == "end" || lexeme == "repeat" || lexeme == "until" || lexeme == "read" || lexeme == "write"){
			category = reserved_word;
		}
		else if(lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "=" || lexeme == "<" || lexeme == ":" || lexeme == ";" || lexeme == ":="){
			category = special_symbol;
		}
		else if(get_type(lexeme[0])== letter){
			category = identifier;
		}
		else if(get_type(lexeme[0]) == digit){
			category = number;
		}
	}

	void reset(){
		category = none;
		lexeme = "";
	}
};

int main() {	

	vector<Token> tokens_list;
	Token next_token;
	next_token.reset();

	ifstream source_file;	
	source_file.open("tiny_sample_code.txt");

	string whole_file;
	string file_line;

	while(getline(source_file, file_line))
	{
	 	whole_file = whole_file + file_line;
	 	whole_file += "\n";	
	}
   cout << whole_file << endl;
	ofstream output_file;
	output_file.open("scanner_output.txt");

   int i = 0;
	char_type current_type;
	string current_lexeme = "";
	char current_char = whole_file[i];

	 while(i < whole_file.length()) {

	   current_type = get_type(current_char);

		if(current_type == white_space || current_type == new_line || current_type == tab){
	   		i++;
	   		current_char = whole_file[i];
	   		current_type = get_type(current_char);
	   }

	   else if(current_type == letter){
	   		while(current_type == letter)
	   		{
	   			current_lexeme = current_lexeme + current_char;
	   			i++;
	   			current_char = whole_file[i];
	   			current_type = get_type(current_char);
	   		}

	   		next_token.lexeme = current_lexeme;
	   		next_token.assign_cat();
	   		tokens_list.push_back(next_token);
	   		next_token.reset();
	   		current_lexeme = "";
		}
		else if(current_type == digit){
				while(current_type == digit)
				{
					current_lexeme = current_lexeme + current_char;
					i++;
					current_char = whole_file[i];
					current_type = get_type(current_char);
				}

				next_token.lexeme = current_lexeme;
				next_token.assign_cat();
				tokens_list.push_back(next_token);
				next_token.reset();
				current_lexeme = "";
		}
		else if(current_type == colon){
				current_lexeme = current_lexeme + ":";
				i++;
				current_char = whole_file[i];
				current_type = get_type(current_char);
				if(current_char == '=')
				{
					current_lexeme = current_lexeme + "=";
					next_token.lexeme = current_lexeme;
					next_token.assign_cat();
					tokens_list.push_back(next_token);
					next_token.reset();
					current_lexeme = "";
					i++;
					current_char = whole_file[i];
					current_type = get_type(current_char);
				}
				current_lexeme = "";
		}
		else if(current_type == symbol){
		   	current_lexeme = current_lexeme + current_char;
		   	next_token.lexeme = current_lexeme;
		   	next_token.assign_cat();
		  		tokens_list.push_back(next_token);
		  		next_token.reset();
		  		current_lexeme = "";
	   		i++;
	   		current_char = whole_file[i];
	   		current_type = get_type(current_char);
      }
		else if(current_type == left_brace){
		   	while(current_type != right_brace)
		   	{
		   		i++;
		   		current_char = whole_file[i];
		   		current_type = get_type(current_char);
		   	}
		   	i++;
		   	current_char = whole_file[i];
		   	current_type = get_type(current_char);
		}
		else
			i++;
			current_char = whole_file[i];
			current_type = get_type(current_char);
	}// end of while

	cout << "tokens number:" << tokens_list.size() << endl;
	cout << whole_file.length() << endl;
	for(int j = 0; j < tokens_list.size(); j++)
	{
		tokens_list[j].assign_cat();
		output_file << tokens_list[j].lexeme << " --> " << stringify_enum(tokens_list[j].category) << endl;
	}

return 0;

}
