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


#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "changes.hpp"
#include "constraint_rule.hpp"
#include "definition.hpp"
#include "graph.hpp"
#include "normal_rule.hpp"
#include "rule.hpp"



class Program{

//***************************** MEMBER VARIABLES *****************************//

  protected:
        ruleSet _fact_rules;

        ruleSet _definite_rules;

        ruleSet _normal_rules;

        ruleSet _constraint_rules;

        /// pairs (sccIndex, Rule*). All NDrules are s.t. 
        /// all predicates of body- are in the same SCC than the head of the rule
        ruleMultimap _non_definite_rules;

        Graph _graph;

        /// index of current scc
        int _currentScc;

        ruleMultimap::iterator _current_non_definite_rule;

        /// of _non_definite_rules corresponding to _currentScc
        std::pair<ruleMultimap::iterator, ruleMultimap::iterator> _current_range;

        ruleSet _prohibited_rules;

        int _max_answer_number;

        int _answer_number;

        int _verbosity;

        int _choice_points;

        int _bad_choice;

        /// back-track stack
        chanboolStack _chan_bool_stack;

        Changes* _first_state;


//******************************* CONSTRUCTORS *******************************//

    public:
        Program();


//******************************** DESTRUCTOR ********************************//

        ~Program();


//********************************** GETTERS *********************************//

        inline ruleSet& getProhibitedRules();

        inline ruleMultimap::iterator getCurrentNonDefiniteRule();

        inline int getCurrentScc();

        inline Graph& getGraph();

        inline int getChoicePoints();


//********************************** SETTERS *********************************//

        inline void setAnswerNumber(int i);

        inline void setCurrentNonDefiniteRule(ruleMultimap::iterator it);

        inline void setCurrentScc(int i);

        inline void setVerbosity(int i);


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream& os, Program& p);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        inline void addFactRule(Rule*);

        inline void addNormalRule(Rule*);

        inline void addConstraintRule(Rule*);

        /// shift r from non-definite rules to definite rules
        inline void shiftRuleFromNDtoD(Rule* r);

        inline void shiftRuleFromNDtoDWithoutErasing(Rule* r);

        /// initialize the multimap _non_definite_rules
        void initNonDefiniteRules();

        /// add pair to the multimap _non_definite_rules
        void addNonDefiniteRule(int sccIndex, Rule* r);

        void initBodyPlusRules(); 

        inline void addProhibited(Rule* r, Changes& changes);

        inline bool isDefinite();

        inline Rule* firstNonDefiniteRule();

        inline Rule* nextNonDefiniteRule();

        inline Rule* currentNonDefiniteRule();

        inline Rule* lastNonDefiniteRule();

        inline Rule* previousNonDefiniteRule();

        /// initialize _currentScc and _current_range
        inline bool setFirstNonStratifiedScc();

        inline bool setNextNonStratifiedScc();

        inline int getNextNonStratifiedScc();

        inline bool setPreviousNonStratifiedScc();

        void evaluate();

        bool propagate(Changes& changes);

        bool propagateSolvedPredicates(Changes& changes);

        bool firstApplicationOfNonDefiniteRule(Changes& changes);

        bool nextApplicationOfNonDefiniteRule(Changes& changes);

        bool existsSupportedUnblockedRule(ruleSet&);

        bool existsSupportedNonBlockedProhibitedRule();

        ///add to ruleSet all definite (clone of) constraints
        inline void addDefiniteConstraintRules(ruleSet&);

        inline void prohibiteCurrentInstanceOfNDR(Changes& changes);

        inline void prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(Changes& changes);

        bool firstChoicePoint(Changes& changes, bool& stop);

        bool nextChoicePoint(Changes& changes, bool& stop);

        int answerSetSearch(int n);

        Changes* recordState(bool b);

        void restoreState(bool& b);

        void topState(Changes*& ch);

        inline void restoreDeltasCurrentScc();

        inline void printAnswerSet();

    protected:
        void propagateBis(Changes& changes);

        bool applicationOfNonDefiniteRule(Rule* rule, bool instanceFound, Changes& changes);

        bool choicePoint(Changes& changes, bool& stop, bool f);


}; // class Program


//********************************** GETTERS *********************************//

inline ruleSet& Program::getProhibitedRules(){
    return _prohibited_rules;
}


inline ruleMultimap::iterator Program::getCurrentNonDefiniteRule(){
    return _current_non_definite_rule;
}


inline int Program::getCurrentScc(){
    return _currentScc;
}


inline Graph& Program::getGraph(){
    return _graph;
}


inline int Program::getChoicePoints(){
    return _choice_points;
}


//********************************** SETTERS *********************************//

inline void Program::setAnswerNumber(int i){
    _answer_number = i;
}


inline void Program::setCurrentNonDefiniteRule(ruleMultimap::iterator it){
    _current_non_definite_rule = it;
}


inline void Program::setCurrentScc(int i){
    _currentScc = i;
}

inline void Program::setVerbosity(int i) {
  _verbosity = i;
}

//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, Program& p){
    os << p._fact_rules;
    os << p._definite_rules;
    os << p._normal_rules;
    return os << p._constraint_rules;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline void Program::addFactRule(Rule* r){
    _fact_rules.insert(r);
    if (_verbosity > 1)
	std::cout << *r;
}


inline void Program::addNormalRule(Rule* r){
    if (r->isDefinite()) {
        _definite_rules.insert(r);
        r->getHead()->getPred()->addHeadDefiniteRule(r);
    }
    else {
        _normal_rules.insert(r);
        r->getHead()->getPred()->addHeadNonDefiniteRule(r);
        r->createDefiniteClone();   // creation of a definite clone of rule r
    }
    if (_verbosity > 1)
	std::cout << *r;
}


// [ body-(r) is solved, r can be processed as a definite rule]
// shift r from non-definite rules to definite rules
// i.e., remove r from non-definite rules and add definite clone of r to definite rules
inline void Program::shiftRuleFromNDtoD(Rule* r){
    shiftRuleFromNDtoDWithoutErasing(r);
//     remove r from non-definite rules
    _normal_rules.erase(r);
    delete r;
}


// [ body-(r) is solved, r can be processed as a definite rule]
// shift r from non-definite rules to definite rules
// i.e., remove r from non-definite rules and add definite clone of r to definite rules
inline void Program::shiftRuleFromNDtoDWithoutErasing(Rule* r){
    // remove r from non-definite rules
    r->getHead()->getPred()->removeHeadNonDefiniteRule(r);
    Rule* clone = r->getClone();    // definite clone of r
    r->setClone(NULL);
    // add clone to definite rules
    _definite_rules.insert(clone);
    clone->getHead()->getPred()->addHeadDefiniteRule(clone);
}


inline void Program::addConstraintRule(Rule* r){
    _constraint_rules.insert(r);
    r->createDefiniteClone();   // creation of a definite clone of constraint rule r
    if (_verbosity > 1)
	std::cout << *r;
}


// TODO _constraint_rule can be not empty
inline bool Program::isDefinite(){
    return _normal_rules.empty();
}


// _current_range is supposed to be already set
// return the first NDrule in the range corresponding to current scc
inline Rule* Program::firstNonDefiniteRule(){
    _current_non_definite_rule = _current_range.first;
    if (_current_non_definite_rule != _current_range.second)
        return currentNonDefiniteRule();
    else
        return NULL;
}


inline Rule* Program::nextNonDefiniteRule(){
    _current_non_definite_rule++;
    if (_current_non_definite_rule != _current_range.second) {
        return currentNonDefiniteRule();
    }
    else {
        _current_non_definite_rule--;
        return NULL;
    }
}


inline Rule* Program::currentNonDefiniteRule(){
    return (*_current_non_definite_rule).second;
}


inline Rule* Program::previousNonDefiniteRule(){
    if (_current_non_definite_rule == _current_range.first)
        _current_non_definite_rule = _current_range.second;
    _current_non_definite_rule--;
    return currentNonDefiniteRule();
}


// return last NDrule of the current scc and update _current_non_definite_rule
inline Rule* Program::lastNonDefiniteRule(){
    _current_non_definite_rule = _current_range.second;
    if (_current_non_definite_rule != _current_range.first) {
        _current_non_definite_rule--;
        return currentNonDefiniteRule();
    }
    else
        return NULL;
}


// initialize _currentScc and _current_range
inline bool Program::setFirstNonStratifiedScc(){
    if (_graph.setFirstNonStratifiedScc(_currentScc)) {
        _current_range = _non_definite_rules.equal_range(_currentScc);  //rules from the scc
        return true;
    }
    else
        return false;
}


// return  the next non locally stratified scc
// return numberOfScc() if _currentScc was the last one
inline int Program::getNextNonStratifiedScc(){
    return _graph.getNextNonStratifiedScc();
}


// update _currentScc and _current_range to the next non locally stratified scc
// return false if _currentScc was the last one
inline bool Program::setNextNonStratifiedScc(){
    if (_graph.setNextNonStratifiedScc(_currentScc)) {
        _current_range = _non_definite_rules.equal_range(_currentScc);  //rules from the scc
        return true;
    }
    else{
        return false;
    }
}


inline bool Program::setPreviousNonStratifiedScc(){
    if (_graph.setPreviousNonStratifiedScc(_currentScc)){
        _current_range = _non_definite_rules.equal_range(_currentScc); //rules from the scc
        return true;
    }
    else
        return false;
}


inline void Program::addProhibited(Rule* r, Changes& changes){
    std::pair<ruleSet::iterator, bool> i_b = _prohibited_rules.insert(r);
    if (i_b.second) {   // r was not already prohibited
        changes.addProhibited(r);
        Rule* clone = r->createDefiniteClone(); // creation of a definite clone of constraint rule r
        clone->getBodyPlus()->addBodyPlusDefiniteConstraint(clone);
    }
    else
        delete r;
}


// add to ruleSet all definite (clone of) constraints
inline void Program::addDefiniteConstraintRules(ruleSet& rset){
    for (ruleSet::iterator it = _constraint_rules.begin(); it != _constraint_rules.end(); ++it)
        rset.insert((*it)->getClone());
}


inline void Program::prohibiteCurrentInstanceOfNDR(Changes& ch){
    currentNonDefiniteRule()->prohibiteCurrentInstanceOfNDR(*this, ch);
}


inline void Program::prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(Changes& ch){
    currentNonDefiniteRule()->prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(*this,ch);
}


// restore deltas for each NDrule of _currentScc
inline void Program::restoreDeltasCurrentScc(){
    for (ruleMultimap::iterator it = _current_range.first;  it != _current_range.second; ++it)
        (*it).second->restoreDeltas();
}


inline void Program::printAnswerSet(){
    if (_verbosity > 1)
	std::cout<<"############### Answer set "<< _answer_number<<" ( "<<_choice_points<<" choice points, "<<_bad_choice<< " bad choice)"<<" ###############"<<std::endl;
    else if (_verbosity > 0)
	std::cout<<"############### Answer set "<< _answer_number<<" ###############"<<std::endl;
    Predicate::prettyPrintAllInstances();
    std::cout << std::endl;
}



#endif // _PROGRAM_HPP_
