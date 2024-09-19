#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <ObjectWrapper/object.h>
#include <ObjectWrapper/vector.h>
#include <ObjectWrapper/ow_string.h>
#include <ObjectWrapper/object_identifiers.h>
#include <stdbool.h>


OWID ArgumentParser_Init(OWID offset);




extern OWID OWID_ArgumentComponent_t;
typedef struct {
  OWO_String_t* full_name;
  OWO_String_t* short_name;
  bool accepts_value;
} ArgumentComponent_t;
typedef OWObject_t O_ArgumentComponent_t;

O_ArgumentComponent_t* ArgumentComponent_Construct(OWO_String_t* full_name, OWO_String_t* short_name, bool accepts_value);
#define ArgumentComponent_FullName(this) OWObject_Ref(((ArgumentComponent_t*)OWObject_FindObjectInClass(this, OWID_ArgumentComponent_t))->full_name)
#define ArgumentComponent_ShortName(this) OWObject_Ref(((ArgumentComponent_t*)OWObject_FindObjectInClass(this, OWID_ArgumentComponent_t))->short_name)
#define ArgumentComponent_AcceptsValue(this) ((ArgumentComponent_t*)OWObject_FindObjectInClass(this, OWID_ArgumentComponent_t))->accepts_value

extern OWID OWID_ArgumentParser_t;
typedef struct {
  int current_index;
  int current_sub_index;
  int argc;
  const char** argv;
  OWO_Typed_Vector_t(O_ArgumentComponent_t*)* components;
} ArgumentParser_t;
typedef OWObject_t O_ArgumentParser_t;

O_ArgumentParser_t* ArgumentParser_Construct(int argc, const char** argv);
void ArgumentParser_AddComponent(O_ArgumentParser_t* this, O_ArgumentComponent_t* component);
void ArgumentParser_AttachComponentRef(O_ArgumentParser_t* this, O_ArgumentComponent_t* component);
int ArgumentParser_Process(O_ArgumentParser_t* this, OWO_String_t* argname, OWO_String_t* argval);

#endif
