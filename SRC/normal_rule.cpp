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


#include "constraint_rule.hpp"
#include "normal_rule.hpp"
#include "program.hpp"



//******************************* CONSTRUCTORS *******************************//

NormalRule::NormalRule(BasicLiteral* a, Body* v, const variableSet& s, Graph& g) : Rule(a, v, s, g){
    Node* n = a->getPred()->getNode();
    for (Body::iterator it = v->begin(); it != v->end(); it++) {
        if ((*it)->isNegativeLiteral())
            n->addNegSucc((*it)->getPred()->getNode());
        else if ((*it)->isPositiveLiteral())
            n->addPosSucc((*it)->getPred()->getNode());
    }
}


NormalRule::NormalRule(BasicLiteral* a, Body* v) : Rule(a, v){}


//************************** OTHER MEMBER FUNCTIONS **************************//

// evaluation of a definite non recursive rule
void NormalRule::evaluate(){
    initMarks(NON_MARKED);
    Literal* lit = firstLiteral();
    bool matchFound = lit->firstTrueMatch();
    do {
        Literal* lit0 = lit;
        if (matchFound) {
            if ( (lit = nextLiteral()) )    // NULL if it was the last one
                matchFound = lit->firstTrueMatch();
            else {
                lit = lit0;
                try {
                    Argument* tuple = getHead()->applySubstitutionOnArgs();
                    getHead()->addInstances(*tuple, TRUE_);
                    delete tuple;
                }
                catch (const OutOfRange&) { // out of range funcTerm or numConst
                    // nothing to do
                }
                lit->free();
                matchFound = lit->nextTrueMatch();
            }
        }
        else if ( (lit = previousLiteral()) ) { // NULL if it was the first one
            lit->free();
            matchFound = lit->nextTrueMatch();
        }
    } while (lit);
}


// evaluation of a definite recursive rule
void NormalRule::evaluateRec(){
    initMarks(NON_MARKED);//
    Literal* lit = firstLiteral();
    Literal* recLit = firstRecursiveLiteral();
    recLit->setMark(MARKED);
    bool matchFound = lit->firstTrueMatch();
    do {
        Literal* lit0 = lit;
        if (matchFound) {
            if ( (lit = nextLiteral()) )    // NULL if it was the last one
                matchFound = lit->firstTrueMatch();
            else {
                lit = lit0;
                try {
                    Argument* tuple = getHead()->applySubstitutionOnArgs();
                    getHead()->addInstances(*tuple, TRUE_);
                    delete tuple;
                }
                catch (const OutOfRange&) { // out of range funcTerm or numConst
                    // nothing to do
                }
                lit->free();
                matchFound = lit->nextTrueMatch();
            }
        }
        else if ( (lit = previousLiteral()) ) { // NULL if it was the first one
            lit->free();
            matchFound = lit->nextTrueMatch();
        }
        else {
            recLit->setMark(UNMARKED);
            if ( (recLit = nextRecursiveLiteral()) ) {
                lit = lit0;                 // first literal
                recLit->setMark(MARKED);    // next recursive literal	
				std::vector<Literal*>::iterator it = std::find(_orderedBody.begin(), _orderedBody.end(), recLit);
				_orderedBody.erase(it);
				_orderedBody.insert(_orderedBody.begin(),recLit); // insert current recursive litteral at the beginning of the body
				_orderedBody.initVars();
				lit = firstLiteral();
                matchFound = lit->firstTrueMatch();
            }
        }
    } while (lit);
}


// soft evaluation of a definite rule containing "_not_p(t)" after predicate "p" has been set solved
void NormalRule::evaluate(atomsQueue& aq){
    Literal* lit = NULL;
    Argument* tuple = NULL;
    try {
        initMarks(NON_MARKED);
        lit = firstLiteral();
        statusEnum matchStatus = lit->firstSoftMatch(TRUE_);
        do {
            Literal* lit0 = lit;
            if (matchStatus) {    // a match is found
                if ( (lit = nextLiteral()) ) {  // NULL if it was the last one
                    matchStatus = lit->firstSoftMatch(matchStatus);
                }
                else {
                    lit = lit0; // last literal
                    try {
                        tuple = getHead()->applySubstitutionOnArgs();
                        getHead()->addInstances(*tuple, matchStatus, aq);
                        delete tuple;
                    }
                    catch (const OutOfRange&) { //out of range funcTerm or numConst
                        // nothing to do
                    }
                    lit->free();
                    matchStatus = lit->nextSoftMatch(previousStatus());
                }
            }
            else if ( (lit = previousLiteral()) ) { // NULL if it was the first one
                lit->free();
                matchStatus = lit->nextSoftMatch(previousStatus());
            }
        } while (lit);
    }
    catch (const ContradictoryConclusion&) {
        delete tuple;
        do {
            lit->free();
            lit = previousLiteral();
        } while(lit);
        throw;  //exception relancée
    }
}


// "soft" evaluation of a definite rule
// recursive literals are those whose body+ contains a predicate whose extension has been modified
void NormalRule::evaluateRec(atomsQueue& aq){
    Literal* lit = NULL;
    Argument* tuple = NULL;
    try {
        initMarks(NON_MARKED);//
        lit = firstLiteral();
        Literal* recLit = firstRecursiveLiteral();
        recLit->setMark(MARKED);
        statusEnum matchStatus= lit->firstSoftMatch(TRUE_);
        do {
            Literal* lit0 = lit;
            if (matchStatus) {  // a match is found (TRUE_ or MBT_)
                if ( (lit = nextLiteral()) ) {  // NULL if it was the last one
                    matchStatus = lit->firstSoftMatch(matchStatus);
                }
                else {
                    lit = lit0;
                    try {
                        tuple = getHead()->applySubstitutionOnArgs();
                        getHead()->addInstances(*tuple, matchStatus, aq);
                        delete tuple;
                    }
                    catch (const OutOfRange&) { //out of range funcTerm or numConst
                        // nothing to do
                    }
                    lit->free();
                    matchStatus = lit->nextSoftMatch(previousStatus());
                }
            }
            else if ( (lit = previousLiteral()) ) { // NULL if it was the first one
                lit->free();
                matchStatus = lit->nextSoftMatch(previousStatus());
            }
            else {
            recLit->setMark(UNMARKED);
                if ( (recLit = nextRecursiveLiteral()) ) {
                    lit = lit0;                 // first literal
                    recLit->setMark(MARKED);    // next recursive literal
					std::vector<Literal*>::iterator it = std::find(_orderedBody.begin(), _orderedBody.end(), recLit);
					_orderedBody.erase(it);
					_orderedBody.insert(_orderedBody.begin(),recLit); // insert current recursive litteral at the beginning of the body
					_orderedBody.initVars();
					lit = firstLiteral();
                    matchStatus = lit->firstSoftMatch(TRUE_);
                }
            }
        } while (lit);
    }
    catch (const ContradictoryConclusion&) {
        delete tuple;
        do {
            lit->free();
            lit = previousLiteral();
        } while(lit);
        throw;  //exception relancée, récupérée par program::propagate
    }
}


// a substitution that makes the rule "true" supported and non-blocked is found
// apply substitution and add head to the model
// return false if head was already in or known to be out 
bool NormalRule::addHead(Program& p, Changes& changes){
    try {
        Argument* headTuple = getHead()->applySubstitutionOnArgs();
        try {
            bool added = getHead()->addInstances(*headTuple, TRUE_, changes.getAtomsQueue());
            delete headTuple;
            return added;
        }
        catch (const ContradictoryConclusion&) {
            // create an instance of the rule with current substitution, and prohibite it
            try {
                prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(p, changes);
                delete headTuple;
                return false;
            }
            catch (const ContradictoryConclusion&) {
            // application of the rule leads to contradiction AND
            // prohibition of the rule leads to contradiction
                delete headTuple;
                throw;
            }
        }
    }
    catch (const OutOfRange&) { // out of range funcTerm or numConst in headTuple
        // nothing to do
        return false;
    }
}


// a substitution that makes the "true" rule supported and non-blocked is found, head has been added to the model
// apply substitution and exclude body- from the model
// return false if contradiction : body- contains head
bool NormalRule::addBodyMinus(Program& p, Changes& changes){
    try {
        _bodyMinus->applySubstitutionAndAddNonInstances(p, changes);
        return true;
    }
    catch (const ContradictoryConclusion&) {
        // create an instance of the rule with current substitution, and prohibite it
        try {
            prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(p, changes);
            return false;
        }
        catch (const ContradictoryConclusion&) {
        // application of the rule leads to contradiction AND
        // prohibition of the rule leads to contradiction
            throw;
        }
    }
}


std::ostream& NormalRule::put(std::ostream& os){
    return os << *_head << ":-" << *_body << '.' << std::endl;
}
