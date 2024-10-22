#ifndef GSTD_GSTD_H
#define GSTD_GSTD_H

/*
 * Algorithm
 * Containers
 * Contract
 * Diagnostic
 * IO
 * Macro
 * Memory
 * Meta
 * Type
 * Compiler
 * Async
 * Parallel
 *
 *      |----------------
 *      | Backtrace - module for containing, formatting, manipulating and printing stacktrace.
 *      | Container - module for any containers of data (vector, list, stack and etc.)
 *      | Contract  - module for supporting contract programming
 * gstd | Coroutine - module for async programming
 *      | Error     -
 *      | IO        -
 *      | Macro     -
 *      | Types     -
 *
 * algorithm, async, backtrace, concurrent, container, contract, (error), io, macro, type
 */

#include <gstd/Containers/Containers.h>
#include <gstd/Diagnostic/Diagnostic.h>
#include <gstd/IO/IO.h>
#include <gstd/Macro/Macro.h>
#include <gstd/Memory/Memory.h>
#include <gstd/Meta/Meta.h>
#include <gstd/System/System.h>
#include <gstd/Type/Type.h>
#include <gstd/Utils/Utils.h>

#endif //GSTD_GSTD_H
