/*
 * Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_OOPS_ARRAYPROPERTIES_HPP
#define SHARE_OOPS_ARRAYPROPERTIES_HPP

#include "utilities/globalDefinitions.hpp"
#include "utilities/ostream.hpp"

class ArrayProperties {
 public:
  typedef u1 Type;

#define ARRAY_PROPERTIES_FLAGS_DO(flag)  \
    flag(is_null_restricted    , 1 << 0) \
    flag(is_non_atomic         , 1 << 1) \
    flag(is_invalid            , 1 << 2) \
    /* end of list */

#define ARRAY_PROPERTIES_FLAGS_ENUM_NAME(name, value)    _misc_##name = value,
  enum {
    ARRAY_PROPERTIES_FLAGS_DO(ARRAY_PROPERTIES_FLAGS_ENUM_NAME)
  };
#undef ARRAY_PROPERTIES_FLAGS_ENUM_NAME

  Type _flags;

 public:
  ArrayProperties() : _flags(0) {}
  ArrayProperties(Type flags) : _flags(flags) {}

  Type value() const { return _flags; }

  // Create getters and setters for the flag values.
#define ARRAY_PROPERTIES_FLAGS_GET_SET(name, ignore)          \
  bool name() const { return (_flags & _misc_##name) != 0; } \
  void set_##name(bool b) {         \
    assert(!name(), "set once");    \
    if (b) _flags |= _misc_##name; \
  }
  ARRAY_PROPERTIES_FLAGS_DO(ARRAY_PROPERTIES_FLAGS_GET_SET)
#undef ARRAY_PROPERTIES_FLAGS_GET_SET

  const char* as_string() {
    // Caller must have set a ResourceMark
    stringStream ss;
    if (is_invalid()) {
      return "INVALID";
    } else {
      ss.print("%s", (is_null_restricted() != 0) ? "NULL_RESTRICTED " : "NULLABLE ");
      ss.print("%s", (is_non_atomic() != 0) ? "NON_ATOMIC " : "ATOMIC ");
    }
    return ss.as_string();
  }
};

inline bool operator==(ArrayProperties a, ArrayProperties b) {
  return a._flags == b._flags;
}

#endif // SHARE_OOPS_ARRAYPROPERTIES_HPP
