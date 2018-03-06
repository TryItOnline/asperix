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


#include "arith_expr.hpp"
#include "func_term.hpp"
#include "num_const.hpp"
#include "operator.hpp"
#include "range_term.hpp"
#include "symb_const.hpp"
#include "term.hpp"



//************************* STATIC MEMBER FUNCTIONS **************************//

// delete all terms except variables 
// variables are specific to each rule (deleted with the rule)
void Term::delete_all(){
    NumConst::delete_all();
    RangeTerm::delete_all();
    Operator::delete_all();
    ArithExpr::delete_all();
}


//******************************** DESTRUCTOR ********************************//

Term::~Term(){}
