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


#ifndef _NEGATIVE_LITERAL_HPP_
#define _NEGATIVE_LITERAL_HPP_

#include "basic_literal.hpp"
#include "positive_literal.hpp"



class NegativeLiteral : public BasicLiteral{

//******************************* CONSTRUCTORS *******************************//

    public:
        NegativeLiteral(const NegativeLiteral&);

        NegativeLiteral(const BasicLiteral&);

        NegativeLiteral(Predicate* pred, Argument* tv);


//********************************** SETTERS *********************************//

        inline void setMark(markEnum m);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline bool isNegativeLiteral();

        /// idem for the next matching tuple
        bool nextTrueMatch();

        /// idem for the next matching tuple
        statusEnum nextSoftMatch(statusEnum);

        /// create a new literal by applying current substitution
        virtual Literal* createInstance();

        PositiveLiteral* createPositiveClone(MapVariables& mapVar);

        /// no sense for negative literal
        inline void setDelta();

    protected:
        virtual std::ostream& put(std::ostream&);

        /// try to match atom with some tuple of the extension 
        /// of the predicate ; return true if no match is found
        bool firstTrueMatchBis();

        statusEnum firstSoftMatchBis(statusEnum);


}; // class NegativeLiteral


//********************************** SETTERS *********************************//

inline void NegativeLiteral::setMark(markEnum m){
    _mark = m;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool NegativeLiteral::isNegativeLiteral(){
    return true;
}


// no sense for negative literal 
inline void NegativeLiteral::setDelta(){
    throw RuntimeExcept("set delta for negative literal");
}



#endif // _NEGATIVE_LITERAL_HPP_
