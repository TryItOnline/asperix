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


#include "constraint_rule.hpp"


//******************************* CONSTRUCTORS *******************************//

ConstraintRule::ConstraintRule(Body* v, const variableSet& s, Graph& g) : Rule(NULL,v,s,g){}


ConstraintRule::ConstraintRule(BasicLiteral* a,Body* v) : Rule(a,v){}


ConstraintRule::ConstraintRule(Body* v){
    _head = NULL;
    _body = v;
    _variables = _body->getVariables();
    Body::iterator it = v->begin();
    while (it != v->end()) {
        if ((*it)->isNegativeLiteral()) {
            _bodyMinus->addLiteral(*it);
            it++;
        }
        else if ((*it)->isPositiveLiteral()) {
            Literal* l = new PositiveLiteralOfNDR(*static_cast<PositiveLiteralOfNDR*> (*it));
            _bodyPlus->addLiteral(l);
            // replace (*it)  by l in _body (v)
            Literal* old = *it;
            it = v->insert(it,l);   //insert l before position it
            v->erase(++it); //erase old
            delete (old);
        }
        else {  // special literal (relation, affectation)
            _bodyPlus->addLiteral(*it);
            it++;
        }
    }
}


//******************************** DESTRUCTOR ********************************//

ConstraintRule::~ConstraintRule(){}


//************************** OTHER MEMBER FUNCTIONS **************************//

// ordonnancement du corps de la règle TODO 
// initialisation of _recursiveBody with literals which are not already solved (there is none)
// and initialisation of previous and free vars
void ConstraintRule::orderBody(){
    _orderedBody.clear();
    _recursiveBody.clear();
    _bodyMinus->order(_recursiveBody, *_bodyPlus, _orderedBody);    // copie de body+ et body- vers _orderedBody
    _orderedBody.initVars();
}


// create a new definite constraint rule by cloning
Rule* ConstraintRule::createDefiniteClone(){
    // creation of new variables
    MapVariables mapVar;
    for (variableSet::iterator i = _variables.begin(); i != _variables.end(); i++) {
        const std::string& name = (*i)->getUserName();
        Variable* v = new Variable(name);
        mapVar.insert(std::make_pair(*i, v));
    }
    // creation of new rule
    BasicLiteral* h = NULL;
    if (_head)
        h = _head->createPositiveClone(mapVar);
    Body* b = new Body();
    _bodyPlus->addPositiveClone(*b, mapVar);
    _bodyMinus->addPositiveClone(*b, mapVar);
    _clone = new ConstraintRule(h,b);
    return _clone;
}


std::ostream& ConstraintRule::put(std::ostream& os){
    return os << ":-" << *_body << '.' << std::endl;
}
