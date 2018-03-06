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


#ifndef _EXTPRED_HPP_
#define _EXTPRED_HPP_

#include <string>

#include "arguments.hpp"


#define BUILT_IN(name, pattern) \
bool name(Argument&); \
extern "C" { \
void name(std::string& args, bool (*& f)(Argument&)) \
{ \
    args = #pattern; \
    f = name; \
} \
} \
\
bool name(Argument& args)



#endif // _EXTPRED_HPP_
