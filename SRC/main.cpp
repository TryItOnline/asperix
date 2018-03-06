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


#include <iostream>
#include <cstring>

#include "built_in_literal.hpp"
#include "exceptions.hpp"
#include "func_term.hpp"
#include "num_const.hpp"
#include "program.hpp"


extern Program* readProg(const char*,bool);

using namespace std;


int main(int argc, char** argv){
    char* f = NULL;
    int n ; // maxInt
    int number = 1; // maximum number of answer-set to compute (0 for all)
    bool verbose = false;
    bool error = false;
    char *endptr;
    for (int c = 1; c < argc && !error; c++) {
        if (argv[c][0] == '-') {    // option
            if (strcmp (&argv[c][1], "verbose") == 0)
                verbose = true;
            else if (argv[c][1] == 'N') {
                c++;
                if (c < argc) {
                    n = strtol(&argv[c][0], &endptr, 0);
                    if ((n < 0) || (*endptr != '\0'))
                        error = true;
                    else
                        NumConst::setMaxInt(n); // TODO c'est pas le bon endroit ...
                }
                else
                    error = true;
            }
            else if (argv[c][1] == 'F') {
                c++;
                if (c < argc) {
                    n = strtol(&argv[c][0], &endptr, 0);
                    if ((n < 0) || (*endptr != '\0'))
                        error = true;
                    else
                        FuncTerm::setMaxDepth(n);   // TODO c'est pas le bon endroit ...
                }
                else
                    error = true;
            }
            else if (strcmp (&argv[c][1], "-help") == 0) {
                cout << "usage : asperix [options] input_file" << endl;
                cout << "options :" << endl;
                cout << "\t n : n is the maximum number of answer set to compute (0 for all, default = 1)" << endl;
                cout << "\t -N n : n is the greatest integer in your world (default : 1024)" << endl;
                cout << "\t -F n : n is the maximum depth allowed for functional terms (default : 16)" << endl;
                cout << "\t -verbose : verbose version" << endl;
                cout << "\t --help : print this help" << endl;
                return 0;
            }
            else
                error = true;
        }
        else {
            n = strtol(argv[c], &endptr, 0);
            if ((n < 0) || (*endptr != '\0'))
                f = argv[c];    // input file
            else
                number = n;     // maximum number of answer-set to compute
        }
    }
    if (error) {
        cout << "syntax error - type \"asperix --help\" for help" << endl;
        return 1;
    }
    Program* p = NULL;
    try {
        p = readProg(f,verbose);
	cout << endl;
        if (!p) {
            cerr << "Error in input" << endl;
            return 1;
        }
        p->evaluate();
        if (p->isDefinite()) {
            //cout<<"Program is definite"<<endl;
            if (p->existsSupportedNonBlockedProhibitedRule()) {
                cout << "############### No answer set" << endl;
            }
            else {
                p->setAnswerNumber(1);
                p->printAnswerSet();
            }
        }
        else {
            //cout << "Non definite Program" << endl;
            if (!p->answerSetSearch(number))
                cout << "############### No answer set" << endl;
        }
        if (verbose)
	    cout << "Total choice points : " << p->getChoicePoints() << endl;
        delete p;
        BuiltInLiteral::closeDynamicLibraries();
        return 0;
    }
    catch (const ContradictoryConclusion&) {    // during readProg or evaluate
        cout << "############### No answer set(ContradictoryConclusion)" << endl;
	if (verbose)
	    cout << "Total choice points : " << ((p == NULL) ? 0 : p->getChoicePoints()) << endl;
        delete p;
        BuiltInLiteral::closeDynamicLibraries();
        return 0;
    }
    catch (const RuntimeExcept& e) {
        cerr << e.what() << endl;
        BuiltInLiteral::closeDynamicLibraries();
        return 1;
    }
}
