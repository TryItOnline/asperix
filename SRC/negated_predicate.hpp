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
 * @brief Declarations of class representing not-predicates.
 * @author Claire Lefèvre, Pascal Nicolas, Stéphane Ngoma
 */


#ifndef _NEGATED_PREDICATE_HPP_
#define _NEGATED_PREDICATE_HPP_

#include "predicate.hpp"


/**
 * @brief A class to represent not-predicates.
 * 
 * 
 * 
 * @sa Predicate
 */
class NegatedPredicate : public Predicate{

    friend class Predicate;

    
//******************************* CONSTRUCTORS *******************************//
    
    protected:
        /**
         * @brief Construct a NegatedPredicate.
         * @param name the predicate's name.
         * @param arity the predicate's arity.
         * 
         * @note The string @c "__not__" is prepend to @a name in order to
         *       differenciate this predicate from the "normal" one.
         * 
         * @sa Predicate::Predicate(const std::string&, int)
         */
        NegatedPredicate(const std::string& name, int arity)
         : Predicate("__not__" + name, arity){}

         
//********************************** SETTERS *********************************//

    public:
        /* Marks a predicate as solved or not solved.
         * - solved: true if this predicate is solved, false otherwise.
         */
        virtual inline void setSolved(bool solved);


//************************** OTHER MEMBER FUNCTIONS **************************//

        /**
         * @brief Tests whether a term vector is a negative instance of a
         *        predicate.
         * @param term a term vector.
         * @retval #TRUE_ if @a term is a known negative instance of this
         *         predicate.
         * @retval #NO_ otherwise.
         * 
         * @sa containInstance(const Argument&, int, int),
         *     Predicate::containNonInstance(const Argument&)
         */
        virtual statusEnum containInstance(const Argument& term) const;

        /**
         * @brief .
         * @param term .
         * @param begin .
         * @param end .
         * @return .
         */
        virtual statusEnum containInstance(const Argument& term, int begin, int end) const;


}; // class NegatedPredicate


//********************************** SETTERS *********************************//

// Marks a predicate as solved or not solved
inline void NegatedPredicate::setSolved(bool solved){
    _solved = solved;
}



#endif // _NEGATED_PREDICATE_HPP_
