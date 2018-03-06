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


#ifndef _FUNC_TERM_HPP_
#define _FUNC_TERM_HPP_

#include <iostream>
#include <string>

#include "arguments.hpp"
#include "functor.hpp"
#include "term.hpp"
#include "variable.hpp"



/**
 * functional term has form f(t1,..tn) where f is a functor  
 *                                       and ti are terms (i>=0)
 * if i==0 , it is a symbolic constant
 * each Functerm is represented by an unique object
 * except those that contain variables (because _free_vars are different for each instance)
 */
class FuncTerm : public Term{

//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        static int _maxDepth;


//***************************** MEMBER VARIABLES *****************************//

    protected:
        Functor* _functor;

        Argument* _args;

        /// variables that appear in that term
        variableSet* _variables;

        /// variables that appear for the first time in the atom
        variableSet _free_vars;

        /// length of the longest branch
        int _depth;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        static FuncTerm* new_FuncTerm(const std::string&, Argument*);

        static FuncTerm* new_FuncTerm(Functor*, Argument*);

        static inline void setMaxDepth(int n);


//******************************* CONSTRUCTORS *******************************//

        FuncTerm(Functor*, Argument*, variableSet*, int);


//******************************** DESTRUCTOR ********************************//

        virtual ~FuncTerm();


//********************************** GETTERS *********************************//

        inline Functor* getFunctor();

        inline Argument* getArgs() const;

        inline int getDepth();


//************************** OTHER MEMBER FUNCTIONS **************************//

        bool isFuncTerm();

        bool isConst();

        /// adds its own vars to vs
        inline void addVariables(variableSet&);

        virtual inline void addFreeVar(Variable* v);

        virtual inline void clearVars();

        /// return true if v in _variables
        virtual inline bool containsVar(Variable* v);

        /// t is a ground term
        bool match(Term* t);

        Term* applySubstitution();

        /// free all variables of _free_vars
        inline void free();

        Term* getClone(MapVariables&);

    private:
        virtual inline std::ostream& put(std::ostream&) const;


}; // class FuncTerm


//************************** STATIC MEMBER FUNCTIONS *************************//

inline void FuncTerm::setMaxDepth(int n){
    _maxDepth = n;
}


//********************************** GETTERS *********************************//

inline Functor* FuncTerm::getFunctor(){
    return _functor;
}


inline Argument* FuncTerm::getArgs() const{
    return _args;
}


inline int FuncTerm::getDepth(){
    return _depth;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool FuncTerm::isFuncTerm(){
    return true;
}


inline bool FuncTerm::isConst(){
    return _variables->empty();
}


// adds its own variables to vs
inline void FuncTerm::addVariables(variableSet& vs){
    for (Argument::iterator it = _args->begin(); it != _args->end(); it++)
        (*it)->addVariables(vs);
}


inline void FuncTerm::addFreeVar(Variable* v){
    _free_vars.insert(v);
    _args->addFreeVar(v);
}


inline void FuncTerm::clearVars(){
    _free_vars.clear();
    _args->clearVars();
}


// return true if v in _variables
inline bool FuncTerm::containsVar(Variable* v){
    return (_variables->find(v) != _variables->end());
}


inline void FuncTerm::free(){
    for (variableSet::iterator it = _free_vars.begin(); it != _free_vars.end(); it++)
        (*it)->free();
}


inline std::ostream& FuncTerm::put(std::ostream& os) const{
    return os << *_functor << *_args;
}



#endif // _FUNC_TERM_HPP_
