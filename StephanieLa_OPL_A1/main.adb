-- file to test Matrix_Ops package
-- file written by Demetri to compile Ada code
with Matrix_Ops; use Matrix_Ops;

procedure Main is

    X : constant Matrix :=
    (   (1.0, 2.0, 3.0),
        (4.0, 5.0, 6.0),
        (7.0, 8.0, 9.0)
    );

    Y : constant Matrix :=
    (   (1.0, 2.0, 3.0),
        (4.0, 5.0, 6.0),
        (7.0, 8.0, 9.0)
    );

	Z : Matrix := -- needed to define initial value for Z to get program to compile 
	(	(0.0, 0.0, 0.0),
		(0.0, 0.0, 0.0),
		(0.0, 0.0, 0.0)
	);

begin

    Z := X * Y;

end Main;
