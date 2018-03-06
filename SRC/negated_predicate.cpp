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

/**
 * @file
 * @brief Definition of members of the class representing not-predicates.
 * @author Claire Lefèvre, Pascal Nicolas, Stéphane Ngoma
 */


#include "negated_predicate.hpp"



//************************** OTHER MEMBER FUNCTIONS **************************//

/* Returns #TRUE_ if a term vector @term is a known negative instance of this
 * predicate, #NO_ otherwise.
 */
statusEnum NegatedPredicate::containInstance(const Argument& term) const{
    /* If a predicate is solved, its positive extension is completely known.
     * A term vector @term is then a negative instance iff @term is not a positive instance.
     */
    if (isSolved()) {   // Return true if @term is not a positive instance
        return (_negatedPredicate->containInstance(term) == NO_) ? TRUE_ : NO_;

    } else {            // Return true if @term is a negative instance
        return (_instances.find(term) != _instances.end()) ? TRUE_ : NO_;
    }
}


/* Returns #TRUE_ if a term vector @term is in _orderedInstance vector in
 * [@begin,@end] index interval
 */
statusEnum NegatedPredicate::containInstance(const Argument& term, int begin, int end) const
{
    if (isSolved()) {   // Solved predicate can't be "recursive".
                        // The full extension is considered
        return (_negatedPredicate->containInstance(term) == NO_) ? TRUE_ : NO_;

    } else {
        int index = begin;
        while ((index <= end) && (*(_orderedInstances.at(index).first) != term))
            ++index;

        return (index <= end) ? TRUE_ : NO_;
    }
}
