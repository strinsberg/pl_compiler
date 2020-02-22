begin
  write false,                           $ Tests single expression
  true & (20 + 1 = 30),
  false | (false & true) | false,
  5 - 35 < 200,
  5 = 5,
  45 > 200,
  6 * ( 5 + 7 ),
  8 / 9,
  5 \ 2,
  -4 + 4 / (5 * 4),
  -5,
  A[4] = 10,
  a < 34,
  true & ~true;
end.
