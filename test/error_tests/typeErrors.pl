begin
  const n = 10.5;
  integer array A[n];
  integer a, b, c;
  Boolean e, f, g;
  float h;

  proc set(var integer a)
  begin
    integer b;
    a := b;
  end;

  call set(45);  $ Should be an error
  call set(h);

  a, b, c := e, f, g;
  a := true | false;
  e := 2 + 4;
  b := 4 * 6.8;
  h := a;
  h := e;

  if (4 * 5) -> skip; fi;
  do (n + 3.6) -> skip; od;
end.
