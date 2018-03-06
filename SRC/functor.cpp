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

#include "functor.hpp"
#include "func_term.hpp"



//************************** STATIC MEMBER FUNCTIONS *************************//

/* Searches for a functor with name `name` and arity `arity`, and creates a new
 * one if it doesn't exist
 * Returns the address of the underlying functor
 */
Functor* Functor::newFunctor(const std::string& name, int arity){
    std::ostringstream oss;
    oss << name << '/' << arity;    // cast int2string
    std::string s = oss.str();

    Map::iterator it = _functors().find(s);
    if (it == _functors().end()) {
        Functor* f = new Functor(name, arity);
        it = _functors().insert(std::make_pair(s, f)).first;
    }
    return it->second;
}

// Removes all the functors internally stored
void Functor::deleteAll(){
    for (Map::iterator it = _functors().begin(), end = _functors().end(); it != end; ++it)
        delete it->second;
    _functors().clear();
}


//******************************** DESTRUCTOR ********************************//

Functor::~Functor(){
    for (FuncTermMap::iterator it = _groundTerms.begin(), end = _groundTerms.end(); it != end; ++it)
        delete it->second;
    _groundTerms.clear();

    for (FuncTermSet::iterator it = _nonGroundTerms.begin(), end = _nonGroundTerms.end(); it != end; ++it)
        delete *it;
    _nonGroundTerms.clear();
}


//************************** OTHER MEMBER FUNCTIONS **************************//

/* Searches for a ground FuncTerm with functor `this` and ground argument `a`
 * Returns this FuncTerm, or NULL if it doesn't exist
 */
FuncTerm* Functor::existGroundTerm(const Argument& arg){
    FuncTermMap::iterator it = _groundTerms.find(arg);
    return (it != _groundTerms.end()) ? it->second : NULL;
}


/* Inserts the FuncTerm `term` with functor `this` and ground argument `a` in
 * the set of ground functional terms
 * Returns true iff `term` has been inserted
 */
bool Functor::insertGroundTerm(const Argument& arg, FuncTerm* term){
    return _groundTerms.insert(std::make_pair(arg, term)).second;
}


/* Inserts the FuncTerm `term` with functor `this` in the set of nonground
 * functional terms
 * Returns true iff `term` has been inserted
 */
bool Functor::insertNonGroundTerm(FuncTerm* term){
    return _nonGroundTerms.insert(term).second;
}
