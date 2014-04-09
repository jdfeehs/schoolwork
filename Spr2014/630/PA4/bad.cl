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

		(* result(int num1) : Int { *)
		(* not(22); *) (* illgal use of not passing in a int when expecting a bool *)
		(* ~false;  *) (* wrong return type and use of ~ *)
	    (* }; *)
};
Class A inherits B {};

Class Main {
	num 	   : Int <- 23 +"43"+1; (* adding a string when expecting all in *)
	num_string : String <- "32" + 1 + false; (* for a string adding a int and bool *)
	test       : Bool <- "hello"; (* wrong type string instead bool *)
	testing    : Bool <- not(2);
	testing    : String <- ~2;
	main():C {
	 {
	  (new C).init(1,1);	  (* wrong param types passed in *)
	  (new C).init(1,true,3); (* wrong number of args passed in as params *)
	  (new C).iinit(1,true);  (* no such method *)
	  (new C).result(false);  (* wrong pass in param expected an int *)
	  (new C).result(4);      (* right pass in param but problem with the method *)
	  (new C).main();         (* not a method *)
	  (new C);
	 }
	};
};

Class B inherits C { 
	a 	 : String; (* change of a type different in class C *)
	r 	 : Int;
	r2   : Int;
	b 	 : Bool;
	flag : Bool <- true;
	(*a differenet return type from method in class C*)
	init(x : Int, y : Bool) : Int { 
		while flag loop 
			{
				r <- x + 2 +"String"; (* adding a string to an int result *)
				r2 <- use_later; (*declared in parent but not initlized *)
				b <- y;
			}
		pool
	};
};
Class Cycle_A inherits A{};

Class Cycle_B inherits Cycle_A{};