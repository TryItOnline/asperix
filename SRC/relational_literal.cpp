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


#include <cstdlib>

#include "functor.hpp"
#include "relational_literal.hpp"



//****************************************************************************//
//                              RelationalLiteral                             //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

RelationalLiteral::RelationalLiteral(){}


RelationalLiteral::RelationalLiteral(Term* t1, Term* t2){
    _args = new Argument();
    _args->push_back(t1);
    _args->push_back(t2);
    _variables = new variableSet;
    // each term adds its own vars to _variables
    t1->addVariables(*_variables); 
    t2->addVariables(*_variables);
    _mark = NON_MARKED;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

bool RelationalLiteral::firstTrueMatchBis(){
    if (_free_vars.empty()) {   // atom is already ground
        Argument* tv = applySubstitutionOnArgs();
        bool ok = isTrueRelation(tv);
        delete tv;
        return ok;
    }
    else
        throw RuntimeExcept(" Non ground term when evaluating relational literal");
}


statusEnum RelationalLiteral::firstSoftMatchBis(statusEnum st){
    if (firstTrueMatchBis())
        return st;
    else
        return NO_;
}


bool RelationalLiteral::nextTrueMatch(){
    return false;
}


statusEnum RelationalLiteral::nextSoftMatch(statusEnum){
    return NO_;
}





// specific relations

//****************************************************************************//
//                                  EqLiteral                                 //
//****************************************************************************//


//************************** OTHER MEMBER FUNCTIONS **************************//

bool EqLiteral::isTrueRelation(Argument* couple){
    Term* t1 = couple->at(0);
    Term* t2 = couple->at(1);
    if (t1->isConst() && t2->isConst()) // numeric constants, symbolic constants or functional terms
        return (t1 == t2);
    else
        throw RuntimeExcept(" Non constant term when evaluating relational literal");
}


Literal* EqLiteral::createPositiveClone(MapVariables& mapVar){
    return new EqLiteral(_args->at(0)->getClone(mapVar), _args->at(1)->getClone(mapVar));
}






//****************************************************************************//
//                                 NeqLiteral                                 //
//****************************************************************************//


//************************** OTHER MEMBER FUNCTIONS **************************//

bool NeqLiteral::isTrueRelation(Argument* couple){
    Term* t1 = couple->at(0);
    Term* t2 = couple->at(1);
    if (t1->isConst() && t2->isConst()) // numeric constants, symbolic constants or functional terms
        return (t1 != t2);
    else
        throw RuntimeExcept(" Non constant term when evaluating relational literal");
}


Literal* NeqLiteral::createPositiveClone(MapVariables& mapVar){
    return new NeqLiteral(_args->at(0)->getClone(mapVar), _args->at(1)->getClone(mapVar));
}






//****************************************************************************//
//                                  GeLiteral                                 //
//****************************************************************************//


//************************** OTHER MEMBER FUNCTIONS **************************//

bool GeLiteral::isTrueRelation(Argument* couple){
    Term* t1 = couple->at(0);
    Term* t2 = couple->at(1);
    if (!t1->isConst() || !t2->isConst())
        throw RuntimeExcept(" Non constant or incorrect term when evaluating relational literal");
    else if (t1->isNumConst()) {
        if (t2->isNumConst())
            return (t1->getIntValue() >= t2->getIntValue());
        else
            return false;
    }
    else if (t1->isSymbConst()) {
        if (t2->isSymbConst()) {   // symbolic constants
            if((t1->getStringValue()[0] == '\"')&&(t2->getStringValue()[0] != '\"'))
				return true;
			else if((t1->getStringValue()[0] != '\"')&&(t2->getStringValue()[0] == '\"'))
				return false;
			else 
				return (t1->getStringValue() >= t2->getStringValue());
		}
        else if (t2->isNumConst())
            return true;
        else
            return false;
    }
    else {  // t1->isFuncTerm()
        if (t2->isFuncTerm()) {
	    if ( (t1->isList()) & !(t2->isList()))
		return true;
	    if ( !(t1->isList()) & (t2->isList()))
		return false;
	    else {
		Functor* f1 = t1->getFunctor();
		Functor* f2 = t2->getFunctor();
		if (f1->getArity() != f2->getArity())
		    return (f1->getArity() >= f2->getArity());
		else if (f1->getStringValue() != f2->getStringValue()) {
		    if((f1->getStringValue()[0] == '\"')&&(f2->getStringValue()[0] != '\"'))
					    return true;
				    else if((f1->getStringValue()[0] != '\"')&&(f2->getStringValue()[0] == '\"'))
					    return false;
				    else 
					    return (f1->getStringValue() >= f2->getStringValue());
			    }
		else {
		    Argument* arg1 = t1->getArgs();
		    Argument* arg2 = t2->getArgs();
		    for (int i = 0, n = f1->getArity(); i < n; ++i)
			if ((*arg1)[i] != (*arg2)[i]) {
			    Argument args;
			    args.push_back((*arg1)[i]);
			    args.push_back((*arg2)[i]);
			    return isTrueRelation(&args);
			}
		    return true;
		}
	    }
        }
        else
            return true;
    }
}


Literal* GeLiteral::createPositiveClone(MapVariables& mapVar){
    return new GeLiteral(_args->at(0)->getClone(mapVar), _args->at(1)->getClone(mapVar));
}






//****************************************************************************//
//                                  GtLiteral                                 //
//****************************************************************************//


//************************** OTHER MEMBER FUNCTIONS **************************//

bool GtLiteral::isTrueRelation(Argument* couple){
    Term* t1 = couple->at(0);
    Term* t2 = couple->at(1);
    if (!t1->isConst() || !t2->isConst())
        throw RuntimeExcept(" Non constant or incorrect term when evaluating relational literal");
    else if (t1->isNumConst()) {
        if (t2->isNumConst())
            return (t1->getIntValue() > t2->getIntValue());
        else
            return false;
    }
    else if (t1->isSymbConst()) {
        if (t2->isSymbConst()) {   // symbolic constants
            if((t1->getStringValue()[0] == '\"')&&(t2->getStringValue()[0] != '\"'))
				return true;
			else if((t1->getStringValue()[0] != '\"')&&(t2->getStringValue()[0] == '\"'))
				return false;
			else 
				return (t1->getStringValue() > t2->getStringValue());
		}
        else if (t2->isNumConst())
            return true;
        else
            return false;
    }
    else {  // t1->isFuncTerm()
        if (t2->isFuncTerm()) {
	    if ( (t1->isList()) & !(t2->isList()))
		return true;
	    if ( !(t1->isList()) & (t2->isList()))
		return false;
	    else {
		Functor* f1 = t1->getFunctor();
		Functor* f2 = t2->getFunctor();
		if (f1->getArity() != f2->getArity())
		    return (f1->getArity() > f2->getArity());
		else if (f1->getStringValue() != f2->getStringValue()) {
		    if((f1->getStringValue()[0] == '\"')&&(f2->getStringValue()[0] != '\"'))
					    return true;
				    else if((f1->getStringValue()[0] != '\"')&&(f2->getStringValue()[0] == '\"'))
					    return false;
				    else 
					    return (f1->getStringValue() > f2->getStringValue());
			    }
		else {
		    Argument* arg1 = t1->getArgs();
		    Argument* arg2 = t2->getArgs();
		    for (int i = 0, n = f1->getArity(); i < n; ++i)
			if ((*arg1)[i] != (*arg2)[i]) {
			    Argument args;
			    args.push_back((*arg1)[i]);
			    args.push_back((*arg2)[i]);
			    return isTrueRelation(&args);
			}
		    return false;
		}
            }
        }
        else
            return true;
    }
}


Literal* GtLiteral::createPositiveClone(MapVariables& mapVar){
    return new GtLiteral(_args->at(0)->getClone(mapVar), _args->at(1)->getClone(mapVar));
}
