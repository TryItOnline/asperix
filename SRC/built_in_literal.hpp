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


#ifndef _BUILT_IN_LITERAL_HPP_
#define _BUILT_IN_LITERAL_HPP_

#include <string>
#include <vector>

#include "built_in_predicate.hpp"
#include "literal.hpp"



/**
 * BuiltInLiteral has form #p(t1, t2,..,tn) 
 * where #p is a built-in predicate and ti are terms 
 * input arguments are at the beginning and output arguments at the end
 * input terms must be ground when literal is evaluated
 */
class BuiltInLiteral : public Literal{

//******************************* NESTED TYPES *******************************//

    public:
        /** @brief Type of user functions.
         * 
         * @sa 
         */
        typedef bool (*UserFunction)(const Argument&);


//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        /**
         * @brief Handler for dynamic libraries.
         * 
         * @sa initDynamicLibraries(), closeDynamicLibraries()
         */
        static void* _handle;

        /**
         * @brief List of loaded dynamic libraries.
         * 
         * @sa addDynamicLibrary(const std::string&), closeDynamicLibraries()
         */
        static std::vector<void*> _libs;


//***************************** MEMBER VARIABLES *****************************//

        /**
         * @brief Built-in predicate associated with the literal.
         * 
         * @sa 
         */
        BuiltInPredicate* _BIPred;

        /**
         * @brief Sign of the literal.
         * 
         * This field is @c true when the literal is positive, and @c false when
         * it is negative.
         */
        bool _sign;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        /**
         * @brief Initializes the handler for dynamic libraries.
         * 
         * @sa closeDynamicLibraries()
         */
        static void initDynamicLibraries();

        /**
         * @brief Loads a new dynamic library.
         * @param filename the name of the file that contains the library.
         * @throw RuntimeExcept if an error occurs.
         * 
         * @sa closeDynamicLibraries()
         */
        static void addDynamicLibrary(const std::string& filename);

        /**
         * @brief Closes dynamic libraries.
         * 
         * @sa initDynamicLibraries(), addDynamicLibrary(const std::string&)
         */
        static void closeDynamicLibraries();


//******************************* CONSTRUCTORS *******************************//

        /**
         * @brief Create a built-in literal.
         * @param name name of the underlying function.
         * @param args arguments to be passed to the function @a name.
         * @param sign @c true if the literal is positive, @c false otherwise.
         * @throw RuntimeExcept if no function named @a name can't be found.
         */
        BuiltInLiteral(const std::string& name, Argument* args = NULL, bool sign = true);

    protected:
        BuiltInLiteral(BuiltInPredicate *pred, Argument* args = NULL, bool sign = true);


//******************************** DESTRUCTOR ********************************//

    public:
        /// @brief Destructor.
        ~BuiltInLiteral();


//************************** OTHER MEMBER FUNCTIONS **************************//

        /// idem for the next matching tuple
        bool nextTrueMatch();

        /// idem for the next matching tuple
        statusEnum nextSoftMatch(statusEnum);

        Literal* createPositiveClone(MapVariables& mapVar);

    protected:
        virtual std::ostream& put(std::ostream&);

        /// try to match atom with some tuple of the extension 
        /// of the predicate ; return true if a match is found
        bool firstTrueMatchBis();

        statusEnum firstSoftMatchBis(statusEnum);


}; // class BuiltInLiteral


//************************** OTHER MEMBER FUNCTIONS **************************//

inline std::ostream& BuiltInLiteral::put(std::ostream& os){
    if (!_sign)
        os << '-';
    os << *_BIPred << '(';
    for (Argument::const_iterator it = _args->begin(); it != _args->end(); ++it)
        os << **it << ',';
    return os << ')';
}



#endif // _BUILT_IN_LITERAL_HPP_
