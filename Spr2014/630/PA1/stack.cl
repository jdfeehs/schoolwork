(*
 *  CS630
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Working copy developed by Josh Feehs
 *  This work fully complies with the JMU Honor Code
 *
 *  I copied code from list.cl and edited it to fit a String 
 *)

class Main inherits IO {
   stack : Stack <- new Stack;   -- The data structure for the stack
   curString : String <- " "; --This will hold the input string
   quit : String <- "f";
   arg1 : String;
   arg2 : String;
   a2i : A2I <- new A2I;
main() : Object {
      {
         while quit = "f" loop
         {
            curString <- prompt();
            if curString = "x" then quit <- "t"
            else process(curString)
             fi;
         }
         pool;
      }
   };
   process(curString : String) : Object{
      if curString = "+" then stack.push("+") else
      if curString = "s" then s() else
      if curString = "e" then e() else
      if curString = "d" then d() else
      int()
      fi fi fi fi

   };
   prompt() : String{
      {
         out_string(">");
         in_string();
      } 
   };
   print_list(l : List) : Object {
      if l.isNil() then out_string("\n")
                   else {
                           out_string(l.head());
                           out_string(" ");
                           print_list(l.tail());
                        }
      fi
   };
   plus() : Object{
      {
         arg1 <- stack.get_top();
         stack.pop();
         arg2 <- stack.get_top();
         stack.pop();
         stack.push(a2i.i2c(a2i.c2i(arg1) + a2i.c2i(arg2)));
      }
   };
   s() : Object{
      stack.push("s")
   };
   e() : Object{
      {
      if not stack.isNil() then {
      curString <- stack.get_top(); -- a placeholder
      --get_top of the stack
      -- check to see what it is, and act accordingly
      -- if its s, pop it, pop 2, swap, and push 2
      -- if +, pop it, pop the ints, atoi, and push result
      if curString = "s" then
         {
           stack.pop();
           swap();
         }
      else if curString = "+" then
         { 
           stack.pop();
           plus(); -- placeholder
         }
      
      else curString <- "e"
      fi fi;
      }
      else curString <- "e"
      fi;
      }
   };
   d() : Object {
     stack.print_stack()
   };
   int() : Object {
      stack.push(curString)
   };
   swap() : Object{
      {
      arg1 <- stack.get_top();
      stack.pop();
      arg2 <- stack.get_top();
      stack.pop();
      stack.push(arg1);
      stack.push(arg2); 
      }
   };
};

  class List {
   -- Define operations on empty lists.

   isNil() : Bool { true };

   -- Since abort() has return type Object and head() has return type
   -- Int, we need to have an Int as the result of the method body,
   -- even though abort() never returns.

   head()  : String { { abort(); " "; } };

   -- As for head(), the self is just to make sure the return type of
   -- tail() is correct.

   tail()  : List { { abort(); self; } };

   -- When we cons and element onto the empty list we get a non-empty
   -- list. The (new Cons) expression creates a new list cell of class
   -- Cons, which is initialized by a dispatch to init().
   -- The result of init() is an element of class Cons, but it
   -- conforms to the return type List, because Cons is a subclass of
   -- List.


   cons(i : String) : List {
      (new Cons).init(i, self)
   };

};


(*
 *  Cons inherits all operations from List. We can reuse only the cons
 *  method though, because adding an element to the front of an emtpy
 *  list is the same as adding it to the front of a non empty
 *  list. All other methods have to be redefined, since the behaviour
 *  for them is different from the empty list.
 *
 *  Cons needs two attributes to hold the integer of this list
 *  cell and to hold the rest of the list.
 *
 *  The init() method is used by the cons() method to initialize the
 *  cell.
 *)

class Cons inherits List {

   car : String;   -- The element in this list cell

   cdr : List;  -- The rest of the list

   isNil() : Bool { false };

   head()  : String { car };

   tail()  : List { cdr };

   init(i : String, rest : List) : List {
      {
         car <- i;
         cdr <- rest;
         self;
      }
   };
};  

class Stack inherits IO {
   list : List <- new List;
  
   isNil() : Bool {list.isNil()};
   push(char : String) : Object{
      list <- list.cons(char)
   };

   get_top() : String {
      list.head()
   };

   pop() : Object {
      list <- list.tail()
   };

   print_stack() : Object {
      print_list(list)
   };

   print_list(l : List) : Object {
      if l.isNil() then out_string("")
                   else {
                           out_string(l.head());
                           out_string("\n");
                           print_list(l.tail());
                        }
      fi
   };

};

