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
#include "normal_rule.hpp"
#include "program.hpp"
#include "rule.hpp"



//****************************************************************************//
//                                    Body                                    //
//****************************************************************************//


//******************************** DESTRUCTOR ********************************//

Body::~Body(){
    for (Body::iterator it = begin(); it != end(); it++)
        delete (*it);   // destruction of literals
    clear();
    _variables.clear();
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// return true if current instance of body contains current instance of literal *l
bool Body::containsInstance(BasicLiteral* l){
    Body::iterator it = begin();
    while ((it != end()) && !(*it)->isTheSameInstance(l))
        it++;
    return (it != end());
}


// return true if *l appears as negative literal in body (resp. in positive body if head is negative)
// *l and body are not instanciated
bool Body::containsNotLiteral(BasicLiteral* l){
    bool found = false;
    Body::iterator it = begin();
    if (l->isNegativeLiteral())
        while (it != end() && !found) {
            if ((*it)->isPositiveLiteral())
                found = (*it)->isTheSameLiteral(l);
            it++;
        }
    else
        while (it != end() && !found) {
            if ((*it)->isNegativeLiteral())
                found = (*it)->isTheSameLiteral(l);
            it++;
        }
    return found;
}


void Body::addBodyPlusDefiniteRule(Rule* r){
    int scc = r->getSccIndex();
    for (Body::iterator it = begin(); it != end(); it++)
        if ((*it)->isPositiveLiteral()) // literal *it is a basic positive literal
            (*it)->getPred()->addBodyPlusDefiniteRule(scc,r);
}


void Body::addBodyPlusDefiniteRule(int sccIndex, Rule* r){
    for (Body::iterator it = begin(); it != end(); it++)
        if ((*it)->isPositiveLiteral()) // literal *it is a basic positive literal
            (*it)->getPred()->addBodyPlusDefiniteRule(sccIndex, r);
}


void Body::addBodyPlusNonDefiniteRule(int sccIndex, Rule* r){
    for (Body::iterator it = begin(); it != end(); it++)
        if ((*it)->isPositiveLiteral()) // literal *it is a basic positive literal
            (*it)->getPred()->addBodyPlusNonDefiniteRule(sccIndex, r);
}


void Body::addBodyPlusDefiniteConstraint(Rule* r){
    for (Body::iterator it = begin(); it != end(); it++)
        if ((*it)->isPositiveLiteral()) // literal *it is a basic positive literal
            (*it)->getPred()->addBodyPlusDefiniteConstraint(r);
}


void Body::removeBodyPlusDefiniteConstraint(Rule* r){
    for (Body::iterator it = begin(); it != end(); it++)
        if ((*it)->isPositiveLiteral()) // literal *it is a basic positive literal
            (*it)->getPred()->removeBodyPlusDefiniteConstraint(r);
}


void Body::applySubstitutionAndAddNonInstances(Program& p, Changes& ch){
    Argument* tuple = NULL;
    try {
        for (Body::iterator it = begin(); it != end(); it++) {
            tuple = (*it)->applySubstitutionOnArgs();
            (*it)->addNonInstances(*tuple, ch.getAtomsQueue());
            delete tuple;
        }
    }
    catch (const ContradictoryConclusion&) {
        delete tuple;
        ch.retropropagate(p);
        throw;
    }
}


// split body into recBody and nonRecBody
// "recursive" literals are those whose predicate appears as label of a node in ns
void Body::split(Node::Set& ns, Body& recBody, Body& nonRecBody){
    for (Body::iterator it = begin(); it != end(); it++) {
        if ((*it)->recursive(ns))
            recBody.addLiteral(*it);
        else
            nonRecBody.push_back(*it);
    }
}


// split body into recBody and nonRecBody
// "recursive" literals are those whose predicate is pred
void Body::split(Predicate* pred, Body& recBody, Body& nonRecBody){
    for (Body::iterator it = begin(); it != end(); it++) {
        if ((*it)->hasPred(pred))   // the predicate of literal *it is pred	
            recBody.addLiteral(*it);
        else
            nonRecBody.push_back(*it);
    }
}


// split body into recBody and nonRecBody
// "recursive" literals are those which are not already solved
void Body::split(Body& recBody, Body& nonRecBody){
    for (Body::iterator it = begin(); it != end(); it++) {
        if (!(*it)->isSolved()) // the predicate of literal *it is not already solved	
            recBody.addLiteral(*it);
        else
            nonRecBody.push_back(*it);
    }
}


// order bodyPlus, result in ordBody
// TODO pour l'instant littéraux récursifs en premier
void Body::orderPlus(Body& recBody, Body& nonRecBody, Body& ordBody){
    for (Body::iterator it = recBody.begin(); it != recBody.end(); it++)
        ordBody.addLiteral(*it);    // recursive literals first
    for (Body::iterator it = nonRecBody.begin(); it != nonRecBody.end(); it++)
        ordBody.addLiteral(*it);    // and then non-recursive literals 
}


// order body, result in ordBody
// TODO pour l'instant littéraux récursifs en premier et litteraux négatifs en dernier
void Body::order(Body& recBody, Body& nonRecBody, Body& ordBody){
    orderPlus(recBody, nonRecBody,ordBody);
    for (Body::iterator it = begin(); it !=end(); it++)
        ordBody.addLiteral(*it);    // and  negative literals at the end
}


// initalisation of previous and free vars for each literal
void Body::initVars(){
    variableSet ruleVars;
    variableSet* litVars;
    for (Body::iterator it = begin(); it != end(); it++) {
        (*it)->clearVars();
        litVars = (*it)->getVariables();
        for (variableSet::iterator i = litVars->begin(); i != litVars->end(); i++) {
            if (ruleVars.insert(*i).second) // *i appears for the first time in the rule
                (*it)->addFreeVar(*i);
            else if (!(*it)->isFreeVar(*i)) // *i appears in a previous atom 
                (*it)->addPreviousVar(*i);
        }
    }
}


// create a new body by applying current substitution
Body* Body::createInstance(){
    Body* b = new Body();
    for (Body::iterator it = begin(); it != end(); it++)
        b->addLiteral((*it)->createInstance());
    return b;
}


// create clones of all literals and add them to b
void Body::addPositiveClone(Body& b, MapVariables& mapVar){
    for (Body::iterator it = begin(); it != end(); it++)
        b.addLiteral((*it)->createPositiveClone(mapVar));
}






//****************************************************************************//
//                                    Rule                                    //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

Rule::Rule(){
    _bodyMinus = new Body();
    _bodyPlus = new Body();
    _clone = NULL;
}


Rule::Rule(BasicLiteral* a, Body* v) : _head(a), _body(v), _clone(NULL){
    _bodyMinus = new Body();
    _bodyPlus = new Body();
    for (Body::iterator it = v->begin(); it != v->end(); it++) {
        if ((*it)->isNegativeLiteral())
            _bodyMinus->addLiteral(*it);
        else
            _bodyPlus->addLiteral(*it);
    }
    _variables = v->getVariables();
}


Rule::Rule(BasicLiteral* a, Body* v, const variableSet& s, Graph& g) throw (LogicExcept)
try : _head(a), _body(v), _variables(s), _clone(NULL) {
    _bodyMinus = new Body();
    _bodyPlus = new Body();
    for (Body::iterator it = v->begin(); it != v->end(); it++) {
        if ((*it)->isNegativeLiteral())
            _bodyMinus->addLiteral(*it);
        else
            _bodyPlus->addLiteral(*it);
    }
    if (_bodyPlus->getVariables() != s)
        throw LogicExcept("Non safe rule");
    if (!_bodyMinus->empty() || !a) {   // non definite rule OR constraint rule :
                                        // cast PositiveLiteral into PositiveLiteralOfNDR,
                                        // add true_literal if boby+ is empty
        Body* bodyP = _bodyPlus;
        _bodyPlus = new Body();
        bool positive = false;
        for (Body::iterator it = bodyP->begin(); it != bodyP->end(); it++) {
            if ((*it)->isPositiveLiteral()) {
                positive = true;
                Literal* l = new PositiveLiteralOfNDR(*static_cast<PositiveLiteralOfNDR*>( *it ));
                _bodyPlus->addLiteral(l);
                Body::iterator i = v->begin();  //search for (*it) in _body and replace it by l
                while ((i != v->end()) && (*i != *it))
                    i++;
                i = v->insert(i,l); // insert l before position i
                v->erase(++i);
                delete (*it);
            }
            else
                _bodyPlus->addLiteral(*it);
        }
        bodyP->clear();
        delete bodyP;
        if (!positive) {    // body+ is empty or contains only relationnal of affectation literals
            Predicate* p = Predicate::newTruePredicate(g);
            Argument* tv = new Argument;
            Literal* l = new PositiveLiteralOfNDR(p, tv);   // "true" literal
            _bodyPlus->addLiteral(l);
            _body->push_back(l);
        }
    }
}
catch (const LogicExcept&) {
    delete _body;
    _bodyPlus->clear();
    delete _bodyPlus;
    _bodyMinus->clear();
    delete _bodyMinus;
}


//******************************** DESTRUCTOR ********************************//

Rule::~Rule(){
    delete _head;
    delete _body;
    _bodyPlus->clear();
    delete _bodyPlus;
    _bodyMinus->clear();
    delete _bodyMinus;
    _orderedBody.clear();
    _recursiveBody.clear();
    for (variableSet::iterator i = _variables.begin(); i != _variables.end(); i++)
        delete *i;
    _variables.clear();
    delete _clone;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

void Rule::clear(){
    _head = NULL;
    _body = NULL;
    _variables.clear();
    _clone = NULL;
}


// ordonnancement du corps de la règle TODO (pour l'instant, littéraux "récursifs" en 1er)
// initialisation des atomes recursifs _recursiveBody
// et initialisation des previous et free vars
void Rule::orderBody(Node::Set& ns){
    Body nonRecBody;
    _orderedBody.clear();
    _recursiveBody.clear();
    _bodyPlus->split(ns, _recursiveBody, nonRecBody);
    _bodyPlus->orderPlus(_recursiveBody, nonRecBody, _orderedBody);
    nonRecBody.clear();
    _orderedBody.initVars();
}


// at least one instance of predicate pred has been added to its extension
//
// ordonnancement du corps de la règle TODO (pour l'instant, littéraux "récursifs" en 1er)
// initialisation of _recursiveBody with literals whose predicate is pred
// and initialisation of previous and free vars
void Rule::orderBody(Predicate* pred){
    Body nonRecBody;
    _orderedBody.clear();
    _recursiveBody.clear();
    _bodyPlus->split(pred, _recursiveBody, nonRecBody);
    _bodyPlus->orderPlus(_recursiveBody, nonRecBody, _orderedBody);
    nonRecBody.clear();
    _orderedBody.initVars();
}


// ordonnancement du corps de la règle TODO (pour l'instant, littéraux "récursifs" en 1er et négatifs en dernier)
// initialisation of _recursiveBody with literals which are nor already solved
// and initialisation of previous and free vars
void Rule::orderBody(){
    Body nonRecBody;
    _orderedBody.clear();
    _recursiveBody.clear();
    _bodyPlus->split(_recursiveBody, nonRecBody);
    _bodyMinus->order(_recursiveBody, nonRecBody, _orderedBody);
    nonRecBody.clear();
    _orderedBody.initVars();
}


// ordonnancement du corps de la règle TODO ()
// initialisation of _recursiveBody with empty set
// and initialisation of previous and free vars
void Rule::orderPositiveBody(){
    _orderedBody.clear();
    _recursiveBody.clear();
    _orderedBody = *_bodyPlus;
    _orderedBody.initVars();
}


// true if the rule is recursive r.t. SCC ns
// i.e. body+ contains at least one predicate that appears as label of a node from ns
bool Rule::recursive(Node::Set& ns){
    return (_bodyPlus->containsOnePredFrom(ns));
}


// evaluation of a non-definite rule (search for a "true" supported and non-blocked instance)
// recursive literals are non-solved one
bool Rule::firstInstanceNonDefiniteRule(){
    Literal* lit = firstLiteral();
    Literal* recLit = NULL;
    setSolved(_recursiveBody.empty());  // rule is solved if all body+ literals are solved
    if (!isSolved()) {
        initMarks(NON_MARKED);
        recLit = firstRecursiveLiteral();   // first non solved literal
        recLit->setMark(MARKED);
    } else
        initMarks(MARKED);  //"exit" rule : to examine only one time
    bool matchFound = lit->firstTrueMatch();
    matchFound = trueInstanceNonDefiniteRule(lit, recLit, matchFound);
    return matchFound;
}


// evaluation of a non-definite rule
bool Rule::nextInstanceNonDefiniteRule(){
    Literal* lit = currentLiteral();
    Literal* recLit = NULL;
    if (!isSolved())
        recLit = recursiveLiteral();    // current non solved literal
    lit->free();
    bool matchFound = lit->nextTrueMatch();
    matchFound = trueInstanceNonDefiniteRule(lit, recLit, matchFound);
    return matchFound;
}


// search for a "true" supported and non-blocked instance of a non-definite rule
// lit is the  current literal
// recLit is the current non solved literal
// matchFound is true iff a match is found for literal lit
bool Rule::trueInstanceNonDefiniteRule(Literal* lit, Literal* recLit, bool matchFound){
    do {
        if (matchFound) {
            if ((lit = nextLiteral()))  // NULL if it was the last one
                matchFound = lit->firstTrueMatch();
            else {
                // lit is NULL
                // a substitution that makes the rule supported and non-blocked is found
            }
        }
        else if ((lit = previousLiteral())) {   // NULL if it was the first one
            lit->free();
            matchFound = lit->nextTrueMatch();
        }
        else if (!isSolved()) {
            recLit->setMark(UNMARKED);
            if ((recLit = nextRecursiveLiteral())) {
                lit = firstLiteral(); 
                recLit->setMark(MARKED);    // next recursive literal
                matchFound = lit->firstTrueMatch();
            }
        }
    } while (lit);
    return matchFound;
}


// search for a "soft"(true or mbt) supported and non-blocked instance of a non-definite rule
// lit is the  current literal
// recLit is the current non solved literal
// matchStatus is the match status for literal lit (no match, mbt match, or true match)
statusEnum Rule::softInstanceNonDefiniteRule(Literal* lit, Literal* recLit, statusEnum matchStatus){
    do {
        if (matchStatus) {  // a match is found for lit
            if ((lit = nextLiteral()))  // NULL if it was the last one
                matchStatus = lit->firstSoftMatch(matchStatus);
            else {
                // lit is NULL
                // a substitution that makes the rule supported and non-blocked is found
            }
        }
        else if ((lit = previousLiteral())) {   // NULL if it was the first one
            lit->free();
            matchStatus = lit->nextSoftMatch(previousStatus());
        }
        else if (!isSolved()) {
            recLit->setMark(UNMARKED);
            if ((recLit = nextRecursiveLiteral())) {
                lit = firstLiteral(); 
                recLit->setMark(MARKED);    // next recursive literal
                matchStatus = lit->firstSoftMatch(TRUE_);
            }
        }
    } while (lit);
    return matchStatus;
}


// search for a "true" supported and non blocked instance of a non-definite rule (constraint rule)
// there is no "recursive literals" : all predicates are solved 
bool Rule::existsInstanceNonDefiniteRule(){
    Literal* lit = firstLiteral();
    Literal* recLit = NULL;
    setSolved(true);    // all rules are considered  solved 
    initMarks(MARKED);  //rule have to be examined only one time (new instances can not appear)
    bool matchFound = lit->firstTrueMatch();
    matchFound = trueInstanceNonDefiniteRule(lit, recLit, matchFound);
    if (matchFound) {   // free bindings for rule variables
        lit = currentLiteral();
        do {
            lit->free();
            lit = previousLiteral();
        } while (lit);
    }
    return matchFound;
}


// search for a "soft" supported instance of a clone rule (constraint rule) 
//  corresponding to a supported and  unblocked instance of a constraint rule
// there is no "recursive literals" (the only new fact is that some predicate is solved)
bool Rule::existsInstanceDefiniteConstraint(){
    Literal* lit = firstLiteral();
    Literal* recLit = NULL;
    setSolved(true);    // all rules are considered  solved 
    initMarks(NON_MARKED);  //rule have to be examined only one time (new instances can not appear)
    statusEnum matchStatus = lit->firstSoftMatch(TRUE_);
    matchStatus = softInstanceNonDefiniteRule(lit, recLit, matchStatus);
    if (matchStatus) {  // free bindings for rule variables
        lit = currentLiteral();
        do {
            lit->free();
            lit = previousLiteral();
        } while (lit);
    }
    return matchStatus;
}


// search for a "soft" supported instance of a clone rule (constraint rule) 
//  corresponding to a supported and  unblocked instance of a constraint rule
// recursive literals are those whose body+ contains a predicate whose extension has been modified (or a predicate that became solved) 
bool Rule::existsInstanceDefiniteRuleRec(){
    initMarks(NON_MARKED);//
    Literal* lit = firstLiteral();
    Literal* recLit = firstRecursiveLiteral();
    recLit->setMark(MARKED);
    setSolved(false);   // recursive body is not empty
    statusEnum matchStatus = lit->firstSoftMatch(TRUE_);
    matchStatus = softInstanceNonDefiniteRule(lit, recLit, matchStatus);
    if (matchStatus) {  // free bindings for rule variables
        lit = currentLiteral();
        do {
            lit->free();
            lit = previousLiteral();
        } while (lit);
    }
    return matchStatus;
}


// create a new constraint rule by applying current substitution
ConstraintRule* Rule::createInstance(){
    Body* b = _bodyMinus->createInstance();
    ConstraintRule* r = new ConstraintRule(b);
    return r;
}


void Rule::prohibiteCurrentInstanceOfNDR(Program& p, Changes& ch){
    Literal* l = getBodyMinus()->isSingleLiteral();
    if (l) {    // single literal (not a) body-, add a to MBT atoms
        l = l->createInstance();
        l->addInstances(*l->getArgs(), MBT_, ch.getAtomsQueue());
        delete l;
    }
    else {
        ConstraintRule* r = createInstance();
        p.addProhibited(r, ch);
    }
}


// the same as 'prohibiteCurrentInstanceOfNDR' but propagate if MBT atom added 
// and thus, throw ContradictoryConclusion, or record extensions
void Rule::prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(Program& p, Changes& ch){
    Literal* l = getBodyMinus()->isSingleLiteral();
    if (l) {    // single literal (not a) body-, add a to MBT atoms
        l = l->createInstance();
        l->addInstances(*l->getArgs(), MBT_, ch.getAtomsQueue());
        delete l;
        // propagate MBT atom added
        if (p.propagate(ch))    // propagation ok, record new extensions
            ch.recordIntermediaryMbtExtensions(p);
        else    // propagation leads to contradiction
            throw ContradictoryConclusion();
    }
    else {
        ConstraintRule* r = createInstance();
        p.addProhibited(r, ch);
    }
}


// create a new definite rule by cloning
Rule* Rule::createDefiniteClone(){
    // creation of new variables
    MapVariables mapVar;
    for (variableSet::iterator i = _variables.begin(); i != _variables.end(); i++) {
        const std::string& name = (*i)->getUserName();
        Variable* v = new Variable(name);
        mapVar.insert(std::make_pair(*i, v));
    }
    // creation of new rule
    BasicLiteral* h = NULL;
    if (_head) h = _head->createPositiveClone(mapVar);
    Body* b = new Body();
    _bodyPlus->addPositiveClone(*b, mapVar);
    _bodyMinus->addPositiveClone(*b, mapVar);
    _clone = new NormalRule(h, b);
    return _clone;
}


// for each literal 'not p(t)' in body- with scc(p)<sccIndex
// erase 'not p(t)' and add '__not__p(t)' in body+
void Rule::shiftLocallySolvedLiteralsFromBMinus2BPlus(int sccIndex){
    Body* bodyMin = _bodyMinus;
    _bodyMinus = new Body();
    _body->clear();
    *_body =*_bodyPlus;
    for (Body::iterator it = bodyMin->begin();it != bodyMin->end();++it) {
        if ((*it)->getSccIndex() < sccIndex) {
            Predicate* notp = (*it)->getPred()->getNegatedPredicate();
            Argument* tv = new Argument(*(*it)->getArgs());
            PositiveLiteralOfNDR * l = new PositiveLiteralOfNDR(notp, tv);
            _bodyPlus->addLiteral(l);
            _body->addLiteral(l);
            notp->addBodyPlusNonDefiniteRule(sccIndex, this);
            delete *it;
        }
        else {  // scc(p) == sccIndex
            _bodyMinus->addLiteral(*it);
            _body->addLiteral(*it);
        }
    }
    bodyMin->clear();
}


// current instance of (*this) is the current supported and non-blocked non definite rule
// record all what we need to later recover current search'state
void Rule::recordCurrentInstanceNDR(){
    _recLit_stack.push(_recursiveLiteral);
    _solved_stack.push(_solved);
    _orderedBody.recordMarks();
    _bodyPlus->recordIndexesAndStatus();
    for (variableSet::iterator it = _variables.begin(); it != _variables.end(); it++)
        (*it)->recordSubstitution();
}


// restore previous instance of (*this) which was supported and non-blocked
// restore all what we need to recover previous search'state
void Rule::restorePreviousInstanceNDR(){
    // les deltas des littéraux ont déjà été restaurés
    orderBody();    // (re)initialise _orderedBody, _recursiveBody, previous et free vars
    _currentLiteral = --_orderedBody.end(); // require _orderedBody not empty
    _recursiveLiteral = _recLit_stack.top();
    _recLit_stack.pop();
    _solved = _solved_stack.top();
    _solved_stack.pop();
    _orderedBody.restoreMarks();
    _bodyPlus->restoreIndexesAndStatus();
    for (variableSet::iterator it = _variables.begin(); it != _variables.end(); it++)
        (*it)->restoreSubstitution();
}


std::ostream& Rule::put(std::ostream& os){
    return os << *_head << ":-" << *_body << '.' << std::endl;
}
