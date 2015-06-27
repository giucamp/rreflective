Reflective is a C++ library providing detailed run-time code reflection, supporting:
- member-data or member-function based properties with getters and setters
- method-based actions, with return type and any number of fully qualified parameters
- vectorized construction\destruction\copy\move of objects, to allow efficient implementation of non-template generic collection
- reflection of class templates, with fully qualified parameters information
- indirection levels of pointers, and constness of each indirection level
- safe up\down casts (often programmers forget that even an implicit cast may involve an offset or an indirection)
- self reflection: reflecting data is itself reflected
- multiple\virtual base classes. Of course, you may not like them, but if they are used somewhere in your code, it's not a problem