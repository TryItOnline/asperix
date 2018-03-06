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


#ifndef _ARGUMENT_HPP_
#define _ARGUMENT_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "definition.hpp"
#include "term.hpp"
#include "variable.hpp"



/**
 * An argument is a term vector
 * it can be arguments of a predicate or a function
 */
class Argument : public Term::Vector{

//******************************* CONSTRUCTORS *******************************//

    public:
        Argument();

        Argument(const Argument&);


//******************************** DESTRUCTOR ********************************//

        virtual ~Argument();


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream& os, const Argument& tv);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        /// adds its own variables to vs
        inline void addVariables(variableSet& vs);

        inline void addFreeVar(Variable* v);

        inline void clearVars();

        /// try to match argument  with the ground terms arg 
        inline bool match(const Argument* arg);

        Argument* applySubstitution();

        bool isTheSameInstance(Argument* arg);

        /// depth of the deepest term
        int getMaxDepth();

        /// return a copy of _args where each variable is replaced by its correspondant in mapVar
        Argument* clone(MapVariables&);


}; // class Argument


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, const Argument& tv){
    if (!tv.empty()) {
        os << '(';
        for (Argument::const_iterator it = tv.begin(); it != tv.end(); it++)
            os << ((it == tv.begin()) ? "" : ",") << **it;
        os << ')';
    }
    return os;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// adds its own variables to vs
inline void Argument::addVariables(variableSet& vs){
    for (Argument::iterator it = begin(); it != end(); it++)
        (*it)->addVariables(vs);
}


inline void Argument::addFreeVar(Variable* v){
// v appears for the first time in this argument
// search in which term it appears first and add v in _free_vars if it is a functional term  
    Argument::iterator it = begin();
    while ((it != end()) && !(*it)->containsVar(v))
        it++;
    if (it != end())
        (*it)->addFreeVar(v);
    else
        throw RuntimeExcept("addFreeVar");
}


inline void Argument::clearVars(){
    for (Argument::iterator it = begin(); it != end(); it++)
        (*it)->clearVars();
}


// try to match argument with the ground argument arg 
inline bool Argument::match(const Argument* arg){
    Argument::iterator i1 = begin();
    Argument::const_iterator i2 = arg->begin();
    while ((i1 != end()) && (*i1)->match(*i2)){
        i1++;
        i2++;
    }
    if (i1 == end())
        return true;
    else
        return false;
}



#endif // _ARGUMENT_HPP_
