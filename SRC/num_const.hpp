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


#ifndef _NUM_CONST_HPP_
#define _NUM_CONST_HPP_

#include <iostream>

#include "definition.hpp"
#include "term.hpp"



/** NumConst is a numeric constant (integer) 
 * its value is in range [-_maxInt.._maxInt]
 */
class NumConst : public Term{

//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        static mapNumConst _mapNumConst;

        static int _maxInt;


//***************************** MEMBER VARIABLES *****************************//

        int _val;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        static NumConst* new_NumConst(int);

        static inline void setMaxInt(int n);

        static void delete_all();


//******************************* CONSTRUCTORS *******************************//

        NumConst();

        NumConst(int);


//******************************** DESTRUCTOR ********************************//

        virtual ~NumConst();


//********************************* OPERATORS ********************************//

    private:
        friend  inline bool operator == (const NumConst& n1, const NumConst& n2);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        inline bool match(Term* t);

        inline bool isNumConst();

        inline bool isConst();

        inline int getIntValue();

    private:
        inline std::ostream& put(std::ostream&) const;


}; // class NumConst


//************************** STATIC MEMBER FUNCTIONS *************************//

inline void NumConst::setMaxInt(int n){
    _maxInt = n;
}


//********************************* OPERATORS ********************************//

inline bool operator == (const NumConst& n1, const NumConst& n2){
    return (n1._val == n2._val);
}



//************************** OTHER MEMBER FUNCTIONS **************************//

inline bool NumConst::match(Term* t){
    return (this == t);
}


inline bool NumConst::isNumConst(){
    return true;
}


inline bool NumConst::isConst(){
    return true;
}

inline int NumConst::getIntValue(){
    return _val;
}


inline std::ostream& NumConst::put(std::ostream& os) const{
    return os << _val;
}



#endif // _NUM_CONST_HPP_
