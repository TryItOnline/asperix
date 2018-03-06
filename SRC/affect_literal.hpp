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


#ifndef _AFFECT_LITERAL_HPP_
#define _AFFECT_LITERAL_HPP_

#include "literal.hpp"



/**
 * AffectLiteral has form X = t 
 * where X is a Variable and t is a term that must be ground when literal is evaluated
 * AffectLiteral can only appear in the positive body of a rule
 */
class AffectLiteral : public Literal{

//***************************** MEMBER VARIABLES *****************************//

    protected:
        Variable* _left_member;


//******************************* CONSTRUCTORS *******************************//

    public:
        AffectLiteral();

        AffectLiteral(Variable* v, Term* t);


//******************************** DESTRUCTOR ********************************//

        virtual ~AffectLiteral();


//************************** OTHER MEMBER FUNCTIONS **************************//

        /// idem for the next matching tuple
        bool nextTrueMatch();

        /// idem for the next matching tuple
        statusEnum nextSoftMatch(statusEnum);

        Literal* createPositiveClone(MapVariables& mapVar);
	

    protected:
        inline std::ostream& put(std::ostream& os);

        /// try to match atom with some tuple of the extension 
        /// of the predicate ; return true if a match is found
        bool firstTrueMatchBis();

        statusEnum firstSoftMatchBis(statusEnum);
	

}; // class AffectLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline std::ostream& AffectLiteral::put(std::ostream& os){
    return os << *_left_member << '=' << *_args->at(0);
}



#endif // _AFFECT_LITERAL_HPP_
