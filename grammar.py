
#containers are Tuple, List, Dictionary, etc.
from collections import defaultdict 

class Grammar:
    def __init__(self):
        self.start = None
        self.terms = set()   # terminals
        # prods[nterm] = list of Production objects
        self.prods = defaultdict(list) # new empty list 

    def add_production(self, nterm, syms):
        production = Production(nterm, syms)
        self.prods[nterm].append(production)# add prod in list 

    def is_nonterminal(self, symbol: str) -> bool: # if symbol non termminal  in prod
        return symbol in self.prods

    def is_terminal(self, symbol: str) -> bool:# if symbol  terminal  in list of term 
        return symbol in self.terms

    def get_alt_nonterminal(self, nonterm: str) -> str:
        nonterm += "'"
        while nonterm in self.prods: # loop in prod for non term with ' dash 
            nonterm += "'"
        return nonterm

    def get_start_prodctions(self): # get a star symbol
        return self.prods[self.start]

    def duplicate(self): # to find duplicate in pro
        from copy import deepcopy
        return deepcopy(self)

    def __str__(self): # print 
        result = "Grammar:\n"
        result += "  Start: " + self.start
        result += "\n  Terminals: " + " ".join(self.terms)
        result += "\n  Nonterminals: " + " ".join(self.prods.keys())
        result += "\n  Productions:"
        for prodlist in self.prods.values(): # loop in values of prods 
            for prod in prodlist:           # ??
                result += "\n    " + str(prod)
        return result


class Production: # remove eps from  non term 
    def __init__(self, nterm: str, syms: list):
        self.nterm = nterm                      
        self.syms = Production.remove_eps(syms) 

    def __eq__(self, other): # if non term and not eps
        return self.nterm == other.nterm and self.syms == other.syms

    def __ne__(self, other): # ??
        return not self == other

    def __hash__(self): # add all يجمعهم بمكان واحد
        result = hash(self.nterm)
        for sym in self.syms:
            result ^= hash(sym)
        return result

    @staticmethod
    def remove_eps(syms: list) -> list:
        for sym in syms: #loop in list 
            if sym != '@': # if there @ 
                break
        else:
            return ['@']
        return list(filter(lambda x: x != '@', syms)) # remove

    def __repr__(self): # return production 
        return "Production({}, {})".format(self.nterm, self.syms)

    def __str__(self): #return production 
        return "{} → {}".format(self.nterm, " ".join(self.syms))