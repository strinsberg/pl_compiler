$ Program to read in a table of 10 integers
$ and to write those same 10 integers
begin
   const n = 10;
   integer array A[n];
   integer i;

   $ Procedure to print the table
   proc print
    begin
      integer i;
      i := 1;
      do ~(i > n) ->
          write A[i];
          i := i + 1;
      od;
    end;

   $ Input Table
   i := 1;
   do ~(i>n) ->
       read A[i];
       i:=i+1;
   od;

   call print; 
end.
