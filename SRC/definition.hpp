/* *************************************************************************
 * Copyright (C) 2007-2013 
 * Claire Lefèvre, Pascal Nicolas, Stéphane Ngoma, Christopher Béatrix
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * claire.lefevre@univ-angers.fr	christopher.beatrix@univ-angers.fr
 ****************************************************************************/   


#ifndef _DEFINITION_HPP_
#define _DEFINITION_HPP_

#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "exceptions.hpp"


class Argument;
class ArithExpr;
class Changes;
class Functor;
class Literal;
class NumConst;
class Operator;
class Predicate;
class RangeTerm;
class Rule;
class SymbConst;
class Term;
class Variable;


typedef enum {NON_MARKED, MARKED, UNMARKED} markEnum;
typedef enum {PLUS, MINUS, MULT, DIV, MOD, ABS} opEnum;
/// status of a tuple w.r.t. a predicate extension
/// not in the extension (NO_), or in the extension with status :
/// "must be true" (true without support : MBT_), true with support (TRUE_)
/// or true with support after being MBT (TRUE_MBT_)
typedef enum {NO_ = 0, MBT_, TRUE_, TRUE_MBT_} statusEnum;

typedef std::map<int, NumConst*> mapNumConst;
typedef std::map<const std::string, Predicate*> mapPredicate;
typedef std::map<opEnum, Operator*> mapOperator;
typedef std::map<std::pair<Term*,Term*>, RangeTerm*> mapRangeTerm;
typedef std::map<std::string, Variable*> MapStrVar;
typedef std::map<Variable*, Variable*> MapVariables;

typedef std::multimap<int, Rule*> ruleMultimap;

typedef std::set<Rule*> ruleSet;
typedef std::set<Variable*> variableSet;
typedef std::set<ArithExpr*> arithExprSet;
typedef std::set<std::string> stringSet;

typedef std::pair<Predicate*,int> atomType;
typedef std::queue<atomType> atomsQueue;

typedef std::pair<Changes*, bool> chanboolPair;
typedef std::stack<chanboolPair> chanboolStack;

typedef std::stack<int> intStack;
typedef std::stack<bool> boolStack;
typedef std::stack<markEnum> markStack;
typedef std::stack<Term*> termStack;
typedef std::stack<statusEnum> statusStack;



#endif // _DEFINITION_HPP_
