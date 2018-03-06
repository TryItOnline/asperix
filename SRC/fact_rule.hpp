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


#ifndef _FACT_RULE_HPP_
#define _FACT_RULE_HPP_

#include "negative_literal.hpp"
#include "rule.hpp"



class FactRule : public Rule{

//******************************* CONSTRUCTORS *******************************//

    public:
        FactRule(BasicLiteral*, const variableSet& s) throw (LogicExcept);


//************************** OTHER MEMBER FUNCTIONS **************************//

        virtual inline bool isNormalRule();

    protected:
        virtual std::ostream& put(std::ostream&);


}; // class FactRule


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool FactRule::isNormalRule(){
    return false;
}



#endif // _FACT_RULE_HPP_
