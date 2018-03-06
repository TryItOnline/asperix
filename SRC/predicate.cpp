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
 * @brief Definition of members of the class representing predicates.
 * @author Claire Lefèvre, Pascal Nicolas, Stéphane Ngoma
 */


#include <sstream>

#include "graph.hpp"
#include "negated_predicate.hpp"



//************************** STATIC MEMBER VARIABLES *************************//

mapPredicate Predicate::_mapPredicate;

Predicate * Predicate::_truePredicate = NULL;


//************************** STATIC MEMBER FUNCTIONS *************************//

// Create a new predicate and its negated predicate
Predicate* Predicate::newPredicateBis(const std::string& name, int arity, Graph& graph){
    Predicate *pred = new Predicate(name, arity);
    pred->_node = graph.new_Node(pred);

    // Create the negated predicate
    Predicate *neg = new NegatedPredicate(name, arity);
    pred->setNegatedPredicate(neg);
    neg->setNegatedPredicate(pred);

    neg->initDelta();

    return pred;
}


Predicate* Predicate::newPredicate(const std::string& s, int n, Graph& graph){
    std::ostringstream oss;
    oss << s << '/' << n;   // cast int2string
    std::string sn = oss.str();
    mapPredicate::iterator it = _mapPredicate.find(sn);
    if (it != _mapPredicate.end())
        return it->second;
    else {
        Predicate *p = newPredicateBis(s, n, graph);
        _mapPredicate.insert(std::make_pair(sn, p));

        // Search for the opposite predicate, and link each other if it exists
        if (sn[0] == '-')
            sn.erase(sn.begin());
        else
            sn.insert(sn.begin(), '-');
        it = _mapPredicate.find(sn);
        if (it != _mapPredicate.end()) {
            Predicate *np = it->second;
            p->setOppositePredicate(np);
            np->setOppositePredicate(p);

            // Add the extension of "np" to the non-extension of "p" (in the same order)
            typedef std::vector< std::pair<const Argument *, statusEnum> >::const_iterator  iterator;
            for (iterator it2 = np->_orderedInstances.begin(); it2 != np->_orderedInstances.end(); ++it2)
                p->addNonInstance(*const_cast<Argument *>( it2->first ));
        }

        return p;
    }
}


// create _truePredicate if it does not exist
Predicate* Predicate::newTruePredicate(Graph& g){
    if (!_truePredicate) {
        _truePredicate = newPredicateBis("", 0, g); // no name, arity 0
        // true instance
        Argument tv; 
        _truePredicate->addTrueInstance(tv);   // the only one instance is true
        _truePredicate->setSolved(true);       // no other instance can appear
    }
    return _truePredicate;
}


Predicate* Predicate::getTruePredicate(){
    return _truePredicate;
}


// initialize _show property of each predicate (if different from default = true)
bool Predicate::initShow(bool defaultShow, const stringSet& pset){
    if (!defaultShow) { // set default to false
        for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it)
            it->second->hide();
    }
    // set exceptions to default value
    bool allExist = true;
    for (stringSet::const_iterator i = pset.begin(); i != pset.end(); ++i) {
        mapPredicate::iterator it = _mapPredicate.find(*i);
        if (it != _mapPredicate.end())
            it->second->setShow(!defaultShow);
        else
            allExist = false;
    }
    return allExist;
}


bool Predicate::existsMbtInstance(){
    mapPredicate::iterator it = _mapPredicate.begin();
    while ((it != _mapPredicate.end()) && (it->second->getMbtNumber() == 0))
        ++it;
    return (it != _mapPredicate.end());
}


void Predicate::deleteAll(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        delete it->second->getNegatedPredicate();
        delete it->second;	
    }
    _mapPredicate.clear();
    if (_truePredicate) {
        delete _truePredicate->getNegatedPredicate();
        delete _truePredicate;
        _truePredicate = NULL;
    }
}


void Predicate::printAllInstances(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        it->second->printInstances();
        it->second->printNonInstances();
    }
}


void Predicate::prettyPrintAllInstances(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        if (it->second->isShown())
            it->second->prettyPrintInstances();
    }
    std::cout << std::endl;
}


void Predicate::prettyPrintAllNonInstances(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        if (it->second->isShown())
            it->second->prettyPrintNonInstances();
    }
    std::cout << std::endl;
}


void Predicate::recordExtensions(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        it->second->recordExtension();
        it->second->recordNonExtension();
    }
}


void Predicate::restoreExtensions(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        it->second->restoreExtension();
        it->second->restoreNonExtension();
    }
}


void Predicate::restoreExtensionsWithoutPop(){
    for (mapPredicate::iterator it = _mapPredicate.begin(); it != _mapPredicate.end(); ++it) {
        it->second->restoreExtensionWithoutPop();
        it->second->restoreNonExtensionWithoutPop();
    }
}


//******************************* CONSTRUCTORS *******************************//

Predicate::Predicate(const std::string& s, int n)
 : _name(s), _arity(n), _show(true), _solved(false), _oppositePredicate(NULL), _negatedPredicate(NULL), 
   _instances(), _orderedInstances(), _mbtNumber(0), _deltaBegin(), _deltaEnd(), _indexStack(), _node(NULL){
     
	_headDefiniteRules = new ruleSet();
	_headNonDefiniteRules = new ruleSet();
	_bodyPlusDefiniteRules = new ruleMultimap();
	_bodyPlusNonDefiniteRules = new ruleMultimap();
	_bodyPlusDefiniteConstraints = new ruleSet();
}


//******************************** DESTRUCTOR ********************************//

Predicate::~Predicate(){
    delete _headDefiniteRules;
    delete _headNonDefiniteRules;
    delete _bodyPlusDefiniteRules;
    delete _bodyPlusNonDefiniteRules;
    delete _bodyPlusDefiniteConstraints;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

/* Returns the status of the term vector @term if it's a known instance of this
 * predicate, #NO_ otherwise.
 */
statusEnum Predicate::containInstance(const Argument& term) const{
    std::map<Argument, statusEnum>::const_iterator it = _instances.find(term);
    return (it != _instances.end()) ?
             it->second :               // @term is in the extension
             NO_;
}


/* Returns the status of a term vector @term if it is in _orderedInstance vector
 * in [@begin,@end] index interval.
 * 
 * WARNING: @term can appear first with MBT and next with TRUE_MBT status, return the last one
 */
statusEnum Predicate::containInstance(const Argument& term, int begin, int end) const{
    int index = end;    // Start at the end in order to find first the last status of @term
    while ((index >= begin) && (*(_orderedInstances.at(index).first) != term))
        --index;

    return (index >= begin) ?
             (_orderedInstances.at(index).second) :     // Status of @term
             NO_;
}


/* Returns true if the term vector @term is a known non-instance of this
 * predicate, false otherwise.
 */
bool Predicate::containNonInstance(const Argument& term) const{
    // A non-instance of a predicate is an instance the negated one
    return (_negatedPredicate->containInstance(term) != NO_);
}


void Predicate::printInstances(){
    std::cout << *this << "[solved : " << _solved << ']' << _instances;
}


void Predicate::printNonInstances(){
    _negatedPredicate->printInstances();
}


void Predicate::prettyPrintInstances(){
    for (std::map<Argument, statusEnum>::iterator it = _instances.begin(); it != _instances.end(); ++it)
        std::cout << *this << it->first << ' ';
}


void Predicate::prettyPrintNonInstances(){
    _negatedPredicate->prettyPrintInstances();
}


// record known extension (for back-track)
void Predicate::recordExtension(){
    _indexStack.push(getEndIndex());
}


// record known non_extension (for back-track)
void Predicate::recordNonExtension(){
    _negatedPredicate->recordExtension();
}


// restore previous extension
void Predicate::restoreExtension(){
    restoreExtensionWithoutPop();
    _indexStack.pop();
}


// restore previous non_extension
void Predicate::restoreNonExtension(){
    _negatedPredicate->restoreExtension();
}


// restore previous extension 
void Predicate::restoreExtensionWithoutPop(){
    int endi = _indexStack.top();  // previous end_index for _ordered_instances
    for (int i = getEndIndex(); i > endi; i--) {
        std::pair<const Argument*, statusEnum> tv_s = _orderedInstances.at(i);
        switch (tv_s.second) {  // status
            case MBT_:
                _instances.erase(*tv_s.first);
                --_mbtNumber;
                break;
            case TRUE_:
                _instances.erase(*tv_s.first);
                break;
            case TRUE_MBT_:
                {
                    std::map<Argument, statusEnum>::iterator it = _instances.find(*tv_s.first);
                    it->second = MBT_;  // update status
                }
                ++_mbtNumber;
                break;
            default:
                throw RuntimeExcept("instance with NO_ status in extension of a predicate");
        }
        _orderedInstances.pop_back();
    }
    _deltaBegin = endi + 1;
    _deltaEnd = endi;
}


// restore previous non_extension
void Predicate::restoreNonExtensionWithoutPop(){
    _negatedPredicate->restoreExtensionWithoutPop();
}


// update extension (for back-track)
void Predicate::updateExtension(){
    _indexStack.pop();                 // pop old value
    _indexStack.push(getEndIndex());   // push new one
}


// update non_extension (for back-track)
void Predicate::updateNonExtension(){
    _negatedPredicate->updateExtension();
}
