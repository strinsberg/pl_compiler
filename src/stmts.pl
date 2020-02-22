begin
  skip;
  read a, b, c;
  write 10 + 4 < 30;
  a, b, c[4] := 3, 5, 6[pos];
  call steveIsGreat;
  if a -> write 10; []
    ~a -> write 30;
  fi;
  do (i < n) -> read A[i]; od;
end.
