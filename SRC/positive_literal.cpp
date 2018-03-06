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

#include "positive_literal.hpp"




//****************************************************************************//
//                               PositiveLiteral                              //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

PositiveLiteral::PositiveLiteral(){}


PositiveLiteral::PositiveLiteral(const PositiveLiteral& l) : BasicLiteral(l){}


PositiveLiteral::PositiveLiteral(std::string* s, Argument* tv, Graph& g) : BasicLiteral(s, tv, g){}


PositiveLiteral::PositiveLiteral(Predicate* pred, Argument* tv) : BasicLiteral(pred, tv){}


//************************** OTHER MEMBER FUNCTIONS **************************//

// create a new literal by applying current substitution
Literal* PositiveLiteral::createInstance(){
    Argument* tv = applySubstitutionOnArgs();
    PositiveLiteral* l = new PositiveLiteral(getPred(), tv);
    return l;
}


// create a new positive literal by cloning 
PositiveLiteral* PositiveLiteral::createPositiveClone(MapVariables& mapVar){
    Argument* tv = cloneArgs(mapVar);
    PositiveLiteral* l = new PositiveLiteral(getPred(), tv);
    return l;
}






//****************************************************************************//
//                            PositiveLiteralOfNDR                            //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

PositiveLiteralOfNDR::PositiveLiteralOfNDR(const PositiveLiteralOfNDR& l)
 : PositiveLiteral(l), _delta_begin(0), _delta_end(-1){}


PositiveLiteralOfNDR::PositiveLiteralOfNDR(Predicate* pred, Argument* tv)
 : PositiveLiteral(pred, tv), _delta_begin(0), _delta_end(-1){}


//************************** OTHER MEMBER FUNCTIONS **************************//

// create a new literal by applying current substitution
Literal* PositiveLiteralOfNDR::createInstance(){
    Argument* tv = applySubstitutionOnArgs();
    PositiveLiteralOfNDR* l = new PositiveLiteralOfNDR(getPred(), tv);
    return l;
}
