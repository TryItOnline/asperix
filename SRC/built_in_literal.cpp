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


#include <dlfcn.h>

#include "built_in_literal.hpp"
#include "exceptions.hpp"



//************************** STATIC MEMBER VARIABLES *************************//

// Handler for dynamic libraries
void* BuiltInLiteral::_handle = NULL;


// List of loaded dynmic libraries
std::vector<void*> BuiltInLiteral::_libs;


//************************** STATIC MEMBER FUNCTIONS *************************//

// Initializes the handler dor dynamic libraries
void BuiltInLiteral::initDynamicLibraries(){
    if (!_handle) {
        _handle = dlopen(NULL, RTLD_LAZY);
        if (!_handle)
            throw RuntimeExcept(dlerror());
    }
}

// Loads a new dynamic library
void BuiltInLiteral::addDynamicLibrary(const std::string& filename){
    std::string name = ((filename.compare(0, 1, "/", 1) == 0) ||
                        (filename.compare(0, 2, "./", 2) == 0) ||
                        (filename.compare(0, 3, "../", 3) == 0))
                     ? ""
                     : "./";
    name += filename;
    name += ".so";

    void *handler = dlopen(name.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handler)
        throw RuntimeExcept(dlerror());

    return _libs.push_back(handler);
}


// Closes dynamic libraries
void BuiltInLiteral::closeDynamicLibraries(){
    typedef std::vector<void*>::const_reverse_iterator  iterator;

    for (iterator it = _libs.rbegin(), end = _libs.rend(); it != end; ++it)
        dlclose(*it);
    _libs.clear();

    if (_handle) {
        dlclose(_handle);
        _handle = NULL;
    }

    BuiltInPredicate::deleteAll();  // TODO Not here
}


//******************************* CONSTRUCTORS *******************************//

BuiltInLiteral::BuiltInLiteral(const std::string& name, Argument* args, bool sign)
 : Literal(args), _BIPred(), _sign(sign){
    // Clear any existing error
    dlerror();

    void (*built_in)(std::string&, BuiltInPredicate::UserFunction&);
    // Recommended way to cast from void* to function pointer
	void **tmp = reinterpret_cast<void**> (&built_in);
    *tmp = dlsym(_handle, name.c_str());

    char* error = dlerror();
    if (error)
        throw RuntimeExcept(error);

    std::string __args;
    BuiltInPredicate::UserFunction function;
    built_in(__args, function);

    _BIPred = BuiltInPredicate::newPredicate(name, __args, function);
}


BuiltInLiteral::BuiltInLiteral(BuiltInPredicate *pred, Argument* args, bool sign)
 : Literal(args), _BIPred(pred), _sign(sign){}


//******************************** DESTRUCTOR ********************************//

BuiltInLiteral::~BuiltInLiteral(){
    _BIPred = NULL;
}


//************************** OTHER MEMBER FUNCTIONS **************************//

// try to match atom with some tuple of the extension 
// of the predicate ; return true if a match is found
bool BuiltInLiteral::firstTrueMatchBis(){
    Argument* terms = applySubstitutionOnArgs();
    try {
        if (!_BIPred->checkArgs(*terms))
            throw RuntimeExcept("Non ground term when evaluating built-in predicate");

        bool ok = _BIPred->callFunction(*terms);
        delete terms;
        return _sign ? ok : !ok;
    }
    catch (const RuntimeExcept&) {
        delete terms;
        throw;
    }
}


statusEnum BuiltInLiteral::firstSoftMatchBis(statusEnum status){
    return firstTrueMatchBis() ? status : NO_;
}


// idem for the next matching tuple
bool BuiltInLiteral::nextTrueMatch(){
    return false;
}


// idem for the next matching tuple
statusEnum BuiltInLiteral::nextSoftMatch(statusEnum){
    return NO_;
}


Literal* BuiltInLiteral::createPositiveClone(MapVariables& mapVar){
    return new BuiltInLiteral(_BIPred, _args->clone(mapVar), _sign);
}
