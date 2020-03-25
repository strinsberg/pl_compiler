begin                   $program begin lexical level 0
 integer i,j; 	        $program variables
 proc P                 $Procedure  
  begin                 $procedure P begin lexical level 1
   proc S               $procedure S nested in P   
    begin               $Procedure S begin lexical level 2
      integer k;
      call P;		$S call P; P's lex parent is prog (at 2 levels rel to S)
    end;                $S ends here
   proc T		$Procedure T nested in P
    begin		$T begins here
     call S;		$T call S; S's lex parent is P( at 1 level rel to T)
    end;		$T ends here
   call S;		$P call S; Ss' lex parent is P (at level 0 rel to P)
 end;                   $P ends here
 call P;                $Call P ; P's lex parent is prog (at level 0 rel to prog) 
end.                    $Program ends here
