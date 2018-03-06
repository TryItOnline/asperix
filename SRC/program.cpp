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


#include "functor.hpp"
#include "program.hpp"



//******************************* CONSTRUCTORS *******************************//

Program::Program(){
    _answer_number  = 0;
    _choice_points = 0;
    _bad_choice = 0;
}


//******************************** DESTRUCTOR ********************************//

Program::~Program(){
    for (ruleSet::iterator it = _fact_rules.begin(); it != _fact_rules.end(); it++)
        delete (*it);
    _fact_rules.clear();
    for (ruleSet::iterator it = _definite_rules.begin(); it != _definite_rules.end(); it++)
        delete (*it);
    _definite_rules.clear();
    for (ruleSet::iterator it = _normal_rules.begin(); it != _normal_rules.end(); it++)
        delete (*it);
    _normal_rules.clear();
    for (ruleSet::iterator it = _constraint_rules.begin(); it != _constraint_rules.end(); it++)
        delete (*it);
    _constraint_rules.clear();
    Predicate::deleteAll();
    Term::delete_all();
    Functor::deleteAll();
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// process locally stratified literals (in a NDrule, locally stratified literals are negative literals with a sccIndex less than sccIndex of the head of the rule)
// that is, shift them from body- (not p(t)) to body+ (__not__p(t))
// and initialize the multimap of sccIndexes associated with their NDrules
void Program::initNonDefiniteRules(){
    for (ruleSet::iterator it = _normal_rules.begin(); it != _normal_rules.end(); ++it) {
        int ruleScc = (*it)->getSccIndex();
        if ((*it)->getBodyMinus()->containsOnePredNotFrom(ruleScc)) {
        // there is a negative literals with a sccIndex less than ruleScc
            if ((*it)->getBodyMinus()->containsOnePredFrom(ruleScc)) {
            // and there is also a negative literals in the same scc than the rule
                (*it)->shiftLocallySolvedLiteralsFromBMinus2BPlus(ruleScc);
                addNonDefiniteRule(ruleScc, *it);
            }
            else {
            // all negative literals are locally stratified
                shiftRuleFromNDtoDWithoutErasing(*it);
            }
        }
        else {
        // there is no locally stratified negative literal
            addNonDefiniteRule(ruleScc, *it);
        }
    }
    initBodyPlusRules();
}


// add pair to the _non_definite_rules multimap 
// and initialize BodyPlusRules maps of predicates
void Program::addNonDefiniteRule(int sccIndex, Rule* r){
    _non_definite_rules.insert(std::make_pair(sccIndex, r));
    r->getBodyPlus()->addBodyPlusNonDefiniteRule(sccIndex, r);
    Rule* clone = r->getClone();
    clone->getBodyPlus()->addBodyPlusDefiniteRule(sccIndex, clone);
}


// initialize BodyPlusRules maps of predicates (rules in which the predicate appears in body-)
void Program::initBodyPlusRules(){
    // definite rules
    for (ruleSet::iterator it = _definite_rules.begin(); it != _definite_rules.end(); ++it)
        (*it)->getBodyPlus()->addBodyPlusDefiniteRule(*it);
    // constraint rules
    for (ruleSet::iterator it = _constraint_rules.begin(); it != _constraint_rules.end(); ++it) {
        Rule* clone = (*it)->getClone();
        clone->getBodyPlus()->addBodyPlusDefiniteConstraint(clone);
    }
}


// evaluation of the definite part of the program
void Program::evaluate(){
    _graph.computeSCC();
    bool ok = _graph.firstScc(_currentScc);
    while (ok){
        ruleSet recRules;   // recursive rules of current SCC
        ruleSet exitRules;  // exit rules of current SCC
        Node::Set ns;       // nodes of current SCC
        _graph.getRulesFromScc(_currentScc, recRules, exitRules, *this);
        _graph.getNodesFromScc(_currentScc, ns);
        // evaluation of exit rules
        for (ruleSet::iterator i = exitRules.begin(); i != exitRules.end(); i++) {
            (*i)->orderBody(ns);
            (*i)->evaluate();
        }
        // evaluation of recursive rules
        _graph.initDelta(_currentScc);
        do {
            for (ruleSet::iterator i = recRules.begin(); i != recRules.end(); i++) {
				(*i)->orderBody(ns);
                (*i)->evaluateRec();
			}
            _graph.setDelta(_currentScc);
        } while (! _graph.emptyDelta(_currentScc));

        _graph.setSolvedIfItIsTheCase(_currentScc);
        ok = _graph.nextScc(_currentScc);
    }
}


// re-evaluation of the definite part of the program after atoms of _atoms_queue are added
// if propagation leads to contradictory conclusion, restore state before propagation
bool Program::propagate(Changes& changes){
    try {
        propagateBis(changes);
        return true;    // everything is ok
    }
    catch (const ContradictoryConclusion&) {
        changes.retropropagate(*this);
        return false;   // propagation leads to contradictory conclusion
    }
}


// re-evaluation of the definite part of the program after atoms of _atoms_queue and _not_atoms_queue are added
void Program::propagateBis(Changes& changes){
    atomsQueue& aq = changes.getAtomsQueue();
    std::pair<Predicate*,int> p_i;
    while (!aq.empty()){
        p_i = aq.front();   // atom to propagate
        aq.pop();
        p_i.first->setDelta(p_i.second);
        ruleMultimap* rulesm = p_i.first->getBodyPlusDefiniteRules();   // rules whose body+ contains literal with predicate of p_i
        // evaluation of rules
        for (ruleMultimap::iterator i = rulesm->begin(); i != rulesm->end(); i++) {
            (*i).second->orderBody(p_i.first);
            (*i).second->evaluateRec(aq);
        }
        ruleSet* rules = p_i.first->getBodyPlusDefiniteConstraints();   // constraint rules whose body+ contains literal with predicate of p_i
        bool found = false;
        ruleSet::iterator it = rules->begin();
        while (it != rules->end() && !found) {
            (*it)->orderBody(p_i.first);    // "recursive literals" are those with predicate p_i.first
            found = (*it)->existsInstanceDefiniteRuleRec();
            it++;
        }
        if (found)  // a constraint rule is supported and unbloked
            throw ContradictoryConclusion();
    }
}


// search for a supported and non-blocked instance of a non definite rule
// if found, add head in the model (and _atoms_queue)
//           add body- out of the model (and _not_atoms_queue)
//  if it leads to contradiction, prohibite the rule
//  if prohibition leads also to contradiction, throw exception
bool Program::firstApplicationOfNonDefiniteRule(Changes& changes){
    try {
        Rule* rule = firstNonDefiniteRule();
        rule->setDelta();
        rule->orderBody();
        bool instanceFound = rule->firstInstanceNonDefiniteRule();
        instanceFound = applicationOfNonDefiniteRule(rule, instanceFound, changes);
        return instanceFound;
    }
    catch (const ContradictoryConclusion&) {
        throw;
    }
}


bool Program::nextApplicationOfNonDefiniteRule(Changes& changes){
    try {
        Rule* rule = currentNonDefiniteRule();
        bool instanceFound = rule->nextInstanceNonDefiniteRule();
        instanceFound = applicationOfNonDefiniteRule(rule, instanceFound, changes);
        if (!instanceFound) {   // all rules have been examined one time, search for a new instance
            instanceFound = firstApplicationOfNonDefiniteRule(changes);
            changes.incNumberNDR();
        }
        return instanceFound;
    }
    catch (const ContradictoryConclusion&) {
        throw;
    }
}


bool Program::applicationOfNonDefiniteRule(Rule* rule, bool instanceFound, Changes& changes){
    try {
        while (rule) {
            if (instanceFound) {
                // a substitution that makes the rule supported and non-blocked is found
                // head is added to the model, and body- is excluded from
                bool added = rule->addHead(*this, changes);
                if (added && rule->addBodyMinus(*this, changes))
                    rule = NULL;
                else    // headTuple was already in or out of the extension ; search for another rule instance
                    instanceFound = rule->nextInstanceNonDefiniteRule();
            }
            else {
                rule = nextNonDefiniteRule();
                if (rule) {
                    changes.incNumberNDR();
                    rule->setDelta();
                    rule->orderBody();
                    instanceFound = rule->firstInstanceNonDefiniteRule();
                }
            }
        }
        return instanceFound;
    }
    catch (const ContradictoryConclusion&) {
        throw;
    }
}


// return true if there exists supported and unblocked instance of a clone rule (constraint) in rset 
// else return false
bool Program::existsSupportedUnblockedRule(ruleSet& rset){
    bool found = false;
    ruleSet::iterator it = rset.begin();
    while ((it != rset.end()) && !found) {
        (*it)->orderPositiveBody(); // no "recursive literals"  
        found = (*it)->existsInstanceDefiniteConstraint();
        ++it;
    }
    return found;
}


// return true if there exists supported and non blocked instance of a prohibited rule
// else return false
bool Program::existsSupportedNonBlockedProhibitedRule(){
    bool found = false;
    ruleSet::iterator it = _prohibited_rules.begin();
    while ((it != _prohibited_rules.end()) && !found) {
        (*it)->initDelta();
        (*it)->orderBody(); //no "recursive literals" : all predicates are solved
        found = (*it)->existsInstanceNonDefiniteRule();
        it++;
    }
    if (!found) {
        ruleSet::iterator it = _constraint_rules.begin();
        while ((it != _constraint_rules.end()) && !found) {
            (*it)->initDelta();
            (*it)->orderBody(); //no "recursive literals" : all predicates are solved
            found = (*it)->existsInstanceNonDefiniteRule();
            it++;
        }
    }
    return found;
}


// _currentScc is considered solved, propagate this information
bool Program::propagateSolvedPredicates(Changes& changes){
    int firstScc = _currentScc; // scc to consider are in range [firstScc, nextScc[
    try {
        int nextScc = getNextNonStratifiedScc();    // numberOfScc if there is not
        do {
            ruleSet rset;
            _graph.setSolved(true, firstScc);
            _graph.addConstraintsContainingNotP(firstScc, rset);
            if (_graph.existsMbtInstance(firstScc) || existsSupportedUnblockedRule(rset)) {
                throw ContradictoryConclusion();
            }
            else {
                ruleMultimap rmap;
                _graph.addDefiniteRulesContainingNotP(firstScc, rmap);
                for (ruleMultimap::iterator it = rmap.begin(); it != rmap.end(); it++) {
                    (*it).second->orderPositiveBody();
                    (*it).second->evaluate(changes.getAtomsQueue());
                }
                propagateBis(changes);
            }
        } while(_graph.nextNonSolvedScc(firstScc, nextScc));    //set firstScc to the next non solved one, less than 'nextScc'. Return false if there is none
        changes.recordIntermediarySccExtensions(*this, nextScc);    // record result of propagation
        return true;
    }
    catch (const ContradictoryConclusion&) {
        changes.retropropagateSolved(*this, _currentScc, firstScc);
        return false;
    }
}


Changes* Program::recordState(bool b){
    Changes* changes = new Changes;
    changes->recordState(*this);
    _chan_bool_stack.push(std::make_pair(changes, b));
    return changes;
}


void Program::restoreState(bool& b){
    chanboolPair changes_with = _chan_bool_stack.top();
    _chan_bool_stack.pop();
    Changes* ch = changes_with.first;
    b = changes_with.second;
    ch->restoreAll(*this);
    delete ch;
}


void Program::topState(Changes*& ch){
    if (!_chan_bool_stack.empty()) {
        chanboolPair changes_with = _chan_bool_stack.top();
        ch = changes_with.first;
    }
    else
        ch = _first_state;
}


bool Program::firstChoicePoint(Changes& changes, bool& stop){
    try {
        bool found = firstApplicationOfNonDefiniteRule(changes);
        return choicePoint(changes, stop, found);
    }
    catch (const ContradictoryConclusion&) {
        stop = false;
        return false;
    }
}


bool Program::nextChoicePoint(Changes& changes, bool& stop){
    try {
        bool found = nextApplicationOfNonDefiniteRule(changes);
        return choicePoint(changes, stop, found);
    }
    catch (const ContradictoryConclusion&) {
        stop = false;
        return false;
    }
}


// search for the next choice point (supported and non-blocked non definite rule for which propagation does not lead to contradiction)
// return true if a choice point is found
// and, if return false, stop indicates if back-track is needed
bool Program::choicePoint(Changes& changes, bool& stop, bool found){
    bool end;
    do {
        while (found && !propagate(changes)) {  // propagation leads to contradictory conclusion
            prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(changes);
            found = nextApplicationOfNonDefiniteRule(changes);
        }
        if (!found) {   //No more supported and non-blocked non definite rule in _currentScc
            // _currentScc is considered solved, propagate this information
            if (propagateSolvedPredicates(changes)) {   // return false if it leads to contradictory conclusion
                if (setNextNonStratifiedScc()) {    // update _currentScc and _current_range
                                                    // return false if it is the last one
                    changes.incNumberNDR();
                    found = firstApplicationOfNonDefiniteRule(changes);
                    end = false;    // search for choice point is not ended
                }
                else {  // all scc have been processed
                    end = true; 
                    _answer_number++;
                    stop = _answer_number == _max_answer_number;
                    printAnswerSet();
                }
            }
            else {  // propagation leads to contradictory conclusion
                stop = false;
                end = true;
            }
        }
        else
            end = true; 
    } while (!end);
    return found;
}


int Program::answerSetSearch(int n){
    _max_answer_number = n;
    _answer_number = 0;
    _choice_points = 0;
    _bad_choice = 0;
    // tester si des contraintes ne sont pas violées après évaluation de la partie définie du prg
    ruleSet rset;
    addDefiniteConstraintRules(rset);   // all definite (clone of) constraints
    if (existsSupportedUnblockedRule(rset))
        return 0;   // contrainte violée, le prg est UNSAT

    initNonDefiniteRules(); // initialize the multimap _non_definite_rules
    setFirstNonStratifiedScc(); //initialize _currentScc and _current_range
    Changes* changes = new Changes;
    _first_state = changes;
    changes->recordExtensions(*this);
    bool stop;
    bool found = firstChoicePoint(*changes, stop);
    bool withRule = true;   // true for the left tree branch
    do {
        if (withRule) { // descente gauche
            while (found) {
                // search for an AS with _current_non_definite_rule (applied)
                _choice_points++;
                changes = recordState(true);
                found = nextChoicePoint(*changes, stop);
            }
            // stop indique s'il faut poursuivre
        }
        if (!_chan_bool_stack.empty() && !stop) {
            restoreState(withRule);
            topState(changes);
            if (withRule) { // remontée gauche
                changes->retropropagate(*this); // restore state before propagation
                // search for an AS without _current_non_definite_rule
                _bad_choice++;
                prohibiteCurrentInstanceOfNDR(*changes);
                if (propagate(*changes)) {  // descente droite
                    changes = recordState(false);
                    found = nextChoicePoint(*changes, stop);
                }
                else {
                    stop = false;   // propagation leads to contradictory conclusion
                }
            }
            else {  //remontée droite
            }
        }
    } while (!_chan_bool_stack.empty() && !stop);
    while (!_chan_bool_stack.empty()) {
        chanboolPair changes_with = _chan_bool_stack.top();
        _chan_bool_stack.pop();
        delete changes_with.first;
    }
    delete _first_state;
    return _answer_number;
}
