#include "parseProgram.h"
#include "lexer.h"
#include <unordered_map>
#include <iostream>
#include <ostream>

std::unordered_map<std::string, int> location_table;
LexicalAnalyzer lexer;

struct InstructionNode *parse_generate_intermediate_representation()
{
    assignVarMemLocations();

    InstructionNode *start = parse_body();

    readInputs();

    return start;
}

void assignVarMemLocations()
{

    Token tkn = lexer.GetToken();

    while (tkn.token_type != END_OF_FILE && tkn.token_type != LBRACE)
    {
        if (tkn.token_type == ID)
        {
            location_table[tkn.lexeme] = next_available;
            next_available++;
        }
        tkn = lexer.GetToken();
    }
}

struct InstructionNode *parse_body()
{

    InstructionNode *start = NULL; // keep track of start of program
    InstructionNode *tail = NULL;  // to be able to add to our list

    Token tkn = lexer.GetToken();

    // we build our first instruction here, if we can
    if (tkn.token_type != END_OF_FILE && tkn.token_type != RBRACE)
    {
        switch (tkn.token_type)
        {
        case ID:
            lexer.UngetToken(1);
            start = parse_assign_stmt();
            break;
        case WHILE:
            start = parse_while_stmt();
            break;
        case IF:
            start = parse_if_stmt();
            break;
        case SWITCH:
            start = parse_switch_stmt();
            break;
        case FOR:
            start = parse_for_stmt();
            break;
        case INPUT:
            start = parse_input_stmt();
            break;
        case OUTPUT:
            start = parse_output_stmt();
            break;
        }
        tail = start;
        tail->next = NULL;
    }
    if (tkn.token_type != RBRACE)
    {
        tkn = lexer.GetToken();
        // we build subsequent instructions here
        while (tkn.token_type != RBRACE)
        {

            switch (tkn.token_type)
            {
            case ID:
                lexer.UngetToken(1);
                tail->next = parse_assign_stmt();
                break;
            case WHILE:
                tail->next = parse_while_stmt();
                break;
            case IF:
                tail->next = parse_if_stmt();
                break;
            case SWITCH:
                tail->next = parse_switch_stmt();
                break;
            case FOR:
                tail->next = parse_for_stmt();
                break;
            case INPUT:
                tail->next = parse_input_stmt();
                break;
            case OUTPUT:
                tail->next = parse_output_stmt();
                break;
            }
            // need to move tail so we can insert at back of list
            while (tail->next != NULL)
            {
                tail = tail->next;
            }
            tkn = lexer.GetToken();
        }
    }
    return start;
}

void readInputs()
{

    Token tkn = lexer.GetToken();

    while (tkn.token_type != END_OF_FILE)
    {
        if (tkn.token_type == NUM)
        {
            inputs.push_back(stoi(tkn.lexeme));
        }
        tkn = lexer.GetToken();
    }
}

struct InstructionNode *parse_assign_stmt()
{
    InstructionNode *new_instr = new InstructionNode;
    new_instr->type = ASSIGN;

    std::string variable = lexer.GetToken().lexeme;
    new_instr->assign_inst.left_hand_side_index = location_table[variable];

    lexer.GetToken();             // gets = sign
    Token val = lexer.GetToken(); // gets first val
    int op1_addy = 0;
    // only need to allocate memory if the val is a num, because an id will already have memory allocated
    if (val.token_type == NUM)
    {
        mem[next_available] = stoi(val.lexeme);
        op1_addy = next_available;
        next_available++;
    }
    else
    {
        op1_addy = location_table[val.lexeme];
    }
    new_instr->assign_inst.operand1_index = op1_addy;
    Token tkn = lexer.GetToken(); // either is operation or semicolon
    if (tkn.token_type == SEMICOLON)
    {
        new_instr->assign_inst.op = OPERATOR_NONE;
    }
    else // assign gets set to the result of an expression
    {
        Token val2 = lexer.GetToken();
        int op2_addy = 0;
        // only need to allocate memory if the val is a num, because an id will already have memory allocated
        if (val2.token_type == NUM)
        {
            mem[next_available] = stoi(val2.lexeme);
            op2_addy = next_available;
            next_available++;
        }
        else
        {
            op2_addy = location_table[val2.lexeme];
        }

        switch (tkn.token_type)
        {
        case PLUS:
            new_instr->assign_inst.op = OPERATOR_PLUS;
            new_instr->assign_inst.operand2_index = op2_addy;
            break;
        case MINUS:
            new_instr->assign_inst.op = OPERATOR_MINUS;
            new_instr->assign_inst.operand2_index = op2_addy;
            break;
        case MULT:
            new_instr->assign_inst.op = OPERATOR_MULT;
            new_instr->assign_inst.operand2_index = op2_addy;
            break;
        case DIV:
            new_instr->assign_inst.op = OPERATOR_DIV;
            new_instr->assign_inst.operand2_index = op2_addy;
            break;
        }

        lexer.GetToken(); // consume the semicolon
    }
    new_instr->next = NULL;
    return new_instr;
}

struct InstructionNode *parse_while_stmt()
{

}

struct InstructionNode *create_cjmp_node(){

    
    // if condition is false, we jump. so, our content inside the if should be the nodes before the jump address
    InstructionNode *eval_if = new InstructionNode;
    eval_if->type = CJMP;
    Token val1 = lexer.GetToken();
    Token eval_op = lexer.GetToken();
    Token val2 = lexer.GetToken();
    switch (eval_op.token_type)
    {
    case GREATER:
        eval_if->cjmp_inst.condition_op = CONDITION_GREATER;
        break;
    case LESS:
        eval_if->cjmp_inst.condition_op = CONDITION_LESS;
        break;
    case NOTEQUAL:
        eval_if->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
        break;
    }
    int op1_addy = 0;
    int op2_addy = 0;
    if (val1.token_type == NUM)
    {
        mem[next_available] = stoi(val1.lexeme);
        op1_addy = next_available;
        next_available++;
    }
    else
    {
        op1_addy = location_table[val1.lexeme];
    }
    if (val2.token_type == NUM)
    {
        mem[next_available] = stoi(val2.lexeme);
        op2_addy = next_available;
        next_available++;
    }
    else
    {
        op2_addy = location_table[val2.lexeme];
    }
    eval_if->cjmp_inst.operand1_index = op1_addy;
    eval_if->cjmp_inst.operand2_index = op2_addy;
    eval_if->next = NULL;
    return eval_if;
}

struct InstructionNode *parse_if_stmt()
{
    InstructionNode *target = new InstructionNode;
    target->type = NOOP;
    target->next = NULL;

    InstructionNode *eval_if = create_cjmp_node();
    eval_if->cjmp_inst.target = target;

    lexer.GetToken(); // consume LBRACE

    InstructionNode *if_stmt_body = parse_body();

    if (if_stmt_body != NULL)
    {
        eval_if->next = if_stmt_body;
        std::cout << "eval if next after assign: " << eval_if->next->type << std::endl;
        std::cout << "body of if:" << std::endl;
        std::cout << if_stmt_body->type << std::endl;
        std::cout << "after if:" << std::endl;
        std::cout << if_stmt_body->next << std::endl;
        while (if_stmt_body->next != NULL)
        {
            std::cout << if_stmt_body->type << std::endl;
            if_stmt_body = if_stmt_body->next;
        }
        if_stmt_body->next = target;
    }
    else
    {
        std::cout << "returned list is null" << std::endl;
        eval_if->next = target;
    }
    std::cout << "eval if next: " << eval_if->next->type << std::endl;
    return eval_if;
}

struct InstructionNode *parse_for_stmt()
{
    //assign for var
    //eval for var
    //for body; go back to it depending on the conditional jump
    //increment var
    //jump to eval
    lexer.GetToken(); //consume LPAREN
    InstructionNode *initialize_var = parse_assign_stmt();
    InstructionNode *target = new InstructionNode;
    target->type = NOOP;
    target->next = NULL;

    InstructionNode *eval_if = create_cjmp_node();
    eval_if->cjmp_inst.target = target;

    lexer.GetToken(); //consume semicolon

    InstructionNode *alter_var = parse_assign_stmt();

    lexer.GetToken(); //consume rparen

    lexer.GetToken(); //consume LBRACE

    InstructionNode *for_body = parse_body();

    InstructionNode *goto_eval = new InstructionNode;
    goto_eval->type = JMP;
    goto_eval->jmp_inst.target = eval_if;

    initialize_var->next = eval_if;
    eval_if->next = for_body;
    for_body->next = alter_var;
    alter_var->next = goto_eval;
    goto_eval->next = target;

    return initialize_var;
}

struct InstructionNode *parse_switch_stmt()
{
}

struct InstructionNode *parse_input_stmt()
{
    Token var = lexer.GetToken();
    InstructionNode *new_instr = new InstructionNode;
    new_instr->type = IN;
    new_instr->next = NULL;
    new_instr->input_inst.var_index = location_table[var.lexeme];
    lexer.GetToken(); // consume semicolon
    return new_instr;
}

struct InstructionNode *parse_output_stmt()
{
    Token var = lexer.GetToken();
    InstructionNode *new_instr = new InstructionNode;
    new_instr->type = OUT;
    new_instr->next = NULL;
    new_instr->output_inst.var_index = location_table[var.lexeme];
    lexer.GetToken(); // consume semicolon
    return new_instr;
}