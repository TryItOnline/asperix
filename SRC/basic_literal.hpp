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


#ifndef _BASIC_LITERAL_HPP_
#define _BASIC_LITERAL_HPP_

#include "literal.hpp"



/**
 * BasicLiteral is a predicate and an argument list or the negation of this
 */
class BasicLiteral : public Literal{

//***************************** MEMBER VARIABLES *****************************//

    protected:
        Predicate* _pred;

        /// match status of the current instance of the literal
        /// for a body l1,l2,..ln match status of li is MBT iff at least one literal lj (j<=i) has MBT status, and match status of li is TRUE iff all literals lj (j<=i) have TRUE status
        statusEnum _status;

        /// current index in predicate extension
        int _current_index;

        int _first_index;

        int _last_index;

        /// back-track stack for _status
        statusStack _status_stack;

        /// back-track stack for _current_index
        intStack _index_stack;


//******************************* CONSTRUCTORS *******************************//

    public:
        BasicLiteral();
	
        BasicLiteral(const BasicLiteral&);
	
        BasicLiteral(std::string* s, Argument* tv, Graph& g);
	
        BasicLiteral(Predicate* pred, Argument* tv);


//********************************** GETTERS *********************************//

        inline Predicate* getPred();

        inline statusEnum getStatus();


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline int getSccIndex();

        /// return true if instanciated literals *this and *l are the same
        bool isTheSameInstance(BasicLiteral* l);

        /// return true if non-instanciated literals *this and *l are the same
        bool isTheSameLiteral(BasicLiteral* l);

        /// return true if the predicate of this literal appears as label of a node from ns
        inline bool hasPredIn(Node::Set& ns);

        /// tv are ground terms
        virtual bool match(const Argument* tv);

        /// idem for the next matching tuple
        bool nextTrueMatch();

        /// idem for the next matching tuple
        statusEnum nextSoftMatch(statusEnum);

        ///return true if atom is in the extension of the predicate (only defined for ground basic literal)
        statusEnum isInstance();

        /// create a new literal by applying current substitution
        Literal* createInstance();

        BasicLiteral* createPositiveClone(MapVariables&);

        /// add tuple (or all tuples represented by tuple) to extension of predicate _pred with status st
        virtual inline void addInstances(Argument& tuple, statusEnum st);

        /// add tuple (or all tuples represented by tuple) to non-extension of predicate _pred
        virtual inline void addNonInstances(Argument& tuple);

        /// add tuple (or all tuples represented by tuple) to extension of predicate _pred (and index of these tuples in aq)
        inline bool addInstances(Argument& tuple, statusEnum st, atomsQueue& aq);

        /// add tuple (or all tuples represented by tuple) to NONextension of predicate _pred (and in naq queue)
        virtual inline bool addNonInstances(Argument& tuple, atomsQueue& naq);

        /// true if atom predicate is in the current SCC
        inline bool recursive(Node::Set&);

        /// true if atom predicate is already solved
        inline bool isSolved();

        inline void setMark(markEnum m);

        inline void setDelta();

        inline void initDelta();

        inline void recordIndex();

        inline void restoreIndex();

        inline
        void recordStatus();

        inline void restoreStatus();

    protected:
        virtual std::ostream& put(std::ostream&);

        /// try to match atom with some true tuple of the extension 
        /// of the predicate ; return true if a match is found
        bool firstTrueMatchBis();

        /// idem for true or mbt tuple 
        statusEnum firstSoftMatchBis(statusEnum);


}; // class BasicLiteral


//********************************** GETTERS *********************************//

inline Predicate* BasicLiteral::getPred(){
    return _pred;
}


inline statusEnum BasicLiteral::getStatus(){
    return _status;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline int BasicLiteral::getSccIndex(){
    return getPred()->getNode()->getSccIndex();
}


// return true if the predicate of this literal appears as label of a node from ns
inline bool BasicLiteral::hasPredIn(Node::Set& ns){
    return (ns.find(getPred()->getNode()) != ns.end());
}


// add tuple (or all tuples represented by tuple) to extension of predicate _pred with status stat
inline void BasicLiteral::addInstances(Argument& tuple, statusEnum stat){
    switch (stat) {
        case TRUE_:
            getPred()->addTrueInstance(tuple);
            break;
        case MBT_:
            getPred()->addMbtInstance(tuple);
            break;
        default:
            throw RuntimeExcept("try to add instance with illegal status");
    }
}


// add tuple (or all tuples represented by tuple) to non-extension of predicate _pred
inline void BasicLiteral::addNonInstances(Argument& tuple){
    getPred()->addNonInstance(tuple);
}


// add tuple (or all tuples represented by tuple) to extension of predicate _pred with status stat
// and add index of these tuples in queue aq
// return true if some tuple has been added (or some status modified)
inline bool BasicLiteral::addInstances(Argument& tuple, statusEnum stat, atomsQueue& aq){
    switch (stat) {
        case TRUE_:
            return getPred()->addTrueInstance(tuple, aq);
        case MBT_:
            return getPred()->addMbtInstance(tuple, aq);
        default:
            throw RuntimeExcept("try to add instance with illegal status");
    }
}


// add tuple (or all tuples represented by tuple) to non-extension of predicate _pred
// and add  these tuples in naq queue
// return true if some tuple has been added
inline bool BasicLiteral::addNonInstances(Argument& tuple, atomsQueue& naq){
    return getPred()->addNonInstance(tuple, naq);
}


inline bool BasicLiteral::recursive(Node::Set& ns){
    return hasPredIn(ns);
}


// true if atom predicate is already solved
inline bool BasicLiteral::isSolved(){
    return _pred->isSolved();
}


inline void BasicLiteral::setMark(markEnum m){
    switch (_mark = m) {
        case NON_MARKED:
            _first_index = _pred->getBeginIndex();
            _last_index = _pred->getDeltaEndIndex();
            break;
        case MARKED:
            _first_index = _pred->getDeltaBeginIndex();
            _last_index = _pred->getDeltaEndIndex();
            break;
        case UNMARKED:
            _first_index = _pred->getBeginIndex();
            _last_index = _pred->getDeltaBeginIndex() - 1;
            break;
        }
}


inline void BasicLiteral::setDelta(){}


inline void BasicLiteral::initDelta(){}


inline void BasicLiteral::recordIndex(){
    _index_stack.push(_current_index);
}


inline void BasicLiteral::restoreIndex(){
    _current_index = _index_stack.top();
    _index_stack.pop();
}


inline void BasicLiteral::recordStatus(){
    _status_stack.push(_status);
}


inline void BasicLiteral::restoreStatus(){
    _status = _status_stack.top();
    _status_stack.pop();
}



#endif // _BASIC_LITERAL_HPP_
