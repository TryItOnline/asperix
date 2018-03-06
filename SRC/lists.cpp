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


#include "extpred.hpp"
#include "list.hpp"
#include "num_const.hpp"



/**
 * @brief Construct a list.
 * @param args[] a term.
 * @param args[] a list.
 * @return @c true iff @a args[] is the list that contains all the previous terms.
 */
BUILT_IN(list, iro){
    typedef Argument::reverse_iterator  iterator;

    List* l = List::getEmptyList();
    for (iterator it = args.rbegin() + 1, end = args.rend(); it != end; ++it)
        l = List::new_List(*it, l);

    return args.back()->match(l);
}


/**
 * @brief Returns the last element of a list.
 * @param args[0] a list.
 * @param args[1] a term.
 * @return @c true iff @a args[1] is the last element in @a args[0].
 */
BUILT_IN(last, io){
    List* l = static_cast<List*>( args[0] );
    if (l->empty())
        return false;

    while (!l->tail()->empty())
        l = l->tail();

    return args[1]->match(l->head());
}


/**
 * @brief Counts the size of a list.
 * @param args[0] a list.
 * @param args[1] an integer.
 * @return @c true iff @a args[1] is the length of @a args[0].
 */
BUILT_IN(length, io){
    List* l = static_cast<List*>( args[0] );
    return args[1]->match( NumConst::new_NumConst(l->length()) );
}


/**
 * @brief Returns the head of a list.
 * @param args[0] a list.
 * @param args[1] a term.
 * @return @c true iff @a args[1] is the first element in @a args[0].
 */
BUILT_IN(head, io){
    List* l = static_cast<List*>( args[0] );
    return (l->empty()) ? false : args[1]->match(l->head());
}


/**
 * @brief Returns the tail of a list.
 * @param args[0] a list.
 * @param args[1] a list.
 * @return @c true iff @a args[1] is the list @a args[0] minus its first element.
 */
BUILT_IN(tail, io){
    List* l = static_cast<List*>( args[0] );
    return (l->empty()) ? false : args[1]->match(l->tail());
}


/**
 * @brief Appends two lists.
 * @param args[0] a list.
 * @param args[1] a list.
 * @param args[2] a list.
 * @return @c true iff @a args[2] is a list that contains all the elements of
 *         @a args[0], then all the elements of @a args[1].
 */
BUILT_IN(append, iio){
    List* l1 = static_cast<List*>( args[0] );
    List* l2 = static_cast<List*>( args[1] );
    return args[2]->match(l1->append(l2));
}


/**
 * @internal
 * @brief Removes the first occurrence of an element in a list.
 * @param list an input list.
 * @param term the selected term.
 * @return The list @a list where the first occurrence of @a term has been
 *         removed.
 */
static List* delFirst(List* list, Term* term){
    if (list->empty())
        return list;

    if ( term->match(list->head()) )
        return list->tail();

    return List::new_List(list->head(), delFirst(list->tail(), term));
}


/**
 * @brief Deletes the first occurrence of a term in a list.
 * @param args[0] a list.
 * @param args[1] a term.
 * @param args[2] a list.
 * @return @c true iff @a args[2] is the list @a args[0] where the first
 *         occurrence of @a args[1] has been removed.
 */
BUILT_IN(delFirst, iio){
    List* l = static_cast<List*>( args[0] );
    l = delFirst(l, args[1]);
    return args[2]->match(l);
}



/**
 * @internal
 * @brief Removes an element in a list.
 * @param list an input list.
 * @param term the selected term.
 * @return The list @a list where all the occurrences of @a term have been
 *         removed.
 */
static List* del(List *list, Term* term){
    if (list->empty())
        return list;

    if ( term->match(list->head()) )
        return del(list->tail(), term);

    return List::new_List(list->head(), del(list->tail(), term));
}


/**
 * @brief Deletes an element in a list.
 * @param args[0] a list.
 * @param args[1] a term.
 * @param args[2] a list.
 * @return @c true iff @a args[2] is the list @a args[0] where all the
 *         occurrences of @a args[1] have been removed.
 */
BUILT_IN(del, iio){
    List* l = static_cast<List*>( args[0] );
    l = del(l, args[1]);
    return args[2]->match(l);
}



/**
 * @internal
 * @brief Removes an element in a list.
 * @param list an input list.
 * @param pos the position in which delete the element (starting with 0).
 * @return The list @a list where the element in position @a pos has been removed.
 * 
 * @note If @a pos is negative or greater than the length of @a list, this
 *       function returns @a list.
 */
static List* delNth(List* list, int pos){
    if ((pos < 0) || list->empty())
        return list;

    if (pos == 0)
        return list->tail();

    return List::new_List(list->head(), delNth(list->tail(), pos - 1));
}


/**
 * @brief Removes an element of a list.
 * @param args[0] a list.
 * @param args[1] an integer.
 * @param args[2] a list.
 * @return @c true iff @a args[2] is the list @a args[0] where the element in
 *         position @a args[1] (starting with 0) has been removed.
 * 
 * @note If @a args[1] is negative or greater than the length of @a args[0],
 *       this function performs an equality test.
 */
BUILT_IN(delNth, iio){
    List* l = static_cast<List*>( args[0] );
    l = delNth(l, args[1]->getIntValue());
    return args[2]->match(l);
}


/**
 * @brief Tests whether a term occurs in a list.
 * @param args[0] a term.
 * @param args[1] a list.
 * @return @c true iff @a args[0] occurs in the list @a args[1].
 */
BUILT_IN(member, ii){
    List* l = static_cast<List*>( args[1] );
    return l->contains(args[0]);
}


/**
 * @brief Reverses a list.
 * @param args[0] a list.
 * @param args[1] a list.
 * @return @c true iff @a args[1] contains all elements in @a args[0], but in
 *         the opposite order.
 */
BUILT_IN(reverse, io){
    List* l = static_cast<List*>( args[0] );
    return args[1]->match(l->reverse());
}


/**
 * @brief Returns a particular element in a list.
 * @param args[0] a list.
 * @param args[1] an integer.
 * @param args[2] a term.
 * @return @c true iff @a args[2] is in the list @a args[0] at the position
 *         @a args[1] (starting with 0).
 */
BUILT_IN(memberNth, iio){
    List* l = static_cast<List*>( args[0] );
    int pos = args[1]->getIntValue();

    for ( ; (pos > 0) && !l->empty(); --pos)
        l = l->tail();

    return ((pos == 0) && !l->empty()) ? args[2]->match(l->head()) : false;
}


/**
 * @internal
 * @brief Inserts an element in specific position in a list.
 * @param list an input list.
 * @param term a term to insert.
 * @param pos the position in which insert the element (starting with 0).
 * @return The list @a list where @a term has been inserted at the position @a pos.
 * 
 * @note If @a pos is negative or greater than the length of @a list + 1, this
 *       function returns @a list.
 */
static List* insNth(List* list, Term* term, int pos){
    if (pos == 0)
        return List::new_List(term, list);

    if ((pos < 0) || list->empty())
        return list;

    return List::new_List(list->head(), insNth(list->tail(), term, pos - 1));
}


/**
 * @brief Inserts an element in a specific position in a list.
 * @param args[0] a list.
 * @param args[1] a term.
 * @param args[2] an integer.
 * @param args[3] a list.
 * @return @c true iff @a args[3] is the list @a args[0] where @a args[1] has
 *         been insterted at the position @a args[2] (starting with 0).
 * 
 * @note if @a args[2] is negative or greater than the length of @a args[0] + 1,
 *       this function performs an equality test.
 */
BUILT_IN(insNth, iiio){
    List* l = static_cast<List*>( args[0] );
    l = insNth(l, args[1], args[2]->getIntValue());
    return args[3]->match(l);
}


/**
 * @brief Inserts an element at the end of a list.
 * @param args[0] a list.
 * @param args[1] a term.
 * @param args[2] a list.
 * @return @c true iff @a args[2] is the list @a args[0] where @a args[1] has
 *         been appended.
 */
BUILT_IN(insLast, iio){
    Argument aux(args);
    aux[1] = List::new_List(args[1], List::getEmptyList());

    return append(aux);
}


/**
 * @brief Checks whether a list is a sublist of another one.
 * @param args[0] a list.
 * @param args[1] a list.
 * @return @c true iff all elements in @a args[0] occur consecutively in @a args[1].
 */
BUILT_IN(subList, ii){
    List* l1 = static_cast<List*>( args[0] );
    if (l1->empty())
        return true;

    List* l2 = static_cast<List*>( args[1] );

    for (List* aux1 = l1, *aux2 = l2 ; !aux2->empty(); aux2 = aux2->tail()) {
        l1 = aux1;
        l2 = aux2;

        do {

            if ( !l1->head()->match(l2->head()) )
                break;

            l1 = l1->tail();
            l2 = l2->tail();

        } while (!l1->empty() && !l2->empty());

        if (l1->empty())
            return true;
        if (l2->empty())
            return false;

    }

    return false;
}


/**
 * @internal
 * @brief Create a range of integers.
 * @param first the lower value.
 * @param last the upper value.
 * @param step the step between two consecutive values in the list.
 * @return A list that is the range [@a first, @a last], step @a step.
 * 
 * @note This function returns an empty list if @a step is null.
 */
static List* range(int first, int last, int step){
    return ( ((step > 0) && (first > last)) || ((step < 0) && (first < last)) || !step )
        ? List::getEmptyList()
        : List::new_List(NumConst::new_NumConst(first), range(first + step, last, step));
}


/**
 * @brief Create a range of values (integers).
 * @param args[0] an integer.
 * @param args[1] an integer.
 * @param args[2] an integer.
 * @param args[3] a list.
 * @return @c true iff @a args[3] is a list that contains all the values between
 *         @a args[0] and @a args[1] (inclusive) step @a args[2].
 */
BUILT_IN(range, iiio){
    List* l = range(args[0]->getIntValue(), args[1]->getIntValue(), args[2]->getIntValue());
    return args[3]->match(l);
}
