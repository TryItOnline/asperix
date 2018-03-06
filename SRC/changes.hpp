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


#ifndef _CHANGES_HPP_
#define _CHANGES_HPP_

#include <iostream>

#include "definition.hpp"



class Program;



class Changes{

//***************************** MEMBER VARIABLES *****************************//

    protected:
        ruleMultimap::iterator _first_NDR;

        int _number_NDR_first_scc;

        int _number_NDR_current_scc;

        int _first_scc;

        /// last  scc to be solved
        int _last_scc;

        /// true if intermediate state has been recorded (scc changing)
        bool _intermediary_scc;

        /// true if intermediate state has been recorded (MBT atom added while prohibiting a rule when searching for a new choice point)
        bool _intermediary_mbt;

        ruleSet _added_prohibited;

        /// atoms added to the model, that need to be propagated
        atomsQueue _atoms_queue;


//******************************* CONSTRUCTORS *******************************//

    public:
        Changes();


//******************************** DESTRUCTOR ********************************//

        virtual ~Changes();


//********************************** GETTERS *********************************//

        inline atomsQueue& getAtomsQueue();


//********************************* OPERATORS ********************************//

    private:
        friend inline std::ostream& operator << (std::ostream& os, Changes& ch);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        inline void addAtom(atomType);

        inline bool intermediary();

        inline void addProhibited(Rule*);

        inline void incNumberNDR();

        void recordState(Program& p);

        void recordExtensions(Program& p);

        void recordCurrentInstanceNDR(ruleMultimap::iterator);

        void recordIntermediarySccExtensions(Program& p, int lastIndex);

        void recordIntermediaryMbtExtensions(Program& p);

        void retropropagate(Program& p);

        void retropropagateSolved(Program& p, int firstScc, int lastScc);

        void restoreAll(Program& p);

        void restoreProhibitedRules(ruleSet&);

        void restorePreviousInstanceNDR(Program& p);


}; // class Changes


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, Changes& ch){
    os << "First_NDR : " ;
    os << &*(*(ch._first_NDR)).second ;
    os << " number of NDR : " << ch._number_NDR_first_scc << std::endl;
    os << "Added prohibited : " << &ch._added_prohibited;

    return os ;
}




//********************************** GETTERS *********************************//


inline atomsQueue& Changes::getAtomsQueue(){
    return _atoms_queue;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

inline void Changes::addAtom(atomType a){
    _atoms_queue.push(a);
}

// return true if an intermediate state has been recorded
inline bool Changes::intermediary(){
    return (_intermediary_scc || _intermediary_mbt);
}


inline void Changes::addProhibited(Rule* r){
    _added_prohibited.insert(r);
}


inline void Changes::incNumberNDR(){
    if (_intermediary_scc)
        _number_NDR_current_scc++;
    else
        _number_NDR_first_scc++;
}



#endif // _CHANGES_HPP_
