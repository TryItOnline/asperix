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


#ifndef _BUILT_IN_PREDICATE_HPP_
#define _BUILT_IN_PREDICATE_HPP_

#include <map>
#include <ostream>
#include <string>

#include "arguments.hpp"



/**
 * BuiltInPredicate has form #p(t1, t2,..,tn) 
 * where #p is a built-in predicate and ti are terms 
 * input arguments are at the beginning and output arguments at the end
 * input terms must be ground when literal is evaluated
 */
class BuiltInPredicate{

//******************************* NESTED TYPES *******************************//
    public:
        /** 
	 * @brief Type of user functions.
         */
        typedef bool (*UserFunction)(Argument&);


//************************** STATIC MEMBER VARIABLES *************************//

    protected:
        /**
         * @brief List of loaded built-in predicates.
         * 
         * @sa newPredicate(const std::string&, const std::string&, UserFunction),
         *     deleteAll()
         */
        static std::map<std::string, BuiltInPredicate*> _mapPredicate;

	
//***************************** MEMBER VARIABLES *****************************//

        /**
         * @brief Name of the built-in predicate.
         * 
         * @note This is the name of the underlined function too.
         * 
         * @sa _function
         * @sa callFunction(Argument&)
         */
        std::string _name;

        /**
         * @brief Minimal number of input arguments.
         * 
         * @sa _outputArgs, _vArgs
         * @sa checkArgs(const Argument&), callFunction(Argument&)
         */
        unsigned int _inputArgs;

        /**
         * @brief Number of output arguments.
         * 
         * @sa _inputArgs
         * @sa checkArgs(const Argument&), callFunction(Argument&)
         */
        unsigned int _outputArgs;

        /**
         * @brief Indicates whether the function accepts variable number of
         *        arguments.
         * 
         * @sa _inputArgs
         * @sa checkArgs(const Argument&), callFunction(Argument&)
         */
        bool _vArgs;

        /**
         * @brief Function pointer used to call the function named #_name.
         * 
         * @sa _name
         * @sa callFunction(Argument&)
         */
        UserFunction _function;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        /**
         * @brief Adds a new built-in predicate.
         * @param name of the predicate/the function.
         * @param args pattern for the arguments.
         * @param f function pointer that will be used.
         * @return The address of the built-in predicate created.
         * 
         * @a args is a sequence of 'i' (for input arguments) and 'o' (for
         * output arguments), each letter representing one argument. There may
         * be a unique 'r', immediately following a 'i', denoting that the
         * function accepts a variable number of input arguments. In this case,
         * there must be at least as many input arguments as 'i' occurring in
         * @a args minus one. Any other character is ignored.
         * 
         * @note If a predicate named @a name already exists, its address is
         *       simply returned.
         */
        static BuiltInPredicate* newPredicate(const std::string& name, const std::string& args, UserFunction f);

        /**
         * @brief Frees all loaded built-in predicates.
         */
        static inline void deleteAll();


//******************************* CONSTRUCTORS *******************************//

    protected:
        /**
         * @brief Create a built-in literal.
         * @param name name of the underlying function.
         * @param args status of arguments to be passed to the function @a name.
         * @param f function pointer that will be used.
         * 
         * @sa newPredicate(const std::string&, const std::string&, UserFunction)
         * @todo TODO Make a stronger check of arguments' status.
         */
        BuiltInPredicate(const std::string& name, const std::string& args, UserFunction f);


//********************************* OPERATORS ********************************//

        friend inline std::ostream& operator << (std::ostream&, const BuiltInPredicate&);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        /**
         * @brief Tests whether the underlying function could be called w.r.t
         *        some arguments.
         * @param args arguments to pass to the function.
         * @retval @c true if all input arguments in @a args are bounded;
         * @retval @c false otherwise.
         * @throw RuntimeExcept if an error occurs.
         */
        bool checkArgs(const Argument& args) const;

        /**
         * @brief Calls the underlying function.
         * @param args arguments to pass to the function.
         * @return The return value of the function, or @c false if there is an
         *         error.
         * 
         * @pre checkArgs(@a args) returns @c true.
         * @sa checkArgs(const Arguments&);
         */
        inline bool callFunction(Argument& args) const;

    protected:
        inline std::ostream& put(std::ostream&) const;


}; // class BuiltInPredicate


//************************** STATIC MEMBER FUNCTIONS *************************//

// Frees all loaded built-in predicates
inline void BuiltInPredicate::deleteAll(){
    typedef std::map<std::string, BuiltInPredicate*>::iterator  iterator;

    for (iterator it = _mapPredicate.begin(), end = _mapPredicate.end(); it != end; ++it)
        delete it->second;
    _mapPredicate.clear();
}


//********************************* OPERATORS ********************************//

inline std::ostream& operator << (std::ostream& os, const BuiltInPredicate& p){
    return p.put(os);
}



//************************** OTHER MEMBER FUNCTIONS **************************//

// Call the underlying function
inline bool BuiltInPredicate::callFunction(Argument& args) const{
    return (_function) ? _function(args) : false;
}


inline std::ostream& BuiltInPredicate::put(std::ostream& os) const{
    return os << '#' << _name;
}



#endif // _BUILT_IN_PREDICATE_HPP_
