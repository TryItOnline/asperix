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


#include <algorithm>

#include "graph.hpp"
#include "predicate.hpp"
#include "program.hpp"
#include "rule.hpp"



//****************************************************************************//
//                                    Node                                    //
//****************************************************************************//


//******************************* CONSTRUCTORS *******************************//

Node::Node(Predicate* p) : _pred(p), _visited(0), _onStack(false){}


//******************************** DESTRUCTOR ********************************//

Node::~Node(){
    _posSucc.clear();
    _negSucc.clear();
}


//********************************* OPERATORS ********************************//

std::ostream& operator << (std::ostream& os,Node& n){
    return os << *n._pred;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

void Node::tarjan(Graph& graph){
    _visited = graph.nextDate();
    _lowLink = _visited;
    _onStack = true;
    graph.pushStack(this);

    visitSuccessors(_posSucc, graph);
    visitSuccessors(_negSucc, graph);
    if (_lowLink == _visited) { // the node is the root of a scc 
                                //nodes are in the stack from top to (*this)
        Node* n;
        do {
            n = graph.topStack();
            graph.popStack();
            n->setOutStack();
            graph.addNode2scc(n);
        } while (n != this);
        graph.saveAndClearSCC();
    }
}


void Node::visitSuccessors(Node::Set& succ, Graph& graph){
    for (Node::Set::iterator it = succ.begin(); it !=succ.end(); it++) {
        if (!(*it)->_visited)   // arc couvrant
            (*it)->tarjan(graph);
        if ((*it)->_onStack)    // arc arrière 
            _lowLink = std::min(_lowLink,(*it)->_lowLink); 
    }
}


// dispatch rules whose head predicate is _pred among recursive and exit rules r.t. scc
// concerned rules are all definite rules whose head predicate is _pred 
// and clones of NDrules whose body- predicates are solved
void Node::addRules(ruleSet& recRules, ruleSet& exitRules, Node::Set& scc, Program& prog){
    // search for non definite rules whose head predicate is _pred and whose body- predicates are solved
    ruleSet* rs = new ruleSet(*_pred->getHeadNonDefiniteRules());   //working copy
    for (ruleSet::iterator it = rs->begin(); it != rs->end(); it++)
        if ((*it)->getBodyMinus()->isSolved()){
            // all predicates that appear in body- are solved
            // the rule can be processed as a definite rule
            prog.shiftRuleFromNDtoD(*it);
        }
    delete rs;
    rs = _pred->getHeadDefiniteRules(); // all definite rules and (just added) clones 
    for (ruleSet::iterator it = rs->begin(); it != rs->end(); it++) {
        if ((*it)->recursive(scc))
            recRules.insert(*it);
        else
            exitRules.insert(*it);
    }
}


// add to rmap definite rules containing negated predicate of _pred
// and whose scc is greater than those of the the node (_sccIndex)
void Node::addDefiniteRulesContainingNotP(ruleMultimap& rmap){
    ruleMultimap* rm = _pred->getNegatedPredicate()->getBodyPlusDefiniteRules();
    ruleMultimap::iterator it = rm->upper_bound(_sccIndex); //first elt whose key is greater than _sccIndex
    for ( ; it != rm->end(); ++it)
        rmap.insert(*it);
}


// add to rset definite clones of constraint rules containing negated predicate of _pred
void Node::addConstraintsContainingNotP(ruleSet& rset){
    ruleSet* rs = _pred->getNegatedPredicate()->getBodyPlusDefiniteConstraints();
    for (ruleSet::iterator it = rs->begin(); it != rs->end(); ++it)
        rset.insert(*it);
}


// true if all negative successors are outer the scc
bool Node::isLocallyStratified(int index){
    Node::Set::iterator i = _negSucc.begin();   // negative successors
    while ((i != _negSucc.end()) && !(*i)->inScc(index))
        ++i;
    return (i == _negSucc.end());
}


// return true if nothing prevents the node's predicate to be solved 
// that is :    each arc, internal to scc, is positive
//              and, for each arc (p,q) where p inner and q outer the scc, q is solved
bool Node::checkSolveness(){
    bool solved = true;
    Node::Set::iterator i = _posSucc.begin();   // positive successors
    while (solved && i != _posSucc.end()) {
        if (!(*i)->inScc(_sccIndex) && !(*i)->isSolved())   // p inner, q outer,and q is not solved
            solved = false;
        else
            i++;
    }
    i = _negSucc.begin();   // negative successors
    while (solved && i != _negSucc.end()) {
        if ((*i)->inScc(_sccIndex) || !(*i)->isSolved())    // negative internal arc 
            solved = false;                                 // or p inner, q outer,and q is not solved
        else
            i++;
    }
    return solved;
}





//****************************************************************************//
//                                    Graph                                   //
//****************************************************************************//



//******************************* CONSTRUCTORS *******************************//

Graph::Graph() : _date(0){}


//******************************** DESTRUCTOR ********************************//

Graph::~Graph(){
    for (Node::Set::iterator it = _nodes.begin(); it!=_nodes.end(); it++)
        delete (*it);
    _nodes.clear();
    _scc.clear();
    _sccVector.clear();
    _non_stratified_sccVector.clear();
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// call by p itself when created
Node* Graph::new_Node(Predicate* p){
    Node* n = new Node(p);
    _nodes.insert(n);
    return n;
}


void Graph::computeSCC(){
    for (Node::Set::iterator it = _nodes.begin(); it != _nodes.end(); it++)
        if (!(*it)->isVisited())
            (*it)->tarjan(*this);
    initNonStratifiedSccVector();
}


void Graph::getRulesFromScc(int index, ruleSet& recRules, ruleSet& exitRules, Program& prog){
    Node::Set& scc = _sccVector.at(index);
    for (Node::Set::iterator i = scc.begin(); i != scc.end(); i++)
        (*i)-> addRules(recRules, exitRules, scc, prog);
}


// return true if at least one predicate of the scc has a MBT instance
bool Graph::existsMbtInstance(int index){
    Node::Set& scc = _sccVector.at(index);
    Node::Set::iterator i = scc.begin();
    while (i != scc.end() && !(*i)->existsMbtInstance()) ++i;
    return (i != scc.end());
}


// add to 'rmap' definite rules whose body contain _not_p, for p a predicate of scc('index')
// and whose scc is greater than 'index'
void Graph::addDefiniteRulesContainingNotP(int index, ruleMultimap& rmap){
    Node::Set& scc = _sccVector.at(index);
    for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
        (*i)->addDefiniteRulesContainingNotP(rmap);
}


// add to rset (definite clones of) constraint rules whose body contain _not_p, for p in scc(index)
void Graph::addConstraintsContainingNotP(int index, ruleSet& rset){
    Node::Set& scc = _sccVector.at(index);
    for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
        (*i)->addConstraintsContainingNotP(rset);
}


// non solved (non locally stratified) scc are those that contain at least one rule with a literal from body- in the same scc than the head (that is, at least one arc internal to the scc is negative)
void Graph::initNonStratifiedSccVector(){
    int size = _sccVector.size();
    for (int i = 0; i < size; ++i)
        if (!isLocallyStratified(i))
            _non_stratified_sccVector.push_back(i);
}


// return true if scc does not contain internal negative arc
bool Graph::isLocallyStratified(int index){
    Node::Set& scc = _sccVector.at(index);
    Node::Set::iterator i = scc.begin();
    while (i != scc.end() && (*i)->isLocallyStratified(index)) ++i;
    return (i == scc.end());
}


// predicates of a SCC are solved if :
//    each arc, internal to scc, is positive
//    and, for each arc (p,q) where p inner and q outer the scc, q is solved
// else all predicates of the SCC are unsolved  
void Graph::setSolvedIfItIsTheCase(int index){
    Node::Set& scc = _sccVector.at(index);
    Node::Set::iterator i = scc.begin();
    while ((i != scc.end()) && (*i)->checkSolveness())
        i++;
    if (i == scc.end()) {   // predicates are solved
        _solved.push_back(true);
        for (i = scc.begin(); i != scc.end(); i++)
            (*i)->setSolved(true);
    }
    else
        _solved.push_back(false);
}


// set solved each predicates of the SCC    
void Graph::setSolved(bool b, int index){
    Node::Set& scc = _sccVector.at(index);
    for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
        (*i)->setSolved(b);
}


void Graph::setDelta(int index){
    Node::Set& scc = _sccVector.at(index);
    for (Node::Set::iterator i = scc.begin(); i != scc.end(); i++)
        (*i)->setDelta();
}


void Graph::initDelta(int index){
    Node::Set& scc = _sccVector.at(index);
    for (Node::Set::iterator i = scc.begin(); i != scc.end(); i++)
        (*i)->initDelta();
}


bool Graph::emptyDelta(int index){
    Node::Set& scc = _sccVector.at(index);
    Node::Set::iterator i = scc.begin();
    while (i != scc.end() &&  (*i)->emptyDelta()) 
        i++;
    return (i == scc.end());
}


// record known extensions and non-extensions of (non solved) predicates whose scc is greater or equal than 'sccIndex' (for back-track)
void Graph::recordExtensions(int sccIndex){
    int max = numberOfScc();
    do {    // record extensions and non-extensions of scc corresponding to 'sccIndex'
        Node::Set& scc = _sccVector.at(sccIndex);
        for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
            (*i)->recordExtensions();
    } while (nextNonSolvedScc(sccIndex, max));  // next scc
}


// restore previous extensions and non-extensions of (non solved) predicates whose scc is greater or equal than 'sccIndex' 
void Graph::restoreExtensions(int sccIndex){
    int max = numberOfScc();
    do {    // record extensions and non-extensions of scc corresponding to 'sccIndex'
        Node::Set& scc = _sccVector.at(sccIndex);
        for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
            (*i)->restoreExtensions();
    } while (nextNonSolvedScc(sccIndex, max));  // next scc
}


void Graph::restoreExtensionsWithoutPop(int sccIndex){
    int max = numberOfScc();
    do {    // record extensions and non-extensions of scc corresponding to 'sccIndex'
        Node::Set& scc = _sccVector.at(sccIndex);
        for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
            (*i)->restoreExtensionsWithoutPop();
    } while (nextNonSolvedScc(sccIndex, max));  // next scc
}


// update extensions and non-extensions of (non solved) predicates whose scc is greater or equal than 'sccIndex'
void Graph::updateExtensions(int sccIndex){
    int max = numberOfScc();
    do {    // update extensions and non-extensions of scc corresponding to 'sccIndex'
        Node::Set& scc = _sccVector.at(sccIndex);
        for (Node::Set::iterator i = scc.begin(); i != scc.end(); ++i)
            (*i)->updateExtensions();
    } while (nextNonSolvedScc(sccIndex, max));  // next scc
}


// restore non solved status for predicates of all (non initialy solved) scc in the range [first, last[
void Graph::restoreNonSolvedScc(int first, int last){
    int scc = first;
    do {
        setSolved(false, scc);
    } while (nextNonSolvedScc(scc, last));  // false if there is no more non solved scc < last
}
