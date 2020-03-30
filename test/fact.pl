$ Calculates factorial for a given n
$ Due to the fast growth of factorials will overflow after 12
begin
  integer n, ans;

  $ Recursively calculates factorial of n and stores it in ans
  proc fact
  begin
    if n = 0 | n = 1 ->
        ans := 1; []
      n > 1 ->
        n := n - 1;
        call fact;
        ans := ans * n;
    fi;
    n := n + 1;
  end;

 
  $ Calculate factorial of n
  ans := 0;
  read n;
  do ~(n < 0) ->
    call fact;
    write ans;
    ans := 0;
    read n;
  od;
end.
