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


#ifndef _LITERAL_HPP_
#define _LITERAL_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "definition.hpp"
#include "graph.hpp"
#include "predicate.hpp"
#include "variable.hpp"


class BasicLiteral;
class PositiveLiteral;



class Literal{   // abstract class - no instance

//******************************* NESTED TYPES *******************************//

    public:
        typedef std::vector<Literal*> Vector;


//***************************** MEMBER VARIABLES *****************************//

    protected:
        Argument* _args;

        /// variables that appear in that atom
        variableSet* _variables;

        /// variables that appear for the first time in body
        variableSet _free_vars;

        /// variables that appear in a previous atom of the body
        variableSet _previous_vars;

        /// NON_MARKED, MARKED, or UNMARKED
        markEnum _mark;

        /// back-track stack for _mark
        markStack _mark_stack;


//******************************* CONSTRUCTORS *******************************//

    public:
        Literal();

        Literal(const Literal&);

        Literal(Argument*);


//******************************** DESTRUCTOR ********************************//

        virtual ~Literal();


//********************************** GETTERS *********************************//

        inline Argument* getArgs();

        inline variableSet* getVariables();

        inline markEnum getMark();


//********************************** SETTERS *********************************//

        virtual inline void setMark(markEnum m);


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream& os, Literal& l);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        ///adds its own variables to vs
        inline void addVariables(variableSet& vs);

        inline void addFreeVar(Variable* v);

        inline bool isFreeVar(Variable* v);

        inline void addPreviousVar(Variable* v);

        inline void clearVars();

        /// free variables of _free_vars
        inline void free();

        /// default method; no sense (error)
        virtual inline Predicate* getPred();

        virtual inline int getSccIndex();

        virtual inline statusEnum getStatus();

        /// return true if instanciated literals *this and *l are the same
        /// default : return false
        virtual inline bool isTheSameInstance(BasicLiteral*);

        /// return true if non-instanciated literals *this and *l are the same
        /// default : return false
        virtual inline bool isTheSameLiteral(BasicLiteral*);

        /// return true if the predicate of this literal appears as label of a node from ns -  default method : return false
        virtual inline bool hasPredIn(Node::Set&);

        /// return true if the predicate of this literal is p
        virtual inline bool hasPred(Predicate*);

        virtual inline bool isPositiveLiteral();

        virtual inline bool isNegativeLiteral();

        /// try to match atom with some true tuple of the extension 
        /// of the predicate ; return true if a match is found
        inline bool firstTrueMatch();

        /// idem for the next matching tuple
        virtual bool nextTrueMatch() = 0;

        /// try to match atom with some true or mbt tuple of the extension 
        /// of the predicate ; return true if a match is found
        inline statusEnum firstSoftMatch(statusEnum);

        /// idem for the next matching tuple
        virtual statusEnum nextSoftMatch(statusEnum) = 0;

        /// return true if atom is in the extension of the predicate (only defined for ground basic literal) - default : no sense
        virtual inline statusEnum isInstance();

        /// create a new literal by applying current substitution - default : no sense
        virtual inline Literal* createInstance();

        /// create a new literal by cloning
        virtual Literal* createPositiveClone(MapVariables&) = 0;

        /// add tuple (or all tuples represented by tuple) to extension of predicate _pred - default : no sense
        virtual inline void addInstances(Argument& tuple, statusEnum);

        /// add tuple (or all tuples represented by tuple) to non-extension of predicate _pred - default : no sense
        virtual inline void addNonInstances(Argument& tuple);

        /// add tuple (or all tuples represented by tuple) to extension of predicate _pred (and index of these tuples in aq) . Return true if at least one tuple has been added  - default : no sense
        virtual inline bool addInstances(Argument& tuple,statusEnum st, atomsQueue& aq);

        /// add tuple (or all tuples represented by tuple) to NONextension of predicate _pred (and in naq queue) - default : no sense
        virtual inline bool addNonInstances(Argument& tuple, atomsQueue& naq);

        inline Argument* applySubstitutionOnArgs();

        /// return a copy of _args where each variable is replaced by its correspondant in mapVar
        inline Argument* cloneArgs(MapVariables&);

        /// (default method) true if atom predicate is in the current SCC
        virtual inline bool recursive(Node::Set&);

        /// true if atom predicate is already solved
        virtual inline bool isSolved();

        /// default : no effect
        virtual inline void setDelta();

        /// default : no effect
        virtual inline void initDelta();

        /// default : no effect (used by PositiveLiteralOfNDR)
        virtual inline void restoreDelta();

        /// default : no effect (used by BasicLiteral)
        virtual inline void recordIndex();

        /// default : no effect (used by BasicLiteral)
        virtual inline void restoreIndex();

        inline void recordMark();

        /// restore _mark and _first/_last_index
        inline void restoreMark();

        /// default : no effect (used by BasicLiteral)
        virtual inline void recordStatus();

        /// default : no effect (used by BasicLiteral)
        virtual inline void restoreStatus();

    protected:
        virtual std::ostream& put(std::ostream&) = 0;

        /// try to match atom with some true tuple of the extension 
        /// of the predicate ; return true if a match is found
        virtual bool firstTrueMatchBis() = 0;

        /// idem for true or mbt tuple of the extension 
        virtual statusEnum firstSoftMatchBis(statusEnum) = 0;


}; // class Literal


//********************************** GETTERS *********************************//

inline Argument* Literal::getArgs(){
    return _args;
}


inline variableSet* Literal::getVariables(){
    return _variables;
}


inline markEnum Literal::getMark(){
    return _mark;
}


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, Literal& a){
    return a.put(os);
}



inline std::ostream& operator << (std::ostream& os, Literal::Vector& av){
    for (Literal::Vector::iterator it = av.begin(); it != av.end(); it++)
        os << ((it == av.begin()) ? "" : ",") << *(*it);
    return os;
}


//********************************** SETTERS *********************************//

// default method
inline void Literal::setMark(markEnum m){
    _mark = m;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// adds its own variables to vs
inline void Literal::addVariables(variableSet& vs){
    for (variableSet::iterator it = _variables->begin(); it != _variables->end(); it++)
        vs.insert(*it);
}


inline void Literal::addFreeVar(Variable* v){
    _free_vars.insert(v);
}


inline bool Literal::isFreeVar(Variable* v){
    return _free_vars.find(v) != _free_vars.end();
}


inline void Literal::addPreviousVar(Variable* v){
    _previous_vars.insert(v);
}


inline void Literal::clearVars(){
    _free_vars.clear();
    _previous_vars.clear();
}


inline void Literal::free(){
    for (variableSet::iterator it = _free_vars.begin(); it != _free_vars.end(); it++)
        (*it)->free();
}


//default method
inline int Literal::getSccIndex(){
    return -1;
}


// default method
inline statusEnum Literal::getStatus(){
    return TRUE_;
}


// return true if instanciated literals *this and *l are the same
// default : return false
inline bool Literal::isTheSameInstance(BasicLiteral*){
    return false;
}


// return true if non-instanciated literals *this and *l are the same
// default : return false
inline bool Literal::isTheSameLiteral(BasicLiteral*){
    return false;
}


// return true if the predicate of this literal appears as label of a node from ns -  default method : return false
inline bool Literal::hasPredIn(Node::Set&){
    return false;
}


// return true if the predicate of this literal is p
inline bool Literal::hasPred(Predicate*){
    return false;
}


inline bool Literal::isPositiveLiteral(){
    return false;
}


inline bool Literal::isNegativeLiteral(){
    return false;
}


// try to match atom with some true tuple of the extension of the predicate ; 
// return true if a match is found ; else return false
// return false if actual substitution on literal leads to out of range integer or function
inline bool Literal::firstTrueMatch(){
    try {
        return firstTrueMatchBis();
    }
    catch (const OutOfRange&) {
        return false;
    }
}


// try to match atom with some true or mbt tuple of the extension of the predicate ; 
// return true if a match is found ; else return false
// return false if actual substitution on literal leads to out of range integer
inline statusEnum Literal::firstSoftMatch(statusEnum st){
    try {
        return firstSoftMatchBis(st);
    }
    catch (const OutOfRange&) {
        return NO_;
    }
}


inline Argument* Literal::applySubstitutionOnArgs(){
    return _args->applySubstitution();
}


// return a copy of _args where each variable is replaced by its correspondant in mapVar
inline Argument* Literal::cloneArgs(MapVariables& mapVar){
    return _args->clone(mapVar);
}


// (default method) true if atom predicate is in the current SCC
inline bool Literal::recursive(Node::Set&){
    return false;
}


// true if atom predicate is already solved
inline bool Literal::isSolved(){
    return true;
}


// default : no effect
inline void Literal::setDelta(){}


// default : no effect
inline void Literal::initDelta(){}


// default : no effect (used by PositiveLiteralOfNDR)
inline void Literal::restoreDelta(){}


// default : no effect (used by BasicLiteral)
inline void Literal::recordIndex(){}


// default : no effect (used by BasicLiteral)
inline void Literal::restoreIndex(){}


inline void Literal::recordMark(){
    _mark_stack.push(_mark);
}


// restore _mark and _first/_last_index
inline void Literal::restoreMark(){
    markEnum m = _mark_stack.top();
    _mark_stack.pop();
    setMark(m); // set mark and set _first_index and _last_index
}


// default : no effect (used by BasicLiteral)
inline void Literal::recordStatus(){}


// default : no effect (used by BasicLiteral)
inline void Literal::restoreStatus(){}


// default method, error
inline Predicate* Literal::getPred(){
    throw RuntimeExcept("Acces to predicate of non-basic literal");
}


// default method, no sense 
inline statusEnum Literal::isInstance(){
    throw RuntimeExcept("Test instance for non-basic literal");
}


// create a new literal by applying current substitution - no sense for non-basic literal
inline Literal* Literal::createInstance(){
    throw RuntimeExcept("create instance for non-basic literal");
}


// default method, no sense  for non-basic literal
inline void Literal::addInstances(Argument&,statusEnum){
    throw RuntimeExcept("Try to add tuple to non-basic literal extension");
}


// default method, no sense for non-basic literal
inline void Literal::addNonInstances(Argument&){
    throw RuntimeExcept("Try to add tuple to non-basic literal non-extension");
}


// default method, no sense for non-basic literal
inline bool Literal::addInstances(Argument&, statusEnum, atomsQueue&){
    throw RuntimeExcept("Try to add tuple to non-basic literal extension");
}


// default method, no sense for non-basic literal
inline bool Literal::addNonInstances(Argument&, atomsQueue&){
    throw RuntimeExcept("Try to add tuple to non-basic literal non-extension");
}



#endif // _LITERAL_HPP_
