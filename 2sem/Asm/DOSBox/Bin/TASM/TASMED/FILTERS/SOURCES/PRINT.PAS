{*******************************************************}
{ Print filter for TASMED.                              }
{ Copyright (C) 1996-99 by Eugene Nonko aka CodeMaster. }
{*******************************************************}

program Print_Filter;

{ Command line format:

    PRINT.EXE <file to print> [<option>...]

  Available options are:

   Printing mode
    /EPSON - Output EPSON-compatible printer codes
    /HP    - Output HP LaserJet-compatible printer codes
    /ASCII - Strip highlight codes (default)

   Output formatting
    /S#    - Output style (default: 1)
    /L??   - Lines per page (default: 64)
    /O???? - Output stream identified (file or device) (default: LPT1) }

{$IFNDEF MSDOS}
  !! Set target to "Real mode Application" and recompile
{$ENDIF}

{$A+,B-,D-,E-,F-,G+,I-,L-,N-,P-,Q-,R-,S-,T-,V-,X+,Y-}
{$M 16384,0,0}

const
  MaxCodeLen = 20;
  NumStyles = 3;

type
  TCode = String[MaxCodeLen];
  TTranslator = record
    Init, Done, PageStart, PageEnd, LineEnd, BoldOn, BoldOff, ItalicOn,
    ItalicOff, UnderlinedOn, UnderlinedOff: TCode
  end;
  TStyle = array[0..9] of Byte;

const
{ Fonts }
  fsNormal = $00;     { Normal }
  fsBold = $01;       { Bold }
  fsItalic = $02;     { Italic }
  fsUnderlined = $04; { Underlined }
{ NB: you can sum up few fonts, for example: fsBold+fsUnderlined }

{ Epson & compatibles }
  Epson: TTranslator = (
    Init: #27'@'#27'x'#1#27'k'#0;
    Done: '';
    PageStart: '';
    PageEnd: #12;
    LineEnd: #13#10;
    BoldOn: #27'E';
    BoldOff: #27'F';
    ItalicOn: #27'4';
    ItalicOff: #27'5';
    UnderlinedOn: #27'-'#1;
    UnderlinedOff: #27'-'#0);

{ HP LaserJet & compatibles }
  HP: TTranslator = (
    Init: #27'E'#27'(10U'#27'&k0S'#27'(s3T';
    Done: '';
    PageStart: '';
    PageEnd: #12;
    LineEnd: #13#10;
    BoldOn: #27'(s3B';
    BoldOff: #27'(s0B';
    ItalicOn: #27'(s1S';
    ItalicOff: #27'(s0S';
    UnderlinedOn: #27'&dD';
    UnderlinedOff: #27'&d@');

{ ASCII (strip highlight codes) }
  ASCII: TTranslator = (
    Init: '';
    Done: '';
    PageStart: '';
    PageEnd: '';
    LineEnd: #13#10;
    BoldOn: '';
    BoldOff: '';
    ItalicOn: '';
    ItalicOff: '';
    UnderlinedOn: '';
    UnderlinedOff: '');

{ Styles }
  Styles: array[1..NumStyles] of TStyle =
   ((fsNormal,             { Symbols }
     fsItalic,             { Comments }
     fsNormal,             { Numbers }
     fsNormal,             { Strings }
     fsNormal,             { Identifiers }
     fsBold,               { Instructions }
     fsNormal,             { Registers }
     fsBold+fsItalic,      { Directives }
     fsNormal,             { Reserved words }
     fsNormal),            { Specifiers }

    (fsNormal,             { Symbols }
     fsUnderlined,         { Comments }
     fsItalic,             { Numbers }
     fsItalic,             { Strings }
     fsNormal,             { Identifiers }
     fsBold,               { Instructions }
     fsBold,               { Registers }
     fsBold+fsItalic,      { Directives }
     fsBold,               { Specifiers }
     fsBold),              { Reserved words }

    (fsNormal,             { Symbols }
     fsUnderlined,         { Comments }
     fsItalic,             { Numbers }
     fsItalic,             { Strings }
     fsNormal,             { Identifiers }
     fsBold,               { Instructions }
     fsItalic,             { Registers }
     fsBold+fsItalic,      { Directives }
     fsBold,               { Specifiers }
     fsBold));             { Reserved words }

var
  InputStream, OutputStream: Text;
  Line, InputStreamName, OutputStreamName: string;
  LinesPerPage, CurLine: Word;
  Style: TStyle;
  Translator: TTranslator;
  OldExitProc: Pointer;
  I, Stl, OldStl: Byte;

  function UpStr(S: string): string;
  var
    I: Byte;
  begin
    for I := 1 to Length(S) do S[I] := UpCase(S[I]);
    UpStr := S
  end;

  function Str2Num(const S: string; Default: Word): Word;
  var
    Error: Integer;
    R: Word;
  begin
    Val(S, R, Error);
    if Error <> 0 then Str2Num := Default else Str2Num := R
  end;

  procedure ParseCommandLine;
  var
    Param: string;
    I: Byte;
    Stl: Word;
  begin
    for I := 1 to ParamCount do
    begin
      Param := UpStr(ParamStr(I));
      if Param[1] in ['-', '/'] then
      begin
        Delete(Param, 1, 1);
        case Param[1] of
          'L' :
            begin
              Delete(Param, 1, 1);
              LinesPerPage := Str2Num(Param, $FFFF);
              if LinesPerPage = $FFFF then Halt
            end;
          'S' :
            begin
              Delete(Param, 1, 1);
              Stl := Str2Num(Param, $FFFF);
              if (Stl > NumStyles) or (Stl = 0) then Halt;
              Style := Styles[Stl]
            end;
          'O' :
            begin
              Delete(Param, 1, 1);
              OutputStreamName := Param
            end
        else
          if Param = 'EPSON' then Translator := Epson else
            if Param = 'ASCII' then Translator := ASCII else
              if Param = 'HP' then Translator := HP else Halt
        end
      end
      else InputStreamName := Param
    end;
    if InputStreamName = '' then Halt
  end;

  procedure InitDefaults;
  begin
    LinesPerPage := 64;
    Translator := ASCII;
    Style := Styles[1];
    OutputStreamName := 'PRN'
  end;

  procedure SetBinaryMode(var T: Text); assembler;
  asm
    les     di, T
    mov     bx, word ptr es:[di]
    mov     ax, 4400h
    int     21h
    test    dx, 0080h
    jz      @@1
    or      dl, 20h
    mov     ax, 4401h
    int     21h
@@1:
  end;

  procedure CleanExit; far;
  begin
    ExitProc := OldExitProc;
    Close(InputStream);
    Close(OutputStream)
  end;

  procedure CompareStyles(Mask: Byte; const On, Off: TCode);
  begin
    if OldStl and Mask <> Stl and Mask then
      if Stl and Mask <> 0 then Write(OutputStream, On) else Write(OutputStream, Off)
  end;

  function EscPressed: Boolean; assembler;
  asm
    mov     ah, 01h
    int     16h
    mov     ax, 0
    jz      @@exit
    int     16h
    cmp     al, 27
    mov     al, 0
    jne     @@exit
    inc     al
@@exit:
  end;

begin
  InitDefaults;
  ParseCommandLine;
  Assign(InputStream, InputStreamName);
  Reset(InputStream);
  if IOResult <> 0 then Halt;
  Assign(OutputStream, OutputStreamName);
  Rewrite(OutputStream);
  if IOResult <> 0 then Halt;
  SetBinaryMode(OutputStream);
  OldExitProc := ExitProc;
  ExitProc := @CleanExit;
  Write(OutputStream, Translator.Init);
  while not EOF(InputStream) do
  begin
    ReadLn(InputStream, Line);
    if EscPressed then Halt;
    Inc(CurLine);
    if CurLine mod LinesPerPage = 1 then Write(OutputStream, Translator.PageStart);
    if IOResult <> 0 then Halt;
    for I := 1 to Length(Line) do
    begin
      if Line[I] = #27 then
        if (I < Length(Line)) and (Line[I+1] in ['0'..'9']) then
          with Translator do
          begin
            Stl := Style[Byte(Line[I+1])-48];
            CompareStyles(fsBold, BoldOn, BoldOff);
            CompareStyles(fsItalic, ItalicOn, ItalicOff);
            CompareStyles(fsUnderlined, UnderlinedOn, UnderlinedOff);
            Inc(I);
            OldStl := Stl;
            Continue
          end;
      Write(OutputStream, Line[I])
    end;
    if CurLine mod LinesPerPage = 0 then Write(OutputStream, Translator.PageEnd) else
      Write(OutputStream, Translator.LineEnd);
    if IOResult <> 0 then Halt
  end;
  ExitProc := OldExitProc;
  if CurLine mod LinesPerPage <> 0 then Write(OutputStream, Translator.PageEnd);
  Write(OutputStream, Translator.Done);
  Close(OutputStream);
  Close(InputStream);
  Erase(InputStream)
end.
