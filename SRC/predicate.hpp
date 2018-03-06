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
 * @brief Declarations of class representing predicates.
 * @author Claire Lefèvre, Pascal Nicolas, Stéphane Ngoma
 */


#ifndef _PREDICATE_HPP_
#define _PREDICATE_HPP_

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "arguments.hpp"
#include "definition.hpp"



class Graph;
class Node;



/// @todo TODO Check the correct initialization of members in constructors.
/// @todo TODO Check getters which return a pointer (for the @c const modifier).
/// @todo TODO Check which functions don't modify the instance.
class Predicate {

//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        static mapPredicate _mapPredicate;

        static Predicate* _truePredicate;


//***************************** MEMBER VARIABLES *****************************//

        const std::string _name;

        int _arity;

        bool _show; 

        /**
         * @brief Indicates whether this predicate is solved.
         * 
         * A predicate is solved when its positive extension is completely known.
         * 
         * @sa isSolved(), setSolved(bool)
         */
        bool _solved;

        /**
         * Corresponding "-p" predicate for true strong negation.
         * @sa getOppositePredicate(), setOppositePredicate(Predicate *)
         * @author Stéphane NGOMA
         */
        Predicate* _oppositePredicate;

        /// corresponding "not_p" predicate for strong negation
        Predicate* _negatedPredicate;

        /// extension of the predicate (a tuple is TRUE_, TRUE_MBT_, or MBT_)
        std::map<Argument, statusEnum> _instances;

        /// ordered extension (a tuple is TRUE_, TRUE_MBT_, or MBT_)
        std::vector< std::pair<const Argument*, statusEnum> > _orderedInstances;

        /// number of MBT tuples in the extension
        int _mbtNumber;

        int _deltaBegin;

        int _deltaEnd;

        /// back-track stack for _instances end index
        intStack _indexStack;

        /// definite rules that define the predicate
        ruleSet* _headDefiniteRules;

        /// normal (but not definite) rules that define the predicate
        ruleSet* _headNonDefiniteRules;

        /// definite rules in which predicate appears in body+
        ruleMultimap* _bodyPlusDefiniteRules;

        /// normal (but not definite) rules in which predicate appears in body+ 
        ruleMultimap* _bodyPlusNonDefiniteRules;

        /// constraint rules (with empty body-) in which predicate appears in body+ 
        ruleSet* _bodyPlusDefiniteConstraints;

        /// the corresponding node in the dependance graph
        Node* _node;


//************************** STATIC MEMBER FUNCTIONS *************************//

    protected:
        /**
         * @internal
         * @brief Create a new predicate and its negated predicate.
         * @param name name of the new predicate.
         * @param arity arity of the new predicate.
         * @param graph dependency graph.
         * 
         * 
         * 
         * @sa newPredicate(const std::string&, int, Graph&)
         *     newTruePredicate(Graph&)
         * @author Stéphane NGOMA
         */
        static Predicate* newPredicateBis(const std::string& name, int arity, Graph& graph);

    public:
        static Predicate* newPredicate(const std::string&, int, Graph&);

        static Predicate* newTruePredicate(Graph&); 

        /// @todo TODO Set this function @c inline.
        static Predicate* getTruePredicate(); 

        static bool initShow(bool defaultShow, const stringSet& pset);

        static bool existsMbtInstance();

        /// delete all predicates
        static void deleteAll();

        static void printAllInstances();

        static void prettyPrintAllInstances();

        static void prettyPrintAllNonInstances();

        /// record all known extensions and non-extensions(for back-track)
        static void recordExtensions();

        /// restore all previous extensions and non-extensions
        static void restoreExtensions();

        static void restoreExtensionsWithoutPop();


//******************************* CONSTRUCTORS *******************************//

    protected:
        Predicate(const std::string&, int);


//******************************** DESTRUCTOR ********************************//

    public:
        virtual ~Predicate();


//********************************** GETTERS *********************************//
	
        inline bool isShown() const;

        /**
         * @brief Indicates if a predicate is solved or not.
         * @return @c true if this predicate is solved, @c false otherwise.
         * 
         * @note A predicate is "solved" when its positive extension is
         *       completely known.
         * 
         * @sa setSolved(bool)
         */
        inline bool isSolved() const;

        /**
         * @brief Tests whether the opposite of a predicate exists or not.
         * @return @c true if the opposite predicate of this predicate is
         *         defined, @c false otherwise.
         * 
         * @note This function is equivalent to
         *       <tt>(getOppositePredicate() != NULL)</tt>.
         * 
         * @sa getOppositePredicate()
         * @author Stéphane NGOMA
         */
        inline bool hasOppositePredicate() const throw();

        /**
         * @brief Returns the negative predicate associated, if any.
         * @return The predicate "<tt> -p</tt>" associated, or @c NULL if it
         *         doesn't exist.
         * @sa hasOppositePredicate()
         * @sa setOppositePredicate(Predicate*)
         * @author Stéphane NGOMA
         */
        inline Predicate* getOppositePredicate() throw();

        inline Predicate* getNegatedPredicate();

        inline int getMbtNumber() const;

        inline int getDeltaBeginIndex() const;

        inline int getDeltaEndIndex() const;

        inline ruleSet* getHeadDefiniteRules();

        inline ruleSet* getHeadNonDefiniteRules();

        inline ruleMultimap* getBodyPlusDefiniteRules();

        inline ruleMultimap* getBodyPlusNonDefiniteRules();

        inline ruleSet* getBodyPlusDefiniteConstraints();

        inline Node* getNode();


//********************************** SETTERS *********************************//

        inline void setShow(bool b);

        /**
         * @brief Indicates that a Predicate must be shown.
         * 
         * @note This function is equivalent to <tt>setShow(true)</tt>.
         * 
         * @sa setShow(bool), hide()
         */
        inline void show();

        /**
         * @brief Indicates that a Predicate must not be shown.
         * 
         * @note This function is equivalent to <tt>setShow(false)</tt>.
         * 
         * @sa setShow(bool), show()
         */
        inline void hide();

        /**
         * @brief Marks a predicate as solved or not solved.
         * @param solved @c true if this predicate is solved, @c false otherwise.
         * 
         * @note A predicate is "solved" when its positive extension is
         *       completely known.
         * 
         * @sa isSolved()
         */
        virtual inline void setSolved(bool solved);

        /**
         * @brief Associate the opposite predicate.
         * @param p the "-p" predicate to be associated.
         * @sa getOppositePredicate()
         * @author Stéphane NGOMA
         */
        inline  void setOppositePredicate(Predicate* p) throw();

        inline void setNegatedPredicate(Predicate* p);

        inline void setDelta();

        inline void setDelta(int index);

        inline void setNode(Node* n);


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream&, const Predicate&);


//************************** OTHER MEMBER FUNCTIONS **************************//

    protected:
        /**
         * @brief Adds (or updates) @a tv with #TRUE_ status to the extension of
         *        the predicate.
         * @param tv the instanciation of the predicate to add to the extension.
         * @return @c true if @a tv has been added or modified, or @c false if
         *         @a tv was already in the extension with #TRUE_ status.
         * 
         * If @a tv was already in the extension with #MBT_ status, it is
         * updated to #TRUE_MBT_.
         * 
         * @sa addTrueInstance(Argument&),
         *     addTrueInstance(Argument&, atomsQueue&)
         * @sa statusEnum
         * @author Stéphane NGOMA
         */
        inline bool addTrueInstance_impl(Argument& tv);

    public:
        inline bool addTrueInstance(Argument&);

        inline bool addTrueInstance(Argument&, atomsQueue&);

    protected:
        /**
         * @brief Adds @a tv with #MBT_ status to extension of the predicate.
         * @param tv the instanciation of the predicate to add to the extension.
         * @return @c true if @a tv has been added, or @c false if @a tv was
         *         already in the extension.
         * @sa addMbtInstance(Argument&), addMbtInstance(Argument&, atomsQueue&)
         * @sa statusEnum
         * @author Stéphane NGOMA
         */
        inline bool addMbtInstance_impl(Argument&);

    public:
        inline bool addMbtInstance(Argument&);

        inline bool addMbtInstance(Argument&, atomsQueue&);

        inline bool addNonInstance(Argument&);

        inline bool addNonInstance(Argument&, atomsQueue&);

        /**
         * @brief Returns the status of a term vector if it's a known instance
         *        of a predicate.
         * @param term a term vector.
         * @return The status of @a term while instanciating this predicate, or
         *         #NO_ if @a term is not an instance of this predicate.
         * 
         * @sa containInstance(const Argument&, int, int),
         *     containNonInstance(const Argument&)
         */
        virtual statusEnum containInstance(const Argument& term) const;

        /**
         * @brief Returns the status of a term vector if it appears inside an
         *        range of the list of a predicate's instances.
         * @param term a term vector.
         * @param begin the range's lower index.
         * @param end the range's upper index.
         * @return The status of @a term while instanciating this predicate if
         *         it lies inside the interval [@a begin,@a end] of the list of
         *         instances, or #NO_ otherwise.
         * 
         * The list used is ordered by the order instances have been found
         * during the search. So if a term vector appears several times with
         * different status, the more recent is returned.
         * 
         * @sa containInstance(const Argument&),
         *     containNonInstance(const Argument&)
         */
        virtual statusEnum containInstance(const Argument& term, int begin, int end) const;

        /**
         * @brief Tests whether a term vector is a known non-instance of a
         *        predicate.
         * @param term a term vector.
         * @return @c true if @a term is a known non-instance of this predicate,
         *         @c false otherwise.
         * 
         * @sa containInstance(const Argument&),
         *     containInstance(const Argument&, int, int)
         */
        bool containNonInstance(const Argument& term) const;

        void printInstances();

        void printNonInstances();

        void prettyPrintInstances();

        void prettyPrintNonInstances();

        inline const Argument* firstTrueInstance(int& i, int begin_i, int end_i);

        inline const Argument* nextTrueInstance(int& i, int end_i);

        inline std::pair<const Argument*, statusEnum> firstSoftInstance(int& i, int begin_i, int end_i);

        inline std::pair<const Argument*, statusEnum> nextSoftInstance(int& i, int end_i);

        inline void addHeadDefiniteRule(Rule*);

        inline void addHeadNonDefiniteRule(Rule*);

        inline void removeHeadNonDefiniteRule(Rule*);

        inline void addBodyPlusDefiniteRule(int scc, Rule*r);

        inline void addBodyPlusNonDefiniteRule(int scc, Rule*r);

        inline void addBodyPlusDefiniteConstraint(Rule*);

        inline void removeBodyPlusDefiniteConstraint(Rule*);

        inline void initDelta();

        inline bool emptyDelta();

        inline int getBeginIndex();

        inline int getEndIndex();

        /// record known extension (for back-track)
        void recordExtension();

        void recordNonExtension();

        /// restore previous extension 
        void restoreExtension();

        void restoreNonExtension();

        /// restore previous extension 
        void restoreExtensionWithoutPop();

        void restoreNonExtensionWithoutPop();

        void updateExtension();

        void updateNonExtension();


}; // class Predicate


//********************************** GETTERS *********************************//

inline bool Predicate::isShown() const{
    return _show;
}


// Indicates if a predicate is solved or not.
inline bool Predicate::isSolved() const{
    return _solved;
}


// Tests whether the opposite of a predicate exists or not
inline bool Predicate::hasOppositePredicate() const throw(){
    return (_oppositePredicate != NULL);
}


// Returns the negative predicate associated, if any
inline Predicate* Predicate::getOppositePredicate() throw(){
    return _oppositePredicate;
}


inline Predicate* Predicate::getNegatedPredicate(){
    return _negatedPredicate;
}


inline int Predicate::getMbtNumber() const{
    return _mbtNumber;
}


inline int Predicate::getDeltaBeginIndex() const{
    return _deltaBegin;
}


inline int Predicate::getDeltaEndIndex() const{
    return _deltaEnd;
}


inline ruleSet* Predicate::getHeadDefiniteRules(){
    return _headDefiniteRules;
}


inline ruleSet* Predicate::getHeadNonDefiniteRules(){
    return _headNonDefiniteRules;
}


inline ruleMultimap* Predicate::getBodyPlusDefiniteRules(){
    return _bodyPlusDefiniteRules;
}


inline ruleMultimap* Predicate::getBodyPlusNonDefiniteRules(){
    return _bodyPlusNonDefiniteRules;
}


inline ruleSet* Predicate::getBodyPlusDefiniteConstraints(){
    return _bodyPlusDefiniteConstraints;
}


inline Node* Predicate::getNode(){
    return _node;
}


//********************************** SETTERS *********************************//

inline void Predicate::setShow(bool b){
    _show = b;
}


// Indicates that a Predicate must be shown
inline void Predicate::show(){
    setShow(true);
}


// Indicates that a Predicate must not be shown
inline void Predicate::hide(){
    setShow(false);
}


/**
 * @brief Marks a predicate as solved or not solved.
 * @param solved @c true if this predicate is solved, @c false otherwise.
 * 
 * @note A predicate is "solved" when its positive extension is
 *       completely known.
 * 
 * @sa isSolved()
 */
inline void Predicate::setSolved(bool solved){
    _solved = solved;
    _negatedPredicate->setSolved(solved);
}


// Associate the opposite predicate
inline void Predicate::setOppositePredicate(Predicate* p) throw(){
    _oppositePredicate = p;
}


inline void Predicate::setNegatedPredicate(Predicate* p){
    _negatedPredicate = p;
}


inline void Predicate::setDelta(){
    _deltaBegin = _deltaEnd + 1;
    _deltaEnd = _orderedInstances.size() - 1;
}


inline void Predicate::setDelta(int index){
    _deltaBegin = _deltaEnd = index;
}


inline void Predicate::setNode(Node* n){
    _node = n;
}


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, const Predicate& p){
    return os << p._name;
}


inline std::ostream& operator << (std::ostream& os, const std::map<Argument, statusEnum>& m){
    os << '{';
    for (std::map<Argument, statusEnum>::const_iterator it = m.begin(); it != m.end(); ++it)
        os << ((it == m.begin()) ? "" : ",") << it->first << it->second;
    return os << '}' << std::endl;
}


//************************** OTHERS MEMBER FUNCTIONS *************************//

// Adds (or updates) tv with TRUE_ status to extension of the predicate
inline bool Predicate::addTrueInstance_impl(Argument& tv){
    std::map<Argument, statusEnum>::iterator it = _instances.find(tv);
    if (it != _instances.end()) {       // tv was already in the extension
        if (it->second == MBT_) {       // with status MBT
            it->second = TRUE_MBT_;
            --_mbtNumber;
            _orderedInstances.push_back(std::make_pair(&it->first, TRUE_MBT_));
            return true;
        } else
            return false;
    } else {
        if (containNonInstance(tv)) {   // tv is known to be out of the extension
            throw ContradictoryConclusion();
        } else {
            std::pair<std::map<Argument, statusEnum>::iterator, bool> i_b = _instances.insert(std::make_pair(tv, TRUE_));
            _orderedInstances.push_back(std::make_pair(&(i_b.first->first), TRUE_));
            return true;
        }
    }
}


// add (or update) tv with TRUE_ status to extension of the predicate
// if an opposite predicate exists, tv is added to its non-extension (if necessary)
// return true if tv has been added or modified (tv was not already in the extension, or with status MBT_)
inline bool Predicate::addTrueInstance(Argument& tv){
    bool added = addTrueInstance_impl(tv);

    // The opposite predicate, if it exists, can't be true too
    if (added && hasOppositePredicate())
        _oppositePredicate->addNonInstance(tv);

    return added;
}


// return true if tv has been added or modified (i.e. it was not already in the extension or with status MBT)
// throw exception "ContradictoryConclusion" if tv was known to be out of the extension
inline bool Predicate::addTrueInstance(Argument& tv, atomsQueue& aq){
    bool added = addTrueInstance_impl(tv);
    if (added) {
        aq.push(std::make_pair(this, _orderedInstances.size() - 1));

        // The opposite predicate, if it exists, can't be true too
        if (hasOppositePredicate() && !_oppositePredicate->isSolved())
            _oppositePredicate->addNonInstance(tv, aq);
    }
    return added;
}


// Adds tv with MBT_ status to the extension of the predicate
inline bool Predicate::addMbtInstance_impl(Argument& tv){
    std::map<Argument, statusEnum>::iterator it = _instances.find(tv);
    if (it != _instances.end()) {       // tv was already in the extension
        return false;
    } else {
        if (containNonInstance(tv)) {   // tv is known to be out of the extension
            throw ContradictoryConclusion();
        } else {
            std::pair<std::map<Argument, statusEnum>::iterator, bool> i_b = _instances.insert(std::make_pair(tv, MBT_));
            _orderedInstances.push_back(std::make_pair(&(i_b.first->first), MBT_));
            ++_mbtNumber;
            return true;
        }
    }
}


// add tv with MBT_ status to extension of the predicate
// if an opposite predicate exists, tv is added to its non-extension (if necessary)
// return true if tv has been added (tv was not already in the extension)
inline bool Predicate::addMbtInstance(Argument& tv){
    bool added = addMbtInstance_impl(tv);

    // The opposite predicate, if it exists, can't be true too
    if (added && hasOppositePredicate())
        _oppositePredicate->addNonInstance(tv);

    return added;
}


// return true if tv has been added (i.e. it was not already in the extension)
// throw exception "ContradictoryConclusion" if tv was known to be out of the extension
inline bool Predicate::addMbtInstance(Argument& tv, atomsQueue& aq){
    bool added = addMbtInstance_impl(tv);
    if (added) {
        aq.push(std::make_pair(this, _orderedInstances.size() - 1));

        // The opposite predicate, if it exists, can't be true too
        if (hasOppositePredicate() && !_oppositePredicate->isSolved())
            _oppositePredicate->addNonInstance(tv, aq);
    }
    return added;
}


inline bool Predicate::addNonInstance(Argument& tv){
    return _negatedPredicate->addTrueInstance(tv);
}


// add tv as non-instance of the predicate, and push it in naq queue
// return true if tv has been added
// throw exception "ContradictoryConclusion" if tv was known to be in the extension
inline bool Predicate::addNonInstance(Argument& tv, atomsQueue& naq){
    return _negatedPredicate->addTrueInstance(tv, naq);
}


inline const Argument * Predicate::firstTrueInstance(int& i, int begin_i, int end_i){
    i = begin_i;
    while ((i <= end_i) && (_orderedInstances.at(i).second == MBT_))
        ++i;
    if (i <= end_i)
        return _orderedInstances.at(i).first;
    else 
        return NULL;
}


inline const Argument * Predicate::nextTrueInstance(int& i, int end_i){
    ++i;
    while ((i <= end_i) && (_orderedInstances.at(i).second == MBT_))
        ++i;
    if (i <= end_i) 
        return _orderedInstances.at(i).first;
    else 
        return NULL;
}


inline std::pair<const Argument *, statusEnum> Predicate::firstSoftInstance(int& i, int begin_i, int end_i){
    i = begin_i;
    if (i <= end_i) 
        return _orderedInstances.at(i);
    else {
        const Argument *p = NULL;
        return (std::make_pair(p, NO_));
    }
}


inline std::pair<const Argument *, statusEnum> Predicate::nextSoftInstance(int& i, int end_i){
    ++i;
    if (i <= end_i) 
        return _orderedInstances.at(i);
    else {
        const Argument *p = NULL;
        return std::make_pair(p, NO_);
    }
}


inline void Predicate::addHeadDefiniteRule(Rule* r){
    _headDefiniteRules->insert(r);
}


inline void Predicate::addHeadNonDefiniteRule(Rule* r){
    _headNonDefiniteRules->insert(r);
}


inline void Predicate::removeHeadNonDefiniteRule(Rule* r){
    _headNonDefiniteRules->erase(r);
}


inline void Predicate::addBodyPlusDefiniteRule(int scc, Rule* r){
    bool trouve=false;
    std::pair<ruleMultimap::iterator, ruleMultimap::iterator> range;
    range = _bodyPlusDefiniteRules->equal_range(scc);
    for (ruleMultimap::iterator it = range.first; it != range.second; ++it)
    {
      if(it->second == r)
	  trouve=true;
    }
    if (!trouve)
      _bodyPlusDefiniteRules->insert(std::make_pair(scc, r));
}


inline void Predicate::addBodyPlusNonDefiniteRule(int scc, Rule* r){
    _bodyPlusNonDefiniteRules->insert(std::make_pair(scc, r));
}


inline void Predicate::addBodyPlusDefiniteConstraint(Rule* r){
    _bodyPlusDefiniteConstraints->insert(r);
}


inline void Predicate::removeBodyPlusDefiniteConstraint(Rule* r){
    _bodyPlusDefiniteConstraints->erase(r);
}


inline void Predicate::initDelta(){
    _deltaBegin = 0; // _orderedInstances.size();
    _deltaEnd = _orderedInstances.size() - 1;
}


inline bool Predicate::emptyDelta(){
    return (_deltaBegin > _deltaEnd);
}


inline int Predicate::getBeginIndex(){
    return 0;
}


inline int Predicate::getEndIndex(){
    return (_orderedInstances.size() - 1);
}



#endif // _PREDICATE_HPP_
