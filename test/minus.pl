$ Just to test different uses of negative numbers
$ Had a problem with how the assembly code was generating
$ and this file was used to test it
begin
  $ Grammar does not allow minus with constants
  $ const n = -10;   $ So this statment is an error
  const n = 10;
  integer i;

  i := -1;
  write n, i, -99;
end.
