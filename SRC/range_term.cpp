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


#include "num_const.hpp"
#include "range_term.hpp"



//************************** STATIC MEMBER VARIABLES *************************//

mapRangeTerm RangeTerm::_mapRangeTerm;


//************************** STATIC MEMBER FUNCTIONS *************************//

RangeTerm* RangeTerm::new_RangeTerm(Term* lb, Term* rb){
    mapRangeTerm::iterator it = _mapRangeTerm.find(std::make_pair(lb, rb));
    if (it != _mapRangeTerm.end())  // already exists
        return (*it).second;
    else {
        RangeTerm* t = new RangeTerm(lb, rb);
        _mapRangeTerm.insert(std::make_pair(std::make_pair(lb, rb), t));
        return t;
    }
}


void RangeTerm::delete_all(){
    for (mapRangeTerm::iterator it = _mapRangeTerm.begin(); it != _mapRangeTerm.end(); it++)
        delete (*it).second;
    _mapRangeTerm.clear();
}


//******************************* CONSTRUCTORS *******************************//

RangeTerm::RangeTerm(Term* lb, Term* rb) : _left_bound(lb), _right_bound(rb){
    // if interval is fixed, expand it
    if (lb->isNumConst() && rb->isNumConst()) {
        int ri = rb->getIntValue();
        for (int i = lb->getIntValue(); i <= ri; i++) {
            Term* t = NumConst::new_NumConst(i);
            _expansion.push_back(t);
        }
    }
}


//******************************** DESTRUCTOR ********************************//

RangeTerm::~RangeTerm(){}
