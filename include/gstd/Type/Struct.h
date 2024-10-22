#ifndef GSTD_STRUCT_H
#define GSTD_STRUCT_H

#define GSTD_DEFAULT_CONSTRUCTOR(type) type() = default

#define GSTD_DELETE_CONSTRUCTOR(type, ...) type(__VA_ARGS__) = delete

#define GSTD_DEFAULT_COPY_CONSTRUCTOR(type) type(const type &value) = default

#define GSTD_DELETE_COPY_CONSTRUCTOR(type) type(const type &value) = delete

#define GSTD_DEFAULT_MOVE_CONSTRUCTOR(type) type(type &&value) GSTD_NOEXCEPT = default

#define GSTD_DELETE_MOVE_CONSTRUCTOR(type) type(type &&value) GSTD_NOEXCEPT = delete

#define GSTD_DEFAULT_DESTRUCTOR(type) ~type() = default

#define GSTD_DELETE_DESTRUCTOR(type) ~type() = delete

#define GSTD_DEFAULT_COPY_ASSIGNMENT(type) type &operator=(const type &value) = default

#define GSTD_DELETE_COPY_ASSIGNMENT(type) type &operator=(const type &value) = delete

#define GSTD_DEFAULT_MOVE_ASSIGNMENT(type) type &operator=(type &&value) = default

#define GSTD_DELETE_MOVE_ASSIGNMENT(type) type &operator=(type &&value) = delete

#endif //GSTD_STRUCT_H
