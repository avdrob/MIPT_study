{
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³   This Unit shoul help you, to include the color palette into your own   ³
³   programs. It contains functions to load and to save a color palette.   ³
³                                                                          ³
³   Turbo Pascal 6.01                         (c) by Markus Herbord 1994   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
}

unit palette;

 {-----------}
   interface
 {-----------}

Procedure GetCol(C : Byte; Var R, G, B : Byte);
Procedure SetCol(c, r, g, b : Byte);
procedure palinst(palname : string);
procedure save_pal(palname : string);

 {----------------}
   implementation
 {----------------}

uses crt,graph;


{***************************************************************************}

Procedure GetCol(C : Byte; Var R, G, B : Byte);
begin
  Port[$3C7] := C;
  R := Port[$3C9];
  G := Port[$3C9];
  B := Port[$3C9];
end;

{***************************************************************************}

Procedure SetCol(C, R, G, B : Byte);
begin
  Port[$3C8] := C;
  Port[$3C9] := R;
  Port[$3C9] := G;
  Port[$3C9] := B;
end;

{***************************************************************************}

procedure palinst(palname : string);
var count, r, g, b : byte;
    fb             : file of byte;

begin
  assign(fb,palname);
  {$I-}
    reset(fb);                                          { Open the PAL file }
  {$I+}
  if IOResult=0 then                                 { opened successfully? }
  begin
    for count:=0 to 255 do                       { count through all colors }
    begin
      read(fb, r, g, b);                             { read color from file }
      setcol(count, r, g, b);                              { set RGB values }
    end;
    close(fb);
  end;
end;

{***************************************************************************}

procedure save_pal(palname : string);
var r,g,b   : byte;
    c,count : byte;
    fb      : file of byte;

begin
  assign(fb,palname);
  {$I-}
    rewrite(fb);                                           { Make PAL file }
  {$I+}
  If IOResult=0 then
  begin
    for c:=0 to 255 do                          { count through all colors }
    begin
      GetCol(c,r,g,b);                                    { Get RGB values }
      write(fb,r, g, b);                                { write RGB values }
    end;
    close(fb);
  end;
end;

{***************************************************************************}

begin

end.    { end of unit }