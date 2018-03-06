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


#include <sstream>

#include "variable.hpp"



//************************** STATIC MEMBER VARIABLES *************************//

// initialization of static variable
int Variable::_nextNumber = 0;


//******************************* CONSTRUCTORS *******************************//

Variable::Variable(){}


Variable::Variable(const std::string& s) : _userName(s){
    // generate the next variable name
    std::stringstream ss;
    ss << "__" << _nextNumber++;    // cast int2string
    ss >> _name;
    _subst = NULL;
}


//******************************** DESTRUCTOR ********************************//

Variable::~Variable(){}


//************************** OTHER MEMBER FUNCTIONS **************************//

bool Variable::match(Term* t){
    if (_subst)
        return (_subst == t);
    else
        return (_subst = t);
}
