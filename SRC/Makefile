############################################################################
# Copyright (C) 2007-2013 
# Claire Lefèvre, Pascal Nicolas, Stéphane Ngoma, Christopher Béatrix
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# claire.lefevre@univ-angers.fr  	christopher.beatrix@univ-angers.fr
############################################################################# 

CXX = g++
CXXFLAGS = -std=c++03 -O3 -W -Wall -fno-strict-aliasing
LDFLAGS = -W -Wall -fno-strict-aliasing -g
SRCS = parser.lex.cpp parser.cpp \
       num_const.cpp symb_const.cpp variable.cpp operator.cpp arith_expr.cpp \
       range_term.cpp list.cpp func_term.cpp arguments.cpp functor.cpp term.cpp \
       predicate.cpp negated_predicate.cpp \
       literal.cpp basic_literal.cpp positive_literal.cpp negative_literal.cpp relational_literal.cpp affect_literal.cpp range_literal.cpp \
       built_in_literal.cpp built_in_predicate.cpp \
       fact_rule.cpp constraint_rule.cpp normal_rule.cpp rule.cpp \
       program.cpp changes.cpp graph.cpp \
       main.cpp
OBJS = $(SRCS:.cpp=.o)


FLEX = flex
FLEXFLAGS = 
BISON = bison
BISONFLAGS = 

.SUFFIXES:


all: ASPeRiX ../lists.so

.PHONY: all clean dep distclean


ASPeRiX: $(OBJS)
	$(CXX) $(LDFLAGS) -rdynamic -o "../asperix" $^ -lfl -ldl
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

../%.so: %.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -Wl,-soname,$@ -o $@ $< -lc


clean:
	rm -f $(OBJS)
	rm -f *~

distclean: clean
	rm -f parser.lex.hpp parser.lex.cpp
	rm -f parser.hpp parser.cpp
	rm -f make.dep


dep: parser.lex.cpp parser.cpp
	$(CXX) -MM $(SRCS) > make.dep
	$(CXX) -MM -MT ../lists.so lists.cpp >> make.dep

-include make.dep



%.lex.hpp %.lex.cpp: %.lex
	$(FLEX) --header-file=$*.lex.hpp -o$*.lex.cpp $(FLEXFLAGS) $<

%.hpp %.cpp: %.ypp
	$(BISON) --defines=$*.hpp -o$*.cpp $(BISONFLAGS) $<


parser.lex.cpp: parser.hpp
parser.cpp: parser.lex.hpp
