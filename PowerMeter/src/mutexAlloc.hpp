#ifndef MUTEX_ALLOC_HPP
#define MUTEX_ALLOC_HPP

#include "mutex.hpp"

// protection ressource allocation de mémoire dynamique
#define MUT_ALLOC

#ifdef MUT_ALLOC
    extern TMutex mutAlloc;
#endif


#endif // MUTEX_ALLOC_HPP
