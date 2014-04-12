class C {
	init(x : Int, y : Bool) : Int {
           {
		a <- x;
		b <- y;
		3;
           }
	};
	a         : Int; (* Declare after use in the method up at top*)
	b 		  : Bool;
	use_later : Int;
};
class B inherits C{
	convertnum:        Int;
	(* Same method as parent but different result for a and b *)
	init(x : Int, y : Bool) : Int {
    	{
    		while y loop{
    		a <- x + 2;
    		if y then b <- not y else b <- false fi;
			}pool;
                42;			
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
	init(x : Int, y : Bool) : Int {
    	{
			42;
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
	testing() : Int {
		{

			d_type.adding(num, num);(*normal dispatch*)

			d_type.ret_bool(string_example,bool_example);(*normal dispatch*)

			d_type.send_in_d_ret_int(d_type);(*normal dispatch*)


			d_type.send_in_d_ret_bool_param(d_type, not bool_example);(*normal dispatch*)

			d_type.init(num, bool_example);(*normal dispatch*)

			(new F).adding(num,num); (*Since F inherits D I tried to call F method which are D inherits since it inherits all of what D has *)

		    d_type@C.init(num, bool_example);(*Static dispatch, so that d_type must conform to the type speified C init method *)
		    d_type@D.init(num, bool_example);(*Static dispatch, so that d_type must conform to the type speified D init method *)
		  
			d_type@D.adding(num,num); (*Static dispatch, so that d_type must conform to the type speified D adding method *)
			
			(new F)@D.init(num, bool_example);(*Static dispatch, so that new F type must conform to the type speified by D init method *)
			(new C)@C.init(num, bool_example);(*Static dispatch, so that new C type  must conform to the type speified by C init metho *)
			(new F)@F.init(num, bool_example);(*Static dispatch, so that new F type must conform to the type speified by F init method *)

			42;
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
Class Main {
	main():Object {
	  (new C).init(1,true)
	};
};


