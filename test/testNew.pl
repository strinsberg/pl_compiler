begin
  integer d;

  record cell
    float x, y;
    Boolean v;
  end

  proc makeCell (float a; float b)
  begin
    cell.x = a;
    cell.y = b;
    cell.v = false;
  end

  proc inc (var int a)
  begin
    a = a + 1;
  end

  d = 0;
  call makeCell(2.0, 1.0 + 6.8);
  call inc(d);

  cell.v = true;
  write cell.x, cell.y, cell.v;

end .
