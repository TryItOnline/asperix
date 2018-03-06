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


#ifndef _RANGE_LITERAL_HPP_
#define _RANGE_LITERAL_HPP_

#include "basic_literal.hpp"
#include "range_term.hpp"



/** RangeLiteral has form P(t1,t2,..) where ti are RangeTerms
 *  RangeLiteral can only appear as head of a rule
 */
class RangeLiteral : public BasicLiteral {

//******************************* CONSTRUCTORS *******************************//

    public:
        RangeLiteral();

        RangeLiteral(std::string* s, Argument* tv, Graph& g);

        RangeLiteral(Predicate* pred, Argument* tv);


//******************************** DESTRUCTOR ********************************//

        virtual ~RangeLiteral();


//************************** OTHER MEMBER FUNCTIONS **************************//

        /// not defined for range literal TODO devrait être défini
        inline statusEnum isInstance();

        /// create a new literal by applying current substitution, no use for a head literal
        virtual inline Literal* createInstance();

        RangeLiteral* createPositiveClone(MapVariables& mapVar);

        /// add tuple (or all tuples represented by tuple) to extension of predicate _pred
        virtual void addInstances(Argument& tuple, statusEnum st);

        /// add tuple (or all tuples represented by tuple) to extension of predicate _pred
        virtual void addNonInstances(Argument& tuple);

        virtual bool addInstances(Argument&, statusEnum st, atomsQueue&);

        inline bool blindAddNonInstances(Argument&, atomsQueue&);

        std::vector<Argument*>* expand(Argument& tv);

        /// idem for the next matching tuple
        inline bool nextTrueMatch();

        /// idem for the next matching tuple
        inline statusEnum nextSoftMatch(statusEnum);

        /// true if atom predicate is in the current SCC
        /// i.e. the predicate is not already solved
        inline bool recursive();

        inline void setMark(markEnum m);

        /// no sense for a head literal 
        inline void setDelta();

        /// no sense for a head literal 
        inline void initDelta();

    private:
        std::vector<Argument*>& expand(RangeTerm::Vector& tv);

    protected:
        /// try to match atom with some tuple of the extension 
        /// of the predicate ; return true if a match is found
        /// no sense for a RangeLiteral which is head literal
        inline bool firstTrueMatchBis();

        inline statusEnum firstSoftMatchBis(statusEnum);


}; // class RangeLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

// not defined for a range literal 
inline statusEnum RangeLiteral::isInstance(){
    throw RuntimeExcept("test instance for a head range literal");
}


// create a new literal by applying current substitution, no use for a head literal
inline Literal* RangeLiteral::createInstance(){
    throw RuntimeExcept("create instance for a head range literal");
}


/* the following members have no sense for head literals 
 */

// try to match atom with some tuple of the extension 
// of the predicate ; return true if a match is found
// no sense for a RangeLiteral which is head literal
inline bool RangeLiteral::firstTrueMatchBis(){
    throw RuntimeExcept("Request match for a head range literal");
}


inline statusEnum RangeLiteral::firstSoftMatchBis(statusEnum){
    throw RuntimeExcept("Request match for a head range literal");
}


// idem for the next matching tuple
inline bool RangeLiteral::nextTrueMatch(){
    throw RuntimeExcept("Request match for a head range literal");
}


// idem for the next matching tuple
inline statusEnum RangeLiteral::nextSoftMatch(statusEnum){
    throw RuntimeExcept("Request match for a head range literal");
}


// true if atom predicate is in the current SCC
// i.e. the predicate is not already solved
inline bool RangeLiteral::recursive(){
    throw RuntimeExcept("Test recursive for a head range literal");
}


inline void RangeLiteral::setMark(markEnum){
    throw RuntimeExcept("Set mark for a head range literal");
}


inline bool RangeLiteral::blindAddNonInstances(Argument&, atomsQueue&){
    throw RuntimeExcept("try to add non-instances of a head range literal");
}


// no sense for a head literal 
inline void RangeLiteral::setDelta(){
    throw RuntimeExcept("set delta for a head range literal");
}


// no sense for a head literal 
inline void RangeLiteral::initDelta(){
    throw RuntimeExcept("init delta for a head range literal");
}



#endif // _RANGE_LITERAL_HPP_
