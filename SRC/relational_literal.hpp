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


#ifndef _RELATIONAL_LITERAL_HPP_
#define _RELATIONAL_LITERAL_HPP_

#include "literal.hpp"



//****************************************************************************//
//                              RelationalLiteral                             //
//****************************************************************************//


/** RelationalLiteral has form t1 rel_op t2 
 * where t1 and t2 are terms that must be ground when literal is evaluated
 */
class RelationalLiteral : public Literal{ // abstract class

//******************************* CONSTRUCTORS *******************************//

    public:
        RelationalLiteral();

        RelationalLiteral(Term* t1, Term* t2);


//************************** OTHER MEMBER FUNCTIONS **************************//

        /// idem for the next matching tuple
        bool nextTrueMatch();

        /// idem for the next matching tuple
        statusEnum nextSoftMatch(statusEnum);

        virtual
        bool isTrueRelation(Argument* couple) = 0;

    protected:
        virtual
        std::ostream& put(std::ostream&) = 0;

        /// try to match atom with some tuple of the extension 
        /// of the predicate ; return true if a match is found
        bool firstTrueMatchBis();

        statusEnum firstSoftMatchBis(statusEnum);


}; // class RelationalLiteral






//****************************************************************************//
//                                  EqLiteral                                 //
//****************************************************************************//


class EqLiteral : public RelationalLiteral{

//******************************* CONSTRUCTORS *******************************//

    public:
        EqLiteral(Term* t1, Term* t2) : RelationalLiteral(t1, t2){}


//************************** OTHER MEMBER FUNCTIONS **************************//

        bool isTrueRelation(Argument* couple);

        Literal* createPositiveClone(MapVariables& mapVar);

    protected:
        inline std::ostream& put(std::ostream& os);


}; // class EqLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline std::ostream& EqLiteral::put(std::ostream& os){
    return os << *_args->at(0) << "==" << *_args->at(1);
}






//****************************************************************************//
//                                 NeqLiteral                                 //
//****************************************************************************//


class NeqLiteral : public RelationalLiteral{

//******************************* CONSTRUCTORS *******************************//

    public:
        NeqLiteral(Term* t1, Term* t2) : RelationalLiteral(t1, t2){}


//************************** OTHER MEMBER FUNCTIONS **************************//

        bool isTrueRelation(Argument* couple);

        Literal* createPositiveClone(MapVariables& mapVar);

    protected:
        inline std::ostream& put(std::ostream& os);


}; // class NeqLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline std::ostream& NeqLiteral::put(std::ostream& os){
    return os << *_args->at(0) << "!=" << *_args->at(1);
}






//****************************************************************************//
//                                  GeLiteral                                 //
//****************************************************************************//


class GeLiteral : public RelationalLiteral{

//******************************* CONSTRUCTORS *******************************//

    public:
        GeLiteral(Term* t1, Term* t2) : RelationalLiteral(t1, t2){}

        
//************************** OTHER MEMBER FUNCTIONS **************************//

        bool isTrueRelation(Argument* couple);

        Literal* createPositiveClone(MapVariables& mapVar);

    protected:
        inline std::ostream& put(std::ostream& os);


}; // class GeLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline std::ostream& GeLiteral::put(std::ostream& os){
    return os << *_args->at(0) << ">=" << *_args->at(1);
}






//****************************************************************************//
//                                  GtLiteral                                 //
//****************************************************************************//


class GtLiteral : public RelationalLiteral{

//******************************* CONSTRUCTORS *******************************//

    public:
        GtLiteral(Term* t1, Term* t2) : RelationalLiteral(t1, t2){}


//************************** OTHER MEMBER FUNCTIONS **************************//

        bool isTrueRelation(Argument* couple);

        Literal* createPositiveClone(MapVariables& mapVar);

    protected:
        inline
        std::ostream& put(std::ostream& os);


}; // class GtLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline std::ostream& GtLiteral::put(std::ostream& os){
    return os << *_args->at(0) << '>' << *_args->at(1);
}



#endif // _RELATIONAL_LITERAL_HPP_
