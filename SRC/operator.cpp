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


#include "operator.hpp"




//****************************************************************************//
//                                  Operator                                  //
//****************************************************************************//


//************************** STATIC MEMBER VARIABLES *************************//

mapOperator Operator::_mapOperator;


//************************** STATIC MEMBER FUNCTIONS *************************//

Operator* Operator::new_Operator(opEnum oper){
    mapOperator::iterator it = _mapOperator.find(oper);
    if (it != _mapOperator.end())   // already exists
        return (*it).second;
    else {
        Operator* op;
        switch (oper) {
            case PLUS :
                op = new PlusOperator(oper);
                break;
            case MINUS :
                op = new MinusOperator(oper);
                break;
            case MULT :
                op = new MultOperator(oper);
                break;
            case DIV :
                op = new DivOperator(oper);
                break;
            case MOD :
                op = new ModOperator(oper);
                break;
            case ABS :
                op = new AbsOperator(oper);
                break;
            default :
                throw RuntimeExcept("Unknown arithmetic operator");
        }
        _mapOperator.insert(std::make_pair(oper,op));
        return op;
    }
}


void Operator::delete_all(){
    for (mapOperator::iterator it = _mapOperator.begin(); it != _mapOperator.end(); it++)
        delete (*it).second;
    _mapOperator.clear();
}


//******************************* CONSTRUCTORS *******************************//

Operator::Operator(opEnum oper) : _oper(oper){}


//******************************** DESTRUCTOR ********************************//

Operator::~Operator(){}






//****************************************************************************//
//                                PlusOperator                                //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

PlusOperator::PlusOperator(opEnum oper) : Operator(oper){}






//****************************************************************************//
//                                MinusOperator                               //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

MinusOperator::MinusOperator(opEnum oper) : Operator(oper){}





//****************************************************************************//
//                                MultOperator                                //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

MultOperator::MultOperator(opEnum oper) : Operator(oper){}






//****************************************************************************//
//                                 DivOperator                                //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

DivOperator::DivOperator(opEnum oper) : Operator(oper){}






//****************************************************************************//
//                                 ModOperator                                //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

ModOperator::ModOperator(opEnum oper) : Operator(oper){}






//****************************************************************************//
//                                 AbsOperator                                //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

AbsOperator::AbsOperator(opEnum oper) : Operator(oper){}
