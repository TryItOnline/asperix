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


#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include <set>
#include <string>

#include "definition.hpp"
#include "term.hpp"



class Variable : public Term{

//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        /// number for the next new variable
        static int _nextNumber;


//***************************** MEMBER VARIABLES *****************************//

        std::string _userName;

        std::string _name;

        Term* _subst;

        /// back-track stack for substitutions (variables values)
        termStack _subst_stack;


//******************************* CONSTRUCTORS *******************************//

    public:
        Variable();

        explicit Variable(const std::string&);


//******************************** DESTRUCTOR ********************************//

        virtual ~Variable();


//********************************** GETTERS *********************************//

        inline const std::string& getUserName() const;


//********************************* OPERATORS ********************************//

    private:
        friend inline bool operator == (const Variable& v1, const Variable& v2);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        inline void addVariables(variableSet& vs);

        virtual inline bool containsVar(Variable* v);

        /// free the variable
        inline void free();

        bool match(Term* t);

        inline Term* applySubstitution();

        inline Variable* getClone(MapVariables&);

        inline void recordSubstitution();

        inline void restoreSubstitution();

    private:
        inline std::ostream& put(std::ostream&) const;


}; // class Variable


//********************************** GETTERS *********************************//

inline const std::string& Variable::getUserName() const{
    return _userName;
}


//********************************* OPERATORS ********************************//

inline bool operator == (const Variable& v1, const Variable& v2){
    return (v1._subst == v2._subst);
}


inline std::ostream& operator << (std::ostream& os, variableSet& vs){
    for (variableSet::iterator it = vs.begin(); it != vs.end(); it++)
        os << ((it == vs.begin()) ? "" : ",") << *(*it);
    return os;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline void Variable::addVariables(variableSet& vs){
    vs.insert(this);
}


inline bool Variable::containsVar(Variable* v){
    return ((*v) == (*this));
}


inline void Variable::free(){
    _subst = NULL;
}


inline Term* Variable::applySubstitution(){
    if (_subst)
        return _subst;
    else
        return this;
}


inline Variable* Variable::getClone(MapVariables& mapVar){
    MapVariables::iterator it = mapVar.find(this);
    if (it != mapVar.end())
        return (*it).second;
    else
        throw RuntimeExcept("getClone for a variable that does not appear in mapVariables");
}


inline void Variable::recordSubstitution(){
    _subst_stack.push(_subst);
}


inline void Variable::restoreSubstitution(){
    _subst = _subst_stack.top();
    _subst_stack.pop();
}


inline std::ostream& Variable::put(std::ostream& os) const{
    return os << (!_userName.empty() ? _userName : _name);
}



#endif // _VARIABLE_HPP_
