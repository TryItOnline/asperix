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


#ifndef _TERM_HPP_
#define _TERM_HPP_

#include <iostream>
#include <vector>

#include "definition.hpp"


class Argument;



class Term{ // abstract class

//******************************* NESTED TYPES *******************************//

    public:
        typedef  std::vector<Term*> Vector;


//************************* STATIC MEMBER FUNCTIONS **************************//

        static void delete_all();


//******************************** DESTRUCTOR ********************************//

        virtual ~Term();


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream& os, Term& t);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        /// adds its own vars to vs
        virtual inline void addVariables(variableSet&);

        ///default value
        virtual inline bool containsVar(Variable*);

        ///default method, only defined for FuncTerm
        virtual inline void addFreeVar(Variable*);

        ///default method, only defined for FuncTerm
        virtual inline void clearVars();

        /// t is a ground term
        virtual bool match(Term* t) = 0;

        /// default value
        virtual inline bool isNumConst();

        /// default value
        virtual inline bool isSymbConst();

        /// default value
        virtual inline bool isConst();

        /// default value
        virtual inline bool isArithExpr();

        /// default value
        virtual inline bool isFuncTerm();

        /// default value
        virtual inline bool isList();

        /// default method
        virtual inline Functor* getFunctor();

        /// default method
        virtual inline Argument* getArgs() const;

        /// default value
        virtual inline int getDepth();

        /// only defined for lists 
        virtual inline bool empty();

        /// TODO erreur , pas de valeur entière par défaut
        virtual inline int getIntValue();

        /// default method
        virtual inline const std::string& getStringValue();

        virtual inline Term* applySubstitution();

        virtual inline Term* getClone(MapVariables&);

    private:
        virtual std::ostream& put(std::ostream&) const = 0;


}; // class Term


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, Term& t){
    return t.put(os);
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// adds its own vars to vs
inline void Term::addVariables(variableSet&){}


//default value
inline bool Term::containsVar(Variable*){
    return false;
}


//default method, only defined for FuncTerm
inline void Term::addFreeVar(Variable*){}


// default method, only defined for FuncTerm
inline void Term::clearVars(){}


// default value
inline bool Term::isNumConst(){
    return false;
}


// default value
inline bool Term::isSymbConst(){
    return false;
}


// default value
inline bool Term::isConst(){
    return false;
}


// default value
inline bool Term::isArithExpr(){
    return false;
}


// default value
inline bool Term::isFuncTerm(){
    return false;
}


// default value
inline bool Term::isList(){
    return false;
}


// default method
inline Term* Term::applySubstitution(){
    return this;
}


// default method
inline Term* Term::getClone(MapVariables&){
    return this;
}


inline bool Term::empty(){
    throw RuntimeExcept("test emptyness for non list term");
}


// TODO erreur , pas de valeur entière par défaut
inline int Term::getIntValue(){
    return 0;
}


// default method
inline const std::string& Term::getStringValue(){
    throw RuntimeExcept("get string value for non symbolic constant");
}


// default method
inline Functor* Term::getFunctor(){
    throw RuntimeExcept("get functor for non functional term");
}


// default method
inline Argument* Term::getArgs() const{
    throw RuntimeExcept("get arguments for non functional term");
}


// default value
inline int Term::getDepth(){
    return 0;
}



#endif // _TERM_HPP_
