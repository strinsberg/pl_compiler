begin
  integer n, ans;

  $ Calculates factorial of n and stores it in ans
  proc fact
  begin
    if n = 0 | n = 1 -> ans := 1; []
       n > 1 -> n := n - 1;
       call fact;
       n := n + 1;
    fi;
    write n, ans;
    ans := n * ans;
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
