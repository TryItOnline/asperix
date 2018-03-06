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


#ifndef _RULE_HPP_
#define _RULE_HPP_

#include <algorithm>
#include <stack>

#include "changes.hpp"
#include "definition.hpp"
#include "literal.hpp"
#include "negative_literal.hpp"
#include "positive_literal.hpp"


class ConstraintRule;
class NormalRule;



//****************************************************************************//
//                                    Body                                    //
//****************************************************************************//


class Body : public Literal::Vector{

//******************************* NESTED TYPES *******************************//

    public:
        typedef std::stack<Body::iterator> IteratorStack;


//***************************** MEMBER VARIABLES *****************************//

    protected:
        variableSet _variables;


//******************************** DESTRUCTOR ********************************//

    public:
        virtual ~Body();


//********************************** GETTERS *********************************//

        inline variableSet& getVariables();


//************************** OTHER MEMBER FUNCTIONS **************************//

        inline void addLiteral(Literal* l);

        inline bool contains(Literal* l);

        /// return true if current instance of body contains current instance of literal *l
        bool containsInstance(BasicLiteral* l);

        /// return true if *l appears as negative literal in body (*l and body are not instanciated)
        bool containsNotLiteral(BasicLiteral* l);

        inline bool containsOnePredFrom(Node::Set& ns);

        inline bool containsOnePredFrom(int scc);

        inline bool containsOnePredNotFrom(int scc);

        /// if body contains exactly one literal return it, else return NULL
        inline Literal* isSingleLiteral();

        void addBodyPlusDefiniteRule(Rule* r);

        void addBodyPlusDefiniteRule(int sccIndex, Rule* r);

        void addBodyPlusNonDefiniteRule(int sccIndex, Rule* r);

        void addBodyPlusDefiniteConstraint(Rule* r);

        void removeBodyPlusDefiniteConstraint(Rule* r);

        void applySubstitutionAndAddNonInstances(Program& p,Changes& ch);

        /// "recursive" literals are those whose predicate appears as label of a node in ns
        void split(Node::Set& ns, Body& recBody, Body& nonRecBody);

        /// "recursive" literals are those whose predicate is pred
        void split(Predicate* pred, Body& recBody, Body& nonRecBody);

        /// "recursive" literals are those which are not already solved
        void split(Body& recBody, Body& nonRecBody);

        void orderPlus(Body& recBody, Body& nonRecBody, Body& ordBody);

        void order(Body& recBody, Body& nonRecBody, Body& ordBody);

        void initVars();

        /// create a new body by applying current substitution
        Body* createInstance();

        /// create clones of literals and add them to b
        void addPositiveClone(Body& b, MapVariables& m);

        /// set delta index of literals of the body+ of a non definite rule
        inline void setDelta();

        /// init delta index of literals of the body+ of a non definite rule
        inline void initDelta();

        /// restore delta indexes of literals of the body
        inline void restoreDeltas();

        /// record marks of literals of the body
        inline void recordMarks();

        /// restore marks of literals of the body
        inline void restoreMarks();

        /// record current indexes and status of literals of the body
        inline void recordIndexesAndStatus();

        /// restore current indexes and status of literals of the body
        inline void restoreIndexesAndStatus();

        /// return true if at least one literal is in the model (all literals must be ground)
        inline bool isBlocked();

        inline bool isSolved();


}; // class Body


//********************************** GETTERS *********************************//

inline variableSet& Body::getVariables(){
    return _variables;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline void Body::addLiteral(Literal* l){
    push_back(l);
    l->addVariables(_variables);
}


inline bool Body::contains(Literal* lit){
    return (find(begin(),end(),lit) != end());
}


// return true if body contains at least one predicate that appears as label of a node from ns
inline bool Body::containsOnePredFrom(Node::Set& ns){
    Body::iterator it = begin();
    while ((it != end()) && !(*it)->hasPredIn(ns))
        ++it;
    return (it != end());
}


// return true if body contains at least one predicate from scc
inline bool Body::containsOnePredFrom(int scc){
    Body::iterator it = begin();
    while ((it != end()) && (*it)->getSccIndex() != scc)
        ++it;
    return (it != end());
}


// return true if body contains at least one predicate from another scc than 'scc'
inline bool Body::containsOnePredNotFrom(int scc){
    Body::iterator it = begin();
    while ((it != end()) && (*it)->getSccIndex() == scc) {
        ++it;
    }
    return (it != end());
}


// if body contains exactly one literal return it, else return NULL
inline Literal* Body::isSingleLiteral(){
    if (size() == 1) 
        return *begin();
    else
        return NULL;
}


// set delta index of literals of the body+ of a non definite rule
inline void  Body::setDelta(){
    for (Body::iterator it = begin(); it != end(); it++)
        (*it)->setDelta();
}


// init delta index for literals of the body+ of a non definite rule
inline void  Body::initDelta(){
    for (Body::iterator it = begin(); it != end(); it++)
        (*it)->initDelta();
}


// restore delta indexes of literals of the body
inline void Body::restoreDeltas(){
    for (Body::iterator it = begin(); it != end(); it++)
        (*it)->restoreDelta();
}


// record marks of literals of the body
inline void Body::recordMarks(){
    for (Body::iterator it = begin(); it != end(); it++)
        (*it)->recordMark();	
}


// restore marks of literals of the body
inline void Body::restoreMarks(){
    for (Body::iterator it = begin(); it != end(); it++)
        (*it)->restoreMark();
}


// record current indexes of literals of the body
inline void Body::recordIndexesAndStatus(){
    for (Body::iterator it = begin(); it != end(); it++) {
        (*it)->recordIndex();
        (*it)->recordStatus();
    }
}


// restore current indexes of literals of the body
inline void Body::restoreIndexesAndStatus(){
    for (Body::iterator it = begin(); it != end(); it++) {
        (*it)->restoreIndex();	
        (*it)->restoreStatus();
    }
}


// require all literals are ground
// return true if at least one literal is in the model 
inline bool Body::isBlocked(){
    Body::iterator it = begin();
    while ((it != end()) && !(*it)->isInstance())
        it++;
    return (it != end());
}


// return true if all literals'predicates are solved
inline bool Body::isSolved(){
    Body::iterator it = begin();
    while ((it != end()) && (*it)->isSolved())
        it++;
    return (it == end());
}





//****************************************************************************//
//                                    Rule                                    //
//****************************************************************************//


class Rule{

//******************************* NESTED TYPES *******************************//

    public:
//         typedef std::set<Rule*> Set;


//***************************** MEMBER VARIABLES *****************************//

    protected:
        BasicLiteral* _head;

        Body* _body;

        /// positive body
        Body* _bodyPlus;

        /// negative body
        Body* _bodyMinus;

        variableSet _variables;

        Body _orderedBody;

        /// current literal of _orderedBody
        Body::iterator _currentLiteral;

        /// recursive literals of _orderedBody
        Body _recursiveBody;

        /// current recursive literal
        Body::iterator _recursiveLiteral;

        /// true if each predicate that appears in body+ is already solved
        bool _solved;

        /// definite clone of this rule
        Rule* _clone;

        /// back-track stack for current recursive literal _recursiveLiteral
        Body::IteratorStack _recLit_stack;

        /// back-track stack for _solved property
        boolStack _solved_stack;


//******************************* CONSTRUCTORS *******************************//

    public:
        Rule();

        ///constructor for normal rules
        Rule(BasicLiteral*, Body*);

        ///constructor for normal rules
        Rule(BasicLiteral*, Body*, const variableSet&, Graph&) throw (LogicExcept);


//******************************** DESTRUCTOR ********************************//

        virtual ~Rule();


//********************************** GETTERS *********************************//

        inline BasicLiteral* getHead();

        inline Body* getBody();

        inline Body* getBodyPlus();

        inline Body* getBodyMinus();

        inline Literal* currentLiteral();

        inline Literal* recursiveLiteral();

        inline bool isSolved();

        inline Rule* getClone();


//********************************** SETTERS *********************************//

        inline void setSolved(bool v);

        inline void setClone(Rule* r);


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream&, Rule&);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        void clear();

        virtual inline bool isNormalRule();

        inline bool isDefinite();

        void orderBody(Node::Set& ns);

        void orderBody(Predicate* pred);

        virtual void orderBody();

        virtual void orderPositiveBody();

        /// free each variable of the rule
        inline void free();

        /// true if the rule is recursive r.t. SCC ns
        bool recursive(Node::Set& ns);

        /// can not be NULL
        inline Literal* firstLiteral();

        inline Literal* nextLiteral();

        inline Literal* previousLiteral();

        /// can not be NULL
        inline Literal* firstRecursiveLiteral();

        inline Literal* nextRecursiveLiteral();

        /// previous match status
        inline statusEnum previousStatus();

        virtual inline void evaluate();

        virtual inline void evaluateRec();

        virtual inline void evaluate(atomsQueue&);

        virtual inline void evaluateRec(atomsQueue&);

        /// TODO non défini pour facts 
        virtual bool firstInstanceNonDefiniteRule();

        /// TODO non défini pour facts 
        virtual bool nextInstanceNonDefiniteRule();

        /// TODO non défini pour facts (en fait défini pour constraint)
        virtual bool existsInstanceNonDefiniteRule();

        /// TODO non défini pour facts (en fait défini pour constraint)
        virtual bool existsInstanceDefiniteConstraint();

        /// TODO non défini pour facts 
        virtual bool existsInstanceDefiniteRuleRec();

        /// apply substitution and add head to the model  TODO non défini pour facts et constraints
        virtual inline bool addHead(Program&,Changes&);

        /// apply substitution and add body- out of the model  TODO non défini pour facts et constraints
        virtual inline bool addBodyMinus(Program&, Changes&);

        /// create a new constraint rule by applying current substitution
        virtual ConstraintRule* createInstance();

        void prohibiteCurrentInstanceOfNDR(Program&, Changes&);

        void prohibiteCurrentInstanceOfNDRWhenSearchingForChoicePoint(Program&, Changes&);

        /// create a new definite rule by cloning
        virtual Rule* createDefiniteClone();

        void shiftLocallySolvedLiteralsFromBMinus2BPlus(int sccIndex);

        /// init atoms iterators in predicate extensions
        inline void initMarks(markEnum);

        /// TODO non défini pour facts et constraints
        virtual inline void setDelta();

        /// TODO défini seulement pour constraints
        virtual inline void initDelta();

        /// restore delta indexes of literals of the body+
        inline void restoreDeltas();

        void recordCurrentInstanceNDR();

        void restorePreviousInstanceNDR();

        /// require _bodyminus is ground
        inline bool isBlocked();

        ///return SCC index of head predicate
        /// or, for a constraint rule, max of SCC indexes of body predicates
        virtual inline int getSccIndex();

    protected:
        virtual std::ostream& put(std::ostream&);

        /// search for a true supported and non-blocked instance of a non-definite rule
        bool trueInstanceNonDefiniteRule(Literal* lit, Literal* recLit, bool matchFound);

        /// search for a "soft" supported (true or mbt) and non-blocked instance of a non-definite rule
        statusEnum softInstanceNonDefiniteRule(Literal* lit, Literal* recLit, statusEnum matchFound);


}; // class Rule


//********************************** GETTERS *********************************//

inline BasicLiteral* Rule::getHead(){
    return _head;
}


inline Body* Rule::getBody(){
    return _body;
}


inline Body* Rule::getBodyPlus(){
    return _bodyPlus;
}


inline Body* Rule::getBodyMinus(){
    return _bodyMinus;
}


inline Literal* Rule::currentLiteral(){
    return *_currentLiteral;
}


inline Literal* Rule::recursiveLiteral(){
    return *_recursiveLiteral;
}


inline bool Rule::isSolved(){
    return _solved;
}


inline Rule* Rule::getClone(){
    return _clone;
}


inline bool Rule::isNormalRule(){
    return true;
}


//********************************** SETTERS *********************************//

inline void Rule::setSolved(bool v){
    _solved = v;
}


inline void Rule::setClone(Rule* r){
    _clone = r;
}


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, Rule& r){
    return r.put(os);
}


inline std::ostream& operator << (std::ostream& os, ruleSet& rs){
    for (ruleSet::iterator it = rs.begin(); it != rs.end(); it++)
        os <<  *(*it);
    return os;
}


inline std::ostream& operator << (std::ostream& os, ruleMultimap& rm){
    for (ruleMultimap::iterator it = rm.begin(); it !=  rm.end() ; it++)
        os << "(scc " << (*it).first << ") "<< *(*it).second;
    return os;
}



//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool Rule::isDefinite(){
    return _bodyMinus->empty();
}


inline void Rule::free(){
    for (variableSet::iterator i = _variables.begin(); i != _variables.end(); i++)
        (*i)->free();
}


// require _orderedBody not empty
inline Literal* Rule::firstLiteral(){
    _currentLiteral = _orderedBody.begin();
    return *_currentLiteral;
}


inline Literal* Rule::nextLiteral(){
    if (++_currentLiteral != _orderedBody.end())
        return *_currentLiteral;
    else {
        _currentLiteral--;
        return NULL;
    }
}


// require _recursiveBody not empty
inline Literal* Rule::firstRecursiveLiteral(){
    _recursiveLiteral = _recursiveBody.begin();
    return *_recursiveLiteral;
}


inline Literal* Rule::nextRecursiveLiteral(){
    if (++_recursiveLiteral != _recursiveBody.end())
        return *_recursiveLiteral;
    else {
        _recursiveLiteral--;
        return NULL;
    }
}


inline Literal* Rule::previousLiteral(){
    if (_currentLiteral != _orderedBody.begin())
        return *(--_currentLiteral);
    else
        return NULL;
}


// previous match status
inline statusEnum Rule::previousStatus(){
    if (_currentLiteral != _orderedBody.begin()) {
        Body::iterator it = _currentLiteral;
        return (*--it)->getStatus();
    }
    else
        return TRUE_;   // no previous status
}


inline void Rule::evaluate(){}


inline void Rule::evaluateRec(){}


inline void Rule::evaluate(atomsQueue&){}


inline void Rule::evaluateRec(atomsQueue&){}


// apply substitution and add head to the model  TODO non défini pour facts et constraints
inline bool Rule::addHead(Program&,Changes&){
    return false;
}


// apply substitution and add body- out of the model  TODO non défini pour facts et constraints
inline bool Rule::addBodyMinus(Program&, Changes&){
    return false;
}


inline void Rule::initMarks(markEnum mark){
    for (Body::iterator i = _orderedBody.begin(); i != _orderedBody.end(); i++)
        (*i)->setMark(mark);
}


// TODO non défini pour facts et constraints
inline void Rule::setDelta(){}


// TODO défini seulement pour constraints
inline void Rule::initDelta(){}


// restore delta indexes of literals of the body+
inline void Rule::restoreDeltas(){
    _bodyPlus->restoreDeltas();
}


// require _bodyminus is ground
inline bool Rule::isBlocked(){
    return _bodyMinus->isBlocked();
}


// return SCC index of head predicate
inline int Rule::getSccIndex(){
    return getHead()->getSccIndex();
}



#endif // _RULE_HPP_
