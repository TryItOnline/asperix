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


#ifndef _OPERATOR_HPP_
#define _OPERATOR_HPP_

#include <iostream>
#include <cstdlib>

#include "definition.hpp"
#include "num_const.hpp"



//****************************************************************************//
//                                  Operator                                  //
//****************************************************************************//



class Operator{  // abstract class

//************************** STATIC MEMBER VARIABLES *************************//

    private:
        static mapOperator _mapOperator;


//***************************** MEMBER VARIABLES *****************************//

    protected:
        opEnum  _oper;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        static Operator* new_Operator(opEnum oper);

        static void delete_all();


//******************************* CONSTRUCTORS *******************************//

        Operator(opEnum oper);


//******************************** DESTRUCTOR ********************************//

        virtual ~Operator();


//********************************* OPERATOR *********************************//

    private:
        friend inline std::ostream& operator << (std::ostream& os, Operator& op);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        virtual inline NumConst* apply(Term* t1, Term* t2);

        virtual inline NumConst* apply(Term* t);

    private:
        virtual std::ostream& put(std::ostream&) const = 0;


}; // class Operator


//********************************* OPERATOR *********************************//

inline std::ostream& operator << (std::ostream& os, Operator& op){
    return op.put(os);
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// default method
inline NumConst* Operator::apply(Term* , Term* ){
    throw RuntimeExcept("can not apply non binary operator to 2 arguments");
}


// default method
inline NumConst* Operator::apply(Term* ){
    throw RuntimeExcept("can not apply non unary operator to one argument");
}





//****************************************************************************//
//                                PlusOperator                                //
//****************************************************************************//



class PlusOperator : public Operator{

//******************************* CONSTRUCTORS *******************************//

    public:
        PlusOperator(opEnum oper);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline NumConst* apply(Term* t1, Term* t2);

    private:
        inline std::ostream& put(std::ostream& os) const;


}; // class PlusOperator


//************************** OTHER MEMBER FUNCTIONS **************************//

inline NumConst* PlusOperator::apply(Term* t1, Term* t2){
    return NumConst::new_NumConst(t1->getIntValue() + t2->getIntValue());
}


inline std::ostream& PlusOperator::put(std::ostream& os) const{
    return os << '+';
}








//****************************************************************************//
//                                MinusOperator                               //
//****************************************************************************//



class MinusOperator : public Operator{

//******************************* CONSTRUCTORS *******************************//

    public:
        MinusOperator(opEnum oper);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline NumConst* apply(Term* t1, Term* t2);

    private:
        inline std::ostream& put(std::ostream& os) const;


}; // class MinusOperator


//************************** OTHER MEMBER FUNCTIONS **************************//

inline NumConst* MinusOperator::apply(Term* t1, Term* t2){
    return NumConst::new_NumConst(t1->getIntValue() - t2->getIntValue());
}


inline std::ostream& MinusOperator::put(std::ostream& os) const{
    return os << '-';
}





//****************************************************************************//
//                                MultOperator                                //
//****************************************************************************//



class MultOperator : public Operator{

//******************************* CONSTRUCTORS *******************************//

    public:
        MultOperator(opEnum oper);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline NumConst* apply(Term* t1, Term* t2);

    private:
        inline std::ostream& put(std::ostream& os) const;


}; // class MultOperator





//************************** OTHER MEMBER FUNCTIONS **************************//


inline NumConst* MultOperator::apply(Term* t1, Term* t2){
    return NumConst::new_NumConst(t1->getIntValue() * t2->getIntValue());
}


inline std::ostream& MultOperator::put(std::ostream& os) const{
    return os << '*';
}








//****************************************************************************//
//                                 DivOperator                                //
//****************************************************************************//



class DivOperator : public Operator{

//******************************* CONSTRUCTORS *******************************//

    public:
        DivOperator(opEnum oper);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline NumConst* apply(Term* t1, Term* t2);

    private:
        inline std::ostream& put(std::ostream& os) const;


}; // class DivOperator


//************************** OTHER MEMBER FUNCTIONS **************************//

//TODO division par 0
inline NumConst* DivOperator::apply(Term* t1, Term* t2){
	return NumConst::new_NumConst(t1->getIntValue() / t2->getIntValue());
}


inline std::ostream& DivOperator::put(std::ostream& os) const{
    return os << '/';
}





//****************************************************************************//
//                                 ModOperator                                //
//****************************************************************************//



class ModOperator : public Operator{

//******************************* CONSTRUCTORS *******************************//

    public:
        ModOperator(opEnum oper);
	

//************************** OTHER MEMBER FUNCTIONS **************************//

        inline NumConst* apply(Term* t1, Term* t2);

    private:
        inline std::ostream& put(std::ostream& os) const;


}; // class ModOperator


//************************** OTHER MEMBER FUNCTIONS **************************//

//TODO division par 0
inline NumConst* ModOperator::apply(Term* t1, Term* t2){
    return NumConst::new_NumConst(t1->getIntValue() % t2->getIntValue());
}


inline std::ostream& ModOperator::put(std::ostream& os) const{
    return os << " mod ";
}





/********** Unary operators ***************/

//****************************************************************************//
//                                 AbsOperator                                //
//****************************************************************************//



class AbsOperator : public Operator{

//******************************* CONSTRUCTORS *******************************//

    public:
        AbsOperator(opEnum oper);


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline NumConst* apply(Term* t);

    private:
        inline std::ostream& put(std::ostream& os) const;


}; // class AbsOperator


//************************** OTHER MEMBER FUNCTIONS **************************//

inline NumConst* AbsOperator::apply(Term* t){
    return NumConst::new_NumConst( std::abs(t->getIntValue()) );
}


inline std::ostream& AbsOperator::put(std::ostream& os) const{
    return os << "abs ";
};



#endif // _OPERATOR_HPP_
