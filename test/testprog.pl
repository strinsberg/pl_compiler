$ test program
begin
  integer a, b, c;
  Boolean d;
  proc check
    begin
      if a = 10 -> d := true; []
        ~(a = 10) -> d := false;
      fi;
    end;

  read a, b;
  c := 0;
  do c < 50 ->
    call check;
    if d = true -> c := a + b; []
      d = false -> c := a*d;
    fi;
  od;
  write c;
end.
