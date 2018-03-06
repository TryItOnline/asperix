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


#include "fact_rule.hpp"



//******************************* CONSTRUCTORS *******************************//

FactRule::FactRule(BasicLiteral* a, const variableSet& s) throw (LogicExcept){
    try {
        _head = a;
        _body = new Body();
        if (s.empty()) {
		try {
                    Argument* tuple = a->applySubstitutionOnArgs();
                    a->addInstances(*tuple, TRUE_);
                    delete tuple;
                }
                catch (const OutOfRange&) { // out of range funcTerm or numConst
                    // nothing to do
                }
        }
        else
            throw LogicExcept("Non safe rule");
    }
    catch (const ContradictoryConclusion&) {
        throw;
    }
    catch (const LogicExcept&){}
}


//************************** OTHER MEMBER FUNCTIONS **************************//

std::ostream& FactRule::put(std::ostream& os){
    return os << *_head << '.' << std::endl;
}
