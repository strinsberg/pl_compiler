$PL Program: Input a table and output it 
$PL TEST program II
begin
   const n = 10;integer array A[n];
   integer i, j;
   proc p
    begin
     integer i,x;
     Boolean found;
     i,x := 1,2;
     j := i;
     found := true;
    end;
$Input Table:
   i,j := 1,1;
   do ~(i>n)->read A[i];i:=i+1;od;
   if i = 11 -> write i;[]
       ~(i = 11) ->  write 0;
   fi; 
$  call p; 
end.

$Fini
