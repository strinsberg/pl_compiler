//-----------------------------------------------------------
// interp.cc
//
// Description: PL interpreter
//
// NOTES:
// - To add data types there will probably need to be a data type
//   associated with each variable or value. Floats can be stored
//   with 2 words, and char is still 1 but need to know it when
//   so we write it as a char and not an int. This can allow
//   all the data to still be stored as integers.
// - Adding parameters probably just means allocating extra
//   variables in the block. These need to be before the defined
//   variables so the displacement of those vars would need to increase
//   by the number of parameters.
// - Calling the proc would need to take
//   the n param values on top of the stack and store them in the
//   parameter variable spots. Variable values will have to be
//   retreived for this initialization. References might be a little
//   trickier as we need to store the address of a varialbe and know
//   when doing things to it that they are modifying the base variable
//   and not the parameter copy.
//-------------------------------------------------------------

// INCLUDES

#include <iostream>
#include <fstream>
#include <iomanip>
#include "interp.h"
// FUNCTIONS

Interpreter::Interpreter( string filename, bool step)
{
//  this->stepping = step;
  stepping = step;
  cout << " Loading..." << endl;
  load_program(filename);
  cout << " Running ..." << endl;
  run_program();
}

void Interpreter::runtime_error( string  message, int line_number)
{
  if ( line_number != -1)
     cerr << "line " << line_number << " : " << message << endl;
  else
     cerr << message << endl;
  running = false;
}

void Interpreter::allocate( int words )
{
  stack_register += words;
  if (stack_register > STORE_SIZE)
     runtime_error(" stack overflow");
}

//---------------------------------
// variable_access = "variable" [ expression "index" ].
//---------------------------------

void Interpreter::variable( int level, int displacement)
{
  int x;

  // Make space for the variable address
  // and find the bottom of this block level
  allocate(1);
  x = base_register;
  
  // Move down to the address that starts the block level we need
  while ( level > 0)
  {
    x = store[x];
    --level;
  }

  // Store the address of the variable on top of the stack
  // displacement is the distance from the base of the block level
  store[stack_register] = x + displacement;
  program_register += 3;
}

void Interpreter::index( int bound, int line_number)
{
  int i = store[stack_register];  // take top value on stack as index
  --stack_register;

  if (i < 1 || i > bound)  // bounds check -- appears this uses 1 based array indexing
     runtime_error(" range error", line_number);
  else
     store[stack_register] = store[stack_register] + i -1;  // -1 for one based
  program_register += 3;
}

//----------------------------------------
// factor = "constant" | variable_access "value" |
//          expression | factor "not".
//-----------------------------------------

void Interpreter::constant( int value )
{
  // Make space for the value and save it to the top of the stack
  allocate(1);
  store[stack_register] = value;
  program_register += 2;
}

//-------------------------------

void Interpreter::value()
{
  // Replace address at top of stack with a value
  // ie. load a variable value?
  int x = store[stack_register];
  store[stack_register] = store[x];
  ++program_register;
}

void Interpreter::plnot()
{
  // Flip the bool bit. 1-0=1 and 1-1=0.
  store[stack_register] = 1 - store[stack_register];
  ++program_register;
}

//---------------------------------------
// term = factor { factor multiplying_operator}.
// multiplying_operator =
//  "multiply" | "divide" | "modulo".
//---------------------------------------

void Interpreter::multiply()
{
  // All binary operations work this way

  ++program_register;  // Move to the next instruction
  --stack_register;  // move stack down because we are combining top 2 values

  // Take the top 2 elements on the stack and multiply them
  store[stack_register] = store[stack_register] * store[stack_register +1];
}

void Interpreter::divide()
{
  ++program_register;
  --stack_register;
  store[stack_register] = store[stack_register] / store[stack_register +1];
}
 
void Interpreter::modulo()
{
  ++program_register;
  --stack_register;
  store[stack_register] = store[stack_register] % store[stack_register +1];
}

//----------------------------------------------
// simple_expression = term [ "minus" ] { term adding_operator }.
//-------------------------------------------------

void Interpreter::minus()
{
   store[stack_register] *= -1;  // Negate the value at stack register
   ++program_register;
}

//---------------------------------------

void Interpreter::add()
{
  ++program_register;
  --stack_register;
  store[stack_register] = store[stack_register] + store[stack_register+1]; 
}

void Interpreter::subtract()
{
  ++program_register;
  --stack_register;
  store[stack_register] = store[stack_register] - store[stack_register+1];
}

//--------------------------------------------
// primary_expression = simple_expression
// [ simple_expression relational_operator ]
// relational_operator = "less" | "equal" | "greater".
//--------------------------------------------

void Interpreter::less()
{
  ++program_register;
  --stack_register;
  store[stack_register] = ( store[stack_register] < 
                          store[stack_register + 1 ]);
}

void Interpreter::equal()
{
  ++program_register;
  --stack_register;
  store[stack_register] = ( store[stack_register] == 
                          store[stack_register + 1 ]);
}

void Interpreter::greater()
{
  ++program_register;
  --stack_register;
  store[stack_register] = ( store[stack_register] > 
                          store[stack_register + 1 ]);
}

//--------------------------------------------
// expression =  primary_expression
// { primary_expression primary_operator }.
// primary_operator = "and" | "or".
//--------------------------------------------

void Interpreter::pland()
{
  ++program_register;
  --stack_register;
  if (store[stack_register] == 1)
    store[stack_register] = store[stack_register + 1];
}


void Interpreter::plor()
{
  ++program_register;
  --stack_register;
  if (store[stack_register] == 0)
    store[stack_register] = store[stack_register + 1];
}

//------------------------------------------------
// read_statement = variable_list "read".
// variable_list = variable_access { variable_access }.
//------------------------------------------------

void Interpreter::read(int count)
{
  int x;
//  cout << "program_register = " << program_register << endl;
//  cout << "stack_register = " << stack_register << endl;
//  cout << "base_register = " << base_register << endl;

  program_register += 2;

  // Move stack to start of variables to read into
  stack_register -= count;
  x = stack_register;
//  cout << "Count = " << count;

  // Read into each variable
  // store[x] is the address of the variable
  // so store[ store[x] ] gives us the actual variables memory location
  // to write the value into
  while ( x < stack_register + count)
  {
    ++x;
    cout << "Input: ";
    cin >> ( store[ store[x] ] );
  }
}

//-------------------------------------------------
// write_statement = expression_list "write".
// expression = expression { expression }.
//-------------------------------------------------

void Interpreter::write (int count)
{
  // Similar to read but we display the value at store[x] instead of
  // using it as an address to find a memory location to read into
  int x;
  program_register += 2;
  stack_register -= count;
  x = stack_register;

  while (x < stack_register + count )
  {
    ++x;
    cout << "  Output: ";
    cout << ( store[x]) << endl;
  }
}

//---------------------------------------
// assignment_statement = variable_list expression_list.
//--------------------------------------

void Interpreter::assign (int count)
{
  int x;
  
  // Stack has count values on top of count addresses
  program_register += 2;
  stack_register = stack_register - 2*count;
  x = stack_register;

  // Take the value in the expression and put it in the variable location
  while ( x < stack_register + count)
  {
    ++x;
    store[store[x]] = store[x + count];
  }
}

//------------------------------------------
// procedure_statement = "call"
//------------------------------------------

void Interpreter::call( int level, int address)
{
  int x;

  allocate(3);
  x = base_register;

  // set x to the address of the block level the proc address is in  
  while ( level > 0)
  {
    x = store[x];
    --level;
  }

  store[stack_register - 2] = x;  // save the address of the proc block level
  store[stack_register - 1] = base_register;  // Save current block level address
  store[stack_register] = program_register + 3;  // Save the next instr address
  
  base_register = stack_register - 2;  // Set the base the proc block level
  program_register = address;  // Go to the address of proc
}

//-----------------------------------------
// if_statement = guarded_command_list "fi".
// do_statement = guarded_command_list
// guarded_command_list = guarded_command { guarded_command}.
// guarded_command = expression "arrow" statement_part "bar".
//----------------------------------------

void Interpreter::arrow( int address)
{
  if ( store[stack_register] == 1 )  // If true execute stmts
     program_register += 2;
  else
     program_register = address;  // Otherwise jump to next section
  --stack_register;
}

//----------------------------------------

void Interpreter::bar( int address)
{
  program_register = address;  // Jump past end of guarded command list
}

//-----------------------------------------

void Interpreter::fi( int line_number)
{
  // this was supposed to force if statments to cover
  // all conditions, but I do no like that as a language
  // feature. To re-enable it comment out the address increment
  // and uncomment the runtime error
  //runtime_error(" if statement fails", line_number);
  program_register += 2;
}

//------------------------------------------------
// procedure_definition = "proc" block "endproc"
// block = { procedure_definition } statement_part.
// statement_part = { statement }.
// statement = empty | read_statement | write_statement
//             | assignment_statement | procedure_statement |
//             | if_statement | do_statement.
//-----------------------------------------------

void Interpreter::proc( int variable_length, int address)
{
  allocate( variable_length );  // Allocate the number of variables
  program_register = address;  // go to proc address ??
}

void Interpreter::endproc()
{
  // Reset registers after proc execution
  // See Interpreter::call
  stack_register = base_register - 1;
  program_register = store[base_register + 2];
  base_register = store[base_register + 1];
}

//--------------------------------
// program = "program" block "endprog".
//-------------------------------

void Interpreter::prog( int variable_length, int address)
{
  base_register = stack_bottom;  // address at (or after?) end of program
//  cout << "base_register = " << stack_bottom << endl;

  stack_register = base_register;
//  cout << "stack_register = " << stack_register << endl;

  allocate( variable_length + 2);  // allocate space for variables

  program_register = address;  // Set program register to start of prog instructions
//  cout << "program_register = " << program_register << endl;
}

//-----------------------------------------

void Interpreter::endprog()
{
  running = false;
}

//-------------------------------------------
// Basic operations
//-------------------------------------------

void Interpreter::load_program( string name)
{
  int x = 0;
  int word;
  const char* tempname = name.c_str(); 
  ifstream program(tempname, ios::in);
  
  if (!program)
  {
    cerr << " Could not open program file" << endl;
    return;
  }

// read program into memory

  while (!program.eof())
  {
    program >> word;
//    cout << " Read word " << word << "  Temp name: " << tempname << endl;
    store[x] = word;
    if ( word == -1) break;
    ++x;
//    cout << "x is " << x << endl;
    
    if ( x >= STORE_SIZE)
    {
      cerr << "program exceeded" << x << " words " << endl;
      cerr << "Not enough memory to load program " << endl;
      return;
     }
   }
  
   stack_bottom = x;

 //  cout << "stack_bottom = " << x << endl;
   
   // clear the rest of memory

   for (int i = stack_bottom; i <= STORE_SIZE; i++)
      store[i] = -1;
}

void Interpreter::run_program()
{
  OperationCode  opcode;
  program_register = 0; 
  running = true;
  
  while (running)
  {
//
    opcode = (OperationCode) store[program_register];
//
 //   cout << "Opcode = " << opcode << endl;
    if ( stepping )
    {
      cout << endl << " press < enter > to execute "
                   << opcode_name[opcode] << " operation" << endl;
      cin.get();
    }

    // Execute program instruction
    switch (opcode)
    {
      case OP_ADD:
         add();
         break;
      case OP_AND:
         pland();
         break;
      case OP_ARROW:
         arrow(store[program_register + 1]);
         break;
      case OP_ASSIGN:
         assign( store[program_register + 1]);
         break;
      case OP_BAR:
         bar(store[program_register + 1]);
         break;
      case OP_CALL:
         call(store[program_register + 1], store[program_register + 2]);
         break;
      case OP_CONSTANT:
         constant(store[program_register + 1]);
         break;
      case OP_DIVIDE:
         divide();
         break;
      case OP_ENDPROC:
         endproc();
         break;
      case OP_ENDPROG:
         endprog();
         break;
      case OP_EQUAL:
         equal();
         break;
      case OP_FI:
         fi( store[program_register + 1]);
         break;
      case OP_GREATER:
         greater();
         break;
      case OP_INDEX:
         index( store[program_register + 1], store[program_register + 2]);
         break;
      case OP_LESS:
         less();
         break;
      case OP_MINUS:
         minus();
         break;
      case OP_MODULO:
         modulo();
         break;
      case OP_MULTIPLY:
         multiply();
         break;
      case OP_NOT:
         plnot();
         break;
      case OP_OR:
         plor();
         break;
      case OP_PROC:
         proc(store[program_register + 1], store[program_register + 2]);
         break;
      case OP_PROG:
         prog(store[program_register + 1], store[program_register + 2]);
         break;
      case OP_READ:
         read(store[program_register + 1]);
         break;
      case OP_SUBTRACT:
         subtract();
         break;
      case OP_VALUE:
         value();
         break;
      case OP_VARIABLE:
         variable(store[program_register + 1], store[program_register + 2]);
         break;
      case OP_WRITE:
         write(store[program_register + 1]);
         break;
      default:
         runtime_error(" FATAL! Damaged Program File!");
         break;
    }
  }
}

void Interpreter::memory_dump(string name) const
{
   const char *tempname = name.c_str();
   ofstream dump ( tempname, ios::out);
   
   if (!dump)
   {
     cerr << " couldn't open dump file " << endl;
     return;
   }

   int i = 0;
   
   while ( i < STORE_SIZE )
   {
     dump << store[i] << ' ';
     if ( ( i+1) % 20 == 0)
        dump << endl;
     i++;
   }
}

  
