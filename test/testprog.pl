$ test program
begin
  integer a, b, c, e;
  Boolean d;
  proc check
    begin
      if a = 10 -> d := true; []
        ~(a = 10) -> d := false;
      fi;
    end;

  read a, b;
  c := 0;
  do c < 10 ->
    call check;
    if d = true -> e := e + b, d := false; []
      d = false -> e := e*a, d := true;
    fi;
c := c+1;
  od;
  write e;
end.
