from enum import Enum
from grammar import Grammar


class _BNFParser:
    '''
    bnf  := prod end | prod bnf
    prod := nterm ':=' rhs
    syms := sym | sym syms
    rhs  := syms | syms '|' rhs
    '''

    class Token(Enum):
        END = 1
        SYM = 2
        KEYWORD = 3

    def __init__(self, buf: str):
        self.pos = 0
        self.tokens = self.tokenize(buf)
        self.grammar = Grammar()
        self.parse_bnf()
        self.fix_grammar()

    def peek(self): # return  tokens(SYM) with their end (END)
        if self.pos < len(self.tokens):
            return self.tokens[self.pos] 
        return (self.Token.END, None)

    def get(self, n=1): # get the next token
        result = self.peek()
        self.pos += n
        return result

    def unget(self, n=1): # to reduce the token
        self.pos -= n

    def fix_grammar(self): # update production dic with the new added production
        all_syms = set()
        for prodlist in self.grammar.prods.values(): 
            
            for prod in prodlist:
                all_syms.update(prod.syms)
        self.grammar.terms = all_syms - self.grammar.prods.keys() - set('@')
        

    def parse_rhs(self): # to get the right hans side of the production
        result = list()
        while True:
            token = self.get() #get token a
            if token[0] == self.Token.END: # if END then delete it from tokens
                self.unget()
                return result
            if token[0] == self.Token.KEYWORD: # if KEYWORD check if | then return the list
                if token[1] == '|': # to divide the production into  sperate rules such  A->a|b ==> A->a, A->b
                    return result
                else:
                    raise SyntaxError("Unexpected token " + str(token))
            
            result.append(token[1]) # append the right hans sid to the list

    def parse_prod(self): #get all productions in the our grammar
        token = self.get() # get the non-terminals
        
        if token[0] != self.Token.SYM: # ensure it's a non-terminal
            raise SyntaxError("Nonterminal expected, got " + str(token))
        nterm = token[1] # store the non-terminal such P , B
        
        if not self.grammar.start: # assian the fist non-terminal to start and add condition to not changed every iteration
            self.grammar.start = nterm
            
        token = self.get() # get move to the next token which is the keyword := to get the right hand side by parse_rhs()
        
        if token[0] != self.Token.KEYWORD or token[1] != ':=': # check the next token is keyword to reach the hand rigt side
            raise SyntaxError("Keyword ':=' expected, got " + str(token))

        while True:
            if self.peek()[0] == self.Token.END: # if the end of production then stop 
                return
            prod_list = self.parse_rhs() # get the right hand side of the non-teminal
            
            if not len(prod_list):
                raise SyntaxError("Empty right hand side of production "
                                  "for nonterminal " + nterm)
            self.grammar.add_production(nterm, prod_list) # add it to the grammmer as production

    def parse_bnf(self): #extract the non-terminal
        while True:
            token = self.peek() # traverse over non-terminal token and thier end 
            
            if token[0] == self.Token.END: # if the first token is the end of terminal then check if the second is not the non-terminal token
                if token[1] is None: # if the end of grammar out
                    return
                else: 
                    self.get() # get the next token and it's END 
            else:
                self.parse_prod() # parse the non-terminal only 

# this method is used to define the grammer pattren to identifiy the the pattren of our regular expression
# it recoginze the whitespace and the keyword (| , =: ,) and the intput symbols

    def tokenize(self, buf: str) -> list: 
        result = list()
        import re        # import re module which contain the functions needed for handling patterns and regular expressions.
        #save the common grammar pattern in a variable to compare it later and find the production for the program
        regexp_space = re.compile(r"[ \t]+")  
        #print(regexp_space)
        regexps = (                                     # tuples of pattren for KEYWORD and it's pattren and SYM(symbol) and it's pattren and END (which indicate the end of production(new line \n) ) and it's pattren
            (self.Token.KEYWORD, re.compile(r"\||:=")), # link KEYWORD value with it's pattren
            (self.Token.SYM, re.compile(r"[^ \t\r\n]+")),# link SYM value with it's pattren
            (self.Token.END, re.compile(r"[\r\n]+")),# link END value with it's pattren
        )

        #print("regexps  \n")
        #print(regexps)

        i = 0
        while i < len(buf): # traverse over our grammer 
            # Skip spaces
            
            m = regexp_space.match(buf, i)  # Check if the given productions matches the pattern that was previously defined at index i if true then it return match object else return none
            if m:
                #print(" in tokenize if  \n")   # m= <re.Match object; span=(6, 8), match='  '>  it contain locations at which the match starts and ends and the actual match value.
                #print(m)
                i = m.end()  # update the index to check the seond production
                #print("in token i \n")
                #print(i)
            if i == len(buf): # end the grammar
                break

            for token, regexp in regexps:
                m = regexp.match(buf, i) # find the KEYWARD and SYM and END 
                
                if m:
                    if token == self.Token.SYM: # if token is SYM(terminal and non terminal symbols)


                        if m.group() == r"'\''": # check if the token name has dash
                            result.append((self.Token.SYM, "'")) # add dash as SYM to list result
                        else:
                            result.append((token, m.group())) # else add token whih it's value without dash
                    else:
                        result.append((token, m.group())) # if KEYWORD and END
                    i = m.end() # save the end of match in i such as m= <re.Match object; span=(6, 8), match='  '> ,8 is the end of match
                    break
            else:
                raise SyntaxError("Unknown token at pos {} ({})".format( # if the token not KEYWORD or END or SYM
                    i, buf[i:i + 10]))

        return result


def parse(bnf: str) -> Grammar: # senf the grammar to start recognize it's pattren and retrun the grammar after know it's pattren
    parser = _BNFParser(bnf)
    return parser.grammar


def main():
    #print(parse(_BNFParser.__doc__))



    main()