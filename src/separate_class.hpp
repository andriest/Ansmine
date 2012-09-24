

#ifndef __SEPARATE_CLASS_HPP__
#define __SEPARATE_CLASS_HPP__

#define DECLARE_SCLASS(Class) \
    inline Class* s_func(){ \
        return s_ptr; \
    } \
    friend class Class

#define P_S(Class) Class* const s = s_func()

#endif
