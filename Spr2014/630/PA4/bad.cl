class C {
	a 	 	  : Int;
	b 	 	  : Bool;
	flag 	  : Bool <- true;
	use_later : Int;
	init(x : Int, y : Bool) : C {
           {
		a <- x;
		b <- y;
		self;
           }
	};

		 result(num1 : Int) : Int { 
		 	{
				 not 22;  (* illegal use of not passing in a int when expecting a bool *)
				 ~false;  (* wrong return type and use of ~ *)
		 	}
	    }; 
};
Class A inherits B {};

Class Main {
	num 	   : Int <- 23 +"43"+1; (* adding a string when expecting all in *)
	num_string : String <- "32" + 1 + false; (* for a string adding a int and bool *)
	test       : Bool <- "hello"; (* wrong type string instead bool *)
	testing    : Bool <- not 2;   (* wrong type of assignment*)
	testing    : String <- ~2;	  (* wrong type of assignment*)

	main():C {
	 {
	  (new C).init(1,1);	  (* wrong param types passed in *)
	  (new C).init(1,true,3); (* wrong number of args passed in as params *)
	  (new C).iinit(1,true);  (* no such method *)
	  (new C).result(false);  (* wrong pass in param expected an int *)
	  (new C).result(4);      (* right pass in param but problem with the method *)
	  (new C).main();         (* not a method *)
	  self.hello();			  (* no such method *)
	  (new B).init(23,2);     (* Static dispatch wrong passed in param*)
	  (new B)@A.init(23,true);(* Expression type B does not conform to declared static dispatch type A. *)
	  (new A)@C.main();       (* Static dispatch to unknown method*)
	  (new A)@Object.print("Hello World!\n"); (*no such fuction print*)
	  (new A2I)@E.ret_self(self); (*Declaration error static dispatch: Expression type A2I does not conform to declared static dispatch type E*)
	  (new Object)@E.copy();
	  (new C);
	  "HI"+1; (*adding a string and int*)
	  (new K)@C.init(1,true); (*class K doesnt exist*)
	  (new K).init(1,ture); (*No such class K and wrong spelling of true*)
	  (new Object)@K.main();(*no class K so distach to undefied class *)
	 }
	};
};
(* class A2I class is declare somewhere else, must include when compliing *)
class E inherits A2I {
		convertnum:        Int;
		convertnum_string: String;
		adding_result:	   Int;
		ret_self(): E {(new C)};
		result(input : String): Int {
			{
			 (let s: A2I <- new A2I in convertnum <- z.i2a(input+1)); (*Wrong input string + int and wrong fuction and z is used but not declared*)
			convertnum_string <- true; (*Wrong assignment*)
			never_seen_before; (*not declared*)
			}
		};
};
Class B inherits C { 
	a 	 : String; (* change of a type different in class C *)
	r 	 : Int;
	r2   : Int;
	b 	 : Bool;
	flag : Bool <- true;
	(*a different return type from method in class C*)
	init(x : Int, y : Bool) : Int { 
		{
		while flag loop 
			{
				r <- x + 2 +"String"; (* adding a string to an int result *)
				r2 <- use_later; (*declared in parent but not initialized *)
				b <- y;
			}
		pool;
		if flag+not true then y <- flag+flase else flag <- y+1 fi; (*improper use of + with bool *)
		}
	};
};
Class Cycle_A inherits A{};

Class Cycle_B inherits Cycle_A{};