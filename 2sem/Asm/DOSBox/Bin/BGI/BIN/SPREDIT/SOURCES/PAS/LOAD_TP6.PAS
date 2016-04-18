{
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³   This is an example, how you can include your sprites into your own     ³
³   pascal programs.                                                       ³
³                                                                          ³
³   Turbo Pascal 6.01                         (c) by Markus Herbord 1994   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
}

{$X+}
program test;

uses graph, crt, palette;

{********************************************************************}

{$F+}
function AutoDtc:Integer;                        { Function AutoDtc }
begin
  AutoDtc := 0;
end;                                             { Ende AutoDtc }
{$F-}

{********************************************************************}

procedure GraphOn;                               { Procedure GraphOn }
var gd,gm           : Integer;

begin
  gd:= InstallUserDriver('svga256',@autodtc);
  gd:= Detect;
  InitGraph(gd,gm,'');
end;                                             { Ende GraphOn }

{********************************************************************}

procedure load_spr(filename : string);
var f    : file;
    pic  : pointer;
    size : word;

begin
  assign(f,filename);
  {$I-}
  reset(f,1);                                                   { open file }
  {$I+}
  if IOResult=0 then
  begin
    size:=filesize(f);                            { how much memory needed? }
    getmem(pic,size);                                     { allocate memory }
    blockread(f,pic^,size);                                  { load picture }
    close(f);
    putimage(0,0,pic^,copyput);
    freemem(pic,size);                 { free memory, absolutly nessessary! }
  end;
end;

{********************************************************************}

begin
  graphon;
  palinst('..\..\examples\book.pal');
  load_spr('..\..\examples\book.spr');
  readkey;
  closegraph;
end.