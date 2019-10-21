cmake_minimum_required(VERSION 3.14)

project(SelSQL)

SET(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/Parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h COMPILE_FLAGS -t)
FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/Parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB Engine Engine/*.cpp Engine/Headers/*.h)
FILE(GLOB Logic Logic/*.cpp Logic/Headers/*.h)
FILE(GLOB Utils Utils/*.cpp Utils/Headers/*.h)

SET(SOURCE_DIR ./)

FILE(GLOB Engine ${SOURCE_DIR}/Engine/*.cpp ${SOURCE_DIR}/Engine/Headers/*.h)
FILE(GLOB Logic ${SOURCE_DIR}/Logic/*.cpp ${SOURCE_DIR}/Logic/Headers/*.h ${SOURCE_DIR}/Logic/Actions/*.cpp ${SOURCE_DIR}/Logic/Actions/Headers/*.h)
FILE(GLOB Utils ${SOURCE_DIR}/Utils/*.cpp ${SOURCE_DIR}/Utils/Headers/*.h)
FILE(GLOB Parser ${SOURCE_DIR}/Parser/*.cpp ${SOURCE_DIR}/Parser/Headers/**.h)

add_executable(SelSQL main.cpp ${FLEX_Lexer_OUTPUTS} ${Engine} ${Logic} ${Utils} ${Parser} Logic/Actions/SelectAction.cpp Logic/Actions/Headers/SelectAction.h Logic/Actions/DeleteAction.cpp Logic/Actions/Headers/DeleteAction.h Logic/Actions/UpdateAction.cpp Logic/Actions/Headers/UpdateAction.h Logic/Actions/InsertAction.cpp Logic/Actions/Headers/InsertAction.h Utils/Structures/BigRequest.h Utils/Structures/BigResponse.h Utils/Structures/Error.h Utils/Structures/Data/DDLdata.h Utils/Structures/Data/DMLdata.h Utils/Structures/Data/DQLdata.h Utils/Structures/Data/Table.h Utils/Structures/Data/Variable.h Utils/Headers/ErrorConstants.h Utils/Structures/Data/Expresion.h Utils/Expression.cpp Parser/Nodes/BaseNode.h Parser/Nodes/RootNode.h Parser/Nodes/ActionNodes/CreateNode.h Parser/Nodes/VariableNode.h Parser/Nodes/ConstraintNode.h Parser/Nodes/ActionNodes/DropNode.h Parser/Nodes/ActionNodes/ShowCreateNode.h Parser/Nodes/ActionNodes/BaseActionNode.h Parser/Headers/Visitor.h Parser/TreeVisitor.cpp Parser/Headers/TreeVisitor.h Parser/Nodes/ColumnNode.h Parser/Nodes/ValuesNodes/BaseValueNode.h Parser/Nodes/ValuesNodes/IntValueNode.h Parser/Nodes/ValuesNodes/CharValueNode.h Parser/Nodes/ValuesNodes/FloatValueNode.h Parser/Nodes/ValuesNodes/NullValueNode.h Parser/Nodes/ActionNodes/InsertNode.h Parser/Nodes/ActionNodes/SelectNode.h Parser/Nodes/ActionNodes/UpdateNode.h Parser/Nodes/ActionNodes/DeleteNode.h Parser/Nodes/ColumnsAndValuesNode.h)