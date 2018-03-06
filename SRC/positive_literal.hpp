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


#ifndef _POSITIVE_LITERAL_HPP_
#define _POSITIVE_LITERAL_HPP_


#include "basic_literal.hpp"



//****************************************************************************//
//                               PositiveLiteral                              //
//****************************************************************************//


/**
 * PositiveLiteral is a predicate and an argument list
 */
class PositiveLiteral : public BasicLiteral{

//******************************* CONSTRUCTORS *******************************//

    public:
        PositiveLiteral();

        PositiveLiteral(const PositiveLiteral&);

        PositiveLiteral(std::string* s, Argument* tv, Graph& g);

        PositiveLiteral(Predicate* pred, Argument* tv);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline bool isPositiveLiteral();

        /// create a new literal by applying current substitution
        virtual Literal* createInstance();

        PositiveLiteral* createPositiveClone(MapVariables&);

        /// return true if the predicate of this literal is p
        virtual inline bool hasPred(Predicate* p);

        /// no sense for positive literal of definite rule
        inline void setDelta();

        /// no sense for positive literal of definite rule
        inline void initDelta();


}; // class PositiveLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool PositiveLiteral::isPositiveLiteral(){
    return true;
}


/// return true if the predicate of this literal is p
inline bool PositiveLiteral::hasPred(Predicate* p){
    return (_pred == p);
}


// no sense for positive literal of a definite rule
inline void PositiveLiteral::setDelta(){
    throw RuntimeExcept("set delta for positive literal of a definite rule");
}


// no sense for positive literal of a definite rule
inline void PositiveLiteral::initDelta(){
    throw RuntimeExcept("init delta for positive literal of a definite rule");
}






//****************************************************************************//
//                            PositiveLiteralOfNDR                            //
//****************************************************************************//


/**
 * PositiveLiteralOfNDR is a positive literal that appears in a non-definite rule (NDR) 
 * or a constraint rule
 */
class PositiveLiteralOfNDR : public PositiveLiteral{

//***************************** MEMBER VARIABLES *****************************//

    protected:
        /// index in predicate extension
        int _delta_begin;

        int _delta_end;

        /// back-track stack for _delta_begin and _delta_end
        intStack _delta_stack;


//******************************* CONSTRUCTORS *******************************//

    public:
        PositiveLiteralOfNDR(const PositiveLiteralOfNDR&);

        PositiveLiteralOfNDR(Predicate* pred, Argument* tv);


//************************** OTHER MEMBER FUNCTIONS **************************//

        /// create a new literal by applying current substitution
        virtual Literal* createInstance();

        inline void setMark(markEnum m);

        inline void setDelta();

        inline void initDelta();

        inline void restoreDelta();

    protected:
        inline void recordDelta();


}; // class PositiveLiteralOfNDR


//************************** OTHER MEMBER FUNCTIONS **************************//

inline void PositiveLiteralOfNDR::initDelta(){
    _delta_begin = 0;
    _delta_end = _pred->getEndIndex();
}


inline void PositiveLiteralOfNDR::setDelta(){
    recordDelta();  // record delta values (in _delta_stack) before change
    _delta_begin = _delta_end + 1;
    _delta_end = _pred->getEndIndex();
}


inline void PositiveLiteralOfNDR::setMark(markEnum m){
    switch (_mark = m) {
        case NON_MARKED :
            _first_index = _pred->getBeginIndex();
            _last_index = _delta_end;
            break;
        case MARKED :
            _first_index = _delta_begin;
            _last_index = _delta_end;
            break;
        case UNMARKED :
            _first_index = _pred->getBeginIndex();
            _last_index = _delta_begin - 1;
                break;
    }
}


// call by setDelta
inline void PositiveLiteralOfNDR::recordDelta(){
    _delta_stack.push(_delta_begin);
    _delta_stack.push(_delta_end);
}


inline void PositiveLiteralOfNDR::restoreDelta(){
    _delta_end = _delta_stack.top();
    _delta_stack.pop();
    _delta_begin = _delta_stack.top();
    _delta_stack.pop();
}



#endif // _POSITIVE_LITERAL_HPP_
