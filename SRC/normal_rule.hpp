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


#ifndef _NORMAL_RULE_HPP_
#define _NORMAL_RULE_HPP_

#include "rule.hpp"


class Program;



class NormalRule : public Rule{

//******************************* CONSTRUCTORS *******************************//

    public:
        ///constructor for normal rules
        NormalRule(BasicLiteral*, Body*, const variableSet&, Graph&);

        NormalRule(BasicLiteral*, Body*);


//************************** OTHER MEMBER FUNCTIONS **************************//

        virtual inline bool isNormalRule();

        inline bool isAutoBlocked();

        /// evaluation of a definite non recursive rule
        void evaluate();

        /// evaluation of a definite recursive rule
        void evaluateRec();

        void evaluate(atomsQueue&);

        ///evaluation of a definite rule
        void evaluateRec(atomsQueue&);

        /// apply substitution and add head to the model
        bool addHead(Program& p, Changes& ch);

        ///apply substitution and exclude body- from the model
        bool addBodyMinus(Program& p, Changes& changes);

        /// for a non definite rule
        inline void setDelta();

    protected:
        virtual std::ostream& put(std::ostream&);


}; // class NormalRule


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool NormalRule::isNormalRule(){
    return true;
}


inline bool NormalRule::isAutoBlocked(){
    return _bodyMinus->contains(_head);
}


// for a non definite rule
inline void NormalRule::setDelta(){
    _bodyPlus->setDelta();
}



#endif // _NORMAL_RULE_HPP_
