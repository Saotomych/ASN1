/*
 * NotEqualsPointer.h
 *
 *  Created on: 10 окт. 2016 г.
 *      Author: alex
 */

#ifndef INCLUDE_NOTEQUALSPOINTER_H_
#define INCLUDE_NOTEQUALSPOINTER_H_

/*
 * equalsPointersAndValues
 * @return: bool is true when both of input pointer is nullptr or both values equals
 */
template<class BerType>
bool notEqualsPointersAndValues(BerType* lhs, BerType* rhs)
{
	bool nullLhs(lhs == nullptr);
	bool nullRhs(rhs == nullptr);

	if ( (nullLhs & nullRhs) == true) return false;
	if ( (nullLhs ^ nullRhs) == true) return true;

	return *lhs != *rhs;
}

#endif /* INCLUDE_NOTEQUALSPOINTER_H_ */
