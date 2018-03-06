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


#include "list.hpp"




//****************************************************************************//
//                                    List                                    //
//****************************************************************************//



//************************** STATIC MEMBER VARIABLES *************************//

Functor* List::_functor = Functor::newFunctor("_l", 2);


List* List::_empty_list = EmptyList::new_EmptyList();


//************************** STATIC MEMBER FUNCTIONS *************************//

// creation of a list with all elements of a
List* List::new_List(Argument* a){
    if (!a)
        return List::_empty_list;
    else {
        List* l = List::_empty_list;
        for (Argument::iterator it = a->end(); it != a->begin(); ) 
            l = List::new_List(*(--it), l);
        return l;
    }
}


// t is the head (term) and l is the tail (list or variable) 
List* List::new_List(Term* t, Term* l){
    Argument* a = new Argument();
    a->push_back(t);
    a->push_back(l);
    return List::new_List(List::_functor, a);
}


// code de FuncTerm::new_FuncTerm(Functor* f,Argument* a) en remplaçant FuncTerm par List
// a contains head and tail
List* List::new_List(Functor* f, Argument* a){
    List* li;
    variableSet* vars = new variableSet;
    a->addVariables(*vars); // each term adds its own variables to vars set
    if (vars->empty() && (li = static_cast<List*>(f->existGroundTerm(*a)))) {   // list already exists
            delete a;
            delete vars;
    }
    else {  // term with vars or first occurence of term
        int depth = a->getMaxDepth() + 1;   //depth of the deepest term
        if (depth <= _maxDepth) {
            li = new List(f,a,vars,depth);
            if (vars->empty())
                f->insertGroundTerm(*a, li);
            else
                f->insertNonGroundTerm(li);
        }
        else {
            delete vars;
            throw OutOfRangeFuncTerm();
        }
    }
    return li;
}


//******************************* CONSTRUCTORS *******************************//

List::List(Functor* f, Argument* a, variableSet* v, int n) : FuncTerm(f, a, v, n){}


//******************************** DESTRUCTOR ********************************//

List::~List(){}


//************************** OTHER MEMBER FUNCTIONS **************************//

Term* List::applySubstitution(){
    Argument* args = _args->applySubstitution();
    try {
        Term* t = List::new_List(_functor, args);
        return t;
    }
    catch (const OutOfRange&) {
        delete args;
        throw;
    }
}


// t is a ground term
bool List::match(Term* t){
    if (!t->isList() || t->empty())
        return false;
    else if ( _args->match(t->getArgs()) ) {
        return true;
    }
    else {
        free();
        return false;
    }
}


Term* List::getClone(MapVariables& mv){
    Argument* args =_args->clone(mv);
    Term* t = List::new_List(_functor, args);
    return t;
}


// t must be ground 
// true if t is member of this list
bool List::contains(Term* t){
    if (head() == t)
        return true;
    else
        return tail()->contains(t);
}


// return the number of elements 
int List::length(){
    return tail()->length() + 1;
}


List* List::reverse(){
    List* l2 = getEmptyList();
    List* l1 = this;
    while (!l1->empty()) {
        l2 = List::new_List(l1->head(), l2);
        l1 = l1->tail();
    }
    return l2;
}


List* List::append(List* l2){
    return List::new_List(head(), tail()->append(l2));
}





//****************************************************************************//
//                                  EmptyList                                 //
//****************************************************************************//



//************************** STATIC MEMBER FUNCTIONS *************************//

EmptyList* EmptyList::new_EmptyList(){ 
    Functor* f = Functor::newFunctor("_nil", 0);
    Argument a;
    EmptyList* t = static_cast<EmptyList*>( f->existGroundTerm(a) );
    if (!t) {   // new symbol
        t = new EmptyList(f);
        f->insertGroundTerm(a, t);
    }
    return t;
}



//******************************* CONSTRUCTORS *******************************//


EmptyList::EmptyList(Functor* f) : List(f, NULL, NULL, 0){}



//******************************** DESTRUCTOR ********************************//


EmptyList::~EmptyList(){}
