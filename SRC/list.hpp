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


#ifndef _LIST_HPP_
#define _LIST_HPP_

#include "func_term.hpp"



//****************************************************************************//
//                                    List                                    //
//****************************************************************************//



/**
 * a liste can be empty (cf EmptyList class) or has the form _l(head, tail) 
 * 			where head is a term and tail is a list
 * 
 * input/oputput format for list is [], [t1, t2, .., tn], or [t | l]
 *
 */
class List : public FuncTerm{

//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        /// list functor '_l'
        static Functor* _functor;

        /// empty list
        static List* _empty_list;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        static List* new_List(Argument* a = NULL);

        /// first term is head, second is tail
        static List* new_List(Term*, Term*);

        static List* new_List(Functor* f,Argument* a);

        static inline List* getEmptyList();


//******************************* CONSTRUCTORS *******************************//

        List(Functor* f,Argument* a , variableSet* v, int n);


//******************************** DESTRUCTOR ********************************//

        virtual ~List();


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline bool isList();

        virtual inline bool empty();

        /// return the first element, or NULL if there is none
        virtual inline  Term* head() const;

        /// return the list without its first element, or NULL if there is none
        virtual inline List* tail() const;

        /// t is a ground term
        bool match(Term* t);

        Term* applySubstitution();

        Term* getClone(MapVariables&);

        // specific functions on lists

        /// true if t is member of this list
        virtual bool contains(Term* t);

        /// return the number of elements 
        virtual int length();

        virtual List* reverse();

        virtual List* append(List*);

    private:
        virtual inline std::ostream& put(std::ostream&) const;


}; // class List


//************************** STATIC MEMBER FUNCTIONS *************************//

inline List* List::getEmptyList(){
    return _empty_list;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool List::isList(){
    return true;
}


// default value
inline bool List::empty(){
    return false;
}


// default value
inline Term* List::head() const{
    return getArgs()->at(0);
}


// default value
inline List* List::tail() const{
    return static_cast<List*>( getArgs()->at(1) );
}


inline std::ostream& List::put(std::ostream& os) const{
    os << '[' << *head();
    List* l = tail();
    while (l->isList() && ! l->empty()) {
        os << ',' << *l->head();
        l = l->tail();
    }
    if (!l->isList()) os << '|' << *l;
    return  os << ']';
}





//****************************************************************************//
//                                  EmptyList                                 //
//****************************************************************************//



/**
 * an empty list is a constant _nil
 *
 * input/oputput format is []
 */
class EmptyList : public List{

//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        static EmptyList* new_EmptyList();


//******************************* CONSTRUCTORS *******************************//

        EmptyList(Functor* f);


//******************************** DESTRUCTOR ********************************//

        virtual ~EmptyList();


//************************** OTHER MEMBER FUNCTIONS **************************//

        virtual inline bool empty();

        virtual inline Term* head() const;

        virtual inline List* tail() const;

        inline bool isConst();

        inline void addVariables(variableSet&);

        virtual inline void addFreeVar(Variable*);

        virtual inline void clearVars();

        virtual inline bool containsVar(Variable*);

        inline bool match(Term* t);

        inline Term* applySubstitution();

        inline void free();

        inline Term* getClone(MapVariables&);

        /// true if t is member of this list
        virtual inline bool contains(Term*);

        /// return the number of elements
        virtual inline int length();

        virtual inline List* reverse();

        virtual inline List* append(List* l);

    private:
        virtual inline std::ostream& put(std::ostream&) const;


}; // class EmptyList


//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool EmptyList::empty(){
    return true;
}


inline Term* EmptyList::head() const{
    return NULL;
}


inline List* EmptyList::tail() const{
    return NULL;
}


inline bool EmptyList::isConst(){
    return true;
}


inline void EmptyList::addVariables(variableSet&){}


inline void EmptyList::addFreeVar(Variable*){}


inline void EmptyList::clearVars(){}


inline bool EmptyList::containsVar(Variable*){
    return false;
}


inline bool EmptyList::match(Term* t){
    return (this == t);
}


inline Term* EmptyList::applySubstitution(){
    return this;
}


inline void EmptyList::free(){}


inline Term* EmptyList::getClone(MapVariables&){
    return this;
}


// true if t is member of this list
inline bool EmptyList::contains(Term*){
    return false;
}


// return the number of elements 
inline int EmptyList::length(){
    return 0;
}


inline List* EmptyList::reverse(){
    return this;
}


inline List* EmptyList::append(List* l){
    return l;
}


inline std::ostream& EmptyList::put(std::ostream& os) const{
    return os << "[]";
}



#endif // _LIST_HPP_
