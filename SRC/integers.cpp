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


#include "extpred.hpp"
#include "symb_const.hpp"
#include "num_const.hpp"




/**
 * @brief Checks whether a term is an integer.
 * @param[in] args[0] a term.
 * @return @c true iff @a args[0] is an integer.
 */
BUILT_IN(integer, i){
    return args[0]->isNumConst();
}


/**
 * @brief Checks whether an integer is the successor of another one.
 * @param[in] args[0] an integer.
 * @param[out] args[1] an integer.
 * @return @c true iff <tt>@a args[1] = @a args[0] + 1</tt>.
 * @throw OutOfRange if @a args[1] is greater than the maximum allowed integer.
 */
BUILT_IN(succ, io){
    if (!args[0]->isNumConst())
        return false;

    NumConst* n = NumConst::new_NumConst(args[0]->getIntValue() + 1);
    return args[1]->match(n);
}


/**
 * @brief Checks whether an integer is the predecessor of another one.
 * @param[in] args[0] an integer.
 * @param[out] args[1] an integer.
 * @return @c true iff <tt>@a args[1] = @a args[0] - 1</tt>.
 * @throw OutOfRange if @a args[1] is less than the minimum allowed integer.
 */
BUILT_IN(prec, io){
    if (!args[0]->isNumConst())
        return false;

    NumConst* n = NumConst::new_NumConst(args[0]->getIntValue() - 1);
    return args[1]->match(n);
}
