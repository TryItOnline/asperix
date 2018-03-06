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


#include "range_literal.hpp"




//******************************* CONSTRUCTORS *******************************//

RangeLiteral::RangeLiteral(){}


RangeLiteral::RangeLiteral(std::string* s, Argument* tv, Graph& g) : BasicLiteral(s, tv, g){}


RangeLiteral::RangeLiteral(Predicate* pred, Argument* tv) : BasicLiteral(pred, tv){}


//******************************** DESTRUCTOR ********************************//

RangeLiteral::~RangeLiteral(){}


//************************** OTHER MEMBER FUNCTIONS **************************//

// add  all tuples represented by 'tv' to extension of predicate _pred
// all terms in tv are ground RangeTerms
void RangeLiteral::addInstances(Argument& tv, statusEnum st){
    std::vector<Argument*>* tuples = expand(tv);
    for (std::vector<Argument*>::iterator it = tuples->begin(); it != tuples->end(); it++) {
        switch (st) {
            case TRUE_ :
                getPred()->addTrueInstance(**it);
                break;
            case MBT_ :
                getPred()->addMbtInstance(**it);
                break;
            default :
                throw RuntimeExcept("try to add instance with illegal status");
        }
        delete (*it);
    }
    delete tuples;
}


// add  all tuples represented by 'tv' to extension of predicate _pred
// all terms in tv are ground RangeTerms
void RangeLiteral::addNonInstances(Argument& tv){
    std::vector<Argument*>* tuples = expand(tv);
    for (std::vector<Argument*>::iterator it = tuples->begin(); it != tuples->end(); it++) {
        getPred()->addNonInstance(**it);
        delete (*it);
    }
    delete tuples;
}


// add  all tuples represented by 'tv' to extension of predicate _pred
//  and add index of these tuples in atoms queue aq
// all terms in tv are ground RangeTerms
bool RangeLiteral::addInstances(Argument& tv, statusEnum st, atomsQueue& aq){
    std::vector<Argument*>* tuples = expand(tv);
    bool added = false;
    for (std::vector<Argument*>::iterator it = tuples->begin(); it != tuples->end(); it++) {
        switch (st) {
            case TRUE_ :
                added = (getPred()->addTrueInstance(**it, aq) || added);
                break;
            case MBT_ :
                added = (getPred()->addMbtInstance(**it, aq) || added);
                break;
            default :
                throw RuntimeExcept("try to add instance with illegal status");
            }
        delete (*it);
    }
    delete tuples;
    return added;
}


// expand ground RangeTerm vector tv 
// return all tuples represented by tv
std::vector<Argument*>* RangeLiteral::expand(Argument& tv){
    std::vector<Argument*>* tuples = new std::vector<Argument*>();
    Argument* tuple = new Argument();
    tuples->push_back(tuple);
    for (Argument::iterator rt_it = tv.begin(); rt_it != tv.end(); rt_it++) {
        std::vector<Argument*>* tuples_bis = new std::vector<Argument*>();
        Term::Vector expansion = dynamic_cast<RangeTerm*>(*rt_it)->getExpansion();
        for (std::vector<Argument*>::iterator tv_it = tuples->begin(); tv_it != tuples->end(); tv_it++) {
            for (Term::Vector::iterator t_it = expansion.begin(); t_it != expansion.end(); t_it++) {
                tuple = new Argument(**tv_it);
                tuple->push_back(*t_it);
                tuples_bis->push_back(tuple);
            }
            delete (*tv_it);
        }
        delete tuples;
        tuples = tuples_bis;
    }
    return tuples;
}


RangeLiteral* RangeLiteral::createPositiveClone(MapVariables& mapVar){
    Argument* tv = cloneArgs(mapVar);
    return (new RangeLiteral(getPred(), tv));
}
