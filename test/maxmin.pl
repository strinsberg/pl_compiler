$ Finds the max and min elements in a list
begin
  const n = 5;
  integer array A[n];
  integer i, min, max;

  $ Input Table of n elements
  proc get_table
  begin
    i := 1;
    do ~(i > n) ->
      read A[i];
      i := i + 1;
    od;
  end;

  $ Read table and set min and max to really high and low numbers
  call get_table;
  min, max := 1000000000, -1000000000;
  i := 1;

  $ Loop through A and find min and max
  do i < n + 1 ->
    if A[i] < min ->
        min := A[i]; []
      ~(A[i] < min) -> skip;
    fi;

    if A[i] > max ->
        max := A[i]; []
      ~(A[i] > max) -> skip;
    fi;

    i := i + 1;
  od;

  write min, max;
end.
