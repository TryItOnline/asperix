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


#include <cstdlib>

#include "literal.hpp"



//******************************* CONSTRUCTORS *******************************//

Literal::Literal(){}


Literal::Literal(const Literal& l){
    _args = new Argument(*l._args);
    _variables = new variableSet(*l._variables);
}


Literal::Literal(Argument* tv) : _mark(NON_MARKED){
    if (tv)
        _args = tv;
    else
        _args = new Argument();
    _variables = new variableSet;
    _args->addVariables(*_variables);   // each term adds its own vars to _variables
}


//******************************** DESTRUCTOR ********************************//

Literal::~Literal(){
    delete _args;
    _variables->clear();
    delete _variables;
}
