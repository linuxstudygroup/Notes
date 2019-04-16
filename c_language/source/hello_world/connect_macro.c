#define MACRO1(name, type) type name_##type##_type

#define MACRO2(name, type) type name##_##type##_type

MACRO1(Bar, int);
MACRO2(Foo, string);

#define ABC abc
#define MACRO3(name, type) type name##_##ABC
MACRO3(Tee, float);
