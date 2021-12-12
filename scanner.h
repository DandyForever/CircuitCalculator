#ifndef yyFlexLexer
#   define yyFlexLexer Scanner_FlexLexer
#   include <FlexLexer.h>
#endif

class Scanner : public yyFlexLexer {
public:
    enum class Token {
        END,
        DEFINE_LINE,
        INCLUDE_LINE,
        INTERNAL_LINE,
        ELEMENT_LINE,
        EDGE_LINE,
        CALL_LINE,
        INVALID_LINE,
        EOL,
    };
    
//    Scanner() {}
    Scanner(std::istream* arg_yyin=0, std::ostream* arg_yyout=0): yyFlexLexer(arg_yyin, arg_yyout) {}
    virtual ~Scanner() {}

    Token get_next_token();
};
