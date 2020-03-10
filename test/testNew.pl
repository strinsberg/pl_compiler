begin
  integer d;

  record cell1, cell2, cell3
    float x, y;
    Boolean v
  end;

  proc makeCells (float a; float b)
  begin
    cell1.x, cell1.y, cell1.v := a, b, false;
    cell2.x, cell2.y, cell2.v := a, b, false;
    cell3.x, cell3.y, cell3.v := a, b, false;
  end;

  proc inc (var integer a)
  begin
    a := a + 1;
  end;

  d := 0;
  call makeCell(2.0, 1.0 + 6.8);
  call inc(d);

  cell1.v := true;
  write cell1.x, cell1.y, cell1.v;

end .
