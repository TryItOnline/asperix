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


#include "symb_const.hpp"



//************************** STATIC MEMBER FUNCTIONS *************************//

SymbConst* SymbConst::new_SymbConst(const std::string& name){
    Functor* f = Functor::newFunctor(name, 0);
    Argument a;
    SymbConst* t = static_cast<SymbConst*>( f->existGroundTerm(a) );
    if (!t) {   // new symbol
        t = new SymbConst(f);
        f->insertGroundTerm(a, t);
    }
    return t;
}


//******************************* CONSTRUCTORS *******************************//

SymbConst::SymbConst(Functor* f) : FuncTerm(f, NULL, NULL, 0){}


//******************************** DESTRUCTOR ********************************//

SymbConst::~SymbConst(){}
