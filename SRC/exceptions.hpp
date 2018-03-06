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


#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <iostream>
#include <stdexcept>



class RuntimeExcept : public std::runtime_error{

    public:
        RuntimeExcept(const std::string& what) : std::runtime_error("*** RunTime ERROR : " + what){}

}; // class RuntimeExcept





class LogicExcept : public std::logic_error{

    public:
        LogicExcept(const std::string& what) : std::logic_error("*** ERROR : " + what){}

}; // class LogicExcept





class OutOfRange : public LogicExcept{

    public:
        OutOfRange(const std::string& what = "") : LogicExcept(what){}

}; // class OutOfRange





class OutOfRangeNumConst : public OutOfRange{
    public:
        OutOfRangeNumConst(const std::string& what = "") : OutOfRange(what){}

}; // class OutOfRangeNumConst





class OutOfRangeFuncTerm : public OutOfRange{
    public:
        OutOfRangeFuncTerm(const std::string& what = "") : OutOfRange(what){}

}; // class OutOfRangeFuncTerm





class ContradictoryConclusion : public LogicExcept{
    public:
        ContradictoryConclusion(const std::string& what = "") : LogicExcept(what){}

}; // class ContradictoryConclusion



#endif // _EXCEPTIONS_HPP_
