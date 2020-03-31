$ Test boolean expressions
$ Output should be 1 0 0 1 1 1 1 0
begin
  const n = 8;
  Boolean array B[n];
  integer i;

  B[1] := true;
  B[2] := false;
  B[3] := true & false & true;
  B[4] := true | false | false;
  B[5] := 1 < 2;
  B[6] := 2 > 1;
  B[7] := 2 = 2;
  B[8] := 2 = 1;

  i := 1;
  do i < n + 1 ->
    write B[i];
    i := i + 1;
  od;
end.
