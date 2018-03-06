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


#ifndef _FUNCTOR_HPP_
#define _FUNCTOR_HPP_

#include <map>
#include <set>
#include <string>

#include "arguments.hpp"



class FuncTerm;



class Functor{

//******************************* NESTED TYPES *******************************//

    private:
        /**
         * @brief Type of container which stores functors in a way that they can
         *        easily be found.
         * 
         * The key associated with a functor @a f is constructed as follows : the
         * name of @a f, a slash (@c '/') and the arity of @a f. This allows two
         * functors with the same name but different arities to be stored in the
         * same container.
         */
        typedef std::map<std::string, Functor*> Map;

        /**
         * @brief Type of container which stores @link functional terms@endlink
         *        w.r.t. theirs arguments.
         * 
         * @sa FuncTermSet
         */
        typedef std::map<Argument, FuncTerm*> FuncTermMap;

        /**
         * @brief Type of container which stores @link functional terms@endlink
         *        with check of unicity.
         * 
         * @sa FuncTermMap
         */
        typedef std::set<FuncTerm*> FuncTermSet;



//************************** STATIC MEMBER VARIABLES *************************//

    private:
        /**
         * @brief Container that stores all the functors.
         * 
         * @sa newFunctor(const std::string&, int), deleteAll()
         */
        static Map& _functors(){
            static Map functors;
            return functors;
        }


//***************************** MEMBER VARIABLES *****************************//

    private:
        /**
         * @brief The functor's name.
         * 
         * @sa _arity
         * @sa getStringValue()
         */
        const std::string _name;

        /**
         * @brief The number of arguments accepted by the functor.
         * 
         * @sa _name
         * @sa getArity()
         */
        int _arity;

        /**
         * @brief Set of ground @link FuncTerm functional terms@endlink.
         * 
         * @sa _nonGroundTerms
         * @sa existGroundTerm(const Argument&),
         *     insertGroundTerm(const Argument&, FuncTerm*)
         */
        FuncTermMap _groundTerms;

        /**
         * @brief Set of @link FuncTerm functional terms@endlink with variables.
         * 
         * @sa _groundTerms
         * @sa insertNonGroundTerm(FuncTerm*)
         */
        FuncTermSet _nonGroundTerms;


//************************** STATIC MEMBER FUNCTIONS *************************//

    public:
        /**
         * @brief Creates a functor.
         * @param name the name of the functor.
         * @param arity the number of arguments the functor accepts.
         * @return The address of the functor named @a name and with arity @a arity.
         * 
         * @note If a Functor named @a name and with arity @a arity already exists,
         *       this function does not create a new one; it just returns the
         *       address of the existing one.
         * 
         * @sa deleteAll()
         */
        static Functor* newFunctor(const std::string& name, int arity = 0);

        /**
         * @brief Removes all the functors internally stored.
         * 
         * This function destroys all the functors previously constructed, and
         * clears the internal container.
         * 
         * @sa newFunctor(const std::string&, int)
         */
        static void deleteAll();


//******************************* CONSTRUCTORS *******************************//

    protected:
        /**
         * @brief Constructs a functor.
         * @param name the name of the functor.
         * @param arity the number of arguments the functor accepts.
         * 
         * @sa newFunctor(const std::string&, int)
         */
        Functor(const std::string& name, int arity) : _name(name), _arity(arity){}

    private:
        /**
         * @brief Copy Constructor (not implemented).
         * @param f a functor to be copied.
         * 
         * @note This class is not copyable, so this constructor is not
         *       implemented.
         * 
         * @sa operator=(const Functor&)
         */
        Functor(const Functor& f);


//******************************** DESTRUCTOR ********************************//

    protected:
        /// @brief Destructor.
        ~Functor();


//********************************** GETTERS *********************************//

    public:
        /**
         * @brief Returns the name of a functor.
         * @return The name of this functor.
         */
        inline const std::string& getStringValue() const throw();

        /**
         * @brief Returns the arity of a functor.
         * @return The number of arguments this functor accepts.
         */
        inline int getArity() const throw();


//********************************* OPERATORS ********************************//

    private:
        /**
         * @brief Assignment operator (not implemented).
         * @param f a functor to be copied.
         * @return @c this.
         * 
         * @note This class is not copyable, so this operator is not
         *       implemented.
         * 
         * @sa Functor(const Functor&)
         */
        Functor& operator = (const Functor& f);


//************************** OTHER MEMBER FUNCTIONS **************************//

    public:
        /**
         * @brief Searches for a ground functional term corresponding to an argument.
         * @param arg the argument to search for.
         * @return The FuncTerm with this functor and ground argument @a a, or
         *         @c NULL if such a FuncTerm doesn't exist.
         * 
         * @sa insertGroundTerm(const Argument&, FuncTerm*)
         */
        FuncTerm* existGroundTerm(const Argument& arg);

        /**
         * @brief Inserts a functional term to the set of ground ones.
         * @param arg the argument of the functional term.
         * @param term the functional term to insert.
         * @return @c true if the functional term @a term has been inserted,
         *         @c false otherwise.
         * 
         * @sa existGroundTerm(const Argument&)
         * @sa insertNonGroundTerm(FuncTerm*)
         */
        bool insertGroundTerm(const Argument& arg, FuncTerm* term);

        /**
         * @brief Inserts a functional term to the set of nonground ones.
         * @param term the functional term to insert.
         * @return @c true if the functional term @a term has been inserted,
         *         @c false otherwise.
         * 
         * @sa insertGroundTerm(const Argument&, FuncTerm*)
         */
        bool insertNonGroundTerm(FuncTerm* term);


}; // class Functor


//********************************* OPERATORS ********************************//

/**
 * @brief Writes a Functor into a stream.
 * @param os an output stream.
 * @param f a functor.
 * @return @a os.
 * 
 * This function write the name of the functor @a f into the output stream
 * @a os.
 * 
 * @sa Functor::getStringValue()
 */
inline std::ostream& operator << (std::ostream& os, const Functor& f){
    return os << f.getStringValue();
}



//********************************** GETTERS *********************************//

// Returns the name of a functor
inline const std::string& Functor::getStringValue() const throw(){
    return _name;
}


// Returns the arity of a functor
inline int Functor::getArity() const throw(){
    return _arity;
}



#endif // _FUNCTOR_HPP_
