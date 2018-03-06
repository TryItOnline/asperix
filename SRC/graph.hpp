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


#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <string>
#include <iostream>
#include <stack>
#include <set>
#include <vector>

#include "definition.hpp"
#include "predicate.hpp"


class Graph;
class Program;



/**********************************************************************************
 * The dependency graph is a graph whose vertices are predicates nodes.
 * There is a positive edge from p1 to p2 iff there is a rule whose head is p1 
 * 							and whose body+ contains p2
 * **********************************************************************/



//****************************************************************************//
//                                    Node                                    //
//****************************************************************************//


class Node{

//******************************* NESTED TYPES *******************************//

    public:
        typedef std::set<Node*> Set;


//***************************** MEMBERS VARIABLES ****************************//

    protected:
        Predicate* _pred;

        Node::Set _posSucc;

        Node::Set _negSucc;

        int _visited;

        int _lowLink;

        bool _onStack;

        /// index of SCC in which the node is
        int _sccIndex;


//******************************* CONSTRUCTORS *******************************//

    public:
        Node(Predicate* p);


//******************************** DESTRUCTOR ********************************//

        virtual ~Node();


//********************************** GETTERS *********************************//

        inline Node::Set& getPosSucc();

        inline Node::Set& getNegSucc();

        inline bool isVisited();

        inline int getSccIndex();


//********************************** SETTERS *********************************//

        inline void setOutStack();

        inline void setSccIndex(int n);


//********************************* OPERATORS ********************************//

    private:
        friend std::ostream& operator << (std::ostream&, Node&);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        inline void addPosSucc(Node* n);

        inline void addNegSucc(Node* n);

        void tarjan(Graph& g);

        void visitSuccessors(Node::Set& succ, Graph& graph);

        void addRules(ruleSet& recRules, ruleSet& exitRules, Node::Set& scc, Program& prog);

        void addDefiniteRulesContainingNotP(ruleMultimap& rmap);

        void addConstraintsContainingNotP(ruleSet& rset);

        inline bool existsMbtInstance();

        /// true if all negative successors are outer the scc
        bool isLocallyStratified(int index);

        bool checkSolveness();

        inline bool isSolved();

        inline void setSolved(bool b);

        inline void setDelta();

        inline void initDelta();

        inline bool emptyDelta();

        inline bool inScc(int n);

        /// record known extensions and non-extensions of _pred (for back-track)
        inline void recordExtensions();

        /// restore previous extensions and non-extensions of _pred
        inline void restoreExtensions();

        inline void restoreExtensionsWithoutPop();

        inline void updateExtensions();


}; // class Node


//********************************** GETTERS *********************************//

inline Node::Set& Node::getPosSucc(){
    return _posSucc;
}


inline Node::Set& Node::getNegSucc(){
    return _negSucc;
}


inline bool Node::isVisited(){
    return _visited;
}


inline int Node::getSccIndex(){
    return _sccIndex;
}


//********************************** SETTERS *********************************//

inline void Node::setOutStack(){
    _onStack = false;
}


inline void Node::setSccIndex(int n){
    _sccIndex = n;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline void Node::addPosSucc(Node* n){
    _posSucc.insert(n);
}


inline void Node::addNegSucc(Node* n){
    _negSucc.insert(n);
}


// return true if the predicate (label of the node) has an instance with MBT status
inline bool Node::existsMbtInstance(){
    return (_pred->getMbtNumber() != 0);
}


inline bool Node::isSolved(){
    return _pred->isSolved();
}


inline void Node::setSolved(bool b){
    _pred->setSolved(b);
}


inline void Node::setDelta(){
    _pred->setDelta();
}


inline void Node::initDelta(){
    _pred->initDelta();
}


inline bool Node::emptyDelta(){
    return _pred->emptyDelta();
}


inline bool Node::inScc(int n){
    return _sccIndex == n;
}


// record known extensions and non-extensions of _pred (for back-track)
void Node::recordExtensions(){
    _pred->recordExtension();
    _pred->recordNonExtension();
}


// restore previous extensions and non-extensions of _pred
void Node::restoreExtensions(){
    _pred->restoreExtension();
    _pred->restoreNonExtension();
}


void Node::restoreExtensionsWithoutPop(){
    _pred->restoreExtensionWithoutPop();
    _pred->restoreNonExtensionWithoutPop();
}


void Node::updateExtensions(){
    _pred->updateExtension();
    _pred->updateNonExtension();
}





//****************************************************************************//
//                                    Graph                                   //
//****************************************************************************//



/**
 * The dependency graph is a graph whose vertices are predicates nodes.
 * There is a positive edge from p1 to p2 iff there is a rule whose head is p1 
 *                          and whose body+ contains p2
 */
class Graph{

//***************************** MEMBER VARIABLES *****************************//

    protected:
        Node::Set _nodes;

        std::stack<Node*> _stack;

        /// current SCC 
        Node::Set _scc;

        /// all SCC
        std::vector<Node::Set> _sccVector;

        /// true at index 'i' iff scc 'i' is solved (stratified)
        std::vector<bool> _solved;

        ///visit of a node 
        int _date;

        /// indexes in _sccVector of non stratified scc
        std::vector<int> _non_stratified_sccVector;

        /// current iterator in _non_stratified_sccVector
        std::vector<int>::iterator _current_non_stratified_scc;



//******************************* CONSTRUCTORS *******************************//

    public:
        Graph();


//******************************** DESTRUCTOR ********************************//

        virtual ~Graph();


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream&, Graph&);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        Node* new_Node(Predicate*);

        void computeSCC();

        inline int nextDate();

        inline void pushStack(Node* n);

        inline void popStack();

        inline Node* topStack();

        inline void addNode2scc(Node* n);

        inline void saveAndClearSCC();

        inline int numberOfScc();

        inline void setSccIndex(int n);

        inline bool firstScc(int& index);

        inline bool nextScc(int& index);

        inline void getNodesFromScc(int index, Node::Set& ns);

        void getRulesFromScc(int index, ruleSet& recRules, ruleSet& exitRules, Program&);

        bool existsMbtInstance(int index);

        void addDefiniteRulesContainingNotP(int index, ruleMultimap& rmap);

        void addConstraintsContainingNotP(int index, ruleSet& rset);

        void initNonStratifiedSccVector();

        /// true if scc does not contain internal negative arc
        bool isLocallyStratified(int index);

        inline int getFirstNonStratifiedScc();

        inline int getNextNonStratifiedScc();

        inline int getPreviousNonStratifiedScc();

        inline bool setFirstNonStratifiedScc(int& index);

        inline bool setNextNonStratifiedScc(int& index);

        inline bool setPreviousNonStratifiedScc(int& index);

        inline int getCurrentSccIndex();

        void setSolvedIfItIsTheCase(int index);

        void setSolved(bool b, int index);

        /// return true if predicates of scc (index) are solved
        inline bool isSolved(int index);

        /// search for the next non solved scc, less then maxIndex
        inline bool nextNonSolvedScc(int& index, int maxIndex);

        void setDelta(int index);

        void initDelta(int index);

        bool emptyDelta(int index);

        /// record known extensions and non-extensions of predicates whose scc is greater or equal than 'sccIndex' (for back-track)
        void recordExtensions(int sccIndex);

        /// restore previous extensions and non-extensions of predicates whose scc is greater or equal than 'sccIndex' 
        void restoreExtensions(int sccIndex);

        void restoreExtensionsWithoutPop(int sccIndex);

        void updateExtensions(int sccIndex);

        void restoreNonSolvedScc(int first, int last);


}; // class Graph


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, Node::Set& ns){
    os << '{';
    for (Node::Set::iterator it = ns.begin(); it != ns.end(); it++)
        os << ((it == ns.begin()) ? "" : ",") << *(*it);
    return os << '}';
}


inline std::ostream& operator << (std::ostream& os , Graph& g){
    for (Node::Set::iterator it = g._nodes.begin(); it != g._nodes.end(); it++)
        os << *(*it) << " + "<< (*it)->getPosSucc() << '-' << (*it)->getNegSucc() << std::endl;
    return os;
}


inline std::ostream& operator << (std::ostream& os, std::vector<Node::Set>& v){
    os << "SCC : ";
    for (std::vector<Node::Set>::iterator it = v.begin(); it !=  v.end(); it++)
        os << ((it == v.begin()) ? "" : " ; ") << (*it);
    return os << std::endl;
}



//************************** OTHER MEMBER FUNCTIONS **************************//


inline int Graph::nextDate(){
    return ++_date;
}


inline void Graph::pushStack(Node* n){
    _stack.push(n);
}


inline void Graph::popStack(){
    _stack.pop();
}


inline Node* Graph::topStack(){
    return _stack.top();
}


inline void Graph::addNode2scc(Node* n){
    _scc.insert(n);
}


inline void Graph::saveAndClearSCC(){
    _sccVector.push_back(_scc);
    setSccIndex(_sccVector.size()-1);
    _scc.clear();
}


inline int Graph::numberOfScc(){
    return _sccVector.size();
}


inline bool Graph::firstScc(int& index){
    index = 0 ;
    return !(_sccVector.empty());
}


inline bool Graph::nextScc(int& index){
    index++;
    return (_sccVector.size() - index > 0);
}


inline void Graph::setSccIndex(int n){
    for (Node::Set::iterator it = _scc.begin(); it != _scc.end(); it++)
        (*it)->setSccIndex(n);
}


inline void Graph::getNodesFromScc(int index, Node::Set& ns){
    ns = _sccVector.at(index);
}


inline int Graph::getFirstNonStratifiedScc(){
    if (_non_stratified_sccVector.begin() != _non_stratified_sccVector.end())
        return getCurrentSccIndex();
    else
        return numberOfScc();
}


inline bool Graph::setFirstNonStratifiedScc(int& index){
    _current_non_stratified_scc = _non_stratified_sccVector.begin();
    if (_current_non_stratified_scc != _non_stratified_sccVector.end()){
        index = getCurrentSccIndex();
        return true;
    }
    else
        return false;
}


inline int Graph::getNextNonStratifiedScc(){
    _current_non_stratified_scc++;
    int scc;
    if (_current_non_stratified_scc != _non_stratified_sccVector.end())
        scc = getCurrentSccIndex();
    else
        scc = numberOfScc();
    _current_non_stratified_scc--;
    return scc;
}


inline bool Graph::setNextNonStratifiedScc(int& index){
    _current_non_stratified_scc++;
    if (_current_non_stratified_scc != _non_stratified_sccVector.end()) {
        index = getCurrentSccIndex();
        return true;
    }
    else {
        _current_non_stratified_scc--;
        return false;
    }
}


inline int Graph::getPreviousNonStratifiedScc(){
    if (_current_non_stratified_scc == _non_stratified_sccVector.begin())
        return numberOfScc();
    else {
        _current_non_stratified_scc--;
        int index = getCurrentSccIndex();
        _current_non_stratified_scc++;
        return index;
    }
}


inline bool Graph::setPreviousNonStratifiedScc(int& index){
    if (_current_non_stratified_scc == _non_stratified_sccVector.begin())
        return false;
    else {
        _current_non_stratified_scc--;
        index = getCurrentSccIndex();
        return true;
    }
}


// return index in _sccVector of the current non stratified scc
inline int Graph::getCurrentSccIndex(){
    return *_current_non_stratified_scc;
}


// return true if predicates of scc (index) are solved (stratified)
inline bool Graph::isSolved(int index){
    return _solved.at(index);
}


// search for the next non (initialy) solved scc, less than 'maxIndex'
inline bool Graph::nextNonSolvedScc(int& index, int maxIndex){
    do {
        ++index;
    } while ((index < maxIndex) && isSolved(index));
    return (index < maxIndex);
}



#endif // _GRAPH_HPP_
