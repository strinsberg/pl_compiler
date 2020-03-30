$ First collects 10 integers for a table
$ Then each input is a number to search for in the list
$ During the search phase input 0 to quit
$ The number searched for and its index will be output for each query
begin
  const n = 10;
  const p = -1;
  integer array A[n];
  integer x, i;
  Boolean found;

  proc Search
  begin
    integer m;

    i, m := 1, n;
    do i < m ->
      if A[i] = x -> m := i; []
        ~ (A[i] = x)-> i := i + 1;
      fi;
    od;

    found := A[i] = x;
  end;

  $Input Table:
  i := 1;
  do ~ (i>n) -> read A[i]; i := i + 1; od;

  $Test Search:
  read x;

  do ~(x = 0) ->
    call Search;

    if found -> write x, i; []
      ~ found -> write p;
    fi;

    read x;
  od;

end.
