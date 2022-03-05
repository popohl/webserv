# Lexer

## Token types
WHITESPACE	-> space & tab
NEWLINE		-> newline & comment
OBRACKET	-> {
CBRACKET	-> }
SEMICOLON	-> ;
DOT			-> .
SLASH		-> /
ON			-> on
OFF			-> off
NAME		-> a name in alphanumeric characters
EXT			-> an extension

# Parser

## Classes

For the tree:
```c++
class AstNode {};

class ConfigFileNode: AstNode
{
	ServerNode *serverList;
}
class ServerNode: AstNode
{
	LocationNode	*locationList;
	RuleNode		*serverRules;
}
class LocationNode: AstNode
{
	RuleNode *locationRules;
}
class RuleNode: AstNode
{
	Token	RuleName;
	Token	*RuleArguments;
}
```

# Interpreter

## Validation

The interpreter needs to validate a few elements:
	- upload\_enable and upload\_path go together, if one is present, the other as well
	- same for cgi\_ext and cgi\_path
