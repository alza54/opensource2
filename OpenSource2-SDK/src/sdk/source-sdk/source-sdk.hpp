#pragma once

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define PAD_CLASS(sz)                           \
 private:                                       \
  std::uint8_t COMBINE2(pad_, __COUNTER__)[sz]; \
                                                \
 public:

//#include "../math/classes/vector.hpp"
//#include "../math/classes/vmatrix.hpp"
//
//#include "classes/bitflag.hpp"
//#include "classes/cutlmap.hpp"
//#include "classes/ccsgoinput.hpp"
//#include "classes/entity/centityidentity.hpp"
//#include "classes/entity/centityinstance.hpp"
//#include "classes/entity/cgamescenenode.hpp"
//#include "classes/entity/ccollisionproperty.hpp"
//#include "classes/entity/c_baseentity.hpp"
//#include "classes/entity/c_basemodelentity.hpp"
//#include "classes/entity/cbaseplayercontroller.hpp"
//#include "classes/entity/ccsplayercontroller.hpp"
//#include "classes/entity/c_csplayerpawn.hpp"
//#include "classes/entity/c_chicken.hpp"
//#include "classes/entity/c_plantedc4.hpp"
//#include "classes/entity/bones/cbonesystem.hpp"
//#include "classes/econ/ceconitem.hpp"
//#include "classes/econ/ceconitemschema.hpp"
//#include "classes/econ/ceconitemsystem.hpp"
//#include "classes/cgameentitysystem.hpp"
