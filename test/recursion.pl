$ Uses recursion to make a given integer 0
$ Will stack overflow if n < -287 or  n > 287
begin
  integer n;

  $ Increment a negative number until it reaches 0
  proc zero_neg
  begin
    if n = 0 -> skip; []
      n < 0 ->
        write n;
        n := n + 1;
        call zero_neg;
    fi;
  end;

  $ Decrement or inrement a number until it reaches 0
  proc zero
  begin
    if n = 0 -> skip; []
      n > 0 ->
        write n;
        n := n - 1;
        call zero; []
      n < 0 ->
        call zero_neg;
    fi;
  end;

  $ Read number and zero it
  read n;
  call zero;
  write n;
end.
