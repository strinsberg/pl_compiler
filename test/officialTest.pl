begin
  const n = 10; integer array A[n];
  integer x, i, b, c, a; Boolean found, d;

  proc Search
    begin integer m;
    skip;
    d := false | true & false;
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
  b, c := -2, 3;
  a := (b * c) - 5 \ 2 / 4;
  do ~ (i>n) -> read A[i]; i := i + 1; od;

  $Test Search:
  read x;
  do ~(x = 0) ->
    call Search;
    if found -> write x, i; []
      ~ found -> write x;
    fi;
    read x;
  od;
end.
