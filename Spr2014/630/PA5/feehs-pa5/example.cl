class A {

	attrInt : Int;
	attrStr : String <- "hello";

	method(form : Int) : Int { 10 };

};

class C inherits A {

	cAttr : Bool;

	method2(form : String) : Bool { true };

};

class Main inherits IO {
   
   a : A <- new A;
   b : Int <- 10;   
   main(): Int {
	{
          1 + 1;	
          2 - 1;
          3 * 4;
          10 / 5;
          "Hello world!";
	  ~2;
	}
   };
};

