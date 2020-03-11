begin
  const n = 10;         $ Max allowed n
  integer i, in, ans;   $ Loop counter, input variable, final answer
  integer array DP[n];  $ DP array to avoid re-calculating fib numbers

  $ Calculate the nth fib number in the range [0-10]
  proc fib (var integer ans; integer n)
  begin
    integer a1, a2;           $ Intermediate answers

    if ~(DP[n - 1] = -1) ->   $ Already calculated
      ans := DP[n - 1];
    [] (n = 1 | n = 0) ->     $ fib 0 and 1 = 1
      ans := 1;
    [] (n > 1) ->             $ answer is fib(n-1) + fib(n-2)
      call fib(n - 1, a1);
      call fib(n - 2, a2);
      ans := a1 + a2;
      DP[n - 1] := ans;
    fi;
  end;

  $ Initialize DP to all -1 to signify not yet calculated
  i := 0;
  do i < n ->
    DP[i] := -1;
  od;

  read in;              $ Get a number from the user
  call fib(ans, in);    $ Calc fib of ans
  write ans;            $ Display answer
end.
