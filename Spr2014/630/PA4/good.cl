class C {
	a : Int;
	b : Bool;
	r : Int <- 0; 

	set_r(num : Int) : SELF_TYPE {
		{
			r <- num;
			self;
		}
	};
	add(num1 : Int, num2: Int) : B {
		(let x: Int in { 
			 x <- num1 + num2; 
			 (new B).set_r(x);
			 }
		)
	};
	init(x : Int, y : Bool) : C {
           {
		a <- x;
		b <- y;
		self;
           }
	};

};
class A {

};
class B inherits C {

};
class D {

};
Class Main {
	main():C {
	  (new C).init(1,true)
	};
};
