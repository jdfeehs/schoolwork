class C {
	init(x : Int, y : Bool) : C {
           {
		a <- x;
		b <- y;
		self;
           }
	};
	a         : Int; (* Declare after use in the method up at top*)
	b 		  : Bool;
	use_later : Int;
};
class B inherits C{
	convertnum:        Int;
	(* Same method as parent but different result for a and b *)
	init(x : Int, y : Bool) : C {
    	{
    		while y loop{
    		a <- x + 2;
    		if y then b <- not y else b <- false fi;
			}pool;
			self;
        }
	};
	
};
class D inherits C{
	(*Basic Assigment testing *)
	result: 		   		Int <- 0;
	adding_result:	   		Int <- 0;
	num: 			   		Int <- 4;
	test_num1:				Int <- 100;
	test_num2:				Int <- 200;
	d_type:			   		D;
	string_example:	   		String <- "Hello";
	object_example:    		Object <- new IO;
	string_example_object:  Object <- string_example;
	bool_example:			Bool <-true;
	self_ret(): D {self};
	init(x : Int, y : Bool) : C {
    	{
			self;
        }
	};
	result(input : Int): Int {
		{
			result <- input;
			use_later <- 25;
		}
	};
	adding(num1 : Int, num2: Int) : Int{
			adding_result <- num1 + num2
	};
	ret_bool(name: String, bool_example : Bool) : Bool{
			not bool_example
	};
	(* Sending in type D as param *)	
	send_in_d_ret_int(d_class_type : D): Int{
		0
	};
	send_in_d_ret_bool_param(d_class_type : D, bool_example : Bool): Bool{
		true
	};
	testing() : C {
		{
			d_type.self_ret();(*normal dispatch*)
			self.self_ret();(*shorthand self dispatch*)

			d_type.adding(num, num);(*normal dispatch*)
			self.adding(num, num);(*shorthand self dispatch*)

			d_type.ret_bool(string_example,bool_example);(*normal dispatch*)
			self.ret_bool(string_example,bool_example);(*shorthand self dispatch*)

			d_type.send_in_d_ret_int(d_type);(*normal dispatch*)
			self.send_in_d_ret_int(d_type);(*shorthand self dispatch*)

			d_type.send_in_d_ret_int(new F);(*Class F inherits D so calling d method passing in F as a subtype using normal dispatch*)
			self.send_in_d_ret_int(new F);  (*Class F inherits D so calling d method passing in F as a subtype using shorthand self disptach*)

			d_type.send_in_d_ret_bool_param(d_type, not bool_example);(*normal dispatch*)
			self.send_in_d_ret_bool_param(d_type, not bool_example);(*shorthand self dispatch*)

			d_type.init(num, bool_example);(*normal dispatch*)
			self.init(num, bool_example);(*shorthand self dispatch*)

			(new F).adding(num,num); (*Since F inherits D I tried to call F method which are D inherits since it inherits all of what D has *)
			(new F).send_in_d_ret_int(new F); (*Since F inherits D I tried to call F method which are D inherits since it inherits all of what D has *)

		    d_type@C.init(num, bool_example);(*Static dispatch, so that d_type must conform to the type speified C init method *)
		    d_type@D.init(num, bool_example);(*Static dispatch, so that d_type must conform to the type speified D init method *)
		  
		    d_type@Object.type_name(); (*Static dispatch, so that d_type must conform to the type speified type_name method in the Object class which is a String Refernce maunal Object class section *)
			d_type@D.adding(num,num); (*Static dispatch, so that d_type must conform to the type speified D adding method *)
			
			(new F)@D.init(num, bool_example);(*Static dispatch, so that new F type must conform to the type speified by D init method *)
			(new C)@C.init(num, bool_example);(*Static dispatch, so that new C type  must conform to the type speified by C init metho *)
			(new F)@F.init(num, bool_example);(*Static dispatch, so that new F type must conform to the type speified by F init method *)

			(new C)@Object.type_name(); (*Static dispatch, so that C must conform to the type speified type_name method in the Object class which is a String Refernce maunal Object class section, since Objec is the root of inherantce, this statment is legal *)
			(new F)@Object.type_name(); (*Static dispatch, so that F must conform to the type speified type_name method in the Object class which is a String Refernce maunal Object class section,  since Objec is the root of inherantce, this statment is legal  *)
			self;
		}
	};
	(* Testing out basic valid statement in COOL *)
	test() : Object{
		{		
			~test_num2;
			string_example;
			not bool_example;
			test_num1 + test_num2;
			test_num2 - test_num1;
			test_num2 * test_num1;
			test_num2 / test_num1;
			isvoid self;
		}
	};
	
};
class F inherits D {};
(* class A2I class is declare somewhere else, must include when compliing *)
class E inherits A2I {
		convertnum:        Int;
		adding_result:	   Int;
		ret_self(): E {self};
		result(input : String): Int {
			(let z: A2I <- new A2I in convertnum <- z.a2i(input))
		};
};
Class Main {
	main():C {
	  (new C).init(1,true)
	};
};


