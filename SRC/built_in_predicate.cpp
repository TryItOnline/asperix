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


#include "built_in_predicate.hpp"
#include "exceptions.hpp"



//************************** STATIC MEMBER VARIABLES *************************//

// List of loaded built-in predicates
std::map<std::string, BuiltInPredicate*> BuiltInPredicate::_mapPredicate;


//************************** STATIC MEMBER FUNCTIONS *************************//

// Adds a new built-in predicate
BuiltInPredicate* BuiltInPredicate::newPredicate(const std::string& name, const std::string& args, UserFunction f){
    typedef std::map<std::string, BuiltInPredicate*>::iterator   iterator;

    // Search for existing built-in predicate
    iterator it = _mapPredicate.find(name);
    if (it != _mapPredicate.end())
        return it->second;

    // Create new built-in predicate
    BuiltInPredicate* pred = new BuiltInPredicate(name, args, f);
    return _mapPredicate.insert(std::make_pair(name, pred)).first->second;
}


//******************************* CONSTRUCTORS *******************************//

// Create a built-in literal
BuiltInPredicate::BuiltInPredicate(const std::string& name, const std::string& args, UserFunction f)
 : _name(name), _inputArgs(), _outputArgs(), _vArgs(), _function(f){
    typedef std::string::const_iterator iterator;

    for (iterator it = args.begin(), end = args.end(); it != end; ++it)
        switch (*it) {
            case 'i':
                ++_inputArgs;
                break;

            case 'o':
                ++_outputArgs;
                break;

            case 'r':
                if (!_vArgs) {
                    _vArgs = true;
                    if (_inputArgs > 0)
                        --_inputArgs;
                }
                break;
        }
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// Tests whether the underlying function could be called w.r.t some arguments
bool BuiltInPredicate::checkArgs(const Argument& args) const{
    typedef Argument::const_reverse_iterator    iterator;

    if (args.size() < _inputArgs + _outputArgs)
        throw RuntimeExcept('#' + _name + ": Too few arguments");
    if (!_vArgs && (args.size() > _inputArgs + _outputArgs))
        throw RuntimeExcept('#' + _name + ": Too many arguments");

    for (iterator it = args.rbegin() + _outputArgs, end = args.rend(); it != end; ++it)
        if (!(*it)->isConst())
            throw RuntimeExcept('#' + _name + ": Input argument not ground");

    return true;
}
