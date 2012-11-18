/*!
 * @author Denis Vashchuk
 *
 * This file describe enums used for determine method and metrix used in process
*/

#ifndef ENUMS_H
#define ENUMS_H

//this enum define method for calculating point class
enum CalculateMethod { CALCULATE_METHOD_K_NEIGBORDS, CALCULATE_METHOD_STANTARD };
//this enum define calculating metrix  for calculating point class
enum CalculateMetrix { CALCULATE_METRIX_EUCLIDEAN, CALCULATE_METRIX_DIRCOS, CALCULATE_METRIX_TINIMOTO};

#endif // ENUMS_H
