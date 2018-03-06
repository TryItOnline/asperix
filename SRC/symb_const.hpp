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


#ifndef _SYMB_CONST_HPP_
#define _SYMB_CONST_HPP_

#include <string>

#include "definition.hpp"
#include "func_term.hpp"



/**
 * symbolic constant is a functional term with arity = 0
 * 
 */
class SymbConst : public FuncTerm{

//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        static
        SymbConst* new_SymbConst(const std::string& name);


//******************************* CONSTRUCTORS *******************************//

        SymbConst(Functor*);


//******************************** DESTRUCTOR ********************************//

        virtual ~SymbConst();


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline bool isConst();

        inline bool isSymbConst();

        inline const std::string& getStringValue();

        inline void addVariables(variableSet&);

        virtual inline void addFreeVar(Variable*);

        virtual inline void clearVars();

        virtual inline bool containsVar(Variable*);

        inline bool match(Term* t);

        inline Term* applySubstitution();

        inline void free();

        inline Term* getClone(MapVariables&);

    private:
        inline std::ostream& put(std::ostream&) const;


}; // class SymbConst


//************************** OTHER MEMBER FUNCTIONS **************************//


inline bool SymbConst::isConst(){
    return true;
}


inline bool SymbConst::isSymbConst(){
    return true;
}


inline const std::string& SymbConst::getStringValue(){
    return _functor->getStringValue();
}


inline void SymbConst::addVariables(variableSet&){}


inline void SymbConst::addFreeVar(Variable*){}


inline void SymbConst::clearVars(){}


inline bool SymbConst::containsVar(Variable*){
    return false;
}


inline bool SymbConst::match(Term* t){
    return (this == t);
}


inline Term* SymbConst::applySubstitution(){
    return this;
}


inline void SymbConst::free(){}


inline Term* SymbConst::getClone(MapVariables&){
    return this;
}


std::ostream& SymbConst::put(std::ostream& os) const{
    return os << *_functor;
}



#endif // _SYMB_CONST_HPP_
