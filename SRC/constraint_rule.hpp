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


#ifndef _CONSTRAINT_RULE_HPP_
#define _CONSTRAINT_RULE_HPP_

#include "rule.hpp"



class ConstraintRule : public Rule{

//******************************* CONSTRUCTORS *******************************//

  public:
        ConstraintRule(Body*, const variableSet&, Graph&);

        ConstraintRule(BasicLiteral*, Body* v);

        ConstraintRule(Body* v);


//******************************** DESTRUCTOR ********************************//

        virtual ~ConstraintRule();


//************************** OTHER MEMBER FUNCTIONS **************************//

        virtual inline bool isNormalRule();

        virtual void orderBody();

        Rule* createDefiniteClone();

        virtual  inline void initDelta();

    protected:
        virtual std::ostream& put(std::ostream&);


}; // class ConstraintRule


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool ConstraintRule::isNormalRule(){
    return false;
}


inline void ConstraintRule::initDelta(){
    _bodyPlus->initDelta();
}



#endif // _CONSTRAINT_RULE_HPP_
