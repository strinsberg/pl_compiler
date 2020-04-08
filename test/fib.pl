begin
  integer temp;
  proc fib
  begin
    integer x, y, z;
    if temp > 1 ->
      x := temp;
      temp := x - 1;
      call fib;
      y := temp;
      temp := x - 2;
      call fib;
      z := temp;
      temp := y + z;
    [] ~(temp > 1) -> skip;
    fi;
  end;
  read temp;
  call fib;
  write temp, 10 - 4, 40 + 2;
  temp, temp, temp := 3, 5, 7;
end.
