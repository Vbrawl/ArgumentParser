#include <ObjectWrapper/object.h>
#include <ObjectWrapper/ow_string.h>
#include "argument_parser.h"

int main() {

  OWO_String_t* fullname = OWString_ConstructSimple("alpha");
  OWO_String_t* shortname = OWString_ConstructSimple("a");
  bool accepts_value = true;

  O_ArgumentComponent_t* component = ArgumentComponent_Construct(fullname, shortname, accepts_value);

  OWO_String_t* fullname2 = ArgumentComponent_FullName(component);
  OWO_String_t* shortname2 = ArgumentComponent_ShortName(component);
  bool accepts_value2 = ArgumentComponent_AcceptsValue(component);

  if(OWString_CompareOWString(fullname, fullname2) != 0) return 1;
  if(OWString_CompareOWString(shortname, shortname2) != 0) return 2;
  if(accepts_value != accepts_value2) return 3;

  OWObject_UnRef(fullname2);
  OWObject_UnRef(shortname2);

  OWObject_UnRef(component);

  OWObject_UnRef(fullname);
  OWObject_UnRef(shortname);


  return 0;
}
