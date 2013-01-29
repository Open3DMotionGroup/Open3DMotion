/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_PYTHON_TOTAL_REF_COUNT_H_
#define _OPEN3DMOTION_PYTHON_TOTAL_REF_COUNT_H_

/** Helper to verify current reference count and avoid memory leaks. 
    @return Sum of reference count over all allocated Python objects. */
size_t PythonTotalRefCount();

#endif
