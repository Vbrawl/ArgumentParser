#include <ObjectWrapper/object.h>
#include <ObjectWrapper/ow_string.h>
#include "argument_parser.h"
#include <stdbool.h>

OWID OWID_ArgumentComponent_t = OWID_UNDEFINED;
OWID OWID_ArgumentParser_t = OWID_UNDEFINED;
void _ArgumentComponent_Destroy(O_ArgumentComponent_t* this);
void _ArgumentParser_Destroy(O_ArgumentParser_t* this);


OWID ArgumentParser_Init(OWID offset) {
  OWID_ArgumentComponent_t = offset++;
  OWID_ArgumentParser_t = offset++;

  return offset;
}

O_ArgumentComponent_t* ArgumentComponent_Construct(OWO_String_t* full_name, OWO_String_t* short_name, bool accepts_value) {
  O_ArgumentComponent_t* this = OWObject_Construct(ArgumentComponent_t, NULL, _ArgumentComponent_Destroy, NULL);
  if(this == NULL) return NULL;
  ArgumentComponent_t* const obj = this->object;

  obj->full_name = full_name ? OWObject_Ref(full_name) : NULL;
  obj->short_name = short_name ? OWObject_Ref(short_name) : NULL;
  obj->accepts_value = accepts_value;
  return this;
}


void _ArgumentComponent_Destroy(O_ArgumentComponent_t* this) {
  ArgumentComponent_t* const obj = OWObject_FindObjectInClass(this, OWID_ArgumentComponent_t);

  OWObject_UnRef(obj->full_name);
  OWObject_UnRef(obj->short_name);
}



O_ArgumentParser_t* ArgumentParser_Construct(int argc, const char** argv) {
  O_ArgumentParser_t* this = OWObject_Construct(ArgumentParser_t, NULL, _ArgumentParser_Destroy, NULL);
  if(this == NULL) return NULL;
  ArgumentParser_t* const obj = this->object;

  obj->current_index = 1;
  obj->current_sub_index = 0;
  obj->argc = argc;
  obj->argv = argv;
  obj->components = OWVector_Construct(1);

  return this;
}

void ArgumentParser_AddComponent(O_ArgumentParser_t* this, O_ArgumentComponent_t* component) {
  ArgumentParser_t* const obj = OWObject_FindObjectInClass(this, OWID_ArgumentParser_t);
  if(obj == NULL) return;

  OWVector_PushBack(obj->components, component);
}

void ArgumentParser_AttachComponentRef(O_ArgumentParser_t* this, O_ArgumentComponent_t* component) {
  ArgumentParser_AddComponent(this, component);
  OWObject_UnRef(component);
}

const char* __Helper__ArgumentParser_ProcessValue(O_ArgumentParser_t* this, bool is_short) {
  ArgumentParser_t* const obj = OWObject_FindObjectInClass(this, OWID_ArgumentParser_t);
  const char* val = "";

  if(is_short) {
    obj->current_sub_index += 1;
  }

  if(!is_short || obj->current_sub_index >= strlen(obj->argv[obj->current_index])) {
    obj->current_sub_index = 0;
    obj->current_index += 1;
  }

  if(obj->current_index < obj->argc)
    val = obj->argv[obj->current_index] + obj->current_sub_index;
  return val;
}

int ArgumentParser_Process(O_ArgumentParser_t* this, OWO_String_t* argname, OWO_String_t* argval) {
  OWString_SetSimple(argname, "");
  OWString_SetSimple(argval, "");

  ArgumentParser_t* const obj = OWObject_FindObjectInClass(this, OWID_ArgumentParser_t);
  if(obj == NULL) return -1;

  if(obj->current_index >= obj->argc
     || obj->current_sub_index + 1 >= strlen(obj->argv[obj->current_index])) {
    return 1;
  }

  const char* opt = obj->argv[obj->current_index];
  bool is_full_name = (opt[0] == '-' && opt[1] == '-' && opt[2] != '-' && opt[2] != '\0');
  bool is_short_name = (opt[0] == '-' && opt[1] != '-' && opt[1] != '\0');

  bool found = false;
  bool value_found = false;
  for(size_t i = 0; i < OWVector_GetSize(obj->components) && !found; i++) {
    O_ArgumentComponent_t* component = OWVector_Get(obj->components, i);

    OWO_String_t* full_name = ArgumentComponent_FullName(component);
    if(is_full_name) found = OWString_CompareSimple(full_name, opt+2) == 0;
    else if(is_short_name) {
      OWO_String_t* short_name = ArgumentComponent_ShortName(component);
      found = OWString_GetBuffer(short_name)[0] == *(opt + (obj->current_sub_index) + 1);
      OWObject_UnRef(short_name);
    }
    if(found) OWString_SetOWString(argname, full_name);
    OWObject_UnRef(full_name);


    if(found) {
      if(ArgumentComponent_AcceptsValue(component)) {
        obj->current_sub_index += 1;
        const char* val = __Helper__ArgumentParser_ProcessValue(this, is_short_name);
        OWString_SetSimple(argval, val);
        value_found = true;
      }
    }
    OWObject_UnRef(component);
  }

  if(is_short_name && !value_found) {
    obj->current_sub_index += 1;
    if(obj->current_sub_index >= strlen(obj->argv[obj->current_index])) {
      obj->current_sub_index = 0;
      obj->current_index += 1;
    }
  }
  else {
    obj->current_sub_index = 0;
    obj->current_index += 1;
  }

  return 0;
}






void _ArgumentParser_Destroy(O_ArgumentParser_t* this) {
  ArgumentParser_t* const obj = this->object;

  OWObject_UnRef(obj->components);
}
