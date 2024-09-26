#include "argument_parser.h"
#include <ObjectWrapper/object.h>
#include <ObjectWrapper/ow_string.h>
#include <stdio.h>

O_ArgumentComponent_t* CreateComponent(const char* full_name, const char* short_name, bool requires_value) {
  OWO_String_t* o_full_name = OWString_ConstructSimple(full_name);
  OWO_String_t* o_short_name = OWString_ConstructSimple(short_name);

  O_ArgumentComponent_t* component = ArgumentComponent_Construct(o_full_name, o_short_name, requires_value);

  OWObject_UnRef(o_full_name);
  OWObject_UnRef(o_short_name);

  return component;
}

int TestCase(int argc, const char** argv, bool expected_alpha, bool expected_beta, bool expected_gamma) {
  O_ArgumentParser_t* parser = ArgumentParser_Construct(argc, argv);
  ArgumentParser_AttachComponentRef(parser, CreateComponent("alpha", "a", true));
  ArgumentParser_AttachComponentRef(parser, CreateComponent("beta", "b", true));
  ArgumentParser_AttachComponentRef(parser, CreateComponent("gamma", "g", false));
  bool alpha_found = false;
  bool beta_found = false;
  bool gamma_found = false;

  OWO_String_t* argname = OWString_ConstructEmpty();
  OWO_String_t* argval = OWString_ConstructEmpty();
  while(ArgumentParser_Process(parser, argname, argval) == 0) {
    if(OWString_CompareSimple(argname, "alpha") == 0) alpha_found = true;
    if(OWString_CompareSimple(argname, "beta") == 0) beta_found = true;
    if(OWString_CompareSimple(argname, "gamma") == 0) gamma_found = true;

    printf("Argname: %s\nArgval: %s\n", OWString_GetBuffer(argname), OWString_GetBuffer(argval));
  }
  OWObject_UnRef(argname);
  OWObject_UnRef(argval);

  if(alpha_found != expected_alpha || beta_found != expected_beta || gamma_found != expected_gamma) return 1;
  OWObject_UnRef(parser);
  return 0;
}


int main() {
  OWID owid_offset = OWID_USER_DEFINED;
  owid_offset = ArgumentParser_Init(owid_offset);

  printf("TestCase #1\n");
  {
    char* argv[5];
    argv[0] = "prog_name";
    argv[1] = "--alpha";
    argv[2] = "3";
    argv[3] = "-gb4";
    argv[4] = "Undefined";

    int ret = TestCase(5, argv, true, true, true);
    if(ret != 0) return ret;
  }

  printf("TestCase #2\n");
  {
    char* argv[4];
    argv[0] = "prog_name";
    argv[1] = "--alpha";
    argv[2] = "3";
    argv[3] = "-gb4";

    int ret = TestCase(4, argv, true, true, true);
    if(ret != 0) return ret;
  }

  printf("TestCase #3\n");
  {
    char* argv[4];
    argv[0] = "prog_name";
    argv[1] = "--alpha";
    argv[2] = "3";
    argv[3] = "-b4";

    int ret = TestCase(4, argv, true, true, false);
    if(ret != 0) return ret;
  }

  printf("TestCase #4\n");
  {
    char* argv[4];
    argv[0] = "prog_name";
    argv[1] = "--alpha";
    argv[2] = "3";
    argv[3] = "-g";

    int ret = TestCase(4, argv, true, false, true);
    if(ret != 0) return ret;
  }

  printf("TestCase #5\n");
  {
    char* argv[3];
    argv[0] = "prog_name";
    argv[1] = "-ga";
    argv[2] = "3";

    int ret = TestCase(3, argv, true, false, true);
    if(ret != 0) return ret;
  }

  printf("TestCase #6\n");
  {
    char* argv[4];
    argv[0] = "prog_name";
    argv[1] = "-g";
    argv[2] = "-a";
    argv[3] = "3";

    int ret = TestCase(4, argv, true, false, true);
    if(ret != 0) return ret;
  }

  return 0;
}
