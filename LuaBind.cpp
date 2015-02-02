// --------------------------------------------------------------------------------------------- //
// LuaBind example
//
// This example project for Microsoft(tm) Visual C++ 2008(tm) users
// compiles out-of-the-box. Precompiled binaries can be found in the
// "References" directory in case you wish to use them for other
// projects. Pay attention to the runtime library (multithreaded dll)
// setting and the different library files being used for debug and release
// builds when you do so!
// --------------------------------------------------------------------------------------------- //

// Include the lua headers (the extern "C" is a requirement because we're
// using C++ and lua has been compiled as C code)
/*extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

// This is the only header we need to include for LuaBind to work
#include "luabind/luabind.hpp"

// Some other C++ headers we're most likely going to use
#include <iostream>
#include <string>

// We don't want to write std:: every time we're displaying some debug output
using namespace std;

void HelloWorld(){
	cout << "\n[C++]: Hello world" << endl;
}
int add(int a, int b){
	return a + b;
}


// --------------------------------------------------------------------------------------------- //

// Put your testing code here...

/*
int main() {
  
	lua_State *luaState = lua_open();

  //open luabind
	luabind::open(luaState);
	
	//access to LUA's STDLib
	luaL_openlibs(luaState);



  luabind::module(luaState) [
	  luabind::def("HelloWorld", &HelloWorld),
	  luabind::def("add", &add)
];

  if(int error = luaL_dofile(luaState, "script.lua") != 0 ){
	  std::cout << "\nC++]: ERROR( " << lua_tostring(luaState, -1) << "): Problem with lua script " << std::endl;
	  return 0;
  }
  // ...and here ;)

  lua_close(luaState);
  return 0;
}
*/