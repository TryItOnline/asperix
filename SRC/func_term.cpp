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


#include "func_term.hpp"




//************************** STATIC MEMBER VARIABLES *************************//

// default value
int FuncTerm::_maxDepth = 16;


//************************** STATIC MEMBER FUNCTIONS *************************//

// WARNING : a is deleted if already exists
FuncTerm* FuncTerm::new_FuncTerm(const std::string& s, Argument* a){
    int n = a->size();
    Functor* f = Functor::newFunctor(s, n);
    return FuncTerm::new_FuncTerm(f, a);
}


// WARNING : a is deleted if already exists
FuncTerm* FuncTerm::new_FuncTerm(Functor* f, Argument* a){
    FuncTerm* t;
    variableSet* vars = new variableSet;
    a->addVariables(*vars); // each term adds its own variables to vars set
    if (vars->empty() && (t = f->existGroundTerm(*a))) {    // FuncTerm already exists
        delete a;
        delete vars;
    }
    else {  // term with vars or first occurence of term
        int depth = a->getMaxDepth() + 1;   //depth of the deepest term
        if (depth <= _maxDepth) {
            t = new FuncTerm(f, a, vars, depth);
            if (vars->empty())
                f->insertGroundTerm(*a, t);
            else
                f->insertNonGroundTerm(t);
        }
        else {
            delete vars;
            throw OutOfRangeFuncTerm();
        }
    }
    return t;
}


//******************************* CONSTRUCTORS *******************************//

FuncTerm::FuncTerm(Functor* f, Argument* tv, variableSet* vs, int d)
 : _functor(f), _args(tv), _variables(vs), _depth(d){}


//******************************** DESTRUCTOR ********************************//

FuncTerm::~FuncTerm(){
    delete _args;
    delete _variables;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

Term* FuncTerm::applySubstitution(){
    Argument* args = _args->applySubstitution();
    try {
        Term* t = FuncTerm::new_FuncTerm(_functor, args);
        return t;
    }
    catch (const OutOfRange&) {
        delete args;
        throw;
    }
}


// t is a ground term
bool FuncTerm::match(Term* t){
    if ( !t->isFuncTerm() || (_functor != t->getFunctor()) )
        return false;
    else if ( _args->match(t->getArgs()) )
        return true;
    else {
        free();
        return false;
    }
}


Term* FuncTerm::getClone(MapVariables& mv){
    Argument* args = _args->clone(mv);
    Term* t = FuncTerm::new_FuncTerm(_functor, args);
    return t;
}
