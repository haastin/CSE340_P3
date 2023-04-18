#ifndef __parseProgram__H__
#define __parseProgram__H__

#include "compiler.h"
#include <unordered_map>
#include "lexer.h"

extern std::unordered_map<std::string, int> location_table;

struct InstructionNode* parse_generate_intermediate_representation();
void assignVarMemLocations();
struct InstructionNode* parse_body();
void readInputs();

//they all assume the first thing they will consume with GetToken is data, not any keywords
struct InstructionNode* parse_assign_stmt();
struct InstructionNode* parse_while_stmt();
struct InstructionNode* parse_if_stmt();
struct InstructionNode* parse_switch_stmt();
struct InstructionNode* parse_for_stmt();
struct InstructionNode* parse_input_stmt();
struct InstructionNode* parse_output_stmt();

#endif  //__parseProgram__H__