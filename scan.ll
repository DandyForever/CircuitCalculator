%option C++ noyywrap yylineno
%option yyclass="Scanner"
%option prefix="Scanner_"
%{
#include "scanner.h"
#undef YY_DECL
#define YY_DECL Scanner::Token Scanner::get_next_token()
#define yyterminate() return Scanner::Token::END;
%}

ws  [ \t]+
ws_opt [ \t]*
alpha [A-Za-z]
dig   [0-9]
bad [^\n"?:;*<>| ,()]
include_line include{ws}(({bad}*\/)|({bad}+{bad}*)){ws_opt}\n?
name {alpha}({alpha}|{dig})*
integer [-+]?{dig}+
num1 [-+]?{dig}+\.?([eE][-+]?{dig}+)?
num2 [-+]?{dig}*\.{dig}+([eE][-+]?{dig}+)?
number {num1}|{num2}
define_line (define{ws}{name}{ws_opt}\()({ws_opt}|{name}{ws_opt}(\,{ws_opt}{name}{ws_opt})*)(\){ws_opt}\n)
internal_line {ws}internal{ws}{name}{ws_opt}\n?
edge_attributes \,{ws_opt}{number}R{ws_opt};{ws_opt}{number}C{ws_opt};{ws_opt}{number}L{ws_opt};?{ws_opt}({number}V{ws_opt},{ws_opt}{number};?)?{ws_opt}\n?
element_edge_line {ws}{name}{ws_opt}--{ws_opt}{name}{ws_opt}{edge_attributes}
edge_line {integer}{ws_opt}--{ws_opt}{integer}{ws_opt}{edge_attributes}
call_line ({name}{ws_opt}\()({ws_opt}|{integer}{ws_opt}(\,{ws_opt}{integer}{ws_opt})*)(\){ws_opt}\n?)
invalid_line .*\n?


%%

{ws_opt}\n?      return Token::EOL;
{include_line}  return Token::INCLUDE_LINE;
{internal_line} return Token::INTERNAL_LINE;
{element_edge_line} return Token::ELEMENT_LINE;
{edge_line} return Token::EDGE_LINE;
{call_line} return Token::CALL_LINE;
{define_line} return Token::DEFINE_LINE;
{invalid_line} return Token::INVALID_LINE;

%%
