### VM Types

There are two main kinds of runtime types: primitives (raw data) and objects (references)
Only mixed type allowed in PSVM is string, which is object reference, which have its own instructions
and does not contains object header. Instead it starts with 4 bytes of unsigned int in host endian encoding.

Primitive types:
1.  Bool: boolean value, true or false, under development. Assumed, that this is and 4 bytes long value,
    which equal to 0x00000001 or 0x00000000. Further design is required.
2.  Int: signed integer value, 4 bytes long. Everywhere, except in appendix of `iconst` instruction, stored in host
    endian, using the big endian for `iconst`.
3.  Float: single-precision floating point number, [IEEE 754][1] standard based.
    //TODO: Need to clarify storage format in fconst instruction.
4.  Char: UTF-16 encoded symbol. `cload_[hi|lo]` and `csave_[hi|lo]` takes either high part of local variable or lower
    part to load/save character to/from stack, expected to have two char bytes on its top.

Primitive reference types:
1.  String. It is stored as object reference in local variables and on the stack, but does not contains usual object
    header.
    It could be named as char array, but array starts with pool constant index, which points to the name of
    element type, followed by 4 bytes of integer value for current array length.
    String starts with 1 bytes of encoding type and other flags, 4 bytes of string size in bytes (integer),
    followed by string size length characters without null terminator.
    // TODO: encodings discussion and related VM instructions

Reference types:
1.  Object. It contains header, which have information about object type and the object fields's data
2.  Array. It's header starts with 4 bytes of array length value, then follows array element type information.

[1]: https://ieeexplore.ieee.org/document/8766229