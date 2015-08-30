/*

Copyright (c) 2011, Smart Engines Limited. All rights reserved. 

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of copyright holders.

*/

/**
 * @file   mathoper.h
 * @brief  Definition of mathematical operations. 
 */
#pragma once

#ifndef MATHOPER_H_INCLUDED
#define MATHOPER_H_INCLUDED

/**
 * @defgroup MinUtils_MathOper Mathematical operations
 * @brief    The module specifies mathematical operations which can be used in
 *           image processing functions. All operations are specified by the 
 *           follow constants: @c #OP_MIN (binary minimum), @c #OP_MAX (binary 
 *           maximum), @c #OP_ADD (binary addition), @c #OP_DIF (binary difference), 
 *           @c #OP_ADF (binary absolute difference), @c #OP_MUL (binary 
 *           multiplication), @c #OP_AVE (binary average), and @c #OP_EUC (binary 
 *           Euclidean norm). Additionally, the operations are grouped into several 
 *           enums in accordance with their properties: all binary operations 
 *           (@c #BiOp), associative operations (@c #AsOp), commutative operations 
 *           (@c #CoOp), associative-commutative operations (@c #AsCoOp) and 
 *           idempotent operations (@c #IdOp).
 */

/**
 * @brief   Specifies binary minimum operation.
 * @details The constant specifies the binary minimum operation @f$ f @f$ that
 *          is defined as follows: @f[ f(x, y) = \min(x, y) @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_MIN = 1;

/**
 * @brief   Specifies binary maximum operation.
 * @details The constant specifies the binary maximum operation @f$ f @f$ that
 *          is defined as follows: @f[ f(x, y) = \max(x, y) @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_MAX = 2;

/**
 * @brief   Specifies binary addition operation.
 * @details The constant specifies the binary addition operation @f$ f @f$ that
 *          is defined as follows: @f[ f(x, y) = x + y @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_ADD = 3;

/**
 * @brief   Specifies binary difference operation.
 * @details The constant specifies the binary difference operation @f$ f @f$ 
 *          that is defined as follows: @f[ f(x, y) = x - y @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_DIF = 4;

/**
 * @brief   Specifies binary absolute difference operation.
 * @details The constant specifies the binary absolute difference operation
 *          @f$ f @f$ that is defined as follows: @f[ f(x, y) = \mid x - y \mid @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_ADF = 5;

/**
 * @brief   Specifies binary multiplication operation.
 * @details The constant specifies the binary multiplication operation @f$ f @f$
 *          that is defined as follows: @f[ f(x, y) = x \cdot y @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_MUL = 6;

/**
 * @brief   Specifies binary average operation.
 * @details The constant specifies the binary average operation @f$ f @f$ that 
 *          is defined as follows: @f[ f(x, y) = \frac{x + y}{2} @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_AVE = 7;

/**
 * @brief   Specifies binary Euclidean norm operation.
 * @details The constant specifies the binary Euclidean norm operation @f$ f @f$
 *          that is defined as follows: @f[ f(x, y) = \sqrt{x^2 + y^2} @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_EUC = 8;

/**
 * @brief   Specifies binary division operation.
 * @details The constant specifies the binary multiplication operation @f$ f @f$
 *          that is defined as follows: @f[ f(x, y) = x / y @f]
 * @ingroup MinUtils_MathOper
 */
const int OP_DIV = 9;

const int OP_SSQ = 10; // sum of squares

const int OP_ABS = 11;

const int OP_SQRT = 12;

const int OP_INV = 13;
/**
 * @brief   Specifies binary operations.
 * @details The enum specifies binary operations, that is such ones which involve
 *          two operands. Formally, a binary operation @f$ f @f$ on a set @f$ S @f$
 *          is a binary relation that maps elements of the Cartesian product 
 *          @f$ S \times S @f$ to @f$ S @f$:
*           @f[ f: S \times S \to S @f]
 * @ingroup MinUtils_MathOper
 */
typedef enum
{
  BIOP_MIN = OP_MIN,  ///< Binary minimum operation (see @c #OP_MIN).
  BIOP_MAX = OP_MAX,  ///< Binary maximum operation (see @c #OP_MAX).
  BIOP_ADD = OP_ADD,  ///< Binary addition operation (see @c #OP_ADD).
  BIOP_DIF = OP_DIF,  ///< Binary difference operation (see @c #OP_DIF).
  BIOP_ADF = OP_ADF,  ///< Binary absolute difference operation (see @c #OP_ADF).
  BIOP_MUL = OP_MUL,  ///< Binary multiplication operation (see @c #OP_MUL).
  BIOP_AVE = OP_AVE,  ///< Binary average operation (see @c #OP_AVE).
  BIOP_EUC = OP_EUC,  ///< Binary Euclidean norm operation (see @c #OP_EUC).
  BIOP_DIV = OP_DIV,  ///< Binary divisiob operation (see @c #OP_DIV).
  BIOP_SSQ = OP_SSQ
} BiOp;

/**
 * @brief   Specifies associative operations.
 * @details The enum specifies associative operations that is such ones which 
 *          can be freely regrouped without altering result. Formally, a binary 
 *          operation @f$ f @f$ on a set @f$ S @f$ is called associative if it 
 *          satisfies the associative law:
 *          @f[ f(f(x, y), z) = f(x, f(y, z)) \quad \forall x,y,z \in S @f]
 * @ingroup MinUtils_MathOper
 */
typedef enum
{
  ASOP_MIN = OP_MIN,  ///< Binary minimum operation (see @c #OP_MIN).
  ASOP_MAX = OP_MAX,  ///< Binary maximum operation (see @c #OP_MAX).
  ASOP_ADD = OP_ADD,  ///< Binary addition operation (see @c #OP_ADD).
  ASOP_MUL = OP_MUL,  ///< Binary multiplication operation (see @c #OP_MUL).
  ASOP_EUC = OP_EUC   ///< Binary Euclidean norm operation (see @c #OP_EUC).
} AsOp;

/**
 * @brief   Specifies  commutative operations.
 * @details The enum specifies commutative operations, that is such ones which 
 *          do not depend on the order of the input parameters. Formally, a binary 
 *          operation @f$ f @f$ on a set @f$ S @f$ is called commutative if it 
 *          satisfies the commutative law:
 *          @f[ f(x, y) = f(y, x) \quad \forall x,y \in S @f]
 * @ingroup MinUtils_MathOper
 */
typedef enum
{
  COOP_MIN = OP_MIN,  ///< Binary minimum operation (see @c #OP_MIN).
  COOP_MAX = OP_MAX,  ///< Binary maximum operation (see @c #OP_MAX).
  COOP_ADD = OP_ADD,  ///< Binary addition operation (see @c #OP_ADD).
  COOP_ADF = OP_ADF,  ///< Binary absolute difference operation (see @c #OP_ADF).
  COOP_MUL = OP_MUL,  ///< Binary multiplication operation (see @c #OP_MUL).
  COOP_AVE = OP_AVE,  ///< Binary average operation (see @c #OP_AVE).
  COOP_EUC = OP_EUC,  ///< Binary Euclidean norm operation (see @c #OP_EUC).
  COOP_SSQ = OP_SSQ
} CoOp;

/**
 * @brief   Specifies associative-commutative operations.
 * @details The enum specifies associative-commutative operations, that is such 
 *          ones which have both associative and commutative properties. Formally,
 *          a binary operation @f$ \circ @f$ on a set @f$ S @f$ is called 
 *          associative-commutative if it satisfies both the associative and the 
 *          commutative laws:
 *          @f[ f(f(x, y), z) = f(x, f(y, z)) \quad \forall x,y,z \in S @f]
 *          @f[ f(x, y) = f(y, x) \quad \forall x,y \in S @f]
 * @ingroup MinUtils_MathOper
 */
typedef enum
{
  ASCOOP_MIN = OP_MIN,  ///< Binary minimum operation (see @c #OP_MIN).
  ASCOOP_MAX = OP_MAX,  ///< Binary maximum operation (see @c #OP_MAX).
  ASCOOP_ADD = OP_ADD,  ///< Binary addition operation (see @c #OP_ADD).
  ASCOOP_MUL = OP_MUL,  ///< Binary multiplication operation (see @c #OP_MUL).
  ASCOOP_EUC = OP_EUC   ///< Binary Euclidean norm operation (see @c #OP_EUC).
} AsCoOp;

/**
 * @brief   Specifies idempotent operations.
 * @details The enum specifies idempotent operations, that is such ones which 
 *          can be applied multiple times without changing the result. Formally,
 *          a binary operation @f$ f @f$ on a set @f$ S @f$ is called idempotent
 *          if, whenever it is applied to two equal values @f$ x \in S @f$, it 
 *          gives that value as the result:
 *          @f[ f(x, x) = x \quad \forall x \in S @f]
 * @ingroup MinUtils_MathOper
 */
typedef enum
{
  IDOP_MIN = OP_MIN,  ///< Binary minimum operation (see @c #OP_MIN).
  IDOP_MAX = OP_MAX   ///< Binary maximum operation (see @c #OP_MAX).
} IdOp;

typedef enum
{
  UNOP_ABS = OP_ABS,  ///< Unary operation for computing absolute value.
  UNOP_SQRT = OP_SQRT, ///< Unary operation for computing square root.
  UNOP_INV = OP_INV   ///< Unare operation for bitwise NOT
} UnOp;

#endif  /* MATHOPER_H_INCLUDED */
